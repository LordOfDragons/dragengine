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

#include "decDCollisionDetection.h"
#include "decDCollisionSphere.h"
#include "decDCollisionCylinder.h"
#include "decDCollisionCapsule.h"
#include "decDCollisionBox.h"
#include "decDCollisionTriangle.h"
#include "decDCollisionFrustum.h"
#include "decDCollisionVolumeVisitor.h"
#include "../../exceptions.h"



// Collision Detection Capsule
////////////////////////////////

// Constructors, destructors
//////////////////////////////

decDCollisionCapsule::decDCollisionCapsule(){
	pHalfHeight = 0.5;
	pTopRadius = 0.5;
	pBottomRadius = 0.5;
	pTapered = false;
	ClearOrientation();
}

decDCollisionCapsule::decDCollisionCapsule(const decDVector &center, double halfHeight, double topRadius, double bottomRadius){
	SetPosition(center);
	SetHalfHeight(halfHeight);
	SetTopRadius(topRadius);
	SetBottomRadius(bottomRadius);
	ClearOrientation();
}

decDCollisionCapsule::decDCollisionCapsule(const decDVector &center, double halfHeight, double topRadius, double bottomRadius, const decQuaternion &orientation){
	SetPosition(center);
	SetHalfHeight(halfHeight);
	SetTopRadius(topRadius);
	SetBottomRadius(bottomRadius);
	SetOrientation(orientation);
}

decDCollisionCapsule::~decDCollisionCapsule(){
}



// Double Dispatching Calls
/////////////////////////////

bool decDCollisionCapsule::VolumeHitsVolume(decDCollisionVolume *volume){
	return volume->CapsuleHitsVolume(this);
}

double decDCollisionCapsule::VolumeMoveHitsVolume(decDCollisionVolume *volume, const decDVector &displacement, decDVector *normal){
	return volume->CapsuleMoveHitsVolume(this, displacement, normal);
}



// First Stage Calls
//////////////////////

bool decDCollisionCapsule::SphereHitsVolume(decDCollisionSphere *sphere){
	return SphereHitsCapsule(sphere);
}

bool decDCollisionCapsule::CylinderHitsVolume(decDCollisionCylinder *cylinder){
	return CylinderHitsCapsule(cylinder);
}

bool decDCollisionCapsule::CapsuleHitsVolume(decDCollisionCapsule *capsule){
	return CapsuleHitsCapsule(capsule);
}

bool decDCollisionCapsule::BoxHitsVolume(decDCollisionBox *box){
	return box->CapsuleHitsBox(this);
}

bool decDCollisionCapsule::TriangleHitsVolume(decDCollisionTriangle *triangle){
	return triangle->CapsuleHitsTriangle(this);
}

bool decDCollisionCapsule::FrustumHitsVolume(decDCollisionFrustum *frustum){
	return frustum->CapsuleHitsFrustum(this);
}



double decDCollisionCapsule::SphereMoveHitsVolume(decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return SphereMoveHitsCapsule(sphere, displacement, normal);
}

double decDCollisionCapsule::CylinderMoveHitsVolume(decDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal){
	return CylinderMoveHitsCapsule(cylinder, displacement, normal);
}

double decDCollisionCapsule::CapsuleMoveHitsVolume(decDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	return CapsuleMoveHitsCapsule(capsule, displacement, normal);
}

