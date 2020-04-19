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

#include "meHTVRuleMapping.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleMapping
///////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleMapping::meHTVRuleMapping() : meHTVRule( ertMapping, 4 ){
	pValue = 0.5f;
	pLower = 0.0f;
	pUpper = 1.0f;
	pInversed = false;
	
	GetSlotAt( eisLower ).SetIsInput( true );
	GetSlotAt( eisUpper ).SetIsInput( true );
	GetSlotAt( eisValue ).SetIsInput( true );
	
	GetSlotAt( eosValue ).SetIsInput( false );
}

meHTVRuleMapping::meHTVRuleMapping( const meHTVRuleMapping &rule ) :
meHTVRule( rule ),
pValue( rule.pValue ),
pLower( rule.pLower ),
pUpper( rule.pUpper ),
pInversed( rule.pInversed ){
}

meHTVRuleMapping::~meHTVRuleMapping(){
}



// Management
///////////////

void meHTVRuleMapping::SetValue( float value ){
	pValue = value;
}

void meHTVRuleMapping::SetLower( float lower ){
	pLower = lower;
}

void meHTVRuleMapping::SetUpper( float upper ){
	pUpper = upper;
}

void meHTVRuleMapping::SetInversed( bool inversed ){
	pInversed = inversed;
}



float meHTVRuleMapping::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot != eosValue ) DETHROW( deeInvalidParam );
	
	meHTVRSlot &inputValue = GetSlotAt( eisValue );
	meHTVRSlot &inputLower = GetSlotAt( eisLower );
	meHTVRSlot &inputUpper = GetSlotAt( eisUpper );
	
	float value = pValue;
	float lower = pLower;
	float upper = pUpper;
	
	if( inputValue.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputValue.GetLinkAt( 0 );
		
		value = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
	}
	
	if( inputLower.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputLower.GetLinkAt( 0 );
		
		lower = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
	}
	
	if( inputUpper.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputUpper.GetLinkAt( 0 );
		
		upper = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
	}
	
	float diff = upper - lower;
	
	if( diff < 1e-5f ){
		return 0.0f;
		
	}else{
		if( pInversed ){
			if( value <= lower ){
				return 1.0f;
				
			}else if( value >= upper ){
				return 0.0f;
				
			}else{
				return ( upper - value ) / diff;
			}
			
		}else{
			if( value <= lower ){
				return 0.0f;
				
			}else if( value >= upper ){
				return 1.0f;
				
			}else{
				return ( value - lower ) / diff;
			}
		}
	}
}

decVector meHTVRuleMapping::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	float value = GetOutputSlotValueAt( slot, evalEnv );
	
	return decVector( value, value, value );
}

meHTVRule *meHTVRuleMapping::Copy() const{
	return new meHTVRuleMapping( *this );
}
