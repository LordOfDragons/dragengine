/* 
 * Drag[en]gine OpenGL Graphic Module
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