double decDCollisionCapsule::BoxMoveHitsVolume(decDCollisionBox *box, const decDVector &displacement, decDVector *normal){
	double distance = box->CapsuleMoveHitsBox(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double decDCollisionCapsule::TriangleMoveHitsVolume(decDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal){
	double distance = triangle->CapsuleMoveHitsTriangle(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double decDCollisionCapsule::FrustumMoveHitsVolume(decDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal){
	double distance = frustum->CapsuleMoveHitsFrustum(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double decDCollisionCapsule::PointMoveHitsVolume(const decDVector &point, const decDVector &displacement, decDVector *normal){
	// TODO
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void decDCollisionCapsule::GetEnclosingSphere(decDCollisionSphere *sphere){
	if(! sphere) DETHROW(deeInvalidParam);
	
	// TODO: this is cylinder code => convert to capsule code
	
	// the sphere is not fully optial. the real sphere is slightly smaller and has a
	// different center along the y-axis. the difference is though rather small.
	if(pTopRadius > pBottomRadius){
		sphere->SetAll(pPosition, sqrt(pHalfHeight * pHalfHeight + pTopRadius * pTopRadius));
		
	}else{
		sphere->SetAll(pPosition, sqrt(pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius));
	}
}

void decDCollisionCapsule::GetEnclosingBox(decDCollisionBox *box){
	if(! box) DETHROW(deeInvalidParam);
	double size;
	
	// TODO: this is cylinder code => convert to capsule code
	
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

bool decDCollisionCapsule::IsPointInside(const decDVector &point){
	decDVector localPoint = WorldToLocal(point);
	double radius = pTopRadius;
	double y = localPoint.y;
	
	if(pHalfHeight > 1e-5){
		// this here is not correct if the radi are different. correctly the radius different moves the
		// line segment to check against up or down. this will be done later on
		if(y < -pHalfHeight){
			y = -pHalfHeight;
		}
		if(y > pHalfHeight){
			y = pHalfHeight;
		}
		
		radius = pBottomRadius + (pTopRadius - pBottomRadius) * (y * 0.5 / pHalfHeight + 0.5);
	}
	
	localPoint.y -= y; // localPoint - decDVector(0.0, y, 0.0)
	
	return localPoint.Length() <= radius;
}

decDVector decDCollisionCapsule::ClosestPointTo(const decDVector &point){
	// todo
	return point;
}

decDVector decDCollisionCapsule::NormalAtPoint(const decDVector &point){
	// TODO: implement
	return decDVector(1.0, 0.0, 0.0);
}

bool decDCollisionCapsule::RayHitsVolume(const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance){
	if(pOriented){
		if(pTapered){
			return decDCollisionDetection::RayHitsTaperedCapsule(WorldToLocal(rayOrigin),
				NormalWorldToLocal(rayDirection), decDVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance);
			
		}else{
			return decDCollisionDetection::RayHitsCapsule(WorldToLocal(rayOrigin),
				NormalWorldToLocal(rayDirection), decDVector(), pHalfHeight, pTopRadius,
				hitDistance);
		}
		
	}else{
		if(pTapered){
			return decDCollisionDetection::RayHitsTaperedCapsule(rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance);
			
		}else{
			return decDCollisionDetection::RayHitsCapsule(rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance);
		}
	}
}



// Visiting
/////////////

void decDCollisionCapsule::Visit(decDCollisionVolumeVisitor *visitor){
	if(! visitor) DETHROW(deeInvalidParam);
	visitor->VisitCapsule(this);
}



// collision routines
///////////////////////

bool decDCollisionCapsule::SphereHitsCapsule(decDCollisionSphere *sphere){
	/*
	const decDVector center = sphere->GetCenter();
	return decDCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, center, center)
		<= pRadius + sphere->GetRadius();
	*/
	return false;
}

bool decDCollisionCapsule::CylinderHitsCapsule(decDCollisionCylinder *cylinder){
	return false;
}

bool decDCollisionCapsule::CapsuleHitsCapsule(decDCollisionCapsule *capsule){
	/*
	return decDCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, capsule->pStartPoint, capsule->pEndPoint)
		<= pRadius + capsule->pRadius;
	*/
	return false;
}



double decDCollisionCapsule::SphereMoveHitsCapsule(decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return 1;
}

double decDCollisionCapsule::CylinderMoveHitsCapsule(decDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal){
	return 1;
}

double decDCollisionCapsule::CapsuleMoveHitsCapsule(decDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	return 1;
}



// Management
///////////////

void decDCollisionCapsule::SetPosition(const decDVector &position){
	pPosition = position;
}

void decDCollisionCapsule::SetOrientation(const decQuaternion &orientation){
	decMatrix matrix = decMatrix::CreateFromQuaternion(orientation);
	
	pOrientation = orientation;
	
	pAxisX.Set(matrix.TransformRight());
	pAxisY.Set(matrix.TransformUp());
	pAxisZ.Set(matrix.TransformView());
	
	pOriented = ! pOrientation.IsEqualTo(decQuaternion());
}

void decDCollisionCapsule::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set(1.0, 0.0, 0.0);
	pAxisY.Set(0.0, 1.0, 0.0);
	pAxisZ.Set(0.0, 0.0, 1.0);
	pOriented = false;
}

void decDCollisionCapsule::SetHalfHeight(double halfHeight){
	if(halfHeight < 1e-12){
		pHalfHeight = 0.0;
		
	}else{
		pHalfHeight = halfHeight;
	}
}

void decDCollisionCapsule::SetTopRadius(double topRadius){
	if(topRadius < 1e-12){
		pTopRadius = 0.0;
		
	}else{
		pTopRadius = topRadius;
	}
	
	pTapered = (fabs(pTopRadius - pBottomRadius) > 1e-12);
}

void decDCollisionCapsule::SetBottomRadius(double bottomRadius){
	if(bottomRadius < 1e-12){
		pBottomRadius = 0.0;
		
	}else{
		pBottomRadius = bottomRadius;
	}
	
	pTapered = (fabs(pTopRadius - pBottomRadius) > 1e-12);
}

void decDCollisionCapsule::SetRadius(double radius){
	if(radius < 1e-12){
		pTopRadius = 0.0;
		pBottomRadius = 0.0;
		
	}else{
		pTopRadius = radius;
		pBottomRadius = radius;
	}
	
	pTapered = false;
}



decDVector decDCollisionCapsule::WorldToLocal(const decDVector &point) const{
	decDVector tp = point - pPosition;
	return decDVector(pAxisX * tp, pAxisY * tp, pAxisZ * tp);
}

decDVector decDCollisionCapsule::LocalToWorld(const decDVector &point) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decDVector decDCollisionCapsule::NormalWorldToLocal(const decDVector &normal) const{
	return decDVector(pAxisX * normal, pAxisY * normal, pAxisZ * normal);
}

decDVector decDCollisionCapsule::NormalLocalToWorld(const decDVector &normal) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}



// Private Functions
//////////////////////

void decDCollisionCapsule::pUpdateTaperedParameters(){
}
