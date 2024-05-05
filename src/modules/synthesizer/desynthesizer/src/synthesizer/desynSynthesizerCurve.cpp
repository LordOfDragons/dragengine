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

#include "desynSynthesizerCurve.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>



// Class desynSynthesizerCurve
////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerCurve::desynSynthesizerCurve() :
pType( eetConstant ),
pPoints( NULL ),
pSamples( NULL ),
pFactors( NULL ),
pCount( 0 ),
pFirst( 0.0f ),
pLast( 0.0f ),
pStep( 0.0f )
{
}

desynSynthesizerCurve::desynSynthesizerCurve( const decCurveBezier &curve ) :
pType( eetConstant ),
pPoints( NULL ),
pSamples( NULL ),
pFactors( NULL ),
pCount( 0 ),
pFirst( 0.0f ),
pLast( 0.0f ),
pStep( 0.0f )
{
	SetCurve( curve );
}

desynSynthesizerCurve::desynSynthesizerCurve( const decCurveBezier &curve, float ymin, float ymax ) :
pType( eetConstant ),
pPoints( NULL ),
pSamples( NULL ),
pFactors( NULL ),
pCount( 0 ),
pFirst( 0.0f ),
pLast( 0.0f ),
pStep( 0.0f )
{
	SetCurve( curve, ymin, ymax );
}

desynSynthesizerCurve::~desynSynthesizerCurve(){
	pClear();
}



// Management
///////////////

void desynSynthesizerCurve::SetCurve( const decCurveBezier &curve ){
	pClear();
	
	switch( curve.GetInterpolationMode() ){
	case decCurveBezier::eimConstant:
		pSetCurveConstant( curve, 0.0f, 1.0f );
		break;
		
	case decCurveBezier::eimLinear:
		pSetCurveLinear( curve, 0.0f, 1.0f );
		break;
		
	case decCurveBezier::eimBezier:
		pSetCurveSampled( curve, 0.0f, 1.0f );
		break;
	}
}

