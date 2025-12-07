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
#include "meWVNodeCombine.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/combine/meUHTVRuleCombineSetX.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/combine/meUHTVRuleCombineSetY.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/combine/meUHTVRuleCombineSetZ.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleCombine.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextX : public igdeTextFieldListener{
protected:
	meWVNodeCombine &pNode;
	
public:
	cTextX(meWVNodeCombine &node) : pNode(node){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		const float value = textField->GetFloat();
		if(fabsf(value - pNode.GetRuleCombine()->GetX()) <= FLOAT_SAFE_EPSILON){
			return;
		}
		
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add(
			meUHTVRuleCombineSetX::Ref::NewWith(pNode.GetWindowVegetation().GetVLayer(),
				pNode.GetRuleCombine(), value));
	}
};

class cTextY : public igdeTextFieldListener{
protected:
	meWVNodeCombine &pNode;
	
public:
	cTextY(meWVNodeCombine &node) : pNode(node){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		const float value = textField->GetFloat();
		if(fabsf(value - pNode.GetRuleCombine()->GetY()) <= FLOAT_SAFE_EPSILON){
			return;
		}
		
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add(
			meUHTVRuleCombineSetY::Ref::NewWith(pNode.GetWindowVegetation().GetVLayer(),
				pNode.GetRuleCombine(), value));
	}
};

class cTextZ : public igdeTextFieldListener{
protected:
	meWVNodeCombine &pNode;
	
public:
	cTextZ(meWVNodeCombine &node) : pNode(node){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		const float value = textField->GetFloat();
		if(fabsf(value - pNode.GetRuleCombine()->GetZ()) <= FLOAT_SAFE_EPSILON){
			return;
		}
		
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add(
			meUHTVRuleCombineSetZ::Ref::NewWith(pNode.GetWindowVegetation().GetVLayer(),
				pNode.GetRuleCombine(), value));
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeCombine::meWVNodeCombine(meWindowVegetation &windowVegetation, meHTVRuleCombine *rule) :
meWVNode(windowVegetation, rule),
pRuleCombine(rule)
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	SetTitle("Combine");
	
	// slots
	AddSlot(meWVNodeSlot::Ref::NewWith(env,
		"Vector", "Vector composed of the input values",
		false, *this, meWVNodeSlot::estVector, meHTVRuleCombine::eosVector));
	
	meWVNodeSlot::Ref slot(meWVNodeSlot::Ref::NewWith(env,
		"X", "X component of vector",
		true, *this, meWVNodeSlot::estValue, meHTVRuleCombine::eisX));
	helper.EditFloat(slot, "X component of vector if slot is not connected.",
		pEditX, new cTextX(*this));
	AddSlot(slot);
	
	slot.TakeOverWith(env, "Y", "Y component of vector",
		true, *this, meWVNodeSlot::estValue, meHTVRuleCombine::eisY);
	helper.EditFloat(slot, "Y component of vector if slot is not connected.",
		pEditY, new cTextY(*this));
	AddSlot(slot);
	
	slot.TakeOverWith(env, "Z", "Z component of vector",
		true, *this, meWVNodeSlot::estValue, meHTVRuleCombine::eisZ);
	helper.EditFloat(slot, "Z component of vector if slot is not connected.",
		pEditZ, new cTextZ(*this));
	AddSlot(slot);
	
	// parameters
	pFraParameters.TakeOver(new igdeContainerForm(env));
	AddChild(pFraParameters);
}

meWVNodeCombine::~meWVNodeCombine(){
}



// Management
///////////////

void meWVNodeCombine::Update(){
	meWVNode::Update();
	
	pEditX->SetFloat(pRuleCombine->GetX());
	pEditY->SetFloat(pRuleCombine->GetY());
	pEditZ->SetFloat(pRuleCombine->GetZ());
}
