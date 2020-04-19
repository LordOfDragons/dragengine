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

#include "meHTVRuleVectorMath.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleVectorMath
//////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleVectorMath::meHTVRuleVectorMath() : meHTVRule( ertVectorMath, 4 ){
	pVectorA.Set( 0.5f, 0.5f, 0.5f );
	pVectorB.Set( 0.5f, 0.5f, 0.5f );
	pOperator = eopAdd;
	
	GetSlotAt( eisVectorA ).SetIsInput( true );
	GetSlotAt( eisVectorB ).SetIsInput( true );
	
	GetSlotAt( eosValue ).SetIsInput( false );
	GetSlotAt( eosVector ).SetIsInput( false );
}

meHTVRuleVectorMath::meHTVRuleVectorMath( const meHTVRuleVectorMath &rule ) :
meHTVRule( rule ),
pVectorA( rule.pVectorA ),
pVectorB( rule.pVectorB ),
pOperator( rule.pOperator ){
}

meHTVRuleVectorMath::~meHTVRuleVectorMath(){
}



// Management
///////////////

void meHTVRuleVectorMath::SetVectorA( const decVector &vector ){
	pVectorA = vector;
}

void meHTVRuleVectorMath::SetVectorB( const decVector &vector ){
	pVectorB = vector;
}

void meHTVRuleVectorMath::SetOperator( eOperators oper ){
	pOperator = oper;
}



float meHTVRuleVectorMath::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot < 0 || slot > 1 ) DETHROW( deeInvalidParam );
	
	meHTVRSlot &inputVectorA = GetSlotAt( eisVectorA );
	meHTVRSlot &inputVectorB = GetSlotAt( eisVectorB );
	
	decVector vectorA( pVectorA );
	decVector vectorB( pVectorB );
		
	if( inputVectorA.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputVectorA.GetLinkAt( 0 );
		
		vectorA = link.GetSourceRule()->GetOutputSlotVectorAt( link.GetSourceSlot(), evalEnv );
	}
	
	if( inputVectorB.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputVectorB.GetLinkAt( 0 );
		
		vectorB = link.GetSourceRule()->GetOutputSlotVectorAt( link.GetSourceSlot(), evalEnv );
	}
	
	if( slot == eosVector ){ // invalid case, return 0
		return 0.0f;
		
	}else{ // slot == eosValue
		if( pOperator == eopDot ){
			return vectorA * vectorB;
			
		}else{ // all other operators are invalid, return 0 in that case
			return 0.0f;
		}
	}
}

decVector meHTVRuleVectorMath::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot < 0 || slot > 1 ) DETHROW( deeInvalidParam );
	
	meHTVRSlot &inputVectorA = GetSlotAt( eisVectorA );
	meHTVRSlot &inputVectorB = GetSlotAt( eisVectorB );
	
	decVector vectorA( pVectorA );
	decVector vectorB( pVectorB );
	
	if( inputVectorA.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputVectorA.GetLinkAt( 0 );
		
		vectorA = link.GetSourceRule()->GetOutputSlotVectorAt( link.GetSourceSlot(), evalEnv );
	}
	
	if( inputVectorB.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputVectorB.GetLinkAt( 0 );
		
		vectorB = link.GetSourceRule()->GetOutputSlotVectorAt( link.GetSourceSlot(), evalEnv );
	}
	
	if( slot == eosVector ){
		switch( pOperator  ){
		case eopAdd:
			return vectorA + vectorB;
			
		case eopSubtract:
			return vectorA - vectorB;
			
		case eopAverage:
			return ( vectorA + vectorB ) * 0.5f;
			
		case eopNormalize:{
			const float len = vectorA.Length();
			
			if( len == 0.0f ){
				return decVector( 0.0f, 0.0f, 0.0f );
				
			}else{
				return vectorA /= len;
			}
			}
			
		case eopDot:{
			const float dot = vectorA * vectorB;
			return decVector( dot, dot, dot );
			}
		
		case eopCross:
			return vectorA % vectorB;
			
		default:
			return decVector();
		}
		
	}else{ // slot == eosValue
		float result = 0.0f;
		
		if( pOperator == eopDot ){
			result = vectorA * vectorB;
		} // all other operators are invalid, return 0 in that case
		
		return decVector( result, result, result );
	}
}

meHTVRule *meHTVRuleVectorMath::Copy() const{
	return new meHTVRuleVectorMath( *this );
}