void desynSynthesizerCurve::SetCurve( const decCurveBezier &curve, float ymin, float ymax ){
	pClear();
	
	const float yrange = ymax - ymin;
	if( yrange <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	const float yscale = 1.0f / yrange;
	const float yoffset = yscale * -ymin;
	
	switch( curve.GetInterpolationMode() ){
	case decCurveBezier::eimConstant:
		pSetCurveConstant( curve, yoffset, yscale );
		break;
		
	case decCurveBezier::eimLinear:
		pSetCurveLinear( curve, yoffset, yscale );
		break;
		
	case decCurveBezier::eimBezier:
		pSetCurveSampled( curve, yoffset, yscale );
		break;
	}
}



float desynSynthesizerCurve::Evaluate( float position ) const{
	switch( pType ){
	case eetConstant:
		return pEvaluateConstant( position );
		
	case eetLinear:
		return pEvaluateLinear( position );
		
	case eetSampled:
	default:
		return pEvaluateSampled( position );
	}
}



// Notifications
//////////////////

void desynSynthesizerCurve::pClear(){
	if( pSamples ){
		delete [] pSamples;
		pSamples = NULL;
	}
	
	if( pPoints ){
		delete [] pPoints;
		pPoints = NULL;
	}
	
	if( pFactors ){
		delete [] pFactors;
		pFactors = NULL;
	}
	
	pCount = 0;
	
	pFirst = 0.0f;
	pLast = 0.0f;
	pStep = 0.0f;
}



void desynSynthesizerCurve::pSetCurveConstant( const decCurveBezier &curve, float yoffset, float yscale ){
	pType = eetConstant;
	
	pCount = curve.GetPointCount();
	if( pCount == 0 ){
		return;
	}
	
	pPoints = new decVector2[ pCount ];
	
	int i;
	for( i=0; i<pCount; i++ ){
		pPoints[ i ] = curve.GetPointAt( i ).GetPoint();
		pPoints[ i ].y = pPoints[ i ].y * yscale + yoffset;
	}
}

void desynSynthesizerCurve::pSetCurveLinear( const decCurveBezier &curve, float yoffset, float yscale ){
	pType = eetLinear;
	
	pCount = curve.GetPointCount();
	if( pCount == 0 ){
		return;
	}
	
	pPoints = new decVector2[ pCount ];
	pFactors = new float[ pCount ];
	pFactors[ pCount - 1 ] = 0.0f;
	
	int i;
	for( i=0; i<pCount; i++ ){
		pPoints[ i ] = curve.GetPointAt( i ).GetPoint();
		pPoints[ i ].y = pPoints[ i ].y * yscale + yoffset;
	}
	
	for( i=1; i<pCount; i++ ){
		const int i0 = i - 1;
		const float diff = pPoints[ i ].x - pPoints[ i0 ].x;
		
		if( diff < FLOAT_SAFE_EPSILON ){
			pFactors[ i0 ] = 0.0f;
			
		}else{
			pFactors[ i0 ] = ( pPoints[ i ].y - pPoints[ i0 ].y ) / diff;
		}
	}
	
	pFirst = pPoints[ 0 ].x;
	pLast = pPoints[ pCount - 1 ].x;
}

void desynSynthesizerCurve::pSetCurveSampled( const decCurveBezier &curve, float yoffset, float yscale ){
	pType = eetSampled;
	
	if( curve.GetPointCount() == 0 ){
		pCount = 0;
		return;
	}
	
	pFirst = curve.GetPointAt( 0 ).GetPoint().x;
	pLast = curve.GetPointAt( curve.GetPointCount() - 1 ).GetPoint().x;
	
	const int sampleCount = 100;
	const float step = ( pLast - pFirst ) / ( float )sampleCount;
	
	if( step < FLOAT_SAFE_EPSILON ){
		pCount = 1;
		pStep = 0.0f;
		
		pSamples = new float[ 1 ];
		pFactors = new float[ 1 ];
		
		pSamples[ 0 ] = curve.GetPointAt( 0 ).GetPoint().y * yscale + yoffset;
		pFactors[ 0 ] = 0.0f;
		
	}else{
		decCurveBezierEvaluator evaluator( curve );
		int i;
		
		pCount = sampleCount + 1;
		pStep = ( float )sampleCount / ( pLast - pFirst );
		
		pSamples = new float[ pCount ];
		pFactors = new float[ pCount ];
		pFactors[ pCount - 1 ] = 0.0f;
		
		for( i=0; i<pCount; i++ ){
			pSamples[ i ] = evaluator.EvaluateAt( pFirst + step * ( float )i ) * yscale + yoffset;
		}
		
		for( i=1; i<pCount; i++ ){
			pFactors[ i - 1 ] = pSamples[ i ] - pSamples[ i - 1 ];
		}
	}
}



float desynSynthesizerCurve::pEvaluateConstant( float position ) const{
	if( pCount == 0 ){
		return 0.0f;
	}
	
	int i;
	for( i=1; i<pCount; i++ ){
		if( position < pPoints[ i ].x ){
			break;
		}
	}
	
	return pPoints[ i - 1 ].y;
}

float desynSynthesizerCurve::pEvaluateLinear( float position ) const{
	if( pCount == 0 ){
		return 0.0f;
		
	}else if( pCount ==  1 || position <= pFirst ){
		return pPoints[ 0 ].y;
		
	}else if( position >= pLast ){
		return pPoints[ pCount - 1 ].y;
		
	}else{
		int i;
		for( i=1; i<pCount; i++ ){
			if( position < pPoints[ i ].x ){
				break;
			}
		}
		
		i--;
		return pPoints[ i ].y + ( position - pPoints[ i ].x ) * pFactors[ i ];
	}
}

float desynSynthesizerCurve::pEvaluateSampled( float position ) const{
	if( pCount == 0 ){
		return 0.0f;
		
	}else if( pCount ==  1 ){
		return pSamples[ 0 ];
		
	}else{
		float fsegment;
		const float blend = modff( ( position - pFirst ) * pStep, &fsegment );
		const int segment = ( int )fsegment;
		
		if( segment < 0 ){
			return pSamples[ 0 ];
			
		}else if( segment >= pCount - 1 ){
			return pSamples[ pCount - 1 ];
			
		}else{
			return pSamples[ segment ] + pFactors[ segment ] * blend;
		}
	}
}
