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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRSky.h"
#include "deoglRSkyLink.h"
#include "deoglRSkyInstance.h"

#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSkyLink.h>



// Class deoglRSkyLink
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRSkyLink::deoglRSkyLink( const deSkyLink &link ) :
pController( link.GetController() ),
pRepeat( link.GetRepeat() ),
pSamples( NULL ),
pSampleCount( 0 ),
pUpperLimit( 0.0f ),
pDisabled( false )
{
	pUpdateSamples( link );
}

deoglRSkyLink::~deoglRSkyLink(){
	if( pSamples ){
		delete [] pSamples;
	}
}



// Management
///////////////

float deoglRSkyLink::GetValue( const deoglRSkyInstance &instance ) const{
	if( pDisabled ){
		return 1.0f; // default
	}
	
	float value = instance.GetControllerStateAt( pController );
	
	if( pRepeat > 1 ){
		value *= ( float )pRepeat;
		value -= floorf( value );
	}
	
	value *= pUpperLimit;
	
	if( value <= 0.0f ){
		return pSamples[ 0 ];
		
	}else if( value >= pUpperLimit ){
		return pSamples[ pSampleCount - 1 ];
		
	}else{
		float integral;
		const float fractional = modff( value, &integral );
		const int base = ( int )integral;
		const float value1 = pSamples[ base ];
		const float value2 = pSamples[ base + 1 ];
		
		return value1 * ( 1.0f - fractional ) + value2 * fractional;
	}
}



// Private Functions
//////////////////////

void deoglRSkyLink::pUpdateSamples( const deSkyLink &link ){
	if( pController == -1 ){
		pDisabled = true;
		return;
	}
	
	const int pointCount = link.GetCurve().GetPointCount();
	if( pointCount == 0 ){
		pDisabled = true;
		return;
	}
	
	// determine number of samples to use. right now we use 256 samples
	pSampleCount = 256;
	pSamples = new float[ pSampleCount ];
	pUpperLimit = ( float )( pSampleCount - 1 );
	
	// sample curve
	int i;
	
	if( pointCount == 1 ){
		const float value = link.GetCurve().GetPointAt( 0 ).GetPoint().y;
		for( i=0; i<pSampleCount; i++ ){
			pSamples[ i ] = value;
		}
		
	}else{
		decCurveBezierEvaluator evaluator( link.GetCurve() );
		const float factor = 1.0f / pUpperLimit;
		
		for( i=0; i<pSampleCount; i++ ){
			pSamples[ i ] = evaluator.EvaluateAt( ( float )i * factor );
		}
	}
}
