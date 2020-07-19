/* 
 * FBX Modules
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
#include <inttypes.h>

#include "fbxAnimation.h"
#include "fbxAnimationCurve.h"
#include "fbxAnimationMove.h"
#include "fbxAnimationEvaluateCurve.h"
#include "../fbxProperty.h"

#include <dragengine/common/exceptions.h>


// Class fbxAnimationEvaluateCurve
////////////////////////////////////

// Constructor, destructor
////////////////////////////

fbxAnimationEvaluateCurve::fbxAnimationEvaluateCurve( const fbxAnimationMove &move,
const fbxAnimationCurve *curve, float defaultValue ) :
pMove( move ),
pPropertyTime( NULL ),
pPropertyValue( NULL ),
pLastKeyIndex( -1 ),
pKeyIndex( 0 ),
pPrevKeyTime( 0.0f ),
pPrevKeyValue( defaultValue ),
pNextKeyTime( 0.0f ),
pNextKeyValue( defaultValue ),
pNextFrame( 0 )
{
	if( ! curve || curve->GetPropertyTime().GetValueCount() == 0 ){
		return;
	}
	
	pPropertyTime = &curve->GetPropertyTime();
	pPropertyValue = &curve->GetPropertyValue();
	pLastKeyIndex = pPropertyTime->GetValueCount() - 1;
	pPrevKeyTime = fbxAnimation::ConvTime( pPropertyTime->GetValueAtAsLong( 0 ) );
	pPrevKeyValue = pPropertyValue->GetValueAtAsFloat( 0 );
	pNextKeyTime = pPrevKeyTime;
	pNextKeyValue = pPrevKeyValue;
}

fbxAnimationEvaluateCurve::~fbxAnimationEvaluateCurve(){
}



// Management
///////////////

float fbxAnimationEvaluateCurve::NextValue(){
	const float time = pMove.FrameToTime( pNextFrame++ );
	
	while( time >= pNextKeyTime && pKeyIndex < pLastKeyIndex ){
		pKeyIndex++;
		pPrevKeyTime = pNextKeyTime;
		pPrevKeyValue = pNextKeyValue;
		pNextKeyTime = fbxAnimation::ConvTime( pPropertyTime->GetValueAtAsLong( pKeyIndex ) );
		pNextKeyValue = pPropertyValue->GetValueAtAsFloat( pKeyIndex );
	}
	
	return decMath::linearStep( time, pPrevKeyTime, pNextKeyTime, pPrevKeyValue, pNextKeyValue );
}
