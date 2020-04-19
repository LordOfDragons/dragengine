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
#include "meWVNodeClosestVegetation.h"
#include "../../../world/heightterrain/rules/meHTVRuleClosestVegetation.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/nodeview/igdeNVSlot.h>
#include <deigde/gui/nodeview/igdeNVSlotReference.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextVegetationType : public igdeTextFieldListener{
protected:
	meWVNodeClosestVegetation &pNode;
	
public:
	cTextVegetationType( meWVNodeClosestVegetation &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		if( textField->GetText() == pNode.GetRuleClosestVegetation()->GetVegetationType() ){
			return;
		}
		
// 		igdeUndoReference undo;
// 		undo.TakeOver( new meUHTVRuleCPSetRadius( pNode.GetWindowVegetation().GetVLayer(),
// 			pNode.GetRuleClosestProp(), value ) );
// 		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

class cTextSearchRadius : public igdeTextFieldListener{
protected:
	meWVNodeClosestVegetation &pNode;
	
public:
	cTextSearchRadius( meWVNodeClosestVegetation &node ) : pNode( node ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		if( fabsf( value - pNode.GetRuleClosestVegetation()->GetSearchRadius() ) <= FLOAT_SAFE_EPSILON ){
			return;
		}
		
// 		igdeUndoReference undo;
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
	meWindowVegetation &windowVegetation, meHTVRuleClosestVegetation *rule ) :
meWVNode( windowVegetation, rule ),
pRuleCV( rule )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference formLine;
	
	SetTitle( "Closest Vegetation" );
	
	// slots
	igdeNVSlotReference slot;
	slot.TakeOver( new meWVNodeSlot( env, "Distance", "Distance in meters from closest vegetation",
		false, *this, meWVNodeSlot::estValue, meHTVRuleClosestVegetation::eosDistance ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Direction", "Direction (normalized vector) towards closest vegetation",
		false, *this, meWVNodeSlot::estVector, meHTVRuleClosestVegetation::eosDirection ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
	
	helper.EditFloat( pFraParameters, "Type:", "Set type of vegetation to search for.",
		pEditVegetationType, new cTextVegetationType( *this ) );
	helper.EditFloat( pFraParameters, "Radius:", "Set search radius in meters.",
		pEditSearchRadius, new cTextSearchRadius( *this ) );
}

meWVNodeClosestVegetation::~meWVNodeClosestVegetation(){
}



// Management
///////////////

void meWVNodeClosestVegetation::Update(){
	meWVNode::Update();
	
	pEditVegetationType->SetText( pRuleCV->GetVegetationType() );
	pEditSearchRadius->SetFloat( pRuleCV->GetSearchRadius() );
}
