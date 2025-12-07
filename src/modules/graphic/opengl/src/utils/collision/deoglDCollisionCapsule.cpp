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

#include "deoglDCollisionDetection.h"
#include "deoglDCollisionSphere.h"
#include "deoglDCollisionCylinder.h"
#include "deoglDCollisionCapsule.h"
#include "deoglDCollisionBox.h"
#include "deoglDCollisionTriangle.h"
#include "deoglDCollisionFrustum.h"
#include "deoglDCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Capsule
////////////////////////////////

// Constructors, destructors
//////////////////////////////

deoglDCollisionCapsule::deoglDCollisionCapsule(){
	pHalfHeight = 0.5;
	pTopRadius = 0.5;
	pBottomRadius = 0.5;
	pTapered = false;
	ClearOrientation();
}

deoglDCollisionCapsule::deoglDCollisionCapsule(const decDVector &center, double halfHeight, double topRadius, double bottomRadius){
	SetPosition(center);
	SetHalfHeight(halfHeight);
	SetTopRadius(topRadius);
	SetBottomRadius(bottomRadius);
	ClearOrientation();
}

deoglDCollisionCapsule::deoglDCollisionCapsule(const decDVector &center, double halfHeight, double topRadius, double bottomRadius, const decQuaternion &orientation){
	SetPosition(center);
	SetHalfHeight(halfHeight);
	SetTopRadius(topRadius);
	SetBottomRadius(bottomRadius);
	SetOrientation(orientation);
}

deoglDCollisionCapsule::~deoglDCollisionCapsule(){
}



// Double Dispatching Calls
/////////////////////////////

bool deoglDCollisionCapsule::VolumeHitsVolume(deoglDCollisionVolume *volume){
	return volume->CapsuleHitsVolume(this);
}

double deoglDCollisionCapsule::VolumeMoveHitsVolume(deoglDCollisionVolume *volume, const decDVector &displacement, decDVector *normal){
	return volume->CapsuleMoveHitsVolume(this, displacement, normal);
}



// First Stage Calls
//////////////////////

bool deoglDCollisionCapsule::SphereHitsVolume(deoglDCollisionSphere *sphere){
	return SphereHitsCapsule(sphere);
}

bool deoglDCollisionCapsule::CylinderHitsVolume(deoglDCollisionCylinder *cylinder){
	return CylinderHitsCapsule(cylinder);
}

bool deoglDCollisionCapsule::CapsuleHitsVolume(deoglDCollisionCapsule *capsule){
	return CapsuleHitsCapsule(capsule);
}

bool deoglDCollisionCapsule::BoxHitsVolume(deoglDCollisionBox *box){
	return box->CapsuleHitsBox(this);
}

bool deoglDCollisionCapsule::TriangleHitsVolume(deoglDCollisionTriangle *triangle){
	return triangle->CapsuleHitsTriangle(this);
}

bool deoglDCollisionCapsule::FrustumHitsVolume(deoglDCollisionFrustum *frustum){
	return frustum->CapsuleHitsFrustum(this);
}



double deoglDCollisionCapsule::SphereMoveHitsVolume(deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return SphereMoveHitsCapsule(sphere, displacement, normal);
}

double deoglDCollisionCapsule::CylinderMoveHitsVolume(deoglDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal){
	return CylinderMoveHitsCapsule(cylinder, displacement, normal);
}

double deoglDCollisionCapsule::CapsuleMoveHitsVolume(deoglDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	return CapsuleMoveHitsCapsule(capsule, displacement, normal);
}

