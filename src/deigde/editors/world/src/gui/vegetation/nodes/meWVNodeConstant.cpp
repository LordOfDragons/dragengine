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
#include "meWVNodeConstant.h"
#include "../meWindowVegetation.h"
#include "../../meWindowMain.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/const/meUHTVRuleConstSetVector.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/rules/meHTVRuleConstant.h"

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
	meWVNodeConstant &pNode;
	
public:
	cEditVector( meWVNodeConstant &node ) : pNode( node ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		if( editVector->GetVector().IsEqualTo( pNode.GetRuleConstant()->GetVector() ) ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUHTVRuleConstSetVector( pNode.GetWindowVegetation().GetVLayer(),
			pNode.GetRuleConstant(), editVector->GetVector() ) );
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add( undo );
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNodeConstant::meWVNodeConstant( meWindowVegetation &windowVegetation, meHTVRuleConstant *rule ) :
meWVNode( windowVegetation, rule ),
pRuleConstant( rule )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference formLine;
	
	SetTitle( "Constant" );
	
	// slots
	igdeNVSlotReference slot;
	slot.TakeOver( new meWVNodeSlot( env, "Vector", "Vector value",
		false, *this, meWVNodeSlot::estVector, meHTVRuleConstant::eosVector ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "X", "X value",
		false, *this, meWVNodeSlot::estValue, meHTVRuleConstant::eosX ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Y", "Y value",
		false, *this, meWVNodeSlot::estValue, meHTVRuleConstant::eosY ) );
	AddSlot( slot );
	
	slot.TakeOver( new meWVNodeSlot( env, "Z", "Z value",
		false, *this, meWVNodeSlot::estValue, meHTVRuleConstant::eosZ ) );
	AddSlot( slot );
	
	// parameters
	pFraParameters.TakeOver( new igdeContainerForm( env ) );
	AddChild( pFraParameters );
	
	helper.EditVector( pFraParameters, "Vector:", "Vector value.", pEditVector, new cEditVector( *this ) );
}

meWVNodeConstant::~meWVNodeConstant(){
}



// Management
///////////////

void meWVNodeConstant::Update(){
	meWVNode::Update();
	
	pEditVector->SetVector( pRuleConstant->GetVector() );
}
