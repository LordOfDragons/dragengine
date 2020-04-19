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

#include "meHTVRuleConstant.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleConstant
////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleConstant::meHTVRuleConstant() : meHTVRule( ertConstant, 4 ){
	GetSlotAt( eosVector ).SetIsInput( false );
	GetSlotAt( eosX ).SetIsInput( false );
	GetSlotAt( eosY ).SetIsInput( false );
	GetSlotAt( eosZ ).SetIsInput( false );
}

meHTVRuleConstant::meHTVRuleConstant( const meHTVRuleConstant &rule ) :
meHTVRule( rule ),
pVector( rule.pVector ){
}

meHTVRuleConstant::~meHTVRuleConstant(){
}



// Management
///////////////

void meHTVRuleConstant::SetVector( const decVector &vector ){
	pVector = vector;
}



float meHTVRuleConstant::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot < 0 || slot > 3 ) DETHROW( deeInvalidParam );
	
	if( slot == eosVector ){ // incorrect usage, use x value
		return pVector.x;
		
	}else if( slot == eosX ){
		return pVector.x;
		
	}else if( slot == eosY ){
		return pVector.y;
		
	}else{ // slot == eosZ
		return pVector.z;
	}
}

decVector meHTVRuleConstant::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot < 0 || slot > 3 ) DETHROW( deeInvalidParam );
	
	if( slot == eosVector ){
		return pVector;
		
	}else if( slot == eosX ){
		return decVector( pVector.x, pVector.x, pVector.x );
		
	}else if( slot == eosY ){
		return decVector( pVector.y, pVector.y, pVector.y );
		
	}else{ // slot == eosZ
		return decVector( pVector.z, pVector.z, pVector.z );
	}
}

meHTVRule *meHTVRuleConstant::Copy() const{
	return new meHTVRuleConstant( *this );
}
