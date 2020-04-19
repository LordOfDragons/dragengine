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

#include "meHTVRuleCombine.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleCombine
///////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleCombine::meHTVRuleCombine() : meHTVRule( ertCombine, 4 ){
	pX = 0.0f;
	pY = 0.0f;
	pZ = 0.0f;
	
	GetSlotAt( eisX ).SetIsInput( true );
	GetSlotAt( eisY ).SetIsInput( true );
	GetSlotAt( eisZ ).SetIsInput( true );
	
	GetSlotAt( eosVector ).SetIsInput( false );
}

meHTVRuleCombine::meHTVRuleCombine( const meHTVRuleCombine &rule ) :
meHTVRule( rule ),
pX( rule.pX ),
pY( rule.pY ),
pZ( rule.pZ ){
}

meHTVRuleCombine::~meHTVRuleCombine(){
}



// Management
///////////////

void meHTVRuleCombine::SetX( float x ){
	pX = x;
}

void meHTVRuleCombine::SetY( float y ){
	pY = y;
}

void meHTVRuleCombine::SetZ( float z ){
	pZ = z;
}



float meHTVRuleCombine::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot != eosVector ) DETHROW( deeInvalidParam );
	
	// Incorrect usage, we yield the x component in this case
	meHTVRSlot &inputX = GetSlotAt( eisX );
	
	if( inputX.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputX.GetLinkAt( 0 );
		
		return link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
		
	}else{
		return 0.0f;
	}
}

decVector meHTVRuleCombine::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot != eosVector ) DETHROW( deeInvalidParam );
	
	meHTVRSlot &inputX = GetSlotAt( eisX );
	meHTVRSlot &inputY = GetSlotAt( eisY );
	meHTVRSlot &inputZ = GetSlotAt( eisZ );
	
	decVector vector( pX, pY, pZ );
	
	if( inputX.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputX.GetLinkAt( 0 );
		
		vector.x = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
	}
	
	if( inputY.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputY.GetLinkAt( 0 );
		
		vector.y = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
	}
	
	if( inputZ.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputZ.GetLinkAt( 0 );
		
		vector.z = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
	}
	
	return vector;
}

meHTVRule *meHTVRuleCombine::Copy() const{
	return new meHTVRuleCombine( *this );
}
