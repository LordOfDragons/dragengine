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

#include "meWVNodeSlot.h"
#include "meWVNodePropCount.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/pcount/meUHTVRulePCSetClass.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/pcount/meUHTVRulePCSetRadius.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRulePropCount.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboClass : public igdeComboBoxListener{
protected:
	meWVNodePropCount &pNode;
	
public:
	cComboClass( meWVNodePropCount &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		if( comboBox->GetText() == pNode.GetRulePropCount()->GetPropClass() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new meUHTVRulePCSetClass( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRulePropCount(), comboBox->GetText() ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cActionMenuClass : public igdeActionContextMenu{
protected:
	meWVNodePropCount &pNode;
	
public:
	cActionMenuClass( meWVNodePropCount &node ) : igdeActionContextMenu( "",
		node.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), "Class menu" ),
		pNode( node ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		// TODO
	}
};

class cTextSearchRadius : public igdeTextFieldListener{
protected:
	meWVNodePropCount &pNode;
	
public:
	cTextSearchRadius( meWVNodePropCount &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		if( fabsf( value - pNode.GetRulePropCount()->GetSearchRadius() ) <= FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new meUHTVRulePCSetRadius( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRulePropCount(), value ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodePropCount::meWVNodePropCount( meWindowVegetation &windowVegetation, meHTVRulePropCount *rule ) :
meWVNode( windowVegetation, rule ),
pRulePC( rule )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	SetTitle( "Prop Count" );
	
	pActionMenuClass.TakeOver( new cActionMenuClass( *this ) );
	
	// slots
	igdeNVSlot::Ref slot;
	slot.TakeOver( new meWVNodeSlot( env, "Distance", "Distance in meters from closest prop",
		false, *this, meWVNodeSlot::estValue, meHTVRulePropCount::eosCount ) );
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

meWVNodePropCount::~meWVNodePropCount(){
}



// Management
///////////////

void meWVNodePropCount::Update(){
	meWVNode::Update();
	
	pCBPropClass->SetText( pRulePC->GetPropClass() );
	pEditSearchRadius->SetFloat( pRulePC->GetSearchRadius() );
}

void meWVNodePropCount::UpdateClassLists(){
	const decString selection( pCBPropClass->GetText() );
	
	pCBPropClass->RemoveAllItems();
	
	const igdeGDClassManager &classes = *GetWindowVegetation().GetWorld()->GetGameDefinition()->GetClassManager();
	const int count = classes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDClass &objectClass = *classes.GetAt( i );
		if( objectClass.GetCanInstantiate() ){
			pCBPropClass->AddItem( objectClass.GetName() );
		}
	}
	
	pCBPropClass->SortItems();
	pCBPropClass->StoreFilterItems();
	
	pCBPropClass->SetText( selection );
}
