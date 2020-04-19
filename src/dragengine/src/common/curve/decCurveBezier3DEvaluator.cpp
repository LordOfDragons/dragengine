/* 
 * Drag[en]gine Game Engine
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
