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

#include "decDCollisionSphere.h"
#include "decDCollisionCylinder.h"
#include "decDCollisionCapsule.h"
#include "decDCollisionBox.h"
#include "decDCollisionTriangle.h"
#include "decDCollisionFrustum.h"
#include "decDCollisionDetection.h"
#include "decDCollisionVolumeVisitor.h"
#include "../../exceptions.h"



// Collision Detection Cylinder
/////////////////////////////////

// Constructors, destructors
//////////////////////////////

decDCollisionCylinder::decDCollisionCylinder(){
	pHalfHeight = 0.5;
	pTopRadius = 0.5;
	pBottomRadius = 0.5;
	pTapered = false;
	ClearOrientation();
}

decDCollisionCylinder::decDCollisionCylinder(const decDVector &center, double halfHeight, double topRadius, double bottomRadius){
	SetPosition(center);
	SetHalfHeight(halfHeight);
	SetTopRadius(topRadius);
	SetBottomRadius(bottomRadius);
	ClearOrientation();
}

decDCollisionCylinder::decDCollisionCylinder(const decDVector &center, double halfHeight, double topRadius, double bottomRadius, const decQuaternion &orientation){
	SetPosition(center);
	SetHalfHeight(halfHeight);
	SetTopRadius(topRadius);
	SetBottomRadius(bottomRadius);
	SetOrientation(orientation);
}

decDCollisionCylinder::~decDCollisionCylinder(){
}



// double dispatching calls
/////////////////////////////

bool decDCollisionCylinder::VolumeHitsVolume(decDCollisionVolume *volume){
	return volume->CylinderHitsVolume(this);
}

double decDCollisionCylinder::VolumeMoveHitsVolume(decDCollisionVolume *volume, const decDVector &displacement, decDVector *normal){
	return volume->CylinderMoveHitsVolume(this, displacement, normal);
}



// first stage calls
//////////////////////

bool decDCollisionCylinder::SphereHitsVolume(decDCollisionSphere *sphere){
	return SphereHitsCylinder(sphere);
}

bool decDCollisionCylinder::CylinderHitsVolume(decDCollisionCylinder *Cylinder){
	return CylinderHitsCylinder(Cylinder);
}

bool decDCollisionCylinder::CapsuleHitsVolume(decDCollisionCapsule *capsule){
	return capsule->CylinderHitsCapsule(this);
}

bool decDCollisionCylinder::BoxHitsVolume(decDCollisionBox *box){
	return box->CylinderHitsBox(this);
}

bool decDCollisionCylinder::TriangleHitsVolume(decDCollisionTriangle *triangle){
	return triangle->CylinderHitsTriangle(this);
}

bool decDCollisionCylinder::FrustumHitsVolume(decDCollisionFrustum *frustum){
	return frustum->CylinderHitsFrustum(this);
}



double decDCollisionCylinder::SphereMoveHitsVolume(decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return SphereMoveHitsCylinder(sphere, displacement, normal);
}

double decDCollisionCylinder::CylinderMoveHitsVolume(decDCollisionCylinder *Cylinder, const decDVector &displacement, decDVector *normal){
	return CylinderMoveHitsCylinder(Cylinder, displacement, normal);
}

double decDCollisionCylinder::CapsuleMoveHitsVolume(decDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	return capsule->CylinderMoveHitsCapsule(this, displacement, normal);
}

