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

#include "decCurveBezier3D.h"
#include "decCurveBezier3DPoint.h"
#include "decCurveBezier3DEvaluator.h"

#include "../exceptions.h"



// Class decCurveBezier3DEvaluator
////////////////////////////////////

// Constructor, destructor
////////////////////////////

decCurveBezier3DEvaluator::decCurveBezier3DEvaluator( const decCurveBezier3D &curve ) :
pCurve( curve ){
}

decCurveBezier3DEvaluator::~decCurveBezier3DEvaluator(){
}



// Management
///////////////

decVector decCurveBezier3DEvaluator::EvaluateAt( int segment, float blend ) const{
	const int pointCount = pCurve.GetPointCount();
	
	if( pointCount == 0 ){
		return decVector();
	}
	
	if( segment < 0 ){
		return pCurve.GetPointAt( 0 ).GetPoint();
		
	}else if( segment >= pointCount - 1 ){
		return pCurve.GetPointAt( segment ).GetPoint();
		
	}else{
		if( pCurve.GetInterpolationMode() == decCurveBezier3D::eimConstant ){
			return pCurve.GetPointAt( segment ).GetPoint();
			
		}else if( pCurve.GetInterpolationMode() == decCurveBezier3D::eimLinear ){
			const decCurveBezier3DPoint &point1 = pCurve.GetPointAt( segment );
			const decCurveBezier3DPoint &point2 = pCurve.GetPointAt( segment + 1 );
			const decVector &p1 = point1.GetPoint();
			const decVector &p2 = point2.GetPoint();
			
			if( blend <= 0.0f ){
				return p1;
				
			}else if( blend >= 1.0f ){
				return p2;
				
			}else{
				return p1 * ( 1.0f - blend ) + p2 * blend;
			}
			
		}else{ // pCurve.GetInterpolationMode() == decCurveBezier3D::eimBezier
			const decCurveBezier3DPoint &point1 = pCurve.GetPointAt( segment );
			const decCurveBezier3DPoint &point2 = pCurve.GetPointAt( segment + 1 );
			const float blend1 = 1.0f - blend;
			
			const decVector &p1 = point1.GetPoint();
			const decVector p2( point1.GetHandle2() );
			const decVector p3( point2.GetHandle1() );
			const decVector &p4 = point2.GetPoint();
			const decVector qp1 = p1 * blend1 + p2 * blend;
			const decVector qp2 = p2 * blend1 + p3 * blend;
			const decVector qp3 = p3 * blend1 + p4 * blend;
			const decVector rp1 = qp1 * blend1 + qp2 * blend;
			const decVector rp2 = qp2 * blend1 + qp3 * blend;
			
			return rp1 * blend1 + rp2 * blend;
		}
	}
}
