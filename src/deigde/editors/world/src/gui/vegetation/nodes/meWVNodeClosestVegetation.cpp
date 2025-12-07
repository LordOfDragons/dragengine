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
	cTextVegetationType(meWVNodeClosestVegetation &node) : pNode(node){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		if(textField->GetText() == pNode.GetRuleClosestVegetation()->GetVegetationType()){
			return;
		}
		
// 		igdeUndo::Ref undo;
// 		undo.TakeOver( new meUHTVRuleCPSetRadius( pNode.GetWindowVegetation().GetVLayer(),
// 			pNode.GetRuleClosestProp(), value ) );
// 		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cTextSearchRadius : public igdeTextFieldListener{
protected:
	meWVNodeClosestVegetation &pNode;
	
public:
	cTextSearchRadius(meWVNodeClosestVegetation &node) : pNode(node){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		const float value = textField->GetFloat();
		if(fabsf(value - pNode.GetRuleClosestVegetation()->GetSearchRadius()) <= FLOAT_SAFE_EPSILON){
			return;
		}
		
// 		igdeUndo::Ref undo;
// 		undo.TakeOver( new meUHTVRuleCPSetRadius( pNode.GetWindowVegetation().GetVLayer(),
// 			pNode.GetRuleClosestProp(), value ) );
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
	
	SetTitle("Closest Vegetation");
	
	// slots
	AddSlot(meWVNodeSlot::Ref::NewWith(env,
		"Distance", "Distance in meters from closest vegetation",
		false, *this, meWVNodeSlot::estValue, meHTVRuleClosestVegetation::eosDistance));
	
	AddSlot(meWVNodeSlot::Ref::NewWith(env,
		"Direction", "Direction (normalized vector) towards closest vegetation",
		false, *this, meWVNodeSlot::estVector, meHTVRuleClosestVegetation::eosDirection));
	
	// parameters
	pFraParameters.TakeOver(new igdeContainerForm(env));
	AddChild(pFraParameters);
	
	helper.EditFloat(pFraParameters, "Type:", "Set type of vegetation to search for.",
		pEditVegetationType, new cTextVegetationType(*this));
	helper.EditFloat(pFraParameters, "Radius:", "Set search radius in meters.",
		pEditSearchRadius, new cTextSearchRadius(*this));
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
