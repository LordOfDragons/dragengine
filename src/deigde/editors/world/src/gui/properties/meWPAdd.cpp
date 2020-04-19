/* 
 * Drag[en]gine IGDE World Editor
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

#include "meWPAdd.h"
#include "meWPAddListener.h"
#include "meWindowProperties.h"
#include "../meWindowMain.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/igdeGDCategory.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cActionClassAdd : public igdeAction{
	meWPAdd &pPanel;
	igdeComboBoxFilterReference &pComboClass;
	
public:
	cActionClassAdd( meWPAdd &panel, igdeComboBoxFilterReference &comboClass ) :
	igdeAction( "Add", NULL, "Add class" ), pPanel( panel ), pComboClass( comboClass ){ }
	
	virtual void OnAction(){
		meWorld * const world = pPanel.GetWorld();
		if( ! world || pComboClass->GetText().IsEmpty() ){
			return;
		}
		
		meWorldGuiParameters &guiparams = world->GetGuiParameters();
		decStringSet set( guiparams.GetAddFilterObjectSet() );
		set.Add( pComboClass->GetText() );
		
		guiparams.SetAddFilterObjectSet( set );
	}
};

class cActionClassRemove : public igdeAction{
	meWPAdd &pPanel;
	igdeListBoxReference &pListBox;
	
public:
	cActionClassRemove( meWPAdd &panel, igdeListBoxReference &listBox ) :
	igdeAction( "Remove", NULL, "Remove selected class" ), pPanel( panel ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		meWorld * const world = pPanel.GetWorld();
		if( ! world || ! pListBox->GetSelectedItem() ){
			return;
		}
		
		meWorldGuiParameters &guiparams = world->GetGuiParameters();
		decStringSet set( guiparams.GetAddFilterObjectSet() );
		set.Remove( pListBox->GetSelectedItem()->GetText() );
		
		guiparams.SetAddFilterObjectSet( set );
	}
};

class cActionClassClear : public igdeAction{
	meWPAdd &pPanel;
	
public:
	cActionClassClear( meWPAdd &panel ) : igdeAction( "Clear", NULL, "Remove all classes" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		meWorld * const world = pPanel.GetWorld();
		if( ! world ){
			return;
		}
		world->GetGuiParameters().SetAddFilterObjectSet( decStringSet() );
	}
};


class cActionFilterObjects : public igdeAction{
	meWPAdd &pPanel;
	
public:
	cActionFilterObjects( meWPAdd &panel ) : igdeAction( "Enable Object Filter",
		NULL, "Determines if objects are filtered" ), pPanel( panel ){ }
	
	virtual void OnAction(){
	}
};

class cListObjectClasses : public igdeListBoxListener{
	meWPAdd &pPanel;
	
public:
	cListObjectClasses( meWPAdd &panel ) : pPanel( panel ){ }
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( menu, pPanel.GetActionClassAdd() );
		helper.MenuCommand( menu, pPanel.GetActionClassRemove() );
		helper.MenuCommand( menu, pPanel.GetActionClassClear() );
	}
};

class cActionObjInclusive : public igdeAction{
	meWPAdd &pPanel;
	
public:
	cActionObjInclusive( meWPAdd &panel ) : igdeAction( "Accept if in list", NULL,
		"Determines if objects are accepted or rejected if they are in the list." ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		meWorld * const world = pPanel.GetWorld();
		if( ! world ){
			return;
		}
		world->GetGuiParameters().SetAddFilterObjectInclusive(
			! world->GetGuiParameters().GetAddFilterObjectInclusive() );
	}
};

}



// Class meWPAdd
//////////////////

// Constructor, destructor
////////////////////////////

meWPAdd::meWPAdd( meWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pWorld( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox, formLine;
	
	pListener = new meWPAddListener( *this );
	
	pActionClassAdd.TakeOver( new cActionClassAdd( *this, pComboObjClass ) );
	pActionClassRemove.TakeOver( new cActionClassRemove( *this, pListObjClasses ) );
	pActionClassClear.TakeOver( new cActionClassClear( *this ) );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	// object filter
	helper.GroupBoxFlow( content, groupBox, "Object Filter:" );
	
	helper.CheckBoxOnly( groupBox, pChkFilterObjects, new cActionFilterObjects( *this ), true );
	
	formLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	groupBox->AddChild( formLine );
	helper.ComboBoxFilter( formLine, "Object filter", pComboObjClass, NULL );
	pComboObjClass->SetDefaultSorter();
	helper.Button( formLine, pActionClassAdd );
	
	helper.ListBox( groupBox, 5, "List of object class filters", pListObjClasses,
		new cListObjectClasses( *this ) );
	pListObjClasses->SetDefaultSorter();
	
	helper.CheckBoxOnly( groupBox, pChkObjInclusive, new cActionObjInclusive( *this ), true );
}

meWPAdd::~meWPAdd(){
	SetWorld( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void meWPAdd::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	if( pWorld ){
		pWorld->RemoveNotifier( pListener );
		pWorld->FreeReference();
	}
	
	pWorld = world;
	
	if( world ){
		world->AddNotifier( pListener );
		world->AddReference();
	}
	
	UpdateObjectFilter();
}



void meWPAdd::UpdateObjectFilter(){
	if( ! pWorld ){
		return;
	}
	
	const meWorldGuiParameters &guiparams = pWorld->GetGuiParameters();
	
	pChkFilterObjects->SetChecked( true );
	
	const decString selection( pListObjClasses->GetSelectedItem()
		? pListObjClasses->GetSelectedItem()->GetText() : "" );
	const decStringSet &set = guiparams.GetAddFilterObjectSet();
	const int count = set.GetCount();
	int i;
	
	pListObjClasses->RemoveAllItems();
	for( i=0; i<count; i++ ){
		pListObjClasses->AddItem( set.GetAt( i ) );
	}
	pListObjClasses->SortItems();
	
	if( ! selection.IsEmpty() ){
		pListObjClasses->SetSelection( pListObjClasses->IndexOfItem( selection ) );
	}
	
	pChkObjInclusive->SetChecked( guiparams.GetAddFilterObjectInclusive() );
}

void meWPAdd::OnGameDefinitionChanged(){
	const decString selection( pComboObjClass->GetText() );
	
	pComboObjClass->RemoveAllItems();
	
	if( pWorld ){
		const igdeGDClassManager &classes = *GetGameDefinition()->GetClassManager();
		const int count = classes.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			const igdeGDClass &objectClass = *classes.GetAt( i );
			if( objectClass.GetCanInstanciate() ){
				pComboObjClass->AddItem( objectClass.GetName() );
			}
		}
		
		pComboObjClass->SortItems();
		pComboObjClass->StoreFilterItems();
	}
	
	pComboObjClass->SetText( selection );
}