double deoglDCollisionCapsule::BoxMoveHitsVolume(deoglDCollisionBox *box, const decDVector &displacement, decDVector *normal){
	double distance = box->CapsuleMoveHitsBox(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double deoglDCollisionCapsule::TriangleMoveHitsVolume(deoglDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal){
	double distance = triangle->CapsuleMoveHitsTriangle(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double deoglDCollisionCapsule::FrustumMoveHitsVolume(deoglDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal){
	double distance = frustum->CapsuleMoveHitsFrustum(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double deoglDCollisionCapsule::PointMoveHitsVolume(const decDVector &point, const decDVector &displacement, decDVector *normal){
	// TODO
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void deoglDCollisionCapsule::GetEnclosingSphere(deoglDCollisionSphere *sphere){
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

void deoglDCollisionCapsule::GetEnclosingBox(deoglDCollisionBox *box){
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

bool deoglDCollisionCapsule::IsPointInside(const decDVector &point){
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

decDVector deoglDCollisionCapsule::ClosestPointTo(const decDVector &point){
	// todo
	return point;
}

decDVector deoglDCollisionCapsule::NormalAtPoint(const decDVector &point){
	// TODO: implement
	return decDVector(1.0, 0.0, 0.0);
}

bool deoglDCollisionCapsule::RayHitsVolume(const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance){
	if(pOriented){
		if(pTapered){
			return deoglDCollisionDetection::RayHitsTaperedCapsule(WorldToLocal(rayOrigin),
				NormalWorldToLocal(rayDirection), decDVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance);
			
		}else{
			return deoglDCollisionDetection::RayHitsCapsule(WorldToLocal(rayOrigin),
				NormalWorldToLocal(rayDirection), decDVector(), pHalfHeight, pTopRadius,
				hitDistance);
		}
		
	}else{
		if(pTapered){
			return deoglDCollisionDetection::RayHitsTaperedCapsule(rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance);
			
		}else{
			return deoglDCollisionDetection::RayHitsCapsule(rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance);
		}
	}
}



// Visiting
/////////////

void deoglDCollisionCapsule::Visit(deoglDCollisionVolumeVisitor *visitor){
	if(! visitor) DETHROW(deeInvalidParam);
	visitor->VisitCapsule(this);
}



// collision routines
///////////////////////

bool deoglDCollisionCapsule::SphereHitsCapsule(deoglDCollisionSphere *sphere){
	/*
	const decDVector center = sphere->GetCenter();
	return deoglDCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, center, center)
		<= pRadius + sphere->GetRadius();
	*/
	return false;
}

bool deoglDCollisionCapsule::CylinderHitsCapsule(deoglDCollisionCylinder *cylinder){
	return false;
}

bool deoglDCollisionCapsule::CapsuleHitsCapsule(deoglDCollisionCapsule *capsule){
	/*
	return deoglDCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, capsule->pStartPoint, capsule->pEndPoint)
		<= pRadius + capsule->pRadius;
	*/
	return false;
}



double deoglDCollisionCapsule::SphereMoveHitsCapsule(deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoglDCollisionCapsule::CylinderMoveHitsCapsule(deoglDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoglDCollisionCapsule::CapsuleMoveHitsCapsule(deoglDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	return 1;
}



// Management
///////////////

void deoglDCollisionCapsule::SetPosition(const decDVector &position){
	pPosition = position;
}

void deoglDCollisionCapsule::SetOrientation(const decQuaternion &orientation){
	decMatrix matrix = decMatrix::CreateFromQuaternion(orientation);
	
	pOrientation = orientation;
	
	pAxisX.Set(matrix.TransformRight());
	pAxisY.Set(matrix.TransformUp());
	pAxisZ.Set(matrix.TransformView());
	
	pOriented = ! pOrientation.IsEqualTo(decQuaternion());
}

void deoglDCollisionCapsule::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set(1.0, 0.0, 0.0);
	pAxisY.Set(0.0, 1.0, 0.0);
	pAxisZ.Set(0.0, 0.0, 1.0);
	pOriented = false;
}

void deoglDCollisionCapsule::SetHalfHeight(double halfHeight){
	pHalfHeight = decMath::max(halfHeight, 0.0);
}

void deoglDCollisionCapsule::SetTopRadius(double topRadius){
	pTopRadius = decMath::max(topRadius, 0.0);
	pTapered = (fabs(pTopRadius - pBottomRadius) > 1e-12);
}

void deoglDCollisionCapsule::SetBottomRadius(double bottomRadius){
	pBottomRadius = decMath::max(bottomRadius, 0.0);
	pTapered = (fabs(pTopRadius - pBottomRadius) > 1e-12);
}

void deoglDCollisionCapsule::SetRadius(double radius){
	pTopRadius = decMath::max(radius, 0.0);
	pBottomRadius = pTopRadius;
	pTapered = false;
}



decDVector deoglDCollisionCapsule::WorldToLocal(const decDVector &point) const{
	decDVector tp = point - pPosition;
	return decDVector(pAxisX * tp, pAxisY * tp, pAxisZ * tp);
}

decDVector deoglDCollisionCapsule::LocalToWorld(const decDVector &point) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decDVector deoglDCollisionCapsule::NormalWorldToLocal(const decDVector &normal) const{
	return decDVector(pAxisX * normal, pAxisY * normal, pAxisZ * normal);
}

decDVector deoglDCollisionCapsule::NormalLocalToWorld(const decDVector &normal) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}



// Private Functions
//////////////////////

void deoglDCollisionCapsule::pUpdateTaperedParameters(){
}
