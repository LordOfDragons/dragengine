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

#include "meHTVRuleCurve.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>



// Class meHTVRuleCurve
/////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleCurve::meHTVRuleCurve() : meHTVRule( ertCurve, 2 ){
	pCurve.SetDefaultLinear();
	
	GetSlotAt( eisValue ).SetIsInput( true );
	
	GetSlotAt( eosValue ).SetIsInput( false );
}

meHTVRuleCurve::meHTVRuleCurve( const meHTVRuleCurve &rule ) :
meHTVRule( rule ),
pCurve( rule.pCurve ){
}

meHTVRuleCurve::~meHTVRuleCurve(){
}



// Management
///////////////

void meHTVRuleCurve::SetCurve( const decCurveBezier &curve ){
	pCurve = curve;
}



float meHTVRuleCurve::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot != eosValue ){
		DETHROW( deeInvalidParam );
	}
	
	const meHTVRSlot &inputValue = GetSlotAt( eisValue );
	float value = 0.0f;
	
	if( inputValue.GetLinkCount() > 0 ){
		meHTVRLink &link = *inputValue.GetLinkAt( 0 );
		value = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
	}
	
	return decCurveBezierEvaluator( pCurve ).EvaluateAt( value );
}

decVector meHTVRuleCurve::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	const float value = GetOutputSlotValueAt( slot, evalEnv );
	return decVector( value, value, value );
}

meHTVRule *meHTVRuleCurve::Copy() const{
	return new meHTVRuleCurve( *this );
}
