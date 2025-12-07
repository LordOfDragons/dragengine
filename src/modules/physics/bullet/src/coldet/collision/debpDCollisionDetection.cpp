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
#include <string.h>

#include "debpDCollisionDetection.h"



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
double debpDCollisionDetection::RayPlane(const decDVector &pp, const decDVector &pn,
const decDVector &rp, const decDVector &rd){
//	return -((pn * rp - (pn * pp)) / (pn * rd));
//	double numer = pn * rp - ( pn * pp );
	double numer = pn * (rp - pp);
	double denom = pn * rd;
	return denom == 0 ? -1.0 : -(numer / denom);
}



decDVector debpDCollisionDetection::ClosestPointOnLine(const decDVector &lineFrom, const decDVector &lineTo, const decDVector &point){
	const decDVector lineDir = lineTo - lineFrom;
	const decDVector offset = point - lineFrom;
	const double lineLenSquared = lineDir.LengthSquared();
	
	if(fabs(lineLenSquared) > DOUBLE_SAFE_EPSILON){
		// lambda = ( lineDir.Normalized() * offset ) / lineDir.Length()
		const double lambda = (lineDir * offset) / lineLenSquared;
		
		if(lambda <= 0.0){
			return lineFrom;
		}
		if(lambda >= 1.0){
			return lineTo;
		}
		
		return lineFrom + lineDir * lambda;
		
	}else{
		return lineFrom;
	}
}

decDVector debpDCollisionDetection::ClosestPointOnRay(const decDVector &rayOrigin, const decDVector &rayDirection, const decDVector &point){
	// rayOrigin + rayDirection * distanceOnRay
	return rayOrigin + rayDirection * (rayDirection * (point - rayOrigin));
}



// Closest Point On Triangle Edge
// ------------------------------
// tp1 = first triangle point
// tp2 = second triangle point
// tp3 = third triangle point
// p = point to check
decDVector debpDCollisionDetection::ClosestPointOnTriangleEdge(const decDVector &tp1,
const decDVector &tp2, const decDVector &tp3, const decDVector &p){
	decDVector cp12 = ClosestPointOnLine(tp1, tp2, p);
	decDVector cp23 = ClosestPointOnLine(tp2, tp3, p);
	decDVector cp31 = ClosestPointOnLine(tp3, tp1, p);
	double l12 = (p - cp12).Length();
	double l23 = (p - cp23).Length();
	double l31 = (p - cp31).Length();
	double minVal = l12;
	decDVector result = cp12;
	if(l23 < minVal){
		minVal = l23;
		result = cp23;
	}
	if(l31 < minVal) result = cp31;
	return result;
}

decDVector debpDCollisionDetection::ClosestPointOnTriangle(const decDVector &tp1,
const decDVector &tp2, const decDVector &tp3, const decDVector &p){
	const decDVector edge0 = tp2 - tp1;
	const decDVector edge1 = tp3 - tp2;
	const decDVector faceNormal = (edge0 % edge1).Normalized();
	decDVector point = p;
	
	decDVector normal = (faceNormal % edge0).Normalized();
	double distance = (point - tp1) * normal;
	if(distance < 0.0){
		point -= normal * distance;
	}
	
	normal = (faceNormal % edge1).Normalized();
	distance = (point - tp2) * normal;
	if(distance < 0.0){
		point -= normal * distance;
	}
	
	normal = (faceNormal % (tp1 - tp3)).Normalized();
	distance = (point - tp3) * normal;
	if(distance < 0.0){
		point -= normal * distance;
	}
	
	return point - faceNormal * ((point - tp1) * faceNormal);
}



