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
#include "meWVNodeComponents.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/comp/meUHTVRuleCompSetVector.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleComponents.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
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

class cEditVector : public igdeEditVectorListener{
protected:
	meWVNodeComponents &pNode;
	
public:
	cEditVector( meWVNodeComponents &node ) : pNode( node ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		if( editVector->GetVector().IsEqualTo( pNode.GetRuleComponents()->GetVector() ) ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleCompSetVector( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleComponents(), editVector->GetVector() ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeComponents::meWVNodeComponents( meWindowVegetation &windowVegetation, meHTVRuleComponents *rule ) :
meWVNode( windowVegetation, rule ),
pRuleComponents( rule )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference formLine;
	
	SetTitle( "Components" );
	
	// slots
	igdeNVSlotReference slot;
	slot.TakeOver( new meWVNodeSlot( env, "X", "X component of vector",
		false, *this, meWVNodeSlot::estValue, meHTVRuleComponents::eosX ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Y", "Y component of vector",
		false, *this, meWVNodeSlot::estValue, meHTVRuleComponents::eosY ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Z", "Z component of vector",
		false, *this, meWVNodeSlot::estValue, meHTVRuleComponents::eosZ ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Vector", "Vector to decompose",
		true, *this, meWVNodeSlot::estVector, meHTVRuleComponents::eisVector ) );
	helper.EditVector( slot, "Input vector.", pEditVector, new cEditVector( *this ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
}

meWVNodeComponents::~meWVNodeComponents(){
}



// Management
///////////////

void meWVNodeComponents::Update(){
	meWVNode::Update();
	
	pEditVector->SetVector( pRuleComponents->GetVector() );
}
