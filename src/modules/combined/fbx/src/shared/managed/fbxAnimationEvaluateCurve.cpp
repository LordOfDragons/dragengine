/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
