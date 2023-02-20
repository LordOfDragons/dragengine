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
#include <string.h>

#include "deoglCollisionDetection.h"



// Collision Detection Routines
/////////////////////////////////

// GENERAL ROUTINES //
//////////////////////

// Ray Plane Intersection
// ----------------------
// pp = plane position
// pn = plane normal
// rp = ray position
// rd = ray direction
float deoglCollisionDetection::RayPlane(const decVector &pp, const decVector &pn,
const decVector &rp, const decVector &rd){
//	return -((pn * rp - (pn * pp)) / (pn * rd));
//	float numer = pn * rp - ( pn * pp );
	float numer = pn * ( rp - pp );
	float denom = pn * rd;
	return denom == 0.0f ? -1.0f : -(numer / denom);
}



decVector deoglCollisionDetection::ClosestPointOnLine( const decVector &lineFrom, const decVector &lineTo, const decVector &point ){
	const decVector lineDir = lineTo - lineFrom;
	const decVector offset = point - lineFrom;
	const float lineLenSquared = lineDir.LengthSquared();
	
	if( fabs( lineLenSquared ) > FLOAT_SAFE_EPSILON ){
		// lambda = ( lineDir.Normalized() * offset ) / lineDir.Length()
		const float lambda = ( lineDir * offset ) / lineLenSquared;
		
		if( lambda <= 0.0f ){
			return lineFrom;
		}
		if( lambda >= 1.0f ){
			return lineTo;
		}
		
		return lineFrom + lineDir * lambda;
		
	}else{
		return lineFrom;
	}
}

decVector deoglCollisionDetection::ClosestPointOnRay( const decVector &rayOrigin, const decVector &rayDirection, const decVector &point ){
	// rayOrigin + rayDirection * distanceOnRay
	return rayOrigin + rayDirection * ( rayDirection * ( point - rayOrigin ) );
}



// Closest Point On Triangle Edge
// ------------------------------
// tp1 = first triangle point
// tp2 = second triangle point
// tp3 = third triangle point
// p = point to check
decVector deoglCollisionDetection::ClosestPointOnTriangleEdge(const decVector &tp1,
const decVector &tp2, const decVector &tp3, const decVector &p){
	decVector cp12 = ClosestPointOnLine(tp1, tp2, p);
	decVector cp23 = ClosestPointOnLine(tp2, tp3, p);
	decVector cp31 = ClosestPointOnLine(tp3, tp1, p);
	float l12 = (p - cp12).Length();
	float l23 = (p - cp23).Length();
	float l31 = (p - cp31).Length();
	float minVal = l12;
	decVector result = cp12;
	if(l23 < minVal){
		minVal = l23;
		result = cp23;
	}
	if(l31 < minVal) result = cp31;
	return result;
}

decVector deoglCollisionDetection::ClosestPointOnTriangle( const decVector &tp1,
const decVector &tp2, const decVector &tp3, const decVector &p ){
	const decVector edge0 = tp2 - tp1;
	const decVector edge1 = tp3 - tp2;
	const decVector faceNormal = ( edge0 % edge1 ).Normalized();
	decVector point = p;
	
	decVector normal = ( faceNormal % edge0 ).Normalized();
	float distance = ( point - tp1 ) * normal;
	if( distance < 0.0f ){
		point -= normal * distance;
	}
	
	normal = ( faceNormal % edge1 ).Normalized();
	distance = ( point - tp2 ) * normal;
	if( distance < 0.0f ){
		point -= normal * distance;
	}
	
	normal = ( faceNormal % ( tp1 - tp3 ) ).Normalized();
	distance = ( point - tp3 ) * normal;
	if( distance < 0.0f ){
		point -= normal * distance;
	}
	
	return point- faceNormal * ( ( point - tp1 ) * faceNormal );
}



// Closest Point On Quad Edge
// --------------------------
// tp1 = first quad point
// tp2 = second quad point
// tp3 = third quad point
// tp4 = fourth quad point
// p = point to check
decVector deoglCollisionDetection::ClosestPointOnQuadEdge(const decVector &tp1,
const decVector &tp2, const decVector &tp3, const decVector &tp4, const decVector &p){
	decVector cp12 = ClosestPointOnLine(tp1, tp2, p);
	decVector cp23 = ClosestPointOnLine(tp2, tp3, p);
	decVector cp34 = ClosestPointOnLine(tp3, tp4, p);
	decVector cp41 = ClosestPointOnLine(tp4, tp1, p);
	float l12 = (p - cp12).Length();
	float l23 = (p - cp23).Length();
	float l34 = (p - cp34).Length();
	float l41 = (p - cp41).Length();
	float minVal = l12;
	decVector result = cp12;
	if(l23 < minVal){
		minVal = l23;
		result = cp23;
	}
	if(l34 < minVal){
		minVal = l34;
		result = cp34;
	}
	if(l41 < minVal) result = cp41;
	return result;
}

