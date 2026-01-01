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
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboOperator : public igdeComboBoxListener{
protected:
	meWVNodeVectorMath &pNode;
	
public:
	typedef deTObjectReference<cComboOperator> Ref;
	cComboOperator(meWVNodeVectorMath &node) : pNode(node){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		if(!pNode.GetRuleVectorMath()){
			return;
		}
		
		const meHTVRuleVectorMath::eOperators op = (meHTVRuleVectorMath::eOperators)(intptr_t)
			comboBox->GetSelectedItem()->GetData();
		if(op == pNode.GetRuleVectorMath()->GetOperator()){
			return;
		}
		
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add(
			meUHTVRuleVecMathSetOp::Ref::New(pNode.GetWindowVegetation().GetVLayer(),
				pNode.GetRuleVectorMath(), op));
	}
};

class cTextVectorA : public igdeEditVectorListener{
protected:
	meWVNodeVectorMath &pNode;
	
public:
	typedef deTObjectReference<cTextVectorA> Ref;
	cTextVectorA(meWVNodeVectorMath &node) : pNode(node){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		const decVector &value = editVector->GetVector();
		if(value.IsEqualTo(pNode.GetRuleVectorMath()->GetVectorA())){
			return;
		}
		
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add(
			meUHTVRuleVecMathSetVectorA::Ref::New(pNode.GetWindowVegetation().GetVLayer(),
				pNode.GetRuleVectorMath(), value));
	}
};

class cTextVectorB : public igdeEditVectorListener{
protected:
	meWVNodeVectorMath &pNode;
	
public:
	typedef deTObjectReference<cTextVectorB> Ref;
	cTextVectorB(meWVNodeVectorMath &node) : pNode(node){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		const decVector &value = editVector->GetVector();
		if(value.IsEqualTo(pNode.GetRuleVectorMath()->GetVectorB())){
			return;
		}
		
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add(
			meUHTVRuleVecMathSetVectorB::Ref::New(pNode.GetWindowVegetation().GetVLayer(),
				pNode.GetRuleVectorMath(), value));
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeVectorMath::meWVNodeVectorMath(meWindowVegetation &windowVegetation, meHTVRuleVectorMath *rule) :
meWVNode(windowVegetation, rule),
pRuleVectorMath(nullptr)
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	SetTitle("Vector-Math");
	
	// slots
	AddSlot(meWVNodeSlot::Ref::New(env,
		"Value", "Value result of operation",
		false, *this, meWVNodeSlot::estValue, meHTVRuleVectorMath::eosValue));
	
	AddSlot(meWVNodeSlot::Ref::New(env,
		"Vector", "Vector result of operation",
		false, *this, meWVNodeSlot::estVector, meHTVRuleVectorMath::eosVector));
	
	meWVNodeSlot::Ref slot(meWVNodeSlot::Ref::New(env,
		"Vector A", "First operand",
		true, *this, meWVNodeSlot::estVector, meHTVRuleVectorMath::eisVectorA));
	helper.EditVector(slot, "First operant if slot is not connected.",
		pEditVectorA, cTextVectorA::Ref::New(*this));
	AddSlot(slot);
	
	slot = meWVNodeSlot::Ref::New(env, "Vector B", "Second operand if required",
		true, *this, meWVNodeSlot::estVector, meHTVRuleVectorMath::eisVectorB);
	helper.EditVector(slot, "Second operant if slot is not connected.",
		pEditVectorB, cTextVectorB::Ref::New(*this));
	AddSlot(slot);
	
	// parameters
	pFraParameters = igdeContainerForm::Ref::New(env);
	AddChild(pFraParameters);
	
	helper.ComboBox(pFraParameters, "Operator:", "Operator to use.", pCBOperator, cComboOperator::Ref::New(*this));
	pCBOperator->AddItem("Add", nullptr, (void*)(intptr_t)meHTVRuleVectorMath::eopAdd);
	pCBOperator->AddItem("Subtract", nullptr, (void*)(intptr_t)meHTVRuleVectorMath::eopSubtract);
	pCBOperator->AddItem("Average", nullptr, (void*)(intptr_t)meHTVRuleVectorMath::eopAverage);
	pCBOperator->AddItem("Normalize", nullptr, (void*)(intptr_t)meHTVRuleVectorMath::eopNormalize);
	pCBOperator->AddItem("Dot", nullptr, (void*)(intptr_t)meHTVRuleVectorMath::eopDot);
	pCBOperator->AddItem("Cross", nullptr, (void*)(intptr_t)meHTVRuleVectorMath::eopCross);
	
	pRuleVectorMath = rule; // required for combo box listener to not fire while list is build
}

meWVNodeVectorMath::~meWVNodeVectorMath(){
}



// Management
///////////////

void meWVNodeVectorMath::Update(){
	meWVNode::Update();
	
	pCBOperator->SetSelectionWithData((void*)(intptr_t)pRuleVectorMath->GetOperator());
	pEditVectorA->SetVector(pRuleVectorMath->GetVectorA());
	pEditVectorB->SetVector(pRuleVectorMath->GetVectorB());
}
