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
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndo.h>
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
		
		igdeUndo::Ref undo;
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
	igdeContainer::Ref formLine;
	
	SetTitle( "Multi-Math" );
	
	// slots
	igdeNVSlot::Ref slot;
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