decVector deoglCollisionDetection::ClosestPointOnQuad( const decVector &tp1,
const decVector &tp2, const decVector &tp3, const decVector &tp4, const decVector &p ){
	const decVector edge0 = tp2 - tp1;
	const decVector edge1 = tp3 - tp2;
	const decVector faceNormal = ( edge0 % edge1 ).Normalized();
	decVector point = p;
	
	decVector normal = ( faceNormal % edge0 ).Normalized();
	float distance = ( point - tp1 ) * normal;
	if( distance < 0.0f ){
		point -= normal * distance;
	}
	
	normal = ( faceNormal % edge1 ).Normalized();
	distance = ( point - tp2 ) * normal;
	if( distance < 0.0f ){
		point-= normal * distance;
	}
	
	normal = ( faceNormal % ( tp4 - tp3 ) ).Normalized();
	distance = ( point - tp3 ) * normal;
	if( distance < 0.0f ){
		point -= normal * distance;
	}
	
	normal = ( faceNormal % ( tp1 - tp4 ) ).Normalized();
	distance = ( point - tp4 ) * normal;
	if( distance < 0.0f ){
		point -= normal * distance;
	}
	
	return point - faceNormal * ( ( point - tp1 ) * faceNormal );
}



// INTERSECTION ROUTINES //
///////////////////////////

// Ray Sphere Intersection Point
// -----------------------------
// rp = ray position
// rd = ray direction
// sc = sphere center
// sr = sphere radius
float deoglCollisionDetection::RaySphere(const decVector &rp, const decVector &rd,
const decVector &sc, float sr){
/* old code. doesn't work. no idea why
	decVector Q = sc - rp;
	float c = Q.Length();
	float v = Q * rd;
	float d = sr*sr - (c*c - v*v);
	if(d < 0.0) return -1.0;
	return v - sqrt(d);
*/
/* old code from me. buggy
	decVector diff = rp - sc;
	float a = rd * rd;
	float b = diff * rd;
	float disc = b * b - a * ( diff * diff - sr * sr );
	if( disc < 0.0001 ){
		return -1;
	}else{
		return ( - b - sqrt( disc ) ) / a;
	}
*/
	float a = rd * rd;
	float b = rd * ( rp - sc );
	float c = rp * rp + sc * sc - ( rp * sc ) * 2.0f - sr * sr;
	float disc = b * b - a * c;
	if( disc < 0.0f ) return -1.0f;
	disc = sqrtf( disc );
	float lambda = -b - disc;
	if( lambda < 0.0f ) lambda = -b + disc;
	return lambda / a;
}



// DISTANCE ROUTINES //
///////////////////////

