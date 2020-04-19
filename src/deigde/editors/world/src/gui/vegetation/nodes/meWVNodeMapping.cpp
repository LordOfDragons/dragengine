/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
#include <deigde/gui/nodeview/igdeNVSlotReference.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
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
		
		igdeUndoReference undo;
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
		
		igdeUndoReference undo;
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
		
		igdeUndoReference undo;
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
		igdeUndoReference undo;
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
	igdeContainerReference formLine;
	
	SetTitle( "Mapping" );
	
	// slots
	igdeNVSlotReference slot;
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
