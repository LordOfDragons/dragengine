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
#include <deigde/gui/nodeview/igdeNVSlotReference.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
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
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleResultSetProb( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleResult(), value ) );
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
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleResultSetVar( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleResult(), value ) );
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
	igdeContainerReference formLine;
	
	SetTitle( "Result" );
	
	// slots
	igdeNVSlotReference slot;
	slot.TakeOver( new meWVNodeSlot( env, "Probability", "Probability in the range from 0 to 1",
		true, *this, meWVNodeSlot::estValue, meHTVRuleResult::eisProbability ) );
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