// Segment Segment Distance
// ------------------------
// as = segment a start position
// ae = segment a end position
// bs = segment b start position
// be = segment b end position
// http://softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm
float deoglCollisionDetection::SegmentSegmentDistance( const decVector &as, const decVector &ae,
const decVector &bs, const decVector &be ){
	decVector u = ae - as;
	decVector v = be - bs;
	decVector w = as - bs;
	float a = u * u;
	float b = u * v;
	float c = v * v;
	float d = u * w;
	float e = v * w;
	float D = a * c - b * b;
	float sc, sN, sD = D;
	float tc, tN, tD = D;
	// segments parallel
	if( D < 0.00001f){
		sN = 0.0f;
		sD = 1.0f;
		tN = e;
		tD = c;
	// segments not parallel
	}else{
		sN = ( b * e - c * d );
		tN = ( a * e - b * d );
		// sc < 0 => the s=0 edge is visible
		if( sN < 0.0f ){
			sN = 0;
			tN = e;
			tD = c;
		// sc > 1 => the s=1 edge is visible
		}else if( sN > sD ){
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}
	// tc < 0 => the t=0 edge is visible
	if( tN < 0.0f ){
		tN = 0.0f;
		// recompute sc for this edge
		if( -d < 0.0f ){
			sN = 0.0f;
		}else if( -d > a){
			sN = sD;
		}else{
			sN = -d;
			sD = a;
		}
	// tc > 1 => the t=1 edge is visible
	}else if( tN > tD ){
		tN = tD;
		// recompute sc for this edge
		if( ( -d + b ) < 0.0f ){
			sN = 0.0f;
		}else if( ( -d + b ) > a ){
			sN = sD;
		}else{
			sN = ( -d + b );
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	if( fabs( sN ) < 0.00001f ){
		sc = 0.0f;
	}else{
		sc = sN / sD;
	}
	if( fabs( tN ) < 0.00001f ){
		tc = 0.0f;
	}else{
		tc = tN / tD;
	}
	// get the difference of the two closest points
	return ( w + ( u * sc ) - ( v * tc ) ).Length();
}

float deoglCollisionDetection::PointRayDistance( const decVector &rayOrigin, const decVector &rayDirection, const decVector &point ){
	// ( point - ClosestPointOnRay( rayOrigin, rayDirection, point ) ).Length()
	// ( point - rayOrigin - rayDirection * ( rayDirection * ( point - rayOrigin ) ) ).Length()
	const decVector offset = point - rayOrigin;
	return ( offset - rayDirection * ( rayDirection * offset ) ).Length();
}



// Collision Tests
////////////////////

bool deoglCollisionDetection::PointInTriangle(const decVector &tp1, const decVector &tp2,
const decVector &tp3, const decVector &p){
	decVector edge1 = tp2 - tp1;
	decVector edge2 = tp3 - tp2;
	decVector edge3 = tp1 - tp3;
	decVector normal = edge1 % edge2;
	if( (edge1 % (p - tp1)) * normal < 0.0f ) return false;
	if( (edge2 % (p - tp2)) * normal < 0.0f ) return false;
	if( (edge3 % (p - tp3)) * normal < 0.0f ) return false;
	return true;
}

bool deoglCollisionDetection::PointInTriangle( const decVector &tp1, const decVector &tp2,
const decVector &tp3, const decVector &tnormal, const decVector &p ){
	decVector edge1 = tp2 - tp1;
	decVector edge2 = tp3 - tp2;
	decVector edge3 = tp1 - tp3;
	if( ( edge1 % ( p - tp1 ) ) * tnormal < 0.0f ) return false;
	if( ( edge2 % ( p - tp2 ) ) * tnormal < 0.0f ) return false;
	if( ( edge3 % ( p - tp3 ) ) * tnormal < 0.0f ) return false;
	return true;
}

bool deoglCollisionDetection::PointInQuad(const decVector &tp1, const decVector &tp2,
const decVector &tp3, const decVector &tp4, const decVector &p){
	float arcsum = 0.0f;
	decVector v1 = p - tp1;
	decVector v2 = p - tp2;
	decVector v3 = p - tp3;
	decVector v4 = p - tp4;
	v1.Normalize();
	v2.Normalize();
	v3.Normalize();
	v4.Normalize();
	arcsum += acosf( v1 * v2 );
	arcsum += acosf( v2 * v3 );
	arcsum += acosf( v3 * v4 );
	arcsum += acosf( v4 * v1 );
	return fabs(arcsum - 2.0f*PI) <= 0.005f;
}



bool deoglCollisionDetection::RayHitsSphere( const decVector &rayOrigin, const decVector &rayDirection,
const decVector &sphereCenter, float sphereRadius, float &hitDistance ){
	float a = rayDirection * rayDirection;
	float b = rayDirection * ( rayOrigin - sphereCenter );
	float c = rayOrigin * rayOrigin + sphereCenter * sphereCenter
		- ( rayOrigin * sphereCenter ) * 2.0f - sphereRadius * sphereRadius;
	float disc = b * b - a * c;
	
	if( disc < 0.0f ){
		return false;
	}
	
	disc = sqrtf( disc );
	float lambda = -b - disc;
	if( lambda < 0.0f ){
		lambda = -b + disc;
	}
	
	hitDistance = lambda / a;
	return true;
}

bool deoglCollisionDetection::SphereHitsSphere( const decVector &sphere1Center, float sphere1Radius,
const decVector &sphere2Center, float sphere2Radius ){
	decVector centerDist = sphere2Center - sphere1Center;
	float radiusDist = sphere1Radius + sphere2Radius;
	
	return ( ( centerDist * centerDist ) <= radiusDist * radiusDist );
}

float deoglCollisionDetection::SphereMoveHitsSphere( const decVector &sphere1Center, float sphere1Radius,
const decVector &sphere2Center, float sphere2Radius, const decVector &displacement, decVector *normal ){
	decVector diff( sphere2Center - sphere1Center );
	float a, b, r, rr, disc, lambda, factor;
	
	// check for a collision at the beginning
	r = sphere2Radius + sphere1Radius;
	factor = diff * diff;
	rr = r * r;
	
	if( factor <= rr ){
		if( normal ){
			factor = diff.Length();
			if( factor <= 0.000001f ){
				normal->Set( -displacement );
			}else{
				normal->Set( diff );
			}
			normal->Normalize();
		}
		return 0.0f;
	}
	
	// check if there exist one or more points on the line with the needed distance
	a = displacement * displacement;
	b = ( diff * displacement ) * 2.0f;
	disc = b * b - a * ( factor - rr ) * 4.0f;
	if( disc < 0.0f ) return 1.0f;
	
	// determine the closer of the two points
	disc = sqrtf( disc );
	factor = 1.0f / ( a * 2.0f );
	lambda = ( -b - disc ) * factor;
	if( lambda < 0.0f ) lambda = ( -b + disc ) * factor;
	
	// check if the point is inside the movement range
	if( lambda <= 0.0f || lambda >= 1.0f ) return 1.0f;
	if( normal ){
		normal->Set( sphere2Center + displacement * lambda - sphere1Center );
		normal->Normalize();
	}
	
	return lambda;
}



bool deoglCollisionDetection::RayHitsBox( const decVector &rayOrigin, const decVector &rayDirection,
const decVector &boxCenter, const decVector &boxHalfExtends, float &hitDistance ){
	decVector point = rayOrigin - boxCenter;
	bool hasBestDistance = false;
	float lambda, hp, factor;
	float bestDistance = 0.0f;
	
	// box faces on the x axis
	if( fabs( rayDirection.x ) > 1e-6f ){
		factor = 1.0f / rayDirection.x;
		
		// face on the positive side
		lambda = ( boxHalfExtends.x - point.x ) * factor;
		if( lambda >= 0.0f && lambda <= 1.0f && ( ! hasBestDistance || lambda < bestDistance ) ){
			hp = point.y + rayDirection.y * lambda;
			if( hp >= -boxHalfExtends.y && hp <= boxHalfExtends.y ){
				hp = point.z + rayDirection.z * lambda;
				if( hp >= -boxHalfExtends.z && hp <= boxHalfExtends.z ){
					hasBestDistance = true;
					bestDistance = lambda;
				}
			}
		}
		
		// face on the negative side
		lambda = ( -boxHalfExtends.x - point.x ) * factor;
		if( lambda >= 0.0f && lambda <= 1.0f && ( ! hasBestDistance || lambda < bestDistance ) ){
			hp = point.y + rayDirection.y * lambda;
			if( hp >= -boxHalfExtends.y && hp <= boxHalfExtends.y ){
				hp = point.z + rayDirection.z * lambda;
				if( hp >= -boxHalfExtends.z && hp <= boxHalfExtends.z ){
					hasBestDistance = true;
					bestDistance = lambda;
				}
			}
		}
	}
	
	// box faces on the y axis
	if( fabs( rayDirection.y ) > 1e-6f ){
		factor = 1.0f / rayDirection.y;
		
		// face on the positive side
		lambda = ( boxHalfExtends.y - point.y ) * factor;
		if( lambda >= 0.0f && lambda <= 1.0f && ( ! hasBestDistance || lambda < bestDistance ) ){
			hp = point.x + rayDirection.x * lambda;
			if( hp >= -boxHalfExtends.x && hp <= boxHalfExtends.x ){
				hp = point.z + rayDirection.z * lambda;
				if( hp >= -boxHalfExtends.z && hp <= boxHalfExtends.z ){
					hasBestDistance = true;
					bestDistance = lambda;
				}
			}
		}
		
		// face on the negative side
		lambda = ( -boxHalfExtends.y - point.y ) * factor;
		if( lambda >= 0.0f && lambda <= 1.0f && ( ! hasBestDistance || lambda < bestDistance ) ){
			hp = point.x + rayDirection.x * lambda;
			if( hp >= -boxHalfExtends.x && hp <= boxHalfExtends.x ){
				hp = point.z + rayDirection.z * lambda;
				if( hp >= -boxHalfExtends.z && hp <= boxHalfExtends.z ){
					hasBestDistance = true;
					bestDistance = lambda;
				}
			}
		}
	}
	
	// box faces on the z axis
	if( fabs( rayDirection.z ) > 1e-6f ){
		factor = 1.0f / rayDirection.z;
		
		// face on the positive side
		lambda = ( boxHalfExtends.z - point.z ) * factor;
		if( lambda >= 0.0f && lambda <= 1.0f && ( ! hasBestDistance || lambda < bestDistance ) ){
			hp = point.x + rayDirection.x * lambda;
			if( hp >= -boxHalfExtends.x && hp <= boxHalfExtends.x ){
				hp = point.y + rayDirection.y * lambda;
				if( hp >= -boxHalfExtends.y && hp <= boxHalfExtends.y ){
					hasBestDistance = true;
					bestDistance = lambda;
				}
			}
		}
		
		// face on the negative side
		lambda = ( -boxHalfExtends.z - point.z ) * factor;
		if( lambda >= 0.0f && lambda <= 1.0f && ( ! hasBestDistance || lambda < bestDistance ) ){
			hp = point.x + rayDirection.x * lambda;
			if( hp >= -boxHalfExtends.x && hp <= boxHalfExtends.x ){
				hp = point.y + rayDirection.y * lambda;
				if( hp >= -boxHalfExtends.y && hp <= boxHalfExtends.y ){
					hasBestDistance = true;
					bestDistance = lambda;
				}
			}
		}
	}
	
	// deliver result
	if( hasBestDistance ){
		hitDistance = bestDistance;
	}
	return hasBestDistance;
}

int deoglCollisionDetection::AABoxIntersectsAABox( const decVector &minExtend1, const decVector &maxExtend1, const decVector &minExtend2, const decVector &maxExtend2 ){
	if( maxExtend1.x < minExtend2.x ) return eirOutside;
	if( maxExtend1.y < minExtend2.y ) return eirOutside;
	if( maxExtend1.z < minExtend2.z ) return eirOutside;
	
	if( minExtend1.x > maxExtend2.x ) return eirOutside;
	if( minExtend1.y > maxExtend2.y ) return eirOutside;
	if( minExtend1.z > maxExtend2.z ) return eirOutside;
	
	if( minExtend1.x < minExtend2.x ) return eirPartial;
	if( minExtend1.y < minExtend2.y ) return eirPartial;
	if( minExtend1.z < minExtend2.z ) return eirPartial;
	
	if( maxExtend1.x > maxExtend2.x ) return eirPartial;
	if( maxExtend1.y > maxExtend2.y ) return eirPartial;
	if( maxExtend1.z > maxExtend2.z ) return eirPartial;
	
	return eirInside;
}



bool deoglCollisionDetection::RayHitsCylinder( const decVector &rayOrigin, const decVector &rayDirection,
const decVector &cylinderCenter, float cylinderHalfHeight, float cylinderRadius, float &hitDistance ){
	decVector localRayOrigin = rayOrigin - cylinderCenter;
	float radius2 = cylinderRadius * cylinderRadius;
	float distance, bestDistance = -1.0f;
	float a, b, c, disc, x, z, y;
	
	// test for collision with the cylinder hull
	a = rayDirection.x * rayDirection.x + rayDirection.z * rayDirection.z;
	b = 2.0f * ( localRayOrigin.x * rayDirection.x + localRayOrigin.z * rayDirection.z );
	c = localRayOrigin.x * localRayOrigin.x + localRayOrigin.z * localRayOrigin.z - radius2;
	
	disc = b * b - 4.0f * a * c;
	
	if( disc > 1e-6f ){
		disc = sqrtf( disc );
		
		distance = ( disc - b ) / ( 2.0f * a );
		if( distance >= 0.0f && distance <= 1.0f ){
			y = localRayOrigin.y + rayDirection.y * distance;
			if( y > -cylinderHalfHeight && y < cylinderHalfHeight ){
				bestDistance = distance;
			}
		}
		
		distance = -( disc + b ) / ( 2.0f * a );
		if( distance >= 0.0f && distance <= 1.0f ){
			y = localRayOrigin.y + rayDirection.y * distance;
			if( y > -cylinderHalfHeight && y < cylinderHalfHeight ){
				if( bestDistance < 0.0f || distance < bestDistance ){
					bestDistance = distance;
				}
			}
		}
	}
	
	// test top and bottom caps
	if( fabs( rayDirection.y ) > 1e-6f ){
		disc = 1.0f / rayDirection.y;
		
		// top cap
		distance = ( cylinderHalfHeight - localRayOrigin.y ) * disc;
		if( distance >= 0.0f && distance <= 1.0f ){
			x = localRayOrigin.x + rayDirection.x * distance;
			z = localRayOrigin.z + rayDirection.z * distance;
			if( x * x + z * z <= radius2 && ( bestDistance < 0.0f || distance < bestDistance ) ){
				bestDistance = distance;
			}
		}
		
		// bottom cap
		distance = ( -cylinderHalfHeight - localRayOrigin.y ) * disc;
		if( distance >= 0.0f && distance <= 1.0f ){
			x = localRayOrigin.x + rayDirection.x * distance;
			z = localRayOrigin.z + rayDirection.z * distance;
			if( x * x + z * z <= radius2 && ( bestDistance < 0.0f || distance < bestDistance ) ){
				bestDistance = distance;
			}
		}
	}
	
	// return result
	if( bestDistance < 0.0 ){
		return false;
		
	}else{
		hitDistance = bestDistance;
		return true;
	}
}

bool deoglCollisionDetection::RayHitsTaperedCylinder( const decVector &rayOrigin, const decVector &rayDirection,
const decVector &cylinderCenter, float cylinderHalfHeight, float cylinderTopRadius, float cylinderBottomRadius,
float &hitDistance ){
	decVector localRayOrigin = rayOrigin - cylinderCenter;
	float radiusDifference = cylinderTopRadius - cylinderBottomRadius;
	float radiusBottom2 = cylinderBottomRadius * cylinderBottomRadius;
	float radiusTop2 = cylinderTopRadius * cylinderTopRadius;
	float distance, bestDistance = -1.0f;
	float a, b, c, disc, x, z, y;
	float f1, f2, f3;
	
	// test for collision with the cylinder hull
	f1 = cylinderBottomRadius * radiusDifference / cylinderHalfHeight;
	f2 = ( radiusDifference * radiusDifference ) / ( 4.0f * cylinderHalfHeight * cylinderHalfHeight );
	f3 = localRayOrigin.y + cylinderHalfHeight;
	
	a = rayDirection.x * rayDirection.x + rayDirection.z * rayDirection.z
		- f2 * rayDirection.y * rayDirection.y;
	b = 2.0f * ( localRayOrigin.x * rayDirection.x + localRayOrigin.z * rayDirection.z )
		- rayDirection.y * ( f2 * 2.0f * f3 + f1 );
	c = localRayOrigin.x * localRayOrigin.x + localRayOrigin.z * localRayOrigin.z
		- radiusBottom2 - f3 * ( f1 + f2 * f3 );
	
	disc = b * b - 4.0f * a * c;
	
	if( disc > 1e-6f ){
		disc = sqrtf( disc );
		
		distance = ( disc - b ) / ( 2.0f * a );
		if( distance >= 0.0f && distance <= 1.0f ){
			y = localRayOrigin.y + rayDirection.y * distance;
			if( y > -cylinderHalfHeight && y < cylinderHalfHeight ){
				bestDistance = distance;
			}
		}
		
		distance = -( disc + b ) / ( 2.0f * a );
		if( distance >= 0.0f && distance <= 1.0f ){
			y = localRayOrigin.y + rayDirection.y * distance;
			if( y > -cylinderHalfHeight && y < cylinderHalfHeight ){
				if( bestDistance < 0.0f || distance < bestDistance ){
					bestDistance = distance;
				}
			}
		}
	}
	
	// test top and bottom caps
	if( fabs( rayDirection.y ) > 1e-6f ){
		disc = 1.0f / rayDirection.y;
		
		// top cap
		distance = ( cylinderHalfHeight - localRayOrigin.y ) * disc;
		if( distance >= 0.0f && distance <= 1.0f ){
			x = localRayOrigin.x + rayDirection.x * distance;
			z = localRayOrigin.z + rayDirection.z * distance;
			if( x * x + z * z <= radiusTop2 && ( bestDistance < 0.0f || distance < bestDistance ) ){
				bestDistance = distance;
			}
		}
		
		// bottom cap
		distance = ( -cylinderHalfHeight - localRayOrigin.y ) * disc;
		if( distance >= 0.0f && distance <= 1.0f ){
			x = localRayOrigin.x + rayDirection.x * distance;
			z = localRayOrigin.z + rayDirection.z * distance;
			if( x * x + z * z <= radiusBottom2 && ( bestDistance < 0.0f || distance < bestDistance ) ){
				bestDistance = distance;
			}
		}
	}
	
	// return result
	if( bestDistance < 0.0f ){
		return false;
		
	}else{
		hitDistance = bestDistance;
		return true;
	}
}



bool deoglCollisionDetection::RayHitsCapsule( const decVector &rayOrigin, const decVector &rayDirection,
const decVector &capsuleCenter, float capsuleHalfHeight, float capsuleRadius, float &hitDistance ){
	decVector localRayOrigin = rayOrigin - capsuleCenter;
	float radius2 = capsuleRadius * capsuleRadius;
	float distance, bestDistance = -1.0f;
	float a, b, c, disc, y;
	
	// test for collision with the capsule hull
	a = rayDirection.x * rayDirection.x + rayDirection.z * rayDirection.z;
	b = 2.0f * ( localRayOrigin.x * rayDirection.x + localRayOrigin.z * rayDirection.z );
	c = localRayOrigin.x * localRayOrigin.x + localRayOrigin.z * localRayOrigin.z - radius2;
	
	disc = b * b - 4.0f * a * c;
	
	if( disc > 1e-6f ){
		disc = sqrtf( disc );
		
		distance = ( disc - b ) / ( 2.0f * a );
		if( distance >= 0.0f && distance <= 1.0f ){
			y = localRayOrigin.y + rayDirection.y * distance;
			if( y > -capsuleHalfHeight && y < capsuleHalfHeight ){
				bestDistance = distance;
			}
		}
		
		distance = -( disc + b ) / ( 2.0f * a );
		if( distance >= 0.0f && distance <= 1.0f ){
			y = localRayOrigin.y + rayDirection.y * distance;
			if( y > -capsuleHalfHeight && y < capsuleHalfHeight ){
				if( bestDistance < 0.0f || distance < bestDistance ){
					bestDistance = distance;
				}
			}
		}
	}
	
	// test top and bottom caps
	if( fabs( rayDirection.y ) > 1e-6f ){
		disc = 1.0f / rayDirection.y;
		
		// top cap
		distance = RaySphere( localRayOrigin, rayDirection, decVector( 0.0f, capsuleHalfHeight, 0.0f ), capsuleRadius );
		if( distance >= 0.0f && distance <= 1.0f && ( bestDistance < 0.0f || distance < bestDistance ) ){
			bestDistance = distance;
		}
		
		// bottom cap
		distance = RaySphere( localRayOrigin, rayDirection, decVector( 0.0f, -capsuleHalfHeight, 0.0f ), capsuleRadius );
		if( distance >= 0.0f && distance <= 1.0f && ( bestDistance < 0.0f || distance < bestDistance ) ){
			bestDistance = distance;
		}
	}
	
	// return result
	if( bestDistance < 0.0f ){
		return false;
		
	}else{
		hitDistance = bestDistance;
		return true;
	}
}

bool deoglCollisionDetection::RayHitsTaperedCapsule( const decVector &rayOrigin, const decVector &rayDirection,
const decVector &capsuleCenter, float capsuleHalfHeight, float capsuleTopRadius, float capsuleBottomRadius,
float &hitDistance ){
	decVector localRayOrigin = rayOrigin - capsuleCenter;
	float radiusTop, radiusBottom, halfHeight;
	float distance, bestDistance = -1.0f;
	float a, b, c, disc, y;
	float radiusDifference;
	float f1, f2, f3;
	
	// adjust parameters
	f1 = ( capsuleBottomRadius - capsuleTopRadius ) / ( 2.0f * capsuleHalfHeight ); // sin(a)
	f2 = sqrtf( 1.0f - f1 * f1 ); // cos(a)
	
	f3 = capsuleBottomRadius * f1;
	halfHeight = capsuleHalfHeight + ( capsuleTopRadius * f1 - f3 ) * 0.5f;
	f3 = localRayOrigin.y + capsuleHalfHeight - f3; // localRayOrigin.y adjusted
	radiusTop = capsuleTopRadius * f2;
	radiusBottom = capsuleBottomRadius * f2;
	
	radiusDifference = radiusTop - radiusBottom;
	
	// test for collision with the capsule hull
	f1 = radiusBottom * radiusDifference / halfHeight;
	f2 = ( radiusDifference * radiusDifference ) / ( 4.0f * halfHeight * halfHeight );
	
	a = rayDirection.x * rayDirection.x + rayDirection.z * rayDirection.z
		- f2 * rayDirection.y * rayDirection.y;
	b = 2.0f * ( localRayOrigin.x * rayDirection.x + localRayOrigin.z * rayDirection.z )
		- rayDirection.y * ( f2 * 2.0f * f3 + f1 );
	c = localRayOrigin.x * localRayOrigin.x + localRayOrigin.z * localRayOrigin.z
		- radiusBottom * radiusBottom - f3 * ( f1 + f2 * f3 );
	
	disc = b * b - 4.0f * a * c;
	
	if( disc > 1e-6f ){
		disc = sqrtf( disc );
		
		distance = ( disc - b ) / ( 2.0f * a );
		if( distance >= 0.0f && distance <= 1.0f ){
			y = f3 + rayDirection.y * distance;
			if( y > 0.0f && y < 2.0f * halfHeight ){
				bestDistance = distance;
			}
		}
		
		distance = -( disc + b ) / ( 2.0f * a );
		if( distance >= 0.0f && distance <= 1.0f ){
			y = f3 + rayDirection.y * distance;
			if( y > 0.0f && y < 2.0f * halfHeight ){
				if( bestDistance < 0.0f || distance < bestDistance ){
					bestDistance = distance;
				}
			}
		}
	}
	
	// test top and bottom caps
	if( fabs( rayDirection.y ) > 1e-6f ){
		disc = 1.0f / rayDirection.y;
		
		// top cap
		distance = RaySphere( localRayOrigin, rayDirection, decVector( 0.0f, capsuleHalfHeight, 0.0f ), capsuleTopRadius );
		if( distance >= 0.0f && distance <= 1.0f && ( bestDistance < 0.0f || distance < bestDistance ) ){
			bestDistance = distance;
		}
		
		// bottom cap
		distance = RaySphere( localRayOrigin, rayDirection, decVector( 0.0f, -capsuleHalfHeight, 0.0f ), capsuleBottomRadius );
		if( distance >= 0.0f && distance <= 1.0f && ( bestDistance < 0.0f || distance < bestDistance ) ){
			bestDistance = distance;
		}
	}
	
	// return result
	if( bestDistance < 0.0f ){
		return false;
		
	}else{
		hitDistance = bestDistance;
		return true;
	}
}



bool deoglCollisionDetection::RayHitsTriangle( const decVector &rayOrigin, const decVector &rayDirection,
const decVector &tri1, const decVector &tri2, const decVector &tri3 ){
	decVector edge1 = tri2 - tri1;
	decVector edge2 = tri3 - tri2;
	decVector trinormal( edge1 % edge2 );
	float dot = rayDirection * trinormal, lambda;
	
	if( dot < -0.00001f || dot > 0.00001f ){
		lambda = ( ( tri1 - rayOrigin ) * trinormal ) / dot;
		
		if( lambda > 0.0f && lambda <= 1.0f ){
			decVector hitPoint( rayOrigin + rayDirection * lambda );
			
			if( ( edge1 % ( hitPoint - tri1 ) ) * trinormal < 0.0f ) return false;
			if( ( edge2 % ( hitPoint - tri2 ) ) * trinormal < 0.0f ) return false;
			decVector edge3( tri1 - tri3 );
			if( ( edge3 % ( hitPoint - tri3 ) ) * trinormal < 0.0f ) return false;
			
			return true;
		}
	}
	
	return false;
}

bool deoglCollisionDetection::RayHitsTriangle( const decVector &rayOrigin, const decVector &rayDirection,
const decVector &tri1, const decVector &tri2, const decVector &tri3, const decVector &trinormal ){
	float dot = rayDirection * trinormal, lambda;
	
	if( dot < -0.00001f || dot > 0.00001f ){
		lambda = ( ( tri1 - rayOrigin ) * trinormal ) / dot;
		
		if( lambda > 0.0f && lambda <= 1.0f ){
			decVector hitPoint( rayOrigin + rayDirection * lambda );
			
			decVector edge( tri2 - tri1 );
			if( ( edge % ( hitPoint - tri1 ) ) * trinormal < 0.0 ) return false;
			edge = tri3 - tri2;
			if( ( edge % ( hitPoint - tri2 ) ) * trinormal < 0.0 ) return false;
			edge = tri1 - tri3;
			if( ( edge % ( hitPoint - tri3 ) ) * trinormal < 0.0 ) return false;
			
			return true;
		}
	}
	
	return false;
}
