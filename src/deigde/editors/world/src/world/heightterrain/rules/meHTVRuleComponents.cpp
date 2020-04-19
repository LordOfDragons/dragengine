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

#include "meHTVRuleComponents.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleComponents
//////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleComponents::meHTVRuleComponents() : meHTVRule( ertComponents, 4 ){
	GetSlotAt( eisVector ).SetIsInput( true );
	
	GetSlotAt( eosX ).SetIsInput( false );
	GetSlotAt( eosY ).SetIsInput( false );
	GetSlotAt( eosZ ).SetIsInput( false );
}

meHTVRuleComponents::meHTVRuleComponents( const meHTVRuleComponents &rule ) :
meHTVRule( rule ),
pVector( rule.pVector ){
}

meHTVRuleComponents::~meHTVRuleComponents(){
}



// Management
///////////////

void meHTVRuleComponents::SetVector( const decVector &vector ){
	pVector = vector;
}



float meHTVRuleComponents::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot < 0 || slot > 2 ) DETHROW( deeInvalidParam );
	
	meHTVRSlot &slotResult = GetSlotAt( eisVector );
	
	if( slotResult.GetLinkCount() > 0 ){
		meHTVRLink &link = *slotResult.GetLinkAt( 0 );
		
		decVector vector = link.GetSourceRule()->GetOutputSlotVectorAt( link.GetSourceSlot(), evalEnv );
		
		if( slot == eosX ){
			return vector.x;
			
		}else if( slot == eosY ){
			return vector.y;
			
		}else{ // slot == eosZ
			return vector.z;
		}
		
	}else{
		if( slot == eosX ){
			return pVector.x;
			
		}else if( slot == eosY ){
			return pVector.y;
			
		}else{ // slot == eosZ
			return pVector.z;
		}
	}
}

decVector meHTVRuleComponents::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	float value = GetOutputSlotValueAt( slot, evalEnv );
	
	return decVector( value, value, value );
}

meHTVRule *meHTVRuleComponents::Copy() const{
	return new meHTVRuleComponents( *this );
}
