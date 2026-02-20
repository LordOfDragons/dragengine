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
#include "meWVNodeClosestVegetation.h"
#include "../../../world/heightterrain/rules/meHTVRuleClosestVegetation.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextVegetationType : public igdeTextFieldListener{
protected:
	meWVNodeClosestVegetation &pNode;
	
public:
	using Ref = deTObjectReference<cTextVegetationType>;
	cTextVegetationType(meWVNodeClosestVegetation &node) : pNode(node){}
	
	void OnTextChanged(igdeTextField *textField) override{
		if(textField->GetText() == pNode.GetRuleClosestVegetation()->GetVegetationType()){
			return;
		}
		
// 		igdeUndo::Ref undo;
// 		undo = meUHTVRuleCPSetRadius::Ref::New(pNode.GetWindowVegetation().GetVLayer(),
// 			pNode.GetRuleClosestProp(), value);
// 		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cTextSearchRadius : public igdeTextFieldListener{
protected:
	meWVNodeClosestVegetation &pNode;
	
public:
	using Ref = deTObjectReference<cTextSearchRadius>;
	cTextSearchRadius(meWVNodeClosestVegetation &node) : pNode(node){}
	
	void OnTextChanged(igdeTextField *textField) override{
		const float value = textField->GetFloat();
		if(fabsf(value - pNode.GetRuleClosestVegetation()->GetSearchRadius()) <= FLOAT_SAFE_EPSILON){
			return;
		}
		
// 		igdeUndo::Ref undo;
// 		undo = meUHTVRuleCPSetRadius::Ref::New(pNode.GetWindowVegetation().GetVLayer(),
// 			pNode.GetRuleClosestProp(), value);
// 		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};
	
}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeClosestVegetation::meWVNodeClosestVegetation(
	meWindowVegetation &windowVegetation, meHTVRuleClosestVegetation *rule) :
meWVNode(windowVegetation, rule),
pRuleCV(rule)
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	SetTitle("@World.WVNodeClosestVegetation.Title");
	
	// slots
	AddSlot(meWVNodeSlot::Ref::New(env,
		"@World.WVNodeClosestVegetation.Output.Distance", "@World.WVNodeClosestVegetation.Output.Distance.ToolTip",
		false, *this, meWVNodeSlot::estValue, meHTVRuleClosestVegetation::eosDistance));
	
	AddSlot(meWVNodeSlot::Ref::New(env,
		"@World.WVNodeClosestVegetation.Output.Direction", "@World.WVNodeClosestVegetation.Output.Direction.ToolTip",
		false, *this, meWVNodeSlot::estVector, meHTVRuleClosestVegetation::eosDirection));
	
	// parameters
	pFraParameters = igdeContainerForm::Ref::New(env);
	AddChild(pFraParameters);
	
	helper.EditFloat(pFraParameters, "@World.WVNodeClosestVegetation.Type", "@World.WVNodeClosestVegetation.Type.ToolTip",
		pEditVegetationType, cTextVegetationType::Ref::New(*this));
	helper.EditFloat(pFraParameters, "@World.WVNodePropCount.Radius", "@World.WVNodePropCount.Radius.ToolTip",
		pEditSearchRadius, cTextSearchRadius::Ref::New(*this));
}

meWVNodeClosestVegetation::~meWVNodeClosestVegetation(){
}



// Management
///////////////

void meWVNodeClosestVegetation::Update(){
	meWVNode::Update();
	
	pEditVegetationType->SetText(pRuleCV->GetVegetationType());
	pEditSearchRadius->SetFloat(pRuleCV->GetSearchRadius());
}
