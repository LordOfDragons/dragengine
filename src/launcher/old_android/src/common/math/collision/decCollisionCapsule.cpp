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

#include "decCollisionDetection.h"
#include "decCollisionSphere.h"
#include "decCollisionCylinder.h"
#include "decCollisionCapsule.h"
#include "decCollisionBox.h"
#include "decCollisionTriangle.h"
#include "decCollisionFrustum.h"
#include "decCollisionVolumeVisitor.h"
#include "../../exceptions.h"



// Collision Detection Capsule
////////////////////////////////

// Constructors, destructors
//////////////////////////////

decCollisionCapsule::decCollisionCapsule(){
	pHalfHeight = 0.5f;
	pTopRadius = 0.5f;
	pBottomRadius = 0.5f;
	pTapered = false;
	ClearOrientation();
}

decCollisionCapsule::decCollisionCapsule(const decVector &center, float halfHeight, float topRadius, float bottomRadius){
	SetPosition(center);
	SetHalfHeight(halfHeight);
	SetTopRadius(topRadius);
	SetBottomRadius(bottomRadius);
	ClearOrientation();
}

decCollisionCapsule::decCollisionCapsule(const decVector &center, float halfHeight, float topRadius, float bottomRadius, const decQuaternion &orientation){
	SetPosition(center);
	SetHalfHeight(halfHeight);
	SetTopRadius(topRadius);
	SetBottomRadius(bottomRadius);
	SetOrientation(orientation);
}

decCollisionCapsule::~decCollisionCapsule(){
}



// Double Dispatching Calls
/////////////////////////////

bool decCollisionCapsule::VolumeHitsVolume(decCollisionVolume *volume){
	return volume->CapsuleHitsVolume(this);
}

float decCollisionCapsule::VolumeMoveHitsVolume(decCollisionVolume *volume, const decVector &displacement, decVector *normal){
	return volume->CapsuleMoveHitsVolume(this, displacement, normal);
}



// First Stage Calls
//////////////////////

bool decCollisionCapsule::SphereHitsVolume(decCollisionSphere *sphere){
	return SphereHitsCapsule(sphere);
}

bool decCollisionCapsule::CylinderHitsVolume(decCollisionCylinder *cylinder){
	return CylinderHitsCapsule(cylinder);
}

bool decCollisionCapsule::CapsuleHitsVolume(decCollisionCapsule *capsule){
	return CapsuleHitsCapsule(capsule);
}

bool decCollisionCapsule::BoxHitsVolume(decCollisionBox *box){
	return box->CapsuleHitsBox(this);
}

bool decCollisionCapsule::TriangleHitsVolume(decCollisionTriangle *triangle){
	return triangle->CapsuleHitsTriangle(this);
}

bool decCollisionCapsule::FrustumHitsVolume(decCollisionFrustum *frustum){
	return frustum->CapsuleHitsFrustum(this);
}



float decCollisionCapsule::SphereMoveHitsVolume(decCollisionSphere *sphere, const decVector &displacement, decVector *normal){
	return SphereMoveHitsCapsule(sphere, displacement, normal);
}

float decCollisionCapsule::CylinderMoveHitsVolume(decCollisionCylinder *cylinder, const decVector &displacement, decVector *normal){
	return CylinderMoveHitsCapsule(cylinder, displacement, normal);
}

float decCollisionCapsule::CapsuleMoveHitsVolume(decCollisionCapsule *capsule, const decVector &displacement, decVector *normal){
	return CapsuleMoveHitsCapsule(capsule, displacement, normal);
}

