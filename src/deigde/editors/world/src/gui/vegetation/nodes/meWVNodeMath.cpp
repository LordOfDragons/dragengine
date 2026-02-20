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
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboOperator : public igdeComboBoxListener{
protected:
	meWVNodeMath &pNode;
	
public:
	using Ref = deTObjectReference<cComboOperator>;
	cComboOperator(meWVNodeMath &node) : pNode(node){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		if(!pNode.GetRuleMath()){
			return;
		}
		
		const meHTVRuleMath::eOperators op = (meHTVRuleMath::eOperators)(intptr_t)
			comboBox->GetSelectedItem()->GetData();
		if(op == pNode.GetRuleMath()->GetOperator()){
			return;
		}
		
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add(
			meUHTVRuleMathSetOp::Ref::New(pNode.GetWindowVegetation().GetVLayer(),
				pNode.GetRuleMath(), op));
	}
};

class cTextValueA : public igdeTextFieldListener{
protected:
	meWVNodeMath &pNode;
	
public:
	using Ref = deTObjectReference<cTextValueA>;
	cTextValueA(meWVNodeMath &node) : pNode(node){}
	
	void OnTextChanged(igdeTextField *textField) override{
		const float value = textField->GetFloat();
		if(fabsf(value - pNode.GetRuleMath()->GetValueA()) <= FLOAT_SAFE_EPSILON){
			return;
		}
		
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add(
			meUHTVRuleMathSetValueA::Ref::New(pNode.GetWindowVegetation().GetVLayer(),
				pNode.GetRuleMath(), value));
	}
};

class cTextValueB : public igdeTextFieldListener{
protected:
	meWVNodeMath &pNode;
	
public:
	using Ref = deTObjectReference<cTextValueB>;
	cTextValueB(meWVNodeMath &node) : pNode(node){}
	
	void OnTextChanged(igdeTextField *textField) override{
		const float value = textField->GetFloat();
		if(fabsf(value - pNode.GetRuleMath()->GetValueB()) <= FLOAT_SAFE_EPSILON){
			return;
		}
		
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add(
			meUHTVRuleMathSetValueB::Ref::New(pNode.GetWindowVegetation().GetVLayer(),
				pNode.GetRuleMath(), value));
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeMath::meWVNodeMath(meWindowVegetation &windowVegetation, meHTVRuleMath *rule) :
meWVNode(windowVegetation, rule),
pRuleMath(nullptr)
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	SetTitle("@World.WVNodeMath.Title");
	
	// slots
	AddSlot(meWVNodeSlot::Ref::New(env,
		"@World.WVNodeMath.Output.Result", "@World.WVNodeMath.Output.Result.ToolTip",
		false, *this, meWVNodeSlot::estValue, meHTVRuleMath::eosResult));
	
	meWVNodeSlot::Ref slot(meWVNodeSlot::Ref::New(env,
		"@World.WVNodeMath.Input.A", "@World.WVNodeMath.Input.A.ToolTip",
		true, *this, meWVNodeSlot::estValue, meHTVRuleMath::eisValueA));
	helper.EditFloat(slot, "@World.WVNodeVectorMath.FirstOperant",
		pEditValueA, cTextValueA::Ref::New(*this));
	AddSlot(slot);
	
	slot = meWVNodeSlot::Ref::New(env, "@World.WVNodeMath.Input.B", "@World.WVNodeMath.Input.B.ToolTip",
		true, *this, meWVNodeSlot::estValue, meHTVRuleMath::eisValueB);
	helper.EditFloat(slot, "@World.WVNodeVectorMath.SecondOperant",
		pEditValueB, cTextValueB::Ref::New(*this));
	AddSlot(slot);
	
	// parameters
	pFraParameters = igdeContainerForm::Ref::New(env);
	AddChild(pFraParameters);
	
	helper.ComboBox(pFraParameters, "@World.WVNodeVectorMath.Operator", "@World.WVNodeVectorMath.Operator.ToolTip", pCBOperator, cComboOperator::Ref::New(*this));
	pCBOperator->SetAutoTranslateItems(true);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Add", nullptr, (void*)(intptr_t)meHTVRuleMath::eopAdd);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Subtract", nullptr, (void*)(intptr_t)meHTVRuleMath::eopSubtract);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Multiply", nullptr, (void*)(intptr_t)meHTVRuleMath::eopMultiply);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Divide", nullptr, (void*)(intptr_t)meHTVRuleMath::eopDivide);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Sine", nullptr, (void*)(intptr_t)meHTVRuleMath::eopSine);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Cosine", nullptr, (void*)(intptr_t)meHTVRuleMath::eopCosine);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Tangent", nullptr, (void*)(intptr_t)meHTVRuleMath::eopTangent);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.ArcSine", nullptr, (void*)(intptr_t)meHTVRuleMath::eopArcSine);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.ArcCosine", nullptr, (void*)(intptr_t)meHTVRuleMath::eopArcCosine);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.ArcTangent", nullptr, (void*)(intptr_t)meHTVRuleMath::eopArcTangent);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Power", nullptr, (void*)(intptr_t)meHTVRuleMath::eopPower);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Exponential", nullptr, (void*)(intptr_t)meHTVRuleMath::eopExponential);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Logarithm", nullptr, (void*)(intptr_t)meHTVRuleMath::eopLogarithm);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Minimum", nullptr, (void*)(intptr_t)meHTVRuleMath::eopMinimum);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Maximum", nullptr, (void*)(intptr_t)meHTVRuleMath::eopMaximum);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Round", nullptr, (void*)(intptr_t)meHTVRuleMath::eopRound);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.LessThan", nullptr, (void*)(intptr_t)meHTVRuleMath::eopLessThan);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.GreaterThan", nullptr, (void*)(intptr_t)meHTVRuleMath::eopGreaterThan);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Equal", nullptr, (void*)(intptr_t)meHTVRuleMath::eopEqual);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.NotEqual", nullptr, (void*)(intptr_t)meHTVRuleMath::eopNotEqual);
	pCBOperator->AddItem("@World.WVNodeMath.Operator.Average", nullptr, (void*)(intptr_t)meHTVRuleMath::eopAverage);
	
	pRuleMath = rule; // required for combo box listener to not fire while list is build
}

meWVNodeMath::~meWVNodeMath(){
}



// Management
///////////////

void meWVNodeMath::Update(){
	meWVNode::Update();
	
	pCBOperator->SetSelectionWithData((void*)(intptr_t)pRuleMath->GetOperator());
	pEditValueA->SetFloat(pRuleMath->GetValueA());
	pEditValueB->SetFloat(pRuleMath->GetValueB());
}
