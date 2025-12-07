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

#include "debpDECollisionDetection.h"
#include "debpCollisionSphere.h"
#include "debpCollisionCylinder.h"
#include "debpCollisionCapsule.h"
#include "debpCollisionBox.h"
#include "debpCollisionTriangle.h"
#include "debpCollisionFrustum.h"
#include "debpCollisionVolumeVisitor.h"

#include <dragengine/common/exceptions.h>



// Collision Detection Capsule
////////////////////////////////

// Constructors, destructors
//////////////////////////////

debpCollisionCapsule::debpCollisionCapsule(){
	pHalfHeight = 0.5f;
	pTopRadius = 0.5f;
	pBottomRadius = 0.5f;
	pTapered = false;
	ClearOrientation();
}

debpCollisionCapsule::debpCollisionCapsule(const decVector &center, float halfHeight, float topRadius, float bottomRadius){
	SetPosition(center);
	SetHalfHeight(halfHeight);
	SetTopRadius(topRadius);
	SetBottomRadius(bottomRadius);
	ClearOrientation();
}

debpCollisionCapsule::debpCollisionCapsule(const decVector &center, float halfHeight, float topRadius, float bottomRadius, const decQuaternion &orientation){
	SetPosition(center);
	SetHalfHeight(halfHeight);
	SetTopRadius(topRadius);
	SetBottomRadius(bottomRadius);
	SetOrientation(orientation);
}

debpCollisionCapsule::~debpCollisionCapsule(){
}



// Double Dispatching Calls
/////////////////////////////

bool debpCollisionCapsule::VolumeHitsVolume(debpCollisionVolume *volume){
	return volume->CapsuleHitsVolume(this);
}

float debpCollisionCapsule::VolumeMoveHitsVolume(debpCollisionVolume *volume, const decVector &displacement, decVector *normal){
	return volume->CapsuleMoveHitsVolume(this, displacement, normal);
}



// First Stage Calls
//////////////////////

bool debpCollisionCapsule::SphereHitsVolume(debpCollisionSphere *sphere){
	return SphereHitsCapsule(sphere);
}

bool debpCollisionCapsule::CylinderHitsVolume(debpCollisionCylinder *cylinder){
	return CylinderHitsCapsule(cylinder);
}

bool debpCollisionCapsule::CapsuleHitsVolume(debpCollisionCapsule *capsule){
	return CapsuleHitsCapsule(capsule);
}

bool debpCollisionCapsule::BoxHitsVolume(debpCollisionBox *box){
	return box->CapsuleHitsBox(this);
}

bool debpCollisionCapsule::TriangleHitsVolume(debpCollisionTriangle *triangle){
	return triangle->CapsuleHitsTriangle(this);
}

bool debpCollisionCapsule::FrustumHitsVolume(debpCollisionFrustum *frustum){
	return frustum->CapsuleHitsFrustum(this);
}



float debpCollisionCapsule::SphereMoveHitsVolume(debpCollisionSphere *sphere, const decVector &displacement, decVector *normal){
	return SphereMoveHitsCapsule(sphere, displacement, normal);
}

float debpCollisionCapsule::CylinderMoveHitsVolume(debpCollisionCylinder *cylinder, const decVector &displacement, decVector *normal){
	return CylinderMoveHitsCapsule(cylinder, displacement, normal);
}

float debpCollisionCapsule::CapsuleMoveHitsVolume(debpCollisionCapsule *capsule, const decVector &displacement, decVector *normal){
	return CapsuleMoveHitsCapsule(capsule, displacement, normal);
}

