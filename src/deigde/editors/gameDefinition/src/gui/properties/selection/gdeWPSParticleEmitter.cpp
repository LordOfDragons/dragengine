/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "gdeWPSParticleEmitter.h"
#include "gdeWPSParticleEmitterListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/filepattern/gdeFilePattern.h"
#include "../../../gamedef/particleemitter/gdeParticleEmitter.h"
#include "../../../undosys/particleemitter/gdeUParticleEmitterSetPath.h"
#include "../../../undosys/particleemitter/gdeUParticleEmitterSetName.h"
#include "../../../undosys/particleemitter/gdeUParticleEmitterSetDescription.h"
#include "../../../undosys/particleemitter/gdeUParticleEmitterSetCategory.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>



// Actions
///////////

namespace{

class cEditPath : public igdeEditPathListener {
	gdeWPSParticleEmitter &pPanel;
	
public:
	cEditPath( gdeWPSParticleEmitter &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		gdeParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if( ! particleEmitter || particleEmitter->GetPath() == editPath->GetPath() ){
			return;
		}
		
		if( pPanel.GetGameDefinition()->GetParticleEmitters().HasWithPath( editPath->GetPath() ) ){
			igdeCommonDialogs::Information( pPanel.GetParentWindow(), "Change particle emitter path",
				"A particle emitter with this path exists already." );
			editPath->SetPath( particleEmitter->GetPath() );
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUParticleEmitterSetPath( particleEmitter, editPath->GetPath() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cTextName : public igdeTextFieldListener {
	gdeWPSParticleEmitter &pPanel;
	
public:
	cTextName( gdeWPSParticleEmitter &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		gdeParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if( ! particleEmitter || particleEmitter->GetName() == textField->GetText() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUParticleEmitterSetName( particleEmitter, textField->GetText() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cTextDescription : public igdeTextAreaListener {
	gdeWPSParticleEmitter &pPanel;
	
public:
	cTextDescription( gdeWPSParticleEmitter &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextArea *textArea ){
		gdeParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if( ! particleEmitter || particleEmitter->GetDescription() == textArea->GetDescription() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUParticleEmitterSetDescription( particleEmitter, textArea->GetText() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cComboCategory : public igdeComboBoxListener {
	gdeWPSParticleEmitter &pPanel;
	
public:
	cComboCategory( gdeWPSParticleEmitter &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		gdeParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if( ! particleEmitter || particleEmitter->GetCategory() == comboBox->GetText() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUParticleEmitterSetCategory( particleEmitter, comboBox->GetText() ) );
		pPanel.GetGameDefinition()->GetUndoSystem()->Add( undo );
	}
};

class cActionJumpToCategory : public igdeAction {
	gdeWPSParticleEmitter &pPanel;
	
public:
	cActionJumpToCategory( gdeWPSParticleEmitter &panel ) : 
	igdeAction( "", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallStrongRight ),
		"Jump to Category" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		gdeParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if( ! particleEmitter ){
			return;
		}
		
		gdeGameDefinition &gameDefinition = *pPanel.GetGameDefinition();
		gdeCategory * const category = gameDefinition.GetCategoriesParticleEmitter()
			.GetWithPath( particleEmitter->GetCategory() );
		if( ! category ){
			return;
		}
		
		gameDefinition.SetActiveCategory( category );
		gameDefinition.SetSelectedObjectType( gdeGameDefinition::eotCategoryParticleEmitter );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetParticleEmitter() );
	}
};

}



// Class gdeWPSParticleEmitter
////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSParticleEmitter::gdeWPSParticleEmitter( gdeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pGameDefinition( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox, frameLine;
	
	pListener = new gdeWPSParticleEmitterListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	// particle emitter
	helper.GroupBox( content, groupBox, "Particle Emitter:" );
	helper.EditPath( groupBox, "Path:", "Path to particle emitter",
		igdeEnvironment::efpltParticleEmitter, pEditPath, new cEditPath( *this ) );
	helper.EditString( groupBox, "Name:", "Particle emitter name", pEditName, new cTextName( *this ) );
	helper.EditString( groupBox, "Description:", "Particle emitter description",
		pEditDescription, 15, 5, new cTextDescription( *this ) );
	
	helper.FormLineStretchFirst( groupBox, "Category: ", "Category", frameLine );
	helper.ComboBoxFilter( frameLine, true, "Category", pCBCategory, new cComboCategory( *this ) );
	pCBCategory->SetDefaultSorter();
	pCBCategory->SetFilterCaseInsentive( true );
	helper.Button( frameLine, pBtnJumpToCategory, new cActionJumpToCategory( *this ), true );
}

gdeWPSParticleEmitter::~gdeWPSParticleEmitter(){
	SetGameDefinition( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void gdeWPSParticleEmitter::SetGameDefinition( gdeGameDefinition *gameDefinition ){
	if( gameDefinition == pGameDefinition ){
		return;
	}
	
	if( pGameDefinition ){
		pGameDefinition->RemoveListener( pListener );
		pGameDefinition->FreeReference();
	}
	
	pGameDefinition = gameDefinition;
	
	if( gameDefinition ){
		gameDefinition->AddListener( pListener );
		gameDefinition->AddReference();
	}
	
	UpdateParticleEmitter();
	
	UpdateCategoryList();
}



gdeParticleEmitter *gdeWPSParticleEmitter::GetParticleEmitter() const{
	return pGameDefinition ? pGameDefinition->GetActiveParticleEmitter() : NULL;
}



void gdeWPSParticleEmitter::UpdateCategoryList(){
	const decString selection( pCBCategory->GetText() );
	
	pCBCategory->RemoveAllItems();
	
	if( pGameDefinition ){
		const gdeCategoryList &categories = pGameDefinition->GetCategoriesParticleEmitter();
		if( categories.GetCount() > 0 ){
			UpdateCategoryList( categories, "" );
		}
		
		pCBCategory->SortItems();
		pCBCategory->StoreFilterItems();
	}
	
	pCBCategory->SetText( selection );
	pCBCategory->SetInvalidValue( ! pCBCategory->GetText().IsEmpty() && ! pCBCategory->GetSelectedItem() );
}

void gdeWPSParticleEmitter::UpdateCategoryList( const gdeCategoryList &list, const char *prefix ){
	const int count = list.GetCount();
	decString text;
	int i;
	
	for( i=0; i<count; i++ ){
		const gdeCategory &category = *list.GetAt( i );
		text.Format( "%s%s", prefix, category.GetName().GetString() );
		pCBCategory->AddItem( text );
		if( category.GetCategories().GetCount() > 0 ){
			text.AppendCharacter( '/' );
			UpdateCategoryList( category.GetCategories(), text );
		}
	}
}



void gdeWPSParticleEmitter::UpdateParticleEmitter(){
	const gdeParticleEmitter * const particleEmitter = GetParticleEmitter();
	
	if( particleEmitter ){
		pEditPath->SetPath( particleEmitter->GetPath() );
		pEditName->SetText( particleEmitter->GetName() );
		pEditDescription->SetText( particleEmitter->GetDescription() );
		pCBCategory->SetText( particleEmitter->GetCategory() );
		pCBCategory->SetInvalidValue( ! pCBCategory->GetText().IsEmpty()
			&& ! pCBCategory->GetSelectedItem() );
		
	}else{
		pEditPath->ClearPath();
		pEditName->ClearText();
		pEditDescription->ClearText();
		pCBCategory->ClearText();
		pCBCategory->SetInvalidValue( false );
	}
	
	const bool enabled = particleEmitter != NULL;
	pEditPath->SetEnabled( enabled );
	pEditName->SetEnabled( enabled );
	pEditDescription->SetEnabled( enabled );
	pCBCategory->SetEnabled( enabled );
}
