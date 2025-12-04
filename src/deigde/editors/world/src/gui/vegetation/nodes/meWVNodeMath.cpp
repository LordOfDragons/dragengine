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
#include "meWVNodeMath.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/math/meUHTVRuleMathSetOp.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/math/meUHTVRuleMathSetValueA.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/math/meUHTVRuleMathSetValueB.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleMath.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/gui/nodeview/igdeNVSlotReference.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndo::Ref.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboOperator : public igdeComboBoxListener{
protected:
	meWVNodeMath &pNode;
	
public:
	cComboOperator( meWVNodeMath &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		if( ! pNode.GetRuleMath() ){
			return;
		}
		
		const meHTVRuleMath::eOperators op = ( meHTVRuleMath::eOperators )( intptr_t )
			comboBox->GetSelectedItem()->GetData();
		if( op == pNode.GetRuleMath()->GetOperator() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new meUHTVRuleMathSetOp( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleMath(), op ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cTextValueA : public igdeTextFieldListener{
protected:
	meWVNodeMath &pNode;
	
public:
	cTextValueA( meWVNodeMath &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		if( fabsf( value - pNode.GetRuleMath()->GetValueA() ) <= FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new meUHTVRuleMathSetValueA( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleMath(), value ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cTextValueB : public igdeTextFieldListener{
protected:
	meWVNodeMath &pNode;
	
public:
	cTextValueB( meWVNodeMath &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		if( fabsf( value - pNode.GetRuleMath()->GetValueB() ) <= FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new meUHTVRuleMathSetValueB( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleMath(), value ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeMath::meWVNodeMath( meWindowVegetation &windowVegetation, meHTVRuleMath *rule ) :
meWVNode( windowVegetation, rule ),
pRuleMath( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference formLine;
	
	SetTitle( "Math" );
	
	// slots
	igdeNVSlotReference slot;
	slot.TakeOver( new meWVNodeSlot( env, "Result", "Result of the operation",
		false, *this, meWVNodeSlot::estValue, meHTVRuleMath::eosResult ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Value A", "First operand",
		true, *this, meWVNodeSlot::estValue, meHTVRuleMath::eisValueA ) );
	helper.EditFloat( slot, "First operant if slot is not connected.",
		pEditValueA, new cTextValueA( *this ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Value B", "Second operand if required",
		true, *this, meWVNodeSlot::estValue, meHTVRuleMath::eisValueB ) );
	helper.EditFloat( slot, "Second operant if slot is not connected.",
		pEditValueB, new cTextValueB( *this ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
	
	helper.ComboBox( pFraParameters, "Operator:", "Operator to use.", pCBOperator, new cComboOperator( *this ) );
	pCBOperator->AddItem( "Add", NULL, ( void* )( intptr_t )meHTVRuleMath::eopAdd );
	pCBOperator->AddItem( "Subtract", NULL, ( void* )( intptr_t )meHTVRuleMath::eopSubtract );
	pCBOperator->AddItem( "Multiply", NULL, ( void* )( intptr_t )meHTVRuleMath::eopMultiply );
	pCBOperator->AddItem( "Divide", NULL, ( void* )( intptr_t )meHTVRuleMath::eopDivide );
	pCBOperator->AddItem( "Sine", NULL, ( void* )( intptr_t )meHTVRuleMath::eopSine );
	pCBOperator->AddItem( "Cosine", NULL, ( void* )( intptr_t )meHTVRuleMath::eopCosine );
	pCBOperator->AddItem( "Tangent", NULL, ( void* )( intptr_t )meHTVRuleMath::eopTangent );
	pCBOperator->AddItem( "ArcSine", NULL, ( void* )( intptr_t )meHTVRuleMath::eopArcSine );
	pCBOperator->AddItem( "ArcCosine", NULL, ( void* )( intptr_t )meHTVRuleMath::eopArcCosine );
	pCBOperator->AddItem( "ArcTangent", NULL, ( void* )( intptr_t )meHTVRuleMath::eopArcTangent );
	pCBOperator->AddItem( "Power", NULL, ( void* )( intptr_t )meHTVRuleMath::eopPower );
	pCBOperator->AddItem( "Exponential", NULL, ( void* )( intptr_t )meHTVRuleMath::eopExponential );
	pCBOperator->AddItem( "Logarithm", NULL, ( void* )( intptr_t )meHTVRuleMath::eopLogarithm );
	pCBOperator->AddItem( "Minimum", NULL, ( void* )( intptr_t )meHTVRuleMath::eopMinimum );
	pCBOperator->AddItem( "Maximum", NULL, ( void* )( intptr_t )meHTVRuleMath::eopMaximum );
	pCBOperator->AddItem( "Round", NULL, ( void* )( intptr_t )meHTVRuleMath::eopRound );
	pCBOperator->AddItem( "LessThan", NULL, ( void* )( intptr_t )meHTVRuleMath::eopLessThan );
	pCBOperator->AddItem( "GreaterThan", NULL, ( void* )( intptr_t )meHTVRuleMath::eopGreaterThan );
	pCBOperator->AddItem( "Equal", NULL, ( void* )( intptr_t )meHTVRuleMath::eopEqual );
	pCBOperator->AddItem( "NotEqual", NULL, ( void* )( intptr_t )meHTVRuleMath::eopNotEqual );
	pCBOperator->AddItem( "Average", NULL, ( void* )( intptr_t )meHTVRuleMath::eopAverage );
	
	pRuleMath = rule; // required for combo box listener to not fire while list is build
}

meWVNodeMath::~meWVNodeMath(){
}



// Management
///////////////

void meWVNodeMath::Update(){
	meWVNode::Update();
	
	pCBOperator->SetSelectionWithData( ( void* )( intptr_t )pRuleMath->GetOperator() );
	pEditValueA->SetFloat( pRuleMath->GetValueA() );
	pEditValueB->SetFloat( pRuleMath->GetValueB() );
}
