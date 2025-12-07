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

#include "deoglDCollisionSphere.h"
#include "deoglDCollisionCylinder.h"
#include "deoglDCollisionCapsule.h"
#include "deoglDCollisionBox.h"
#include "deoglDCollisionTriangle.h"
#include "deoglDCollisionFrustum.h"
#include "deoglDCollisionDetection.h"
#include "deoglDCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Sphere
///////////////////////////////

// constructors, destructors
//////////////////////////////

deoglDCollisionSphere::deoglDCollisionSphere(){
	SetAll(decDVector(0, 0, 0), 1);
}

deoglDCollisionSphere::deoglDCollisionSphere(const decDVector &center, double radius){
	SetAll(center, radius);
}

deoglDCollisionSphere::~deoglDCollisionSphere(){
}



// double dispatching calls
/////////////////////////////

bool deoglDCollisionSphere::VolumeHitsVolume(deoglDCollisionVolume *volume){
	return volume->SphereHitsVolume(this);
}

double deoglDCollisionSphere::VolumeMoveHitsVolume(deoglDCollisionVolume *volume, const decDVector &displacement, decDVector *normal){
	return volume->SphereMoveHitsVolume(this, displacement, normal);
}



// first stage calls
//////////////////////

bool deoglDCollisionSphere::SphereHitsVolume(deoglDCollisionSphere *sphere){
	return SphereHitsSphere(sphere);
}

bool deoglDCollisionSphere::CylinderHitsVolume(deoglDCollisionCylinder *cylinder){
	return cylinder->SphereHitsCylinder(this);
}

bool deoglDCollisionSphere::CapsuleHitsVolume(deoglDCollisionCapsule *capsule){
	return capsule->SphereHitsCapsule(this);
}

bool deoglDCollisionSphere::BoxHitsVolume(deoglDCollisionBox *box){
	return box->SphereHitsBox(this);
}

bool deoglDCollisionSphere::TriangleHitsVolume(deoglDCollisionTriangle *triangle){
	return triangle->SphereHitsTriangle(this);
}

bool deoglDCollisionSphere::FrustumHitsVolume(deoglDCollisionFrustum *frustum){
	return frustum->SphereHitsFrustum(this);
}



double deoglDCollisionSphere::SphereMoveHitsVolume(deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return SphereMoveHitsSphere(sphere, displacement, normal);
}

double deoglDCollisionSphere::CylinderMoveHitsVolume(deoglDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal){
	double distance = cylinder->SphereMoveHitsCylinder(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double deoglDCollisionSphere::CapsuleMoveHitsVolume(deoglDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	double distance = capsule->SphereMoveHitsCapsule(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double deoglDCollisionSphere::BoxMoveHitsVolume(deoglDCollisionBox *box, const decDVector &displacement, decDVector *normal){
	double distance = box->SphereMoveHitsBox(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double deoglDCollisionSphere::TriangleMoveHitsVolume(deoglDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal){
	double distance = triangle->SphereMoveHitsTriangle(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double deoglDCollisionSphere::FrustumMoveHitsVolume(deoglDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal){
	double distance = frustum->SphereMoveHitsFrustum(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double deoglDCollisionSphere::PointMoveHitsVolume(const decDVector &point, const decDVector &displacement, decDVector *normal){
	double lambda = ((pCenter - point) * displacement) / (displacement * displacement);
	if(lambda < 0 || lambda > 1){
		return 1;
	}else{
		if(normal){
			*normal = point - pCenter;
			normal->Normalize();
		}
		return lambda;
	}
}



// Enclosing Volumes
//////////////////////

void deoglDCollisionSphere::GetEnclosingSphere(deoglDCollisionSphere *sphere){
	if(!sphere) DETHROW(deeInvalidParam);
	sphere->SetAll(pCenter, pRadius);
}

void deoglDCollisionSphere::GetEnclosingBox(deoglDCollisionBox *box){
	if(!box) DETHROW(deeInvalidParam);
	box->SetCenter(pCenter);
	box->SetHalfSize(decDVector(pRadius, pRadius, pRadius));
}



// Miscelanous Functions
//////////////////////////

bool deoglDCollisionSphere::IsPointInside(const decDVector &point){
	return (point - pCenter).Length() <= pRadius;
}

decDVector deoglDCollisionSphere::ClosestPointTo(const decDVector &point){
	decDVector result = point - pCenter;
	double length = result.Length();
	if(length <= pRadius){
		return point;
	}else{
		return (result / length) * pRadius;
	}
}

decDVector deoglDCollisionSphere::NormalAtPoint(const decDVector &point){
	decDVector normal = point - pCenter;
	double length = normal.Length();
	
	if(length < 1e-5){
		return decDVector(1.0, 0.0, 0.0);
	}
	
	normal.Normalize();
	return normal;
}

bool deoglDCollisionSphere::RayHitsVolume(const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance){
	return deoglDCollisionDetection::RayHitsSphere(rayOrigin, rayDirection, pCenter, pRadius, hitDistance);
}



// Visiting
/////////////

void deoglDCollisionSphere::Visit(deoglDCollisionVolumeVisitor *visitor){
	if(!visitor) DETHROW(deeInvalidParam);
	visitor->VisitSphere(this);
}



// collision routines
///////////////////////

bool deoglDCollisionSphere::SphereHitsSphere(deoglDCollisionSphere *sphere){
	decDVector centerDist = pCenter - sphere->GetCenter();
	double radiusDist = pRadius + sphere->GetRadius();
	return (centerDist * centerDist) <= radiusDist * radiusDist;
}

double deoglDCollisionSphere::SphereMoveHitsSphere(deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	const decDVector &sc1 = sphere->GetCenter();
	decDVector diff(sc1 - pCenter);
	double a, b, r, rr, disc, lambda, factor;
	// check for a collision at the beginning
	r = sphere->GetRadius() + pRadius;
	factor = diff * diff;
	rr = r * r;
	if(factor <= rr){
		//printf( "[DEBUG] %i\n", __LINE__ );
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
		normal->Set(sc1 + displacement * lambda - pCenter);
		normal->Normalize();
	}
	//printf( "[DEBUG] %i, %f\n", __LINE__, lambda );
	return lambda;
}



// management
///////////////

void deoglDCollisionSphere::SetCenter(const decDVector &center){
	pCenter = center;
}

void deoglDCollisionSphere::SetRadius(double radius){
	pRadius = decMath::max(radius, 0.0);
	pSquareRadius = radius * radius;
}

void deoglDCollisionSphere::SetAll(const decDVector &center, double radius){
	pCenter = center;
	pRadius = decMath::max(radius, 0.0);
	pSquareRadius = radius * radius;
}
