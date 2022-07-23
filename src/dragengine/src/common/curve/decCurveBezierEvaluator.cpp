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

#include "decCurveBezier.h"
#include "decCurveBezierPoint.h"
#include "decCurveBezierEvaluator.h"

#include "../exceptions.h"



// Class decCurveBezierEvaluator
//////////////////////////////////

// Constructor, destructor
////////////////////////////

decCurveBezierEvaluator::decCurveBezierEvaluator( const decCurveBezier &curve ) :
pCurve( curve ){
}

decCurveBezierEvaluator::~decCurveBezierEvaluator(){
}



// Management
///////////////

float decCurveBezierEvaluator::EvaluateAt( float x ) const{
	const int pointCount = pCurve.GetPointCount();
	
	if( pointCount > 0 ){
		int segment = pCurve.FindPointPriorTo( x );
		
		if( segment == -1 ){
			return pCurve.GetPointAt( 0 ).GetPoint().y;
			
		}else if( segment == pointCount - 1 ){
			return pCurve.GetPointAt( segment ).GetPoint().y;
			
		}else{
			if( pCurve.GetInterpolationMode() == decCurveBezier::eimConstant ){
				return pCurve.GetPointAt( segment ).GetPoint().y;
				
			}else if( pCurve.GetInterpolationMode() == decCurveBezier::eimLinear ){
				const decCurveBezierPoint &point1 = pCurve.GetPointAt( segment );
				const decCurveBezierPoint &point2 = pCurve.GetPointAt( segment + 1 );
				const decVector2 &p1 = point1.GetPoint();
				const decVector2 &p2 = point2.GetPoint();
				float diffX = p2.x - p1.x;
				
				if( diffX > 0.0f ){
					return p1.y + ( p2.y - p1.y ) * ( x - p1.x ) / diffX;
					
				}else{
					return p1.y;
				}
				
			}else{ // pCurve.GetInterpolationMode() == decCurveBezier::eimBezier
				// get the curve points
				const decCurveBezierPoint &point1 = pCurve.GetPointAt( segment );
				const decCurveBezierPoint &point2 = pCurve.GetPointAt( segment + 1 );
				
				const decVector2 &p1 = point1.GetPoint();
				decVector2 p2( point1.GetHandle2() );
				decVector2 p3( point2.GetHandle1() );
				const decVector2 &p4 = point2.GetPoint();
				
				// clamp the handle x coordinates to the range
				if( p2.x < p1.x ){
					p2.x = p1.x;
				}
				if( p3.x > p4.x ){
					p3.x = p4.x;
				}
				
				// adjust handles so they do not overlap in the x direction
				const float lens = p4.x - p1.x;
				const float len1 = p2.x - p1.x;
				const float len2 = p4.x - p3.x;
				
				if( len1 + len2 > lens ){
					const float blend1 = lens / ( len1 + len2 );
					const float blend2 = 1.0f - blend1;
					
					p2.x = p2.x * blend1 + p1.x * blend2;
					p2.y = p2.y * blend1 + p1.y * blend2;
					
					p3.x = p3.x * blend1 + p4.x * blend2;
					p3.y = p3.y * blend1 + p4.y * blend2;
				}
				
				// determine the t value for the given x coordinate
				const double c1 = ( double )p4.x - 3.0 * ( double )p3.x + 3.0 * ( double )p2.x - ( double )p1.x;
				const double c2 = 3.0 * ( double )p3.x - 6.0 * ( double )p2.x + 3.0 * ( double )p1.x;
				const double c3 = 3.0 * ( double )p2.x - 3.0 * ( double )p1.x;
				const double c4 = ( double )p1.x - ( double )x;
				
				double t = 0.0;
				
				if( c1 == 0.0 ){
					if( c2 == 0.0 ){
						if( c3 != 0.0 ){
							t = -c4 / c3;
						}
						
					}else{
						const double factor = 1.0 / ( 2.0 * c2 );
						double disc = c3 * c3 - 4.0 * c2 * c4;
						
						if( disc >= 0.0 ){
							disc = sqrt( disc );
							
							t = ( -c3 - disc ) * factor;
							if( InvalidValue( t ) ){
								t = ( -c3 + disc ) * factor;
							}
						}
					}
					
				}else{
					const double p = ( 3.0 * c1 * c3 - c2 * c2 ) / ( 3.0 * c1 * c1 );
					const double q = ( 2.0 * c2 * c2 * c2 ) / ( 27.0 * c1 * c1 * c1 ) - ( c2 * c3 ) / ( 3.0 * c1 * c1 ) + c4 / c1;
					const double pdisc = p / 3.0;
					const double qdisc = q / 2.0;
					const double disc = qdisc * qdisc + pdisc * pdisc * pdisc;
					const double step = ( double )PI * 2.0 / 3.0;
					const double backport = c2 / ( 3.0 * c1 );
					
					if( disc > 0.0 ){
						const double discsqrt = sqrt( disc );
						
						t = cbrt( -qdisc + discsqrt ) + cbrt( -qdisc - discsqrt ) - backport;
						
						if( InvalidValue( t ) ){
							t = 0.0f;
						}
						
					}else{
						const double phi = acos( -qdisc / sqrt( -( pdisc * pdisc * pdisc ) ) ) / 3.0;
						const double pdiscsqrt = sqrt( -pdisc );
						
						t = 2.0 * pdiscsqrt * cos( phi ) - backport;
						if( InvalidValue( t ) ){
							t = 2.0 * pdiscsqrt * cos( phi + step ) - backport;
							//printf( "x=%f phi=%f result=%f\n", x, phi * 3.0, result );
							
							if( InvalidValue( t ) ){
								t = 2.0 * pdiscsqrt * cos( phi + step + step ) - backport;
							}
						}
					}
				}
				
				// evaluate the curve using the found t value
				const float blend2 = ( float )t;
				const float blend1 = 1.0f - blend2;
				const float qp1 = p1.y * blend1 + p2.y * blend2;
				const float qp2 = p2.y * blend1 + p3.y * blend2;
				const float qp3 = p3.y * blend1 + p4.y * blend2;
				const float rp1 = qp1 * blend1 + qp2 * blend2;
				const float rp2 = qp2 * blend1 + qp3 * blend2;
				
				return rp1 * blend1 + rp2 * blend2;
			}
		}
	}
	
	return 0.0f;
}
