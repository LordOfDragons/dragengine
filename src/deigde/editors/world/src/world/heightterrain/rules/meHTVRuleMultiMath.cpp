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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meHTVRuleMultiMath.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleMultiMath
/////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleMultiMath::meHTVRuleMultiMath() : meHTVRule( ertMultiMath, 2 ){
	pOperator = eopAdd;
	
	GetSlotAt( eisValues ).SetIsInput( true );
	
	GetSlotAt( eosResult ).SetIsInput( false );
}

meHTVRuleMultiMath::meHTVRuleMultiMath( const meHTVRuleMultiMath &rule ) :
meHTVRule( rule ),
pOperator( rule.pOperator ){
}

meHTVRuleMultiMath::~meHTVRuleMultiMath(){
}



// Management
///////////////

void meHTVRuleMultiMath::SetOperator( eOperators oper ){
	pOperator = oper;
}



float meHTVRuleMultiMath::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot != eosResult ) DETHROW( deeInvalidParam );
	
	meHTVRSlot &inputValues = GetSlotAt( eisValues );
	
	int l, linkCount = inputValues.GetLinkCount();
	float value, result = 0.0f;
	
	switch( pOperator ){
	case eopAdd:
		for( l=0; l<linkCount; l++ ){
			meHTVRLink &link = *inputValues.GetLinkAt( l );
			
			if( l == 0 ){
				result = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
				
			}else{
				result += link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
			}
		}
		
	case eopMultiply:
		for( l=0; l<linkCount; l++ ){
			meHTVRLink &link = *inputValues.GetLinkAt( l );
			
			if( l == 0 ){
				result = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
				
			}else{
				result *= link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
			}
		}
		
	case eopMinimum:
		for( l=0; l<linkCount; l++ ){
			meHTVRLink &link = *inputValues.GetLinkAt( l );
			
			if( l == 0 ){
				result = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
				
			}else{
				value = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
				
				if( value < result ) result = value;
			}
		}
		
	case eopMaximum:
		for( l=0; l<linkCount; l++ ){
			meHTVRLink &link = *inputValues.GetLinkAt( l );
			
			if( l == 0 ){
				result = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
				
			}else{
				value = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
				
				if( value > result ) result = value;
			}
		}
		
	case eopAverage:
		for( l=0; l<linkCount; l++ ){
			meHTVRLink &link = *inputValues.GetLinkAt( l );
			
			if( l == 0 ){
				result = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
				
			}else{
				result += link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
			}
		}
		
		if( linkCount > 0 ){
			result /= ( float )linkCount;
		}
	}
	
	return result;
}

decVector meHTVRuleMultiMath::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	float value = GetOutputSlotValueAt( slot, evalEnv );
	
	return decVector( value, value, value );
}

meHTVRule *meHTVRuleMultiMath::Copy() const{
	return new meHTVRuleMultiMath( *this );
}