// Closest Point On Quad Edge
// --------------------------
// tp1 = first quad point
// tp2 = second quad point
// tp3 = third quad point
// tp4 = fourth quad point
// p = point to check
decDVector debpDCollisionDetection::ClosestPointOnQuadEdge(const decDVector &tp1,
const decDVector &tp2, const decDVector &tp3, const decDVector &tp4, const decDVector &p){
	decDVector cp12 = ClosestPointOnLine(tp1, tp2, p);
	decDVector cp23 = ClosestPointOnLine(tp2, tp3, p);
	decDVector cp34 = ClosestPointOnLine(tp3, tp4, p);
	decDVector cp41 = ClosestPointOnLine(tp4, tp1, p);
	double l12 = (p - cp12).Length();
	double l23 = (p - cp23).Length();
	double l34 = (p - cp34).Length();
	double l41 = (p - cp41).Length();
	double minVal = l12;
	decDVector result = cp12;
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

decDVector debpDCollisionDetection::ClosestPointOnQuad(const decDVector &tp1,
const decDVector &tp2, const decDVector &tp3, const decDVector &tp4, const decDVector &p){
	const decDVector edge0 = tp2 - tp1;
	const decDVector edge1 = tp3 - tp2;
	const decDVector faceNormal = (edge0 % edge1).Normalized();
	decDVector point = p;
	
	decDVector normal = (faceNormal % edge0).Normalized();
	double distance = (point - tp1) * normal;
	if(distance < 0.0){
		point -= normal * distance;
	}
	
	normal = (faceNormal % edge1).Normalized();
	distance = (point - tp2) * normal;
	if(distance < 0.0){
		point -= normal * distance;
	}
	
	normal = (faceNormal % (tp4 - tp3)).Normalized();
	distance = (point - tp3) * normal;
	if(distance < 0.0){
		point -= normal * distance;
	}
	
	normal = (faceNormal % (tp1 - tp4)).Normalized();
	distance = (point - tp4) * normal;
	if(distance < 0.0){
		point -= normal * distance;
	}
	
	return point - faceNormal * ((point - tp1) * faceNormal);
}



// INTERSECTION ROUTINES //
///////////////////////////

// Ray Sphere Intersection Point
// -----------------------------
// rp = ray position
// rd = ray direction
// sc = sphere center
// sr = sphere radius
double debpDCollisionDetection::RaySphere(const decDVector &rp, const decDVector &rd,
const decDVector &sc, double sr){
	double a = rd * rd;
	double b = rd * (rp - sc);
	double c = rp * rp + sc * sc - (rp * sc) * 2.0 - sr * sr;
	double disc = b * b - a * c;
	if(disc < 0.0) return -1.0;
	disc = sqrt(disc);
	double lambda = -b - disc;
	if(lambda < 0) lambda = -b + disc;
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
double debpDCollisionDetection::SegmentSegmentDistance(const decDVector &as, const decDVector &ae,
const decDVector &bs, const decDVector &be){
	decDVector u = ae - as;
	decDVector v = be - bs;
	decDVector w = as - bs;
	double a = u * u;
	double b = u * v;
	double c = v * v;
	double d = u * w;
	double e = v * w;
	double D = a * c - b * b;
	double sc, sN, sD = D;
	double tc, tN, tD = D;
	// segments parallel
	if(D < 0.00001){
		sN = 0.0;
		sD = 1.0;
		tN = e;
		tD = c;
	// segments not parallel
	}else{
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		// sc < 0 => the s=0 edge is visible
		if(sN < 0){
			sN = 0;
			tN = e;
			tD = c;
		// sc > 1 => the s=1 edge is visible
		}else if(sN > sD){
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}
	// tc < 0 => the t=0 edge is visible
	if(tN < 0){
		tN = 0;
		// recompute sc for this edge
		if(-d < 0){
			sN = 0;
		}else if(-d > a){
			sN = sD;
		}else{
			sN = -d;
			sD = a;
		}
	// tc > 1 => the t=1 edge is visible
	}else if(tN > tD){
		tN = tD;
		// recompute sc for this edge
		if((-d + b) < 0){
			sN = 0;
		}else if((-d + b) > a){
			sN = sD;
		}else{
			sN = (-d + b);
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	if(fabs(sN) < 0.00001){
		sc = 0;
	}else{
		sc = sN / sD;
	}
	if(fabs(tN) < 0.00001){
		tc = 0;
	}else{
		tc = tN / tD;
	}
	// get the difference of the two closest points
	return (w + (u * sc) - (v * tc)).Length();
}

double debpDCollisionDetection::PointRayDistance(const decDVector &rayOrigin, const decDVector &rayDirection, const decDVector &point){
	// ( point - ClosestPointOnRay( rayOrigin, rayDirection, point ) ).Length()
	// ( point - rayOrigin - rayDirection * ( rayDirection * ( point - rayOrigin ) ) ).Length()
	const decDVector offset = point - rayOrigin;
	return (offset - rayDirection * (rayDirection * offset)).Length();
}



// Collision Tests
////////////////////

bool debpDCollisionDetection::PointInTriangle(const decDVector &tp1, const decDVector &tp2,
const decDVector &tp3, const decDVector &p){
	decDVector edge1 = tp2 - tp1;
	decDVector edge2 = tp3 - tp2;
	decDVector edge3 = tp1 - tp3;
	decDVector normal = edge1 % edge2;
	if((edge1 % (p - tp1)) * normal < 0) return false;
	if((edge2 % (p - tp2)) * normal < 0) return false;
	if((edge3 % (p - tp3)) * normal < 0) return false;
	return true;
}

bool debpDCollisionDetection::PointInTriangle(const decDVector &tp1, const decDVector &tp2,
const decDVector &tp3, const decDVector &tnormal, const decDVector &p){
	decDVector edge1 = tp2 - tp1;
	decDVector edge2 = tp3 - tp2;
	decDVector edge3 = tp1 - tp3;
	if((edge1 % (p - tp1)) * tnormal < 0) return false;
	if((edge2 % (p - tp2)) * tnormal < 0) return false;
	if((edge3 % (p - tp3)) * tnormal < 0) return false;
	return true;
}

bool debpDCollisionDetection::PointInQuad(const decDVector &tp1, const decDVector &tp2,
const decDVector &tp3, const decDVector &tp4, const decDVector &p){
	double arcsum = 0.0;
	decDVector v1 = p - tp1;
	decDVector v2 = p - tp2;
	decDVector v3 = p - tp3;
	decDVector v4 = p - tp4;
	v1.Normalize();
	v2.Normalize();
	v3.Normalize();
	v4.Normalize();
	arcsum += acos(v1 * v2);
	arcsum += acos(v2 * v3);
	arcsum += acos(v3 * v4);
	arcsum += acos(v4 * v1);
	return fabs(arcsum - 2*PI) <= 0.005;
}



bool debpDCollisionDetection::RayHitsSphere(const decDVector &rayOrigin, const decDVector &rayDirection,
const decDVector &sphereCenter, double sphereRadius, double &hitDistance){
	double a = rayDirection * rayDirection;
	double b = rayDirection * (rayOrigin - sphereCenter);
	double c = rayOrigin * rayOrigin + sphereCenter * sphereCenter
		- (rayOrigin * sphereCenter) * 2.0 - sphereRadius * sphereRadius;
	double disc = b * b - a * c;
	
	if(disc < 0.0){
		return false;
	}
	
	disc = sqrt(disc);
	double lambda = -b - disc;
	if(lambda < 0.0){
		lambda = -b + disc;
	}
	
	hitDistance = lambda / a;
	return true;
}

bool debpDCollisionDetection::SphereHitsSphere(const decDVector &sphere1Center, double sphere1Radius,
const decDVector &sphere2Center, double sphere2Radius){
	decDVector centerDist = sphere2Center - sphere1Center;
	double radiusDist = sphere1Radius + sphere2Radius;
	
	return ((centerDist * centerDist) <= radiusDist * radiusDist);
}

double debpDCollisionDetection::SphereMoveHitsSphere(const decDVector &sphere1Center, double sphere1Radius,
const decDVector &sphere2Center, double sphere2Radius, const decDVector &displacement, decDVector *normal){
	decDVector diff(sphere2Center - sphere1Center);
	double a, b, r, rr, disc, lambda, factor;
	
	// check for a collision at the beginning
	r = sphere2Radius + sphere1Radius;
	factor = diff * diff;
	rr = r * r;
	
	if(factor <= rr){
		if(normal){
			factor = diff.Length();
			if(factor <= 0.000001){
				normal->Set(-displacement);
			}else{
				normal->Set(diff);
			}
			normal->Normalize();
		}
		return 0.0;
	}
	
	// check if there exist one or more points on the line with the needed distance
	a = displacement * displacement;
	b = (diff * displacement) * 2.0;
	disc = b * b - a * (factor - rr) * 4.0;
	if(disc < 0.0) return 1.0;
	
	// determine the closer of the two points
	disc = sqrt(disc);
	factor = 1.0 / (a * 2.0);
	lambda = (-b - disc) * factor;
	if(lambda < 0) lambda = (-b + disc) * factor;
	
	// check if the point is inside the movement range
	if(lambda <= 0.0 || lambda >= 1.0) return 1.0;
	if(normal){
		normal->Set(sphere2Center + displacement * lambda - sphere1Center);
		normal->Normalize();
	}
	
	return lambda;
}



bool debpDCollisionDetection::RayHitsBox(const decDVector &rayOrigin, const decDVector &rayDirection,
const decDVector &boxCenter, const decDVector &boxHalfExtends, double &hitDistance){
	decDVector point = rayOrigin - boxCenter;
	bool hasBestDistance = false;
	double lambda, hp, factor;
	double bestDistance = 0.0;
	
	// box faces on the x axis
	if(fabs(rayDirection.x) > 1e-12){
		factor = 1.0 / rayDirection.x;
		
		// face on the positive side
		lambda = (boxHalfExtends.x - point.x) * factor;
		if(lambda >= 0.0 && lambda <= 1.0 && (!hasBestDistance || lambda < bestDistance)){
			hp = point.y + rayDirection.y * lambda;
			if(hp >= -boxHalfExtends.y && hp <= boxHalfExtends.y){
				hp = point.z + rayDirection.z * lambda;
				if(hp >= -boxHalfExtends.z && hp <= boxHalfExtends.z){
					hasBestDistance = true;
					bestDistance = lambda;
				}
			}
		}
		
		// face on the negative side
		lambda = (-boxHalfExtends.x - point.x) * factor;
		if(lambda >= 0.0 && lambda <= 1.0 && (!hasBestDistance || lambda < bestDistance)){
			hp = point.y + rayDirection.y * lambda;
			if(hp >= -boxHalfExtends.y && hp <= boxHalfExtends.y){
				hp = point.z + rayDirection.z * lambda;
				if(hp >= -boxHalfExtends.z && hp <= boxHalfExtends.z){
					hasBestDistance = true;
					bestDistance = lambda;
				}
			}
		}
	}
	
	// box faces on the y axis
	if(fabs(rayDirection.y) > 1e-12){
		factor = 1.0 / rayDirection.y;
		
		// face on the positive side
		lambda = (boxHalfExtends.y - point.y) * factor;
		if(lambda >= 0.0 && lambda <= 1.0 && (!hasBestDistance || lambda < bestDistance)){
			hp = point.x + rayDirection.x * lambda;
			if(hp >= -boxHalfExtends.x && hp <= boxHalfExtends.x){
				hp = point.z + rayDirection.z * lambda;
				if(hp >= -boxHalfExtends.z && hp <= boxHalfExtends.z){
					hasBestDistance = true;
					bestDistance = lambda;
				}
			}
		}
		
		// face on the negative side
		lambda = (-boxHalfExtends.y - point.y) * factor;
		if(lambda >= 0.0 && lambda <= 1.0 && (!hasBestDistance || lambda < bestDistance)){
			hp = point.x + rayDirection.x * lambda;
			if(hp >= -boxHalfExtends.x && hp <= boxHalfExtends.x){
				hp = point.z + rayDirection.z * lambda;
				if(hp >= -boxHalfExtends.z && hp <= boxHalfExtends.z){
					hasBestDistance = true;
					bestDistance = lambda;
				}
			}
		}
	}
	
	// box faces on the z axis
	if(fabs(rayDirection.z) > 1e-12){
		factor = 1.0 / rayDirection.z;
		
		// face on the positive side
		lambda = (boxHalfExtends.z - point.z) * factor;
		if(lambda >= 0.0 && lambda <= 1.0 && (!hasBestDistance || lambda < bestDistance)){
			hp = point.x + rayDirection.x * lambda;
			if(hp >= -boxHalfExtends.x && hp <= boxHalfExtends.x){
				hp = point.y + rayDirection.y * lambda;
				if(hp >= -boxHalfExtends.y && hp <= boxHalfExtends.y){
					hasBestDistance = true;
					bestDistance = lambda;
				}
			}
		}
		
		// face on the negative side
		lambda = (-boxHalfExtends.z - point.z) * factor;
		if(lambda >= 0.0 && lambda <= 1.0 && (!hasBestDistance || lambda < bestDistance)){
			hp = point.x + rayDirection.x * lambda;
			if(hp >= -boxHalfExtends.x && hp <= boxHalfExtends.x){
				hp = point.y + rayDirection.y * lambda;
				if(hp >= -boxHalfExtends.y && hp <= boxHalfExtends.y){
					hasBestDistance = true;
					bestDistance = lambda;
				}
			}
		}
	}
	
	// deliver result
	if(hasBestDistance){
		hitDistance = bestDistance;
	}
	return hasBestDistance;
}

int debpDCollisionDetection::AABoxIntersectsAABox(const decDVector &minExtend1, const decDVector &maxExtend1, const decDVector &minExtend2, const decDVector &maxExtend2){
	if(maxExtend1.x < minExtend2.x) return eirOutside;
	if(maxExtend1.y < minExtend2.y) return eirOutside;
	if(maxExtend1.z < minExtend2.z) return eirOutside;
	
	if(minExtend1.x > maxExtend2.x) return eirOutside;
	if(minExtend1.y > maxExtend2.y) return eirOutside;
	if(minExtend1.z > maxExtend2.z) return eirOutside;
	
	if(minExtend1.x < minExtend2.x) return eirPartial;
	if(minExtend1.y < minExtend2.y) return eirPartial;
	if(minExtend1.z < minExtend2.z) return eirPartial;
	
	if(maxExtend1.x > maxExtend2.x) return eirPartial;
	if(maxExtend1.y > maxExtend2.y) return eirPartial;
	if(maxExtend1.z > maxExtend2.z) return eirPartial;
	
	return eirInside;
}



bool debpDCollisionDetection::RayHitsCylinder(const decDVector &rayOrigin, const decDVector &rayDirection,
const decDVector &cylinderCenter, double cylinderHalfHeight, double cylinderRadius, double &hitDistance){
	decDVector localRayOrigin = rayOrigin - cylinderCenter;
	double radius2 = cylinderRadius * cylinderRadius;
	double distance, bestDistance = -1.0;
	double a, b, c, disc, x, z, y;
	
	// test for collision with the cylinder hull
	a = rayDirection.x * rayDirection.x + rayDirection.z * rayDirection.z;
	b = 2.0 * (localRayOrigin.x * rayDirection.x + localRayOrigin.z * rayDirection.z);
	c = localRayOrigin.x * localRayOrigin.x + localRayOrigin.z * localRayOrigin.z - radius2;
	
	disc = b * b - 4.0 * a * c;
	
	if(disc > 1e-12){
		disc = sqrt(disc);
		
		distance = (disc - b) / (2.0 * a);
		if(distance >= 0.0 && distance <= 1.0){
			y = localRayOrigin.y + rayDirection.y * distance;
			if(y > -cylinderHalfHeight && y < cylinderHalfHeight){
				bestDistance = distance;
			}
		}
		
		distance = -(disc + b) / (2.0 * a);
		if(distance >= 0.0 && distance <= 1.0){
			y = localRayOrigin.y + rayDirection.y * distance;
			if(y > -cylinderHalfHeight && y < cylinderHalfHeight){
				if(bestDistance < 0.0 || distance < bestDistance){
					bestDistance = distance;
				}
			}
		}
	}
	
	// test top and bottom caps
	if(fabs(rayDirection.y) > 1e-12){
		disc = 1.0 / rayDirection.y;
		
		// top cap
		distance = (cylinderHalfHeight - localRayOrigin.y) * disc;
		if(distance >= 0.0 && distance <= 1.0){
			x = localRayOrigin.x + rayDirection.x * distance;
			z = localRayOrigin.z + rayDirection.z * distance;
			if(x * x + z * z <= radius2 && (bestDistance < 0.0 || distance < bestDistance)){
				bestDistance = distance;
			}
		}
		
		// bottom cap
		distance = (-cylinderHalfHeight - localRayOrigin.y) * disc;
		if(distance >= 0.0 && distance <= 1.0){
			x = localRayOrigin.x + rayDirection.x * distance;
			z = localRayOrigin.z + rayDirection.z * distance;
			if(x * x + z * z <= radius2 && (bestDistance < 0.0 || distance < bestDistance)){
				bestDistance = distance;
			}
		}
	}
	
	// return result
	if(bestDistance < 0.0){
		return false;
		
	}else{
		hitDistance = bestDistance;
		return true;
	}
}

bool debpDCollisionDetection::RayHitsTaperedCylinder(const decDVector &rayOrigin, const decDVector &rayDirection,
const decDVector &cylinderCenter, double cylinderHalfHeight, double cylinderTopRadius, double cylinderBottomRadius,
double &hitDistance){
	decDVector localRayOrigin = rayOrigin - cylinderCenter;
	double radiusDifference = cylinderTopRadius - cylinderBottomRadius;
	double radiusBottom2 = cylinderBottomRadius * cylinderBottomRadius;
	double radiusTop2 = cylinderTopRadius * cylinderTopRadius;
	double distance, bestDistance = -1.0;
	double a, b, c, disc, x, z, y;
	double f1, f2, f3;
	
	// test for collision with the cylinder hull
	f1 = cylinderBottomRadius * radiusDifference / cylinderHalfHeight;
	f2 = (radiusDifference * radiusDifference) / (4.0 * cylinderHalfHeight * cylinderHalfHeight);
	f3 = localRayOrigin.y + cylinderHalfHeight;
	
	a = rayDirection.x * rayDirection.x + rayDirection.z * rayDirection.z
		- f2 * rayDirection.y * rayDirection.y;
	b = 2.0 * (localRayOrigin.x * rayDirection.x + localRayOrigin.z * rayDirection.z)
		- rayDirection.y * (f2 * 2.0 * f3 + f1);
	c = localRayOrigin.x * localRayOrigin.x + localRayOrigin.z * localRayOrigin.z
		- radiusBottom2 - f3 * (f1 + f2 * f3);
	
	disc = b * b - 4.0 * a * c;
	
	if(disc > 1e-12){
		disc = sqrt(disc);
		
		distance = (disc - b) / (2.0 * a);
		if(distance >= 0.0 && distance <= 1.0){
			y = localRayOrigin.y + rayDirection.y * distance;
			if(y > -cylinderHalfHeight && y < cylinderHalfHeight){
				bestDistance = distance;
			}
		}
		
		distance = -(disc + b) / (2.0 * a);
		if(distance >= 0.0 && distance <= 1.0){
			y = localRayOrigin.y + rayDirection.y * distance;
			if(y > -cylinderHalfHeight && y < cylinderHalfHeight){
				if(bestDistance < 0.0 || distance < bestDistance){
					bestDistance = distance;
				}
			}
		}
	}
	
	// test top and bottom caps
	if(fabs(rayDirection.y) > 1e-12){
		disc = 1.0 / rayDirection.y;
		
		// top cap
		distance = (cylinderHalfHeight - localRayOrigin.y) * disc;
		if(distance >= 0.0 && distance <= 1.0){
			x = localRayOrigin.x + rayDirection.x * distance;
			z = localRayOrigin.z + rayDirection.z * distance;
			if(x * x + z * z <= radiusTop2 && (bestDistance < 0.0 || distance < bestDistance)){
				bestDistance = distance;
			}
		}
		
		// bottom cap
		distance = (-cylinderHalfHeight - localRayOrigin.y) * disc;
		if(distance >= 0.0 && distance <= 1.0){
			x = localRayOrigin.x + rayDirection.x * distance;
			z = localRayOrigin.z + rayDirection.z * distance;
			if(x * x + z * z <= radiusBottom2 && (bestDistance < 0.0 || distance < bestDistance)){
				bestDistance = distance;
			}
		}
	}
	
	// return result
	if(bestDistance < 0.0){
		return false;
		
	}else{
		hitDistance = bestDistance;
		return true;
	}
}



bool debpDCollisionDetection::RayHitsCapsule(const decDVector &rayOrigin, const decDVector &rayDirection,
const decDVector &capsuleCenter, double capsuleHalfHeight, double capsuleRadius, double &hitDistance){
	decDVector localRayOrigin = rayOrigin - capsuleCenter;
	double radius2 = capsuleRadius * capsuleRadius;
	double distance, bestDistance = -1.0;
	double a, b, c, disc, y;
	
	// test for collision with the capsule hull
	a = rayDirection.x * rayDirection.x + rayDirection.z * rayDirection.z;
	b = 2.0 * (localRayOrigin.x * rayDirection.x + localRayOrigin.z * rayDirection.z);
	c = localRayOrigin.x * localRayOrigin.x + localRayOrigin.z * localRayOrigin.z - radius2;
	
	disc = b * b - 4.0 * a * c;
	
	if(disc > 1e-12){
		disc = sqrt(disc);
		
		distance = (disc - b) / (2.0 * a);
		if(distance >= 0.0 && distance <= 1.0){
			y = localRayOrigin.y + rayDirection.y * distance;
			if(y > -capsuleHalfHeight && y < capsuleHalfHeight){
				bestDistance = distance;
			}
		}
		
		distance = -(disc + b) / (2.0 * a);
		if(distance >= 0.0 && distance <= 1.0){
			y = localRayOrigin.y + rayDirection.y * distance;
			if(y > -capsuleHalfHeight && y < capsuleHalfHeight){
				if(bestDistance < 0.0 || distance < bestDistance){
					bestDistance = distance;
				}
			}
		}
	}
	
	// test top and bottom caps
	if(fabs(rayDirection.y) > 1e-12){
		disc = 1.0 / rayDirection.y;
		
		// top cap
		distance = RaySphere(localRayOrigin, rayDirection, decDVector(0.0, capsuleHalfHeight, 0.0), capsuleRadius);
		if(distance >= 0.0 && distance <= 1.0 && (bestDistance < 0.0 || distance < bestDistance)){
			bestDistance = distance;
		}
		
		// bottom cap
		distance = RaySphere(localRayOrigin, rayDirection, decDVector(0.0, -capsuleHalfHeight, 0.0), capsuleRadius);
		if(distance >= 0.0 && distance <= 1.0 && (bestDistance < 0.0 || distance < bestDistance)){
			bestDistance = distance;
		}
	}
	
	// return result
	if(bestDistance < 0.0){
		return false;
		
	}else{
		hitDistance = bestDistance;
		return true;
	}
}

bool debpDCollisionDetection::RayHitsTaperedCapsule(const decDVector &rayOrigin, const decDVector &rayDirection,
const decDVector &capsuleCenter, double capsuleHalfHeight, double capsuleTopRadius, double capsuleBottomRadius,
double &hitDistance){
	decDVector localRayOrigin = rayOrigin - capsuleCenter;
	double radiusTop, radiusBottom, halfHeight;
	double distance, bestDistance = -1.0;
	double a, b, c, disc, y;
	double radiusDifference;
	double f1, f2, f3;
	
	// adjust parameters
	f1 = (capsuleBottomRadius - capsuleTopRadius) / (2.0 * capsuleHalfHeight); // sin(a)
	f2 = sqrt(1.0 - f1 * f1); // cos(a)
	
	f3 = capsuleBottomRadius * f1;
	halfHeight = capsuleHalfHeight + (capsuleTopRadius * f1 - f3) * 0.5;
	f3 = localRayOrigin.y + capsuleHalfHeight - f3; // localRayOrigin.y adjusted
	radiusTop = capsuleTopRadius * f2;
	radiusBottom = capsuleBottomRadius * f2;
	
	radiusDifference = radiusTop - radiusBottom;
	
	// test for collision with the capsule hull
	f1 = radiusBottom * radiusDifference / halfHeight;
	f2 = (radiusDifference * radiusDifference) / (4.0 * halfHeight * halfHeight);
	
	a = rayDirection.x * rayDirection.x + rayDirection.z * rayDirection.z
		- f2 * rayDirection.y * rayDirection.y;
	b = 2.0 * (localRayOrigin.x * rayDirection.x + localRayOrigin.z * rayDirection.z)
		- rayDirection.y * (f2 * 2.0 * f3 + f1);
	c = localRayOrigin.x * localRayOrigin.x + localRayOrigin.z * localRayOrigin.z
		- radiusBottom * radiusBottom - f3 * (f1 + f2 * f3);
	
	disc = b * b - 4.0 * a * c;
	
	if(disc > 1e-12){
		disc = sqrt(disc);
		
		distance = (disc - b) / (2.0 * a);
		if(distance >= 0.0 && distance <= 1.0){
			y = f3 + rayDirection.y * distance;
			if(y > 0.0 && y < 2.0 * halfHeight){
				bestDistance = distance;
			}
		}
		
		distance = -(disc + b) / (2.0 * a);
		if(distance >= 0.0 && distance <= 1.0){
			y = f3 + rayDirection.y * distance;
			if(y > 0.0 && y < 2.0 * halfHeight){
				if(bestDistance < 0.0 || distance < bestDistance){
					bestDistance = distance;
				}
			}
		}
	}
	
	// test top and bottom caps
	if(fabs(rayDirection.y) > 1e-12){
		disc = 1.0 / rayDirection.y;
		
		// top cap
		distance = RaySphere(localRayOrigin, rayDirection, decDVector(0.0, capsuleHalfHeight, 0.0), capsuleTopRadius);
		if(distance >= 0.0 && distance <= 1.0 && (bestDistance < 0.0 || distance < bestDistance)){
			bestDistance = distance;
		}
		
		// bottom cap
		distance = RaySphere(localRayOrigin, rayDirection, decDVector(0.0, -capsuleHalfHeight, 0.0), capsuleBottomRadius);
		if(distance >= 0.0 && distance <= 1.0 && (bestDistance < 0.0 || distance < bestDistance)){
			bestDistance = distance;
		}
	}
	
	// return result
	if(bestDistance < 0.0){
		return false;
		
	}else{
		hitDistance = bestDistance;
		return true;
	}
}



bool debpDCollisionDetection::RayHitsTriangle(const decDVector &rayOrigin, const decDVector &rayDirection,
const decDVector &tri1, const decDVector &tri2, const decDVector &tri3){
	decDVector edge1 = tri2 - tri1;
	decDVector edge2 = tri3 - tri2;
	decDVector trinormal(edge1 % edge2);
	double dot = rayDirection * trinormal, lambda;
	
	if(dot < -0.00001 || dot > 0.00001){
		lambda = ((tri1 - rayOrigin) * trinormal) / dot;
		
		if(lambda > 0.0 && lambda <= 1.0){
			decDVector hitPoint(rayOrigin + rayDirection * lambda);
			
			if((edge1 % (hitPoint - tri1)) * trinormal < 0.0) return false;
			if((edge2 % (hitPoint - tri2)) * trinormal < 0.0) return false;
			decDVector edge3(tri1 - tri3);
			if((edge3 % (hitPoint - tri3)) * trinormal < 0.0) return false;
			
			return true;
		}
	}
	
	return false;
}

bool debpDCollisionDetection::RayHitsTriangle(const decDVector &rayOrigin, const decDVector &rayDirection,
const decDVector &tri1, const decDVector &tri2, const decDVector &tri3, const decDVector &trinormal){
	double dot = rayDirection * trinormal, lambda;
	
	if(dot < -0.00001 || dot > 0.00001){
		lambda = ((tri1 - rayOrigin) * trinormal) / dot;
		
		if(lambda > 0.0 && lambda <= 1.0){
			decDVector hitPoint(rayOrigin + rayDirection * lambda);
			
			decDVector edge(tri2 - tri1);
			if((edge % (hitPoint - tri1)) * trinormal < 0.0) return false;
			edge = tri3 - tri2;
			if((edge % (hitPoint - tri2)) * trinormal < 0.0) return false;
			edge = tri1 - tri3;
			if((edge % (hitPoint - tri3)) * trinormal < 0.0) return false;
			
			return true;
		}
	}
	
	return false;
}
