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

#include "meWVNodeSlot.h"
#include "meWVNodeClosestProp.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/cprop/meUHTVRuleCPSetClass.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/cprop/meUHTVRuleCPSetRadius.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleClosestProp.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/browse/igdeDialogBrowserObjectClass.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/gui/nodeview/igdeNVSlotReference.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboClass : public igdeComboBoxListener{
protected:
	meWVNodeClosestProp &pNode;
	
public:
	cComboClass( meWVNodeClosestProp &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		if( comboBox->GetText() == pNode.GetRuleClosestProp()->GetPropClass() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleCPSetClass( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleClosestProp(), comboBox->GetText() ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cActionClassSelect : public igdeAction{
	meWVNodeClosestProp &pNode;
public:
	cActionClassSelect( meWVNodeClosestProp &node ) :
		igdeAction( "", NULL, "Select Object Class" ), pNode( node ){ }
	
	virtual void OnAction(){
		if( ! pNode.GetGameDefinition() ){
			return;
		}
		
		decString propClass( pNode.GetRuleClosestProp()->GetPropClass() );
		if( igdeDialogBrowserObjectClass::SelectObjectClass( &pNode, propClass ) ){
			igdeUndoReference undo;
			undo.TakeOver( new meUHTVRuleCPSetClass( pNode.GetWindowVegetation().GetVLayer(),
				pNode.GetRuleClosestProp(), propClass ) );
			pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
		}
	}
};

class cActionMenuClass : public igdeActionContextMenu{
	meWVNodeClosestProp &pNode;
public:
	cActionMenuClass( meWVNodeClosestProp &node ) : igdeActionContextMenu( "",
		node.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), "Class menu" ),
		pNode( node ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		// TODO
	}
};

class cTextSearchRadius : public igdeTextFieldListener{
protected:
	meWVNodeClosestProp &pNode;
	
public:
	cTextSearchRadius( meWVNodeClosestProp &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		if( fabsf( value - pNode.GetRuleClosestProp()->GetSearchRadius() ) <= FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleCPSetRadius( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleClosestProp(), value ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeClosestProp::meWVNodeClosestProp( meWindowVegetation &windowVegetation, meHTVRuleClosestProp *rule ) :
meWVNode( windowVegetation, rule ),
pRuleCP( rule )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference formLine;
	
	SetTitle( "Closest Prop" );
	
	pActionMenuClass.TakeOver( new cActionMenuClass( *this ) );
	
	// slots
	igdeNVSlotReference slot;
	slot.TakeOver( new meWVNodeSlot( env, "Distance", "Distance in meters from closest prop",
		false, *this, meWVNodeSlot::estValue, meHTVRuleClosestProp::eosDistance ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Direction", "Direction (normalized vector) towards closest prop",
		false, *this, meWVNodeSlot::estVector, meHTVRuleClosestProp::eosDirection ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
	
	helper.FormLineStretchFirst( pFraParameters, "Class:", "Select class name of prop to search for.", formLine );
	helper.ComboBoxFilter( formLine, true, "Select class name of prop to search for.",
		pCBPropClass, new cComboClass( *this ) );
	helper.Button( formLine, pBtnPropClass, pActionMenuClass );
	pActionMenuClass->SetWidget( pBtnPropClass );
	
	helper.EditFloat( pFraParameters, "Radius:", "Set search radius in meters.",
		pEditSearchRadius, new cTextSearchRadius( *this ) );
}

meWVNodeClosestProp::~meWVNodeClosestProp(){
}



// Management
///////////////

void meWVNodeClosestProp::Update(){
	meWVNode::Update();
	
	pCBPropClass->SetText( pRuleCP->GetPropClass() );
	pEditSearchRadius->SetFloat( pRuleCP->GetSearchRadius() );
}

void meWVNodeClosestProp::UpdateClassLists(){
	const decString selection( pCBPropClass->GetText() );
	
	pCBPropClass->RemoveAllItems();
	
	const igdeGDClassManager &classes = *GetWindowVegetation().GetWorld()->GetGameDefinition()->GetClassManager();
	const int count = classes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDClass &objectClass = *classes.GetAt( i );
		if( objectClass.GetCanInstanciate() ){
			pCBPropClass->AddItem( objectClass.GetName() );
		}
	}
	
	pCBPropClass->SortItems();
	pCBPropClass->StoreFilterItems();
	
	pCBPropClass->SetText( selection );
}
