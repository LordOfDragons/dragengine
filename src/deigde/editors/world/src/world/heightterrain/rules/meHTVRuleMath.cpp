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

#include "meHTVRuleMath.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleMath
////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleMath::meHTVRuleMath() : meHTVRule( ertMath, 3 ){
	pValueA = 0.5f;
	pValueB = 0.5f;
	pOperator = eopAdd;
	
	GetSlotAt( eisValueA ).SetIsInput( true );
	GetSlotAt( eisValueB ).SetIsInput( true );
	
	GetSlotAt( eosResult ).SetIsInput( false );
}

meHTVRuleMath::meHTVRuleMath( const meHTVRuleMath &rule ) :
meHTVRule( rule ),
pValueA( rule.pValueA ),
pValueB( rule.pValueB ),
pOperator( rule.pOperator ){
}

meHTVRuleMath::~meHTVRuleMath(){
}



// Management
///////////////

void meHTVRuleMath::SetValueA( float value ){
	pValueA = value;
}

void meHTVRuleMath::SetValueB( float value ){
	pValueB = value;
}

void meHTVRuleMath::SetOperator( eOperators oper ){
	pOperator = oper;
}



float meHTVRuleMath::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot != eosResult ) DETHROW( deeInvalidParam );
	
	meHTVRSlot &inputValueA = GetSlotAt( eisValueA );
	meHTVRSlot &inputValueB = GetSlotAt( eisValueB );
	
	float valueA = pValueA;
	float valueB = pValueB;
	
	if( inputValueA.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputValueA.GetLinkAt( 0 );
		
		valueA = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
	}
	
	if( inputValueB.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputValueB.GetLinkAt( 0 );
		
		valueB = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
	}
	
	switch( pOperator ){
	case eopAdd:
		return valueA + valueB;
		
	case eopSubtract:
		return valueA - valueB;
		
	case eopMultiply:
		return valueA * valueB;
		
	case eopDivide:
		if( valueB == 0.0f ){
			return 0.0f;
			
		}else{
			return valueA / valueB;
		}
		
	case eopSine:
		return sinf( valueA * DEG2RAD );
		
	case eopCosine:
		return cosf( valueA * DEG2RAD );
		
	case eopTangent:
		return tanf( valueA * DEG2RAD );
		
	case eopArcSine:
		return asinf( valueA ) / DEG2RAD;
		
	case eopArcCosine:
		return acosf( valueA ) / DEG2RAD;
		
	case eopArcTangent:
		return atanf( valueA ) / DEG2RAD;
		
	case eopPower:
		return powf( valueA, valueB );
		
	case eopExponential:
		return expf( valueA );
		
	case eopLogarithm:
		return logf( valueA );
		
	case eopMinimum:
		if( valueA < valueB ){
			return valueA;
			
		}else{
			return valueB;
		}
		
	case eopMaximum:
		if( valueA > valueB ){
			return valueA;
			
		}else{
			return valueB;
		}
		
	case eopRound:
		if( valueB < 1e-5f ){
			return valueA;
			
		}else{
			// dirty and not that precise but should do the trick for what we need it
			float halfStep = valueB * 0.5f;
			return valueA - ( fmodf( valueA + halfStep, valueB ) - halfStep );
		}
		
	case eopLessThan:
		if( valueA <= valueB ){
			return 1.0f;
			
		}else{
			return 0.0f;
		}
		
	case eopGreaterThan:
		if( valueA >= valueB ){
			return 1.0f;
			
		}else{
			return 0.0f;
		}
		
	case eopEqual:
		if( fabsf( valueA - valueB ) < 1e-5f ){
			return 1.0f;
			
		}else{
			return 0.0f;
		}
		
	case eopNotEqual:
		if( fabsf( valueA - valueB ) > 1e-5f ){
			return 1.0f;
			
		}else{
			return 0.0f;
		}
		
	case eopAverage:
		return ( valueA + valueB ) * 0.5f;
		
	default:
		return 0.0f;
	}
}

decVector meHTVRuleMath::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	float value = GetOutputSlotValueAt( slot, evalEnv );
	
	return decVector( value, value, value );
}

meHTVRule *meHTVRuleMath::Copy() const{
	return new meHTVRuleMath( *this );
}
