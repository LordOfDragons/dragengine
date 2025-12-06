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
#include "meWVNodeResult.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/result/meUHTVRuleResultSetProb.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/result/meUHTVRuleResultSetVar.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleResult.h"

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

class cTextProbability : public igdeTextFieldListener{
protected:
	meWVNodeResult &pNode;
	
public:
	cTextProbability( meWVNodeResult &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		if( fabsf( value - pNode.GetRuleResult()->GetProbability() ) <= FLOAT_SAFE_EPSILON ){
			return;
		}
		
		meUHTVRuleResultSetProb::Ref undo(meUHTVRuleResultSetProb::Ref::NewWith(
			pNode.GetWindowVegetation().GetVLayer(), pNode.GetRuleResult(), value));
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cTextVariation : public igdeTextFieldListener{
protected:
	meWVNodeResult &pNode;
	
public:
	cTextVariation( meWVNodeResult &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const int value = textField->GetInteger();
		if( value == pNode.GetRuleResult()->GetVariation() ){
			return;
		}
		
		meUHTVRuleResultSetVar::Ref undo(meUHTVRuleResultSetVar::Ref::NewWith(
			pNode.GetWindowVegetation().GetVLayer(), pNode.GetRuleResult(), value));
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeResult::meWVNodeResult( meWindowVegetation &windowVegetation, meHTVRuleResult *rule ) :
meWVNode( windowVegetation, rule ),
pRuleResult( rule )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	SetTitle( "Result" );
	
	// slots
	meWVNodeSlot::Ref slot(meWVNodeSlot::Ref::NewWith(
		env, "Probability", "Probability in the range from 0 to 1", true, *this, meWVNodeSlot::estValue, meHTVRuleResult::eisProbability));
	helper.EditFloat( slot, "Probability if slot is not connected.",
		pEditProbability, new cTextProbability( *this ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Variation", "Variation to use",
		true, *this, meWVNodeSlot::estValue, meHTVRuleResult::eisVariation ) );
	helper.EditInteger( slot, "Variation if slot is not connected.",
		pEditVariation, new cTextVariation( *this ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
}

meWVNodeResult::~meWVNodeResult(){
}



// Management
///////////////

void meWVNodeResult::Update(){
	meWVNode::Update();
	
	pEditProbability->SetFloat( pRuleResult->GetProbability() );
	pEditVariation->SetInteger( pRuleResult->GetVariation() );
}

bool meWVNodeResult::CanDelete() const{
	return false;
}

bool meWVNodeResult::CanDuplicate() const{
	return false;
}