double decDCollisionCylinder::BoxMoveHitsVolume(decDCollisionBox *box, const decDVector &displacement, decDVector *normal){
	double distance = box->CylinderMoveHitsBox(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double decDCollisionCylinder::TriangleMoveHitsVolume(decDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal){
	double distance = triangle->CylinderMoveHitsTriangle(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double decDCollisionCylinder::FrustumMoveHitsVolume(decDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal){
	double distance = frustum->CylinderMoveHitsFrustum(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double decDCollisionCylinder::PointMoveHitsVolume(const decDVector &point, const decDVector &displacement, decDVector *normal){
	// TODO
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void decDCollisionCylinder::GetEnclosingSphere(decDCollisionSphere *sphere){
	if(! sphere) DETHROW(deeInvalidParam);
	
	// the sphere is not fully optial. the real sphere is slightly smaller and has a
	// different center along the y-axis. the difference is though rather small.
	if(pTopRadius > pBottomRadius){
		sphere->SetAll(pPosition, sqrt(pHalfHeight * pHalfHeight + pTopRadius * pTopRadius));
		
	}else{
		sphere->SetAll(pPosition, sqrt(pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius));
	}
}

void decDCollisionCylinder::GetEnclosingBox(decDCollisionBox *box){
	if(! box) DETHROW(deeInvalidParam);
	double size;
	
	// TODO: better approximation
	if(pTopRadius > pBottomRadius){
		size = sqrt(pHalfHeight * pHalfHeight + pTopRadius * pTopRadius);
		
	}else{
		size = sqrt(pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius);
	}
	
	box->SetCenter(pPosition);
	box->SetHalfSize(decDVector(size, size, size));
	box->ClearOrientation();
}



// Miscelanous Functions
//////////////////////////

bool decDCollisionCylinder::IsPointInside(const decDVector &point){
	const decDVector localPoint = WorldToLocal(point);
	
	if(localPoint.y < -pHalfHeight || localPoint.y > pHalfHeight){
		return false;
	}
	
	double radius = pTopRadius;
	
	if(pHalfHeight > 1e-5){
		radius = pBottomRadius + (pTopRadius - pBottomRadius) * (localPoint.y * 0.5 / pHalfHeight + 0.5);
	}
	
	return sqrt(localPoint.x * localPoint.x + localPoint.z * localPoint.z) <= radius;
}

decDVector decDCollisionCylinder::ClosestPointTo(const decDVector &point){
	// TODO: implement
	return point;
}

decDVector decDCollisionCylinder::NormalAtPoint(const decDVector &point){
	// TODO: implement
	return decDVector(1.0, 0.0, 0.0);
}

bool decDCollisionCylinder::RayHitsVolume(const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance){
	if(pOriented){
		if(pTapered){
			return decDCollisionDetection::RayHitsTaperedCylinder(WorldToLocal(rayOrigin),
				NormalWorldToLocal(rayDirection), decDVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance);
			
		}else{
			return decDCollisionDetection::RayHitsCylinder(WorldToLocal(rayOrigin),
				NormalWorldToLocal(rayDirection), decDVector(), pHalfHeight, pTopRadius,
				hitDistance);
		}
		
	}else{
		if(pTapered){
			return decDCollisionDetection::RayHitsTaperedCylinder(rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance);
			
		}else{
			return decDCollisionDetection::RayHitsCylinder(rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance);
		}
	}
}



// Visiting
/////////////

void decDCollisionCylinder::Visit(decDCollisionVolumeVisitor *visitor){
	if(! visitor) DETHROW(deeInvalidParam);
	visitor->VisitCylinder(this);
}



// collision routines
///////////////////////

bool decDCollisionCylinder::SphereHitsCylinder(decDCollisionSphere *sphere){
	return false;
}

bool decDCollisionCylinder::CylinderHitsCylinder(decDCollisionCylinder *Cylinder){
	return false;
}



double decDCollisionCylinder::SphereMoveHitsCylinder(decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return 1;
}

double decDCollisionCylinder::CylinderMoveHitsCylinder(decDCollisionCylinder *Cylinder, const decDVector &displacement, decDVector *normal){
	return 1;
}



// Management
///////////////

void decDCollisionCylinder::SetPosition(const decDVector &position){
	pPosition = position;
}

void decDCollisionCylinder::SetOrientation(const decQuaternion &orientation){
	decMatrix matrix = decMatrix::CreateFromQuaternion(orientation);
	
	pOrientation = orientation;
	
	pAxisX.Set(matrix.TransformRight());
	pAxisY.Set(matrix.TransformUp());
	pAxisZ.Set(matrix.TransformView());
	
	pOriented = ! pOrientation.IsEqualTo(decQuaternion());
}

void decDCollisionCylinder::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set(1.0, 0.0, 0.0);
	pAxisY.Set(0.0, 1.0, 0.0);
	pAxisZ.Set(0.0, 0.0, 1.0);
	pOriented = false;
}

void decDCollisionCylinder::SetHalfHeight(double halfHeight){
	if(halfHeight < 1e-12){
		pHalfHeight = 0.0;
		
	}else{
		pHalfHeight = halfHeight;
	}
}

void decDCollisionCylinder::SetTopRadius(double topRadius){
	if(topRadius < 1e-12){
		pTopRadius = 0.0;
		
	}else{
		pTopRadius = topRadius;
	}
	
	pTapered = (fabs(pTopRadius - pBottomRadius) > 1e-12);
}

void decDCollisionCylinder::SetBottomRadius(double bottomRadius){
	if(bottomRadius < 1e-12){
		pBottomRadius = 0.0;
		
	}else{
		pBottomRadius = bottomRadius;
	}
	
	pTapered = (fabs(pTopRadius - pBottomRadius) > 1e-12);
}

void decDCollisionCylinder::SetRadius(double radius){
	if(radius < 1e-12){
		pTopRadius = 0.0;
		pBottomRadius = 0.0;
		
	}else{
		pTopRadius = radius;
		pBottomRadius = radius;
	}
	
	pTapered = false;
}



decDVector decDCollisionCylinder::WorldToLocal(const decDVector &point) const{
	decDVector tp = point - pPosition;
	return decDVector(pAxisX * tp, pAxisY * tp, pAxisZ * tp);
}

decDVector decDCollisionCylinder::LocalToWorld(const decDVector &point) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decDVector decDCollisionCylinder::NormalWorldToLocal(const decDVector &normal) const{
	return decDVector(pAxisX * normal, pAxisY * normal, pAxisZ * normal);
}

decDVector decDCollisionCylinder::NormalLocalToWorld(const decDVector &normal) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}
