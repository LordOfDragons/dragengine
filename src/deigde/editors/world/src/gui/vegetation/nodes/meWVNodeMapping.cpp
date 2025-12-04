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
#include "meWVNodeMapping.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/mapping/meUHTVRuleMapSetLower.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/mapping/meUHTVRuleMapSetUpper.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/mapping/meUHTVRuleMapSetValue.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/mapping/meUHTVRuleMapToggleInversed.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleMapping.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextLower : public igdeTextFieldListener{
protected:
	meWVNodeMapping &pNode;
	
public:
	cTextLower( meWVNodeMapping &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		if( fabsf( value - pNode.GetRuleMapping()->GetLower() ) <= FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new meUHTVRuleMapSetLower( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleMapping(), value ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cTextUpper : public igdeTextFieldListener{
protected:
	meWVNodeMapping &pNode;
	
public:
	cTextUpper( meWVNodeMapping &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		if( fabsf( value - pNode.GetRuleMapping()->GetUpper() ) <= FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new meUHTVRuleMapSetUpper( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleMapping(), value ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cTextValue : public igdeTextFieldListener{
protected:
	meWVNodeMapping &pNode;
	
public:
	cTextValue( meWVNodeMapping &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		if( fabsf( value - pNode.GetRuleMapping()->GetValue() ) <= FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new meUHTVRuleMapSetValue( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleMapping(), value ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cActionInversed : public igdeAction{
protected:
	meWVNodeMapping &pNode;
	
public:
	cActionInversed( meWVNodeMapping &node ) :
		igdeAction( "Inversed", NULL, "Invert value" ), pNode( node ){ }
	
	virtual void OnAction(){
		igdeUndo::Ref undo;
		undo.TakeOver( new meUHTVRuleMapToggleInversed(
			pNode.GetWindowVegetation().GetVLayer(), pNode.GetRuleMapping() ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeMapping::meWVNodeMapping( meWindowVegetation &windowVegetation, meHTVRuleMapping *rule ) :
meWVNode( windowVegetation, rule ),
pRuleMapping( rule )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	SetTitle( "Mapping" );
	
	// slots
	igdeNVSlot::Ref slot;
	slot.TakeOver( new meWVNodeSlot( env, "Value", "Mapped value",
		false, *this, meWVNodeSlot::estValue, meHTVRuleMapping::eosValue ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Lower", "Lower value",
		true, *this, meWVNodeSlot::estValue, meHTVRuleMapping::eisLower ) );
	helper.EditFloat( slot, "Lower value if slot is not connected.",
		pEditLower, new cTextLower( *this ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Upper", "Upper value",
		true, *this, meWVNodeSlot::estValue, meHTVRuleMapping::eisUpper ) );
	helper.EditFloat( slot, "Upper value if slot is not connected.",
		pEditUpper, new cTextUpper( *this ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Value", "Value to map",
		true, *this, meWVNodeSlot::estValue, meHTVRuleMapping::eisValue ) );
	helper.EditFloat( slot, "Value to map if slot is not connected.",
		pEditValue, new cTextValue( *this ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
	
	helper.CheckBox( pFraParameters, pChkInversed, new cActionInversed( *this ) );
}

meWVNodeMapping::~meWVNodeMapping(){
}



// Management
///////////////

void meWVNodeMapping::Update(){
	meWVNode::Update();
	
	pEditLower->SetFloat( pRuleMapping->GetLower() );
	pEditUpper->SetFloat( pRuleMapping->GetUpper() );
	pEditValue->SetFloat( pRuleMapping->GetValue() );
	pChkInversed->SetChecked( pRuleMapping->GetInversed() );
}