float decCollisionCapsule::BoxMoveHitsVolume(decCollisionBox *box, const decVector &displacement, decVector *normal){
	float distance = box->CapsuleMoveHitsBox(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

float decCollisionCapsule::TriangleMoveHitsVolume(decCollisionTriangle *triangle, const decVector &displacement, decVector *normal){
	float distance = triangle->CapsuleMoveHitsTriangle(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

float decCollisionCapsule::FrustumMoveHitsVolume(decCollisionFrustum *frustum, const decVector &displacement, decVector *normal){
	float distance = frustum->CapsuleMoveHitsFrustum(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

float decCollisionCapsule::PointMoveHitsVolume(const decVector &point, const decVector &displacement, decVector *normal){
	// TODO
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void decCollisionCapsule::GetEnclosingSphere(decCollisionSphere *sphere){
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

void decCollisionCapsule::GetEnclosingBox(decCollisionBox *box){
	if(! box) DETHROW(deeInvalidParam);
	float size;
	
	// TODO: this is cylinder code => convert to capsule code
	
	// TODO: better approximation
	if(pTopRadius > pBottomRadius){
		size = sqrt(pHalfHeight * pHalfHeight + pTopRadius * pTopRadius);
		
	}else{
		size = sqrt(pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius);
	}
	
	box->SetCenter(pPosition);
	box->SetHalfSize(decVector(size, size, size));
	box->ClearOrientation();
}



// Miscelanous Functions
//////////////////////////

bool decCollisionCapsule::IsPointInside(const decVector &point){
	decVector localPoint = WorldToLocal(point);
	float radius = pTopRadius;
	float y = localPoint.y;
	
	if(pHalfHeight > 1e-5f){
		// this here is not correct if the radi are different. correctly the radius different moves the
		// line segment to check against up or down. this will be done later on
		if(y < -pHalfHeight){
			y = -pHalfHeight;
		}
		if(y > pHalfHeight){
			y = pHalfHeight;
		}
		
		radius = pBottomRadius + (pTopRadius - pBottomRadius) * (y * 0.5f / pHalfHeight + 0.5f);
	}
	
	localPoint.y -= y; // localPoint - decDVector(0.0f, y, 0.0f)
	
	return localPoint.Length() <= radius;
}

decVector decCollisionCapsule::ClosestPointTo(const decVector &point){
	// todo
	return point;
}

decVector decCollisionCapsule::NormalAtPoint(const decVector &point){
	// TODO: implement
	return decVector(1.0, 0.0, 0.0);
}

bool decCollisionCapsule::RayHitsVolume(const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance){
	if(pOriented){
		if(pTapered){
			return decCollisionDetection::RayHitsTaperedCapsule(WorldToLocal(rayOrigin),
				NormalWorldToLocal(rayDirection), decVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance);
			
		}else{
			return decCollisionDetection::RayHitsCapsule(WorldToLocal(rayOrigin),
				NormalWorldToLocal(rayDirection), decVector(), pHalfHeight, pTopRadius,
				hitDistance);
		}
		
	}else{
		if(pTapered){
			return decCollisionDetection::RayHitsTaperedCapsule(rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance);
			
		}else{
			return decCollisionDetection::RayHitsCapsule(rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance);
		}
	}
}



// Visiting
/////////////

void decCollisionCapsule::Visit(decCollisionVolumeVisitor *visitor){
	if(! visitor) DETHROW(deeInvalidParam);
	visitor->VisitCapsule(this);
}



// collision routines
///////////////////////

bool decCollisionCapsule::SphereHitsCapsule(decCollisionSphere *sphere){
	printf("decCollisionCapsule::SphereHitsCapsule not implemented yet.\n");
	/*
	const decVector center = sphere->GetCenter();
	return decCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, center, center)
		<= pRadius + sphere->GetRadius();
	*/
	return false;
}

bool decCollisionCapsule::CylinderHitsCapsule(decCollisionCylinder *cylinder){
	printf("decCollisionCapsule::CylinderHitsCapsule not implemented yet.\n");
	return false;
}

bool decCollisionCapsule::CapsuleHitsCapsule(decCollisionCapsule *capsule){
	printf("decCollisionCapsule::CapsuleHitsCapsule not implemented yet.\n");
	/*
	return decCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, capsule->pStartPoint, capsule->pEndPoint)
		<= pRadius + capsule->pRadius;
	*/
	return false;
}



float decCollisionCapsule::SphereMoveHitsCapsule(decCollisionSphere *sphere, const decVector &displacement, decVector *normal){
	printf("decCollisionCapsule::SphereMoveHitsCapsule not implemented yet.\n");
	return 1.0f;
}

float decCollisionCapsule::CylinderMoveHitsCapsule(decCollisionCylinder *cylinder, const decVector &displacement, decVector *normal){
	printf("decCollisionCapsule::CylinderMoveHitsCapsule not implemented yet.\n");
	return 1.0f;
}

float decCollisionCapsule::CapsuleMoveHitsCapsule(decCollisionCapsule *capsule, const decVector &displacement, decVector *normal){
	printf("decCollisionCapsule::CapsuleMoveHitsCapsule not implemented yet.\n");
	return 1.0f;
}



// Management
///////////////

void decCollisionCapsule::SetPosition(const decVector &position){
	pPosition = position;
}

void decCollisionCapsule::SetOrientation(const decQuaternion &orientation){
	decMatrix matrix = decMatrix::CreateFromQuaternion(orientation);
	
	pOrientation = orientation;
	
	pAxisX.Set(matrix.TransformRight());
	pAxisY.Set(matrix.TransformUp());
	pAxisZ.Set(matrix.TransformView());
	
	pOriented = ! pOrientation.IsEqualTo(decQuaternion());
}

void decCollisionCapsule::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set(1.0f, 0.0f, 0.0f);
	pAxisY.Set(0.0f, 1.0f, 0.0f);
	pAxisZ.Set(0.0f, 0.0f, 1.0f);
	pOriented = false;
}

void decCollisionCapsule::SetHalfHeight(float halfHeight){
	if(halfHeight < 1e-6f){
		pHalfHeight = 0.0f;
		
	}else{
		pHalfHeight = halfHeight;
	}
}

void decCollisionCapsule::SetTopRadius(float topRadius){
	if(topRadius < 1e-6f){
		pTopRadius = 0.0f;
		
	}else{
		pTopRadius = topRadius;
	}
	
	pTapered = (fabsf(pTopRadius - pBottomRadius) > 1e-6f);
}

void decCollisionCapsule::SetBottomRadius(float bottomRadius){
	if(bottomRadius < 1e-6f){
		pBottomRadius = 0.0f;
		
	}else{
		pBottomRadius = bottomRadius;
	}
	
	pTapered = (fabsf(pTopRadius - pBottomRadius) > 1e-6f);
}

void decCollisionCapsule::SetRadius(float radius){
	if(radius < 1e-6f){
		pTopRadius = 0.0f;
		pBottomRadius = 0.0f;
		
	}else{
		pTopRadius = radius;
		pBottomRadius = radius;
	}
	
	pTapered = false;
}



decVector decCollisionCapsule::WorldToLocal(const decVector &point) const{
	decVector tp = point - pPosition;
	return decVector(pAxisX * tp, pAxisY * tp, pAxisZ * tp);
}

decVector decCollisionCapsule::LocalToWorld(const decVector &point) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decVector decCollisionCapsule::NormalWorldToLocal(const decVector &normal) const{
	return decVector(pAxisX * normal, pAxisY * normal, pAxisZ * normal);
}

decVector decCollisionCapsule::NormalLocalToWorld(const decVector &normal) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}



// Private Functions
//////////////////////

void decCollisionCapsule::pUpdateTaperedParameters(){
}