float debpCollisionCapsule::BoxMoveHitsVolume(debpCollisionBox *box, const decVector &displacement, decVector *normal){
	float distance = box->CapsuleMoveHitsBox(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

float debpCollisionCapsule::TriangleMoveHitsVolume(debpCollisionTriangle *triangle, const decVector &displacement, decVector *normal){
	float distance = triangle->CapsuleMoveHitsTriangle(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

float debpCollisionCapsule::FrustumMoveHitsVolume(debpCollisionFrustum *frustum, const decVector &displacement, decVector *normal){
	float distance = frustum->CapsuleMoveHitsFrustum(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

float debpCollisionCapsule::PointMoveHitsVolume(const decVector &point, const decVector &displacement, decVector *normal){
	// TODO
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void debpCollisionCapsule::GetEnclosingSphere(debpCollisionSphere *sphere){
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

void debpCollisionCapsule::GetEnclosingBox(debpCollisionBox *box){
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

bool debpCollisionCapsule::IsPointInside(const decVector &point){
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

decVector debpCollisionCapsule::ClosestPointTo(const decVector &point){
	// todo
	return point;
}

decVector debpCollisionCapsule::NormalAtPoint(const decVector &point){
	// TODO: implement
	return decVector(1.0, 0.0, 0.0);
}

bool debpCollisionCapsule::RayHitsVolume(const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance){
	if(pOriented){
		if(pTapered){
			return debpDECollisionDetection::RayHitsTaperedCapsule(WorldToLocal(rayOrigin),
				NormalWorldToLocal(rayDirection), decVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance);
			
		}else{
			return debpDECollisionDetection::RayHitsCapsule(WorldToLocal(rayOrigin),
				NormalWorldToLocal(rayDirection), decVector(), pHalfHeight, pTopRadius,
				hitDistance);
		}
		
	}else{
		if(pTapered){
			return debpDECollisionDetection::RayHitsTaperedCapsule(rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance);
			
		}else{
			return debpDECollisionDetection::RayHitsCapsule(rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance);
		}
	}
}



// Visiting
/////////////

void debpCollisionCapsule::Visit(debpCollisionVolumeVisitor *visitor){
	if(! visitor) DETHROW(deeInvalidParam);
	visitor->VisitCapsule(this);
}



// collision routines
///////////////////////

bool debpCollisionCapsule::SphereHitsCapsule(debpCollisionSphere *sphere){
	printf("debpCollisionCapsule::SphereHitsCapsule not implemented yet.\n");
	/*
	const decVector center = sphere->GetCenter();
	return debpDECollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, center, center)
		<= pRadius + sphere->GetRadius();
	*/
	return false;
}

bool debpCollisionCapsule::CylinderHitsCapsule(debpCollisionCylinder *cylinder){
	printf("debpCollisionCapsule::CylinderHitsCapsule not implemented yet.\n");
	return false;
}

bool debpCollisionCapsule::CapsuleHitsCapsule(debpCollisionCapsule *capsule){
	printf("debpCollisionCapsule::CapsuleHitsCapsule not implemented yet.\n");
	/*
	return debpDECollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, capsule->pStartPoint, capsule->pEndPoint)
		<= pRadius + capsule->pRadius;
	*/
	return false;
}



float debpCollisionCapsule::SphereMoveHitsCapsule(debpCollisionSphere *sphere, const decVector &displacement, decVector *normal){
	printf("debpCollisionCapsule::SphereMoveHitsCapsule not implemented yet.\n");
	return 1.0f;
}

float debpCollisionCapsule::CylinderMoveHitsCapsule(debpCollisionCylinder *cylinder, const decVector &displacement, decVector *normal){
	printf("debpCollisionCapsule::CylinderMoveHitsCapsule not implemented yet.\n");
	return 1.0f;
}

float debpCollisionCapsule::CapsuleMoveHitsCapsule(debpCollisionCapsule *capsule, const decVector &displacement, decVector *normal){
	printf("debpCollisionCapsule::CapsuleMoveHitsCapsule not implemented yet.\n");
	return 1.0f;
}



// Management
///////////////

void debpCollisionCapsule::SetPosition(const decVector &position){
	pPosition = position;
}

void debpCollisionCapsule::SetOrientation(const decQuaternion &orientation){
	decMatrix matrix = decMatrix::CreateFromQuaternion(orientation);
	
	pOrientation = orientation;
	
	pAxisX.Set(matrix.TransformRight());
	pAxisY.Set(matrix.TransformUp());
	pAxisZ.Set(matrix.TransformView());
	
	pOriented = ! pOrientation.IsEqualTo(decQuaternion());
}

void debpCollisionCapsule::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set(1.0f, 0.0f, 0.0f);
	pAxisY.Set(0.0f, 1.0f, 0.0f);
	pAxisZ.Set(0.0f, 0.0f, 1.0f);
	pOriented = false;
}

void debpCollisionCapsule::SetHalfHeight(float halfHeight){
	pHalfHeight = decMath::max(halfHeight, 0.0f);
}

void debpCollisionCapsule::SetTopRadius(float topRadius){
	pTopRadius = decMath::max(topRadius, 0.0f);
	pTapered = (fabsf(pTopRadius - pBottomRadius) > 1e-6f);
}

void debpCollisionCapsule::SetBottomRadius(float bottomRadius){
	pBottomRadius = decMath::max(bottomRadius, 0.0f);
	pTapered = (fabsf(pTopRadius - pBottomRadius) > 1e-6f);
}

void debpCollisionCapsule::SetRadius(float radius){
	pTopRadius = decMath::max(radius, 0.0f);
	pBottomRadius = pTopRadius;
	pTapered = false;
}



decVector debpCollisionCapsule::WorldToLocal(const decVector &point) const{
	decVector tp = point - pPosition;
	return decVector(pAxisX * tp, pAxisY * tp, pAxisZ * tp);
}

decVector debpCollisionCapsule::LocalToWorld(const decVector &point) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decVector debpCollisionCapsule::NormalWorldToLocal(const decVector &normal) const{
	return decVector(pAxisX * normal, pAxisY * normal, pAxisZ * normal);
}

decVector debpCollisionCapsule::NormalLocalToWorld(const decVector &normal) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}



// Private Functions
//////////////////////

void debpCollisionCapsule::pUpdateTaperedParameters(){
}
