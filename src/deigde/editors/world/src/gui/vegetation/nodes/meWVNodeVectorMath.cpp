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
#include "meWVNodeVectorMath.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/vecmath/meUHTVRuleVecMathSetOp.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/vecmath/meUHTVRuleVecMathSetVectorA.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/vecmath/meUHTVRuleVecMathSetVectorB.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleVectorMath.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
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
	meWVNodeVectorMath &pNode;
	
public:
	cComboOperator( meWVNodeVectorMath &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		if( ! pNode.GetRuleVectorMath() ){
			return;
		}
		
		const meHTVRuleVectorMath::eOperators op = ( meHTVRuleVectorMath::eOperators )( intptr_t )
			comboBox->GetSelectedItem()->GetData();
		if( op == pNode.GetRuleVectorMath()->GetOperator() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleVecMathSetOp( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleVectorMath(), op ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cTextVectorA : public igdeEditVectorListener{
protected:
	meWVNodeVectorMath &pNode;
	
public:
	cTextVectorA( meWVNodeVectorMath &node ) : pNode( node ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		const decVector &value = editVector->GetVector();
		if( value.IsEqualTo( pNode.GetRuleVectorMath()->GetVectorA() ) ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleVecMathSetVectorA( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleVectorMath(), value ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cTextVectorB : public igdeEditVectorListener{
protected:
	meWVNodeVectorMath &pNode;
	
public:
	cTextVectorB( meWVNodeVectorMath &node ) : pNode( node ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		const decVector &value = editVector->GetVector();
		if( value.IsEqualTo( pNode.GetRuleVectorMath()->GetVectorB() ) ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleVecMathSetVectorB( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleVectorMath(), value ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeVectorMath::meWVNodeVectorMath( meWindowVegetation &windowVegetation, meHTVRuleVectorMath *rule ) :
meWVNode( windowVegetation, rule ),
pRuleVectorMath( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference formLine;
	
	SetTitle( "Vector-Math" );
	
	// slots
	igdeNVSlotReference slot;
	slot.TakeOver( new meWVNodeSlot( env, "Value", "Value result of operation",
		false, *this, meWVNodeSlot::estValue, meHTVRuleVectorMath::eosValue ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Vector", "Vector result of operation",
		false, *this, meWVNodeSlot::estVector, meHTVRuleVectorMath::eosVector ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Vector A", "First operand",
		true, *this, meWVNodeSlot::estVector, meHTVRuleVectorMath::eisVectorA ) );
	helper.EditVector( slot, "First operant if slot is not connected.",
		pEditVectorA, new cTextVectorA( *this ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Vector B", "Second operand if required",
		true, *this, meWVNodeSlot::estVector, meHTVRuleVectorMath::eisVectorB ) );
	helper.EditVector( slot, "Second operant if slot is not connected.",
		pEditVectorB, new cTextVectorB( *this ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
	
	helper.ComboBox( pFraParameters, "Operator:", "Operator to use.", pCBOperator, new cComboOperator( *this ) );
	pCBOperator->AddItem( "Add", NULL, ( void* )( intptr_t )meHTVRuleVectorMath::eopAdd );
	pCBOperator->AddItem( "Subtract", NULL, ( void* )( intptr_t )meHTVRuleVectorMath::eopSubtract );
	pCBOperator->AddItem( "Average", NULL, ( void* )( intptr_t )meHTVRuleVectorMath::eopAverage );
	pCBOperator->AddItem( "Normalize", NULL, ( void* )( intptr_t )meHTVRuleVectorMath::eopNormalize );
	pCBOperator->AddItem( "Dot", NULL, ( void* )( intptr_t )meHTVRuleVectorMath::eopDot );
	pCBOperator->AddItem( "Cross", NULL, ( void* )( intptr_t )meHTVRuleVectorMath::eopCross );
	
	pRuleVectorMath = rule; // required for combo box listener to not fire while list is build
}

meWVNodeVectorMath::~meWVNodeVectorMath(){
}



// Management
///////////////

void meWVNodeVectorMath::Update(){
	meWVNode::Update();
	
	pCBOperator->SetSelectionWithData( ( void* )( intptr_t )pRuleVectorMath->GetOperator() );
	pEditVectorA->SetVector( pRuleVectorMath->GetVectorA() );
	pEditVectorB->SetVector( pRuleVectorMath->GetVectorB() );
}
