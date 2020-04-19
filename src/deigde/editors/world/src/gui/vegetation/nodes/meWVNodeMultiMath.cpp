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
#include "meWVNodeMultiMath.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/multimath/meUHTVRuleMultiMathSetOp.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleMultiMath.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/gui/nodeview/igdeNVSlotReference.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboOperator : public igdeComboBoxListener{
protected:
	meWVNodeMultiMath &pNode;
	
public:
	cComboOperator( meWVNodeMultiMath &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		if( ! pNode.GetRuleMultiMath() ){
			return;
		}
		
		const meHTVRuleMultiMath::eOperators op = ( meHTVRuleMultiMath::eOperators )( intptr_t )
			comboBox->GetSelectedItem()->GetData();
		if( op == pNode.GetRuleMultiMath()->GetOperator() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleMultiMathSetOp( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleMultiMath(), op ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeMultiMath::meWVNodeMultiMath( meWindowVegetation &windowVegetation, meHTVRuleMultiMath *rule ) :
meWVNode( windowVegetation, rule ),
pRuleMultiMath( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference formLine;
	
	SetTitle( "Multi-Math" );
	
	// slots
	igdeNVSlotReference slot;
	slot.TakeOver( new meWVNodeSlot( env, "Result", "Result of the operation",
		false, *this, meWVNodeSlot::estValue, meHTVRuleMultiMath::eosResult ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Values", "Input values",
		true, *this, meWVNodeSlot::estValue, meHTVRuleMultiMath::eisValues ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
	
	helper.ComboBox( pFraParameters, "Operator:", "Operator to use.", pCBOperator, new cComboOperator( *this ) );
	pCBOperator->AddItem( "Add", NULL, ( void* )( intptr_t )meHTVRuleMultiMath::eopAdd );
	pCBOperator->AddItem( "Multiply", NULL, ( void* )( intptr_t )meHTVRuleMultiMath::eopMultiply );
	pCBOperator->AddItem( "Minimum", NULL, ( void* )( intptr_t )meHTVRuleMultiMath::eopMinimum );
	pCBOperator->AddItem( "Maximum", NULL, ( void* )( intptr_t )meHTVRuleMultiMath::eopMaximum );
	pCBOperator->AddItem( "Average", NULL, ( void* )( intptr_t )meHTVRuleMultiMath::eopAverage );
	
	pRuleMultiMath = rule; // required for combo box listener to not fire while list is build
}

meWVNodeMultiMath::~meWVNodeMultiMath(){
}



// Management
///////////////

void meWVNodeMultiMath::Update(){
	pCBOperator->SetSelectionWithData( ( void* )( intptr_t )pRuleMultiMath->GetOperator() );
}
