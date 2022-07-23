/* 
 * Drag[en]gine IGDE Game Definition Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "gdeWPSCategory.h"
#include "gdeWPSCategoryListener.h"
#include "../gdeWPPatternList.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../undosys/category/gdeUCategorySetName.h"
#include "../../../undosys/category/gdeUCategorySetDescription.h"
#include "../../../undosys/category/gdeUCategorySetAutoCategorizePattern.h"
#include "../../../undosys/category/gdeUCategoryToggleHidden.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/skin/gdeSkin.h"
#include "../../../gamedef/particleemitter/gdeParticleEmitter.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cTextName : public igdeTextFieldListener {
	gdeWPSCategory &pPanel;
	
public:
	cTextName( gdeWPSCategory &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		gdeCategory * const category = pPanel.GetCategory();
		if( ! category || textField->GetText() == category->GetName() ){
			return;
		}
		
		if( category->GetParent()
		&& category->GetParent()->GetCategories().HasNamed( textField->GetText() ) ){
			igdeCommonDialogs::Information( pPanel.GetParentWindow(), "Rename category",
				"A category with this name exists already." );
			textField->SetText( category->GetName() );
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUCategorySetName( pPanel.GetGameDefinition(),
			category, pPanel.GetCategoryType(), textField->GetText() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cTextDescription : public igdeTextAreaListener {
	gdeWPSCategory &pPanel;
	
public:
	cTextDescription( gdeWPSCategory &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextArea *textArea ){
		gdeCategory * const category = pPanel.GetCategory();
		if( ! category || textArea->GetText() == category->GetDescription() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUCategorySetDescription( pPanel.GetGameDefinition(),
			category, pPanel.GetCategoryType(), textArea->GetText() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cEditAutoCategorizePattern : public gdeWPPatternList {
	gdeWPSCategory &pPanel;
	
public:
	cEditAutoCategorizePattern( gdeWPSCategory &panel ) : gdeWPPatternList(
		panel.GetEnvironment().GetUIHelper(), 3, "Auto Categorize Pattern" ),
		pPanel( panel ){ }
	
	virtual igdeUndo *UndoSet( const decStringSet &patterns ){
		gdeCategory * const category = pPanel.GetCategory();
		if( ! category || category->GetAutoCategorizePattern() == patterns ){
			return NULL;
		}
		return new gdeUCategorySetAutoCategorizePattern( pPanel.GetGameDefinition(),
			category, pPanel.GetCategoryType(), patterns );
	}
};

class cActionHidden : public igdeAction {
	gdeWPSCategory &pPanel;
	
public:
	cActionHidden( gdeWPSCategory &panel ) : igdeAction( "Hidden", "Hide in browsers" ), pPanel( panel ){ }
	
	virtual void Update(){
		const gdeCategory * const category = pPanel.GetCategory();
		SetEnabled( category );
		SetSelected( category && category->GetHidden() );
	}
	
	virtual void OnAction(){
		gdeCategory * const category = pPanel.GetCategory();
		if( ! category ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUCategoryToggleHidden( pPanel.GetGameDefinition(),
			category, pPanel.GetCategoryType() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cListElements : public igdeListBoxListener {
	gdeWPSCategory &pPanel;
	
public:
	cListElements( gdeWPSCategory &panel ) : pPanel( panel ){ }
	
	virtual void OnDoubleClickItem( igdeListBox *listBox, int index ){
		JumpToElement( *listBox, index );
	}
	
	void JumpToElement( igdeListBox &listBox, int index ){
		gdeCategory * const category = pPanel.GetCategory();
		if( ! category ){
			return;
		}
		
		const igdeListItem &item = *listBox.GetItemAt( index );
		if( ! item.GetData() ){
			return;
		}
		
		gdeGameDefinition &gameDefinition = *pPanel.GetGameDefinition();
		
		switch( pPanel.GetCategoryType() ){
		case gdeUCategoryBase::ectObjectClass:
			gameDefinition.SetActiveObjectClass( ( gdeObjectClass* )item.GetData() );
			gameDefinition.SetSelectedObjectType( gdeGameDefinition::eotObjectClass );
			break;
			
		case gdeUCategoryBase::ectSkin:
			gameDefinition.SetActiveSkin( ( gdeSkin* )item.GetData() );
			gameDefinition.SetSelectedObjectType( gdeGameDefinition::eotSkin );
			break;
			
		case gdeUCategoryBase::ectSky:
			break;
			
		case gdeUCategoryBase::ectParticleEmitter:
			gameDefinition.SetActiveParticleEmitter( ( gdeParticleEmitter* )item.GetData() );
			gameDefinition.SetSelectedObjectType( gdeGameDefinition::eotParticleEmitter );
			break;
			
		default:
			break;
		}
	}
};

}



// Class gdeWPSCategory
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSCategory::gdeWPSCategory( gdeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pGameDefinition( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox, frameLine;
	
	pListener = new gdeWPSCategoryListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	// category
	helper.GroupBox( content, groupBox, "Category:" );
	helper.EditString( groupBox, "Name:", "Category name", pEditName, new cTextName( *this ) );
	helper.EditString( groupBox, "Description:", "Category description",
		pEditDescription, 15, 5, new cTextDescription( *this ) );
	helper.CheckBox( groupBox, pChkHidden, new cActionHidden( *this ) );
	
	// auto categorize
	helper.GroupBoxFlow( content, groupBox, "Auto Categorize:" );
	pAutoCategorizePattern.TakeOver( new cEditAutoCategorizePattern( *this ) );
	groupBox->AddChild( pAutoCategorizePattern );
	
	// element listing
	helper.GroupBoxFlow( content, groupBox, "Assigned Elements:" );
	helper.ListBox( groupBox, 10, "Assigned Elements. Double click to jump to element",
		pListElements, new cListElements( *this ) );
	pListElements->SetDefaultSorter();
}

gdeWPSCategory::~gdeWPSCategory(){
	SetGameDefinition( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void gdeWPSCategory::SetGameDefinition( gdeGameDefinition *gameDefinition ){
	if( gameDefinition == pGameDefinition ){
		return;
	}
	
	gdeWPPatternList &pattern = ( gdeWPPatternList& )( igdeWidget& )pAutoCategorizePattern;
	pattern.SetUndoSystem( NULL );
	pattern.SetPatternList( NULL );
	
	if( pGameDefinition ){
		pGameDefinition->RemoveListener( pListener );
		pGameDefinition->FreeReference();
	}
	
	pGameDefinition = gameDefinition;
	
	if( gameDefinition ){
		gameDefinition->AddListener( pListener );
		gameDefinition->AddReference();
		
		pattern.SetUndoSystem( gameDefinition->GetUndoSystem() );
	}
	
	UpdateCategory();
}

gdeCategory *gdeWPSCategory::GetCategory() const{
	return pGameDefinition ? pGameDefinition->GetActiveCategory() : NULL;
}

gdeUCategoryBase::eCategoryType gdeWPSCategory::GetCategoryType() const{
	gdeCategory *category = GetCategory();
	if( ! category ){
		DETHROW( deeInvalidParam );
	}
	
	while( category->GetParent() ){
		category = category->GetParent();
	}
	
	if( pGameDefinition->GetCategoriesObjectClass().Has( category ) ){
		return gdeUCategoryBase::ectObjectClass;
		
	}else if( pGameDefinition->GetCategoriesSkin().Has( category ) ){
		return gdeUCategoryBase::ectSkin;
		
	}else if( pGameDefinition->GetCategoriesSky().Has( category ) ){
		return gdeUCategoryBase::ectSky;
		
	}else if( pGameDefinition->GetCategoriesParticleEmitter().Has( category ) ){
		return gdeUCategoryBase::ectParticleEmitter;
		
	}else{
		DETHROW( deeInvalidParam );
	}
}



void gdeWPSCategory::UpdateCategory(){
	gdeWPPatternList &pattern = ( gdeWPPatternList& )( igdeWidget& )pAutoCategorizePattern;
	const gdeCategory * const category = GetCategory();
	
	if( category ){
		pEditName->SetText( category->GetName() );
		pEditDescription->SetText( category->GetDescription() );
		
		pattern.SetPatternList( &category->GetAutoCategorizePattern() );
		pattern.UpdateList();
		
	}else{
		pEditName->ClearText();
		pEditDescription->ClearText();
		pattern.SetPatternList( NULL );
	}
	
	pChkHidden->GetAction()->Update();
	
	const bool enabled = category != NULL;
	pEditName->SetEnabled( enabled );
	pEditDescription->SetEnabled( enabled );
	
	UpdateListElements();
}

void gdeWPSCategory::UpdateListElements(){
	pListElements->RemoveAllItems();
	
	const gdeCategory * const category = GetCategory();
	if( ! category || ! pGameDefinition ){
		return;
	}
	
	const decString path( category->GetPath() );
	
	switch( GetCategoryType() ){
	case gdeUCategoryBase::ectObjectClass:{
		const gdeObjectClassList &list = pGameDefinition->GetObjectClasses();
		const int count = list.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			gdeObjectClass * const oclass = list.GetAt( i );
			if( oclass->GetCategory() == path ){
				pListElements->AddItem( oclass->GetName(), NULL, oclass );
			}
		}
		}break;
		
	case gdeUCategoryBase::ectSkin:{
		const gdeSkinList &list = pGameDefinition->GetSkins();
		const int count = list.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			gdeSkin * const skin = list.GetAt( i );
			if( skin->GetCategory() == path ){
				pListElements->AddItem( skin->GetName(), NULL, skin );
			}
		}
		}break;
		
	case gdeUCategoryBase::ectSky:
		break;
		
	case gdeUCategoryBase::ectParticleEmitter:{
		const gdeParticleEmitterList &list = pGameDefinition->GetParticleEmitters();
		const int count = list.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			gdeParticleEmitter * const pemitter = list.GetAt( i );
			if( pemitter->GetCategory() == path ){
				pListElements->AddItem( pemitter->GetName(), NULL, pemitter );
			}
		}
		}break;
		
	default:
		break;
	}
	
	pListElements->SortItems();
	if( pListElements->GetItemCount() > 0 ){
		pListElements->SetSelection( 0 );
	}
}
