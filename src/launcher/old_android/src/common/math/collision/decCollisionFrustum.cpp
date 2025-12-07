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

#include "decCollisionSphere.h"
#include "decCollisionCylinder.h"
#include "decCollisionCapsule.h"
#include "decCollisionBox.h"
#include "decCollisionTriangle.h"
#include "decCollisionFrustum.h"
#include "decCollisionVolumeVisitor.h"
#include "../../exceptions.h"



// Collision Detection Frustum
////////////////////////////////

// constructors, destructors
//////////////////////////////

decCollisionFrustum::decCollisionFrustum(){
	pNormalLeft.Set(1.0f, 0.0f, 0.0f);
	pNormalRight.Set(-1.0f, 0.0f, 0.0f);
	pNormalTop.Set(0.0f, -1.0f, 0.0f);
	pNormalBottom.Set(0.0f, 1.0f, 0.0f);
	pNormalNear.Set(0.0f, 0.0f, 1.0f);
	pNormalFar.Set(0.0f, 0.0f, -1.0f);
	pDistLeft = 0.0f;
	pDistRight = 0.0f;
	pDistTop = 0.0f;
	pDistBottom = 0.0f;
	pDistNear = 0.0f;
	pDistFar = 0.0f;
}

decCollisionFrustum::~decCollisionFrustum(){
}



// float dispatching calls
/////////////////////////////

bool decCollisionFrustum::VolumeHitsVolume(decCollisionVolume *volume){
	return volume->FrustumHitsVolume(this);
}

float decCollisionFrustum::VolumeMoveHitsVolume(decCollisionVolume *volume, const decVector &displacement, decVector *normal){
	return volume->FrustumMoveHitsVolume(this, displacement, normal);
}



// first stage calls
//////////////////////

bool decCollisionFrustum::SphereHitsVolume(decCollisionSphere *sphere){
	return SphereHitsFrustum(sphere);
}

bool decCollisionFrustum::CylinderHitsVolume(decCollisionCylinder *cylinder){
	return CylinderHitsFrustum(cylinder);
}

bool decCollisionFrustum::CapsuleHitsVolume(decCollisionCapsule *capsule){
	return CapsuleHitsFrustum(capsule);
}

bool decCollisionFrustum::BoxHitsVolume(decCollisionBox *box){
	return BoxHitsFrustum(box);
}

bool decCollisionFrustum::TriangleHitsVolume(decCollisionTriangle *triangle){
	return TriangleHitsFrustum(triangle);
}

bool decCollisionFrustum::FrustumHitsVolume(decCollisionFrustum *frustum){
	return FrustumHitsFrustum(frustum);
}



float decCollisionFrustum::SphereMoveHitsVolume(decCollisionSphere *sphere, const decVector &displacement, decVector *normal){
	return SphereMoveHitsFrustum(sphere, displacement, normal);
}

float decCollisionFrustum::CylinderMoveHitsVolume(decCollisionCylinder *cylinder, const decVector &displacement, decVector *normal){
	return CylinderMoveHitsFrustum(cylinder, displacement, normal);
}

float decCollisionFrustum::CapsuleMoveHitsVolume(decCollisionCapsule *capsule, const decVector &displacement, decVector *normal){
	return CapsuleMoveHitsFrustum(capsule, displacement, normal);
}

float decCollisionFrustum::BoxMoveHitsVolume(decCollisionBox *box, const decVector &displacement, decVector *normal){
	return BoxMoveHitsFrustum(box, displacement, normal);
}

float decCollisionFrustum::TriangleMoveHitsVolume(decCollisionTriangle *triangle, const decVector &displacement, decVector *normal){
	return TriangleMoveHitsFrustum(triangle, displacement, normal);
}

float decCollisionFrustum::FrustumMoveHitsVolume(decCollisionFrustum *frustum, const decVector &displacement, decVector *normal){
	return FrustumMoveHitsFrustum(frustum, displacement, normal);
}

float decCollisionFrustum::PointMoveHitsVolume(const decVector &point, const decVector &displacement, decVector *normal){
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void decCollisionFrustum::GetEnclosingSphere(decCollisionSphere *sphere){
	if(! sphere) DETHROW(deeInvalidParam);
	// todo
}

void decCollisionFrustum::GetEnclosingBox(decCollisionBox *box){
	if(! box) DETHROW(deeInvalidParam);
	// todo
}



// Miscelanous Functions
//////////////////////////

bool decCollisionFrustum::IsPointInside(const decVector &point){
	return (pNormalLeft * point >= pDistLeft) && (pNormalTop * point >= pDistTop)
		&& (pNormalRight * point >= pDistRight) && (pNormalBottom * point >= pDistBottom)
		&& (pNormalNear * point >= pDistNear) && (pNormalFar * point >= pDistFar);
}

decVector decCollisionFrustum::ClosestPointTo(const decVector &point){
	decVector result = point;
	float dot;
	
	dot = pNormalLeft * result - pDistLeft;
	if(dot < 0.0f){
		result -= pNormalLeft * dot;
	}
	
	dot = pNormalTop * result - pDistTop;
	if(dot < 0.0f){
		result -= pNormalTop * dot;
	}
	
	dot = pNormalRight * result - pDistRight;
	if(dot < 0.0f){
		result -= pNormalRight * dot;
	}
	
	dot = pNormalBottom * result - pDistBottom;
	if(dot < 0.0f){
		result -= pNormalBottom * dot;
	}
	
	dot = pNormalNear * result - pDistNear;
	if(dot < 0.0f){
		result -= pNormalNear * dot;
	}
	
	dot = pNormalFar * result - pDistFar;
	if(dot < 0.0f){
		result -= pNormalFar * dot;
	}
	
	return result;
}



// Visiting
/////////////

void decCollisionFrustum::Visit(decCollisionVolumeVisitor *visitor){
	if(! visitor) DETHROW(deeInvalidParam);
	visitor->VisitFrustum(this);
}



// collision routines
///////////////////////

bool decCollisionFrustum::SphereHitsFrustum(decCollisionSphere *sphere){
	const decVector &center = sphere->GetCenter();
	const decVector &diff = center - ClosestPointTo(center);
	const float radius = sphere->GetRadius();
	
	return diff * diff < radius * radius;
	
	//return IntersectSphere(sphere) != eitOutside;
}

bool decCollisionFrustum::CylinderHitsFrustum(decCollisionCylinder *cylinder){
	return false;
}

bool decCollisionFrustum::CapsuleHitsFrustum(decCollisionCapsule *capsule){
	return false;
}

bool decCollisionFrustum::BoxHitsFrustum(decCollisionBox *box){
	decVector boxCenter = box->GetCenter();
	decVector boxSize = box->GetHalfSize();
	float nearDot, farDot;
	// near plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalNear.x) +
		boxSize.y * fabsf(pNormalNear.y) + boxSize.z * fabsf(pNormalNear.z);
	farDot = pDistNear - boxCenter * pNormalNear;
	if(farDot > nearDot) return false;
	// far plane (ok)
	nearDot = boxSize.x * fabsf(pNormalFar.x) +
		boxSize.y * fabsf(pNormalFar.y) + boxSize.z * fabsf(pNormalFar.z);
	farDot = pDistFar - boxCenter * pNormalFar;
	if(farDot > nearDot) return false;
	// left plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalLeft.x) +
		boxSize.y * fabsf(pNormalLeft.y) + boxSize.z * fabsf(pNormalLeft.z);
	farDot = pDistLeft - boxCenter * pNormalLeft;
	if(farDot > nearDot) return false;
	// right plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalRight.x) +
		boxSize.y * fabsf(pNormalRight.y) + boxSize.z * fabsf(pNormalRight.z);
	farDot = pDistRight - boxCenter * pNormalRight;
	if(farDot > nearDot) return false;
	// top plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalTop.x) +
		boxSize.y * fabsf(pNormalTop.y) + boxSize.z * fabsf(pNormalTop.z);
	farDot = pDistTop - boxCenter * pNormalTop;
	if(farDot > nearDot) return false;
	// bottom plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalBottom.x) +
		boxSize.y * fabsf(pNormalBottom.y) + boxSize.z * fabsf(pNormalBottom.z);
	farDot = pDistBottom - boxCenter * pNormalBottom;
	if(farDot > nearDot) return false;
	// good
	return true;
}

bool decCollisionFrustum::TriangleHitsFrustum(decCollisionTriangle *triangle){
	return false;
}
/*
bool decCollisionFrustum::OrientedBoxHitsFrustum(decCollisionOrientedBox *box){
	// NOT WORKING
	decVector boxCenter = box->GetCenter();
	decVector boxSize = box->GetHalfSize();
	float nearDot, farDot;
	// near plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalNear.x) +
		boxSize.y * fabsf(pNormalNear.y) + boxSize.z * fabsf(pNormalNear.z);
	farDot = boxCenter * pNormalNear + pDistNear;
	if(nearDot + farDot < 0) return false;
	// far plane (ok)
	nearDot = boxSize.x * fabsf(pNormalFar.x) +
		boxSize.y * fabsf(pNormalFar.y) + boxSize.z * fabsf(pNormalFar.z);
	farDot = boxCenter * pNormalFar + pDistFar;
	if(nearDot + farDot < 0) return false;
	// left plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalLeft.x) +
		boxSize.y * fabsf(pNormalLeft.y) + boxSize.z * fabsf(pNormalLeft.z);
	farDot = boxCenter * pNormalLeft + pDistLeft;
	if(nearDot + farDot < 0) return false;
	// right plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalRight.x) +
		boxSize.y * fabsf(pNormalRight.y) + boxSize.z * fabsf(pNormalRight.z);
	farDot = boxCenter * pNormalRight + pDistRight;
	if(nearDot + farDot < 0) return false;
	// top plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalTop.x) +
		boxSize.y * fabsf(pNormalTop.y) + boxSize.z * fabsf(pNormalTop.z);
	farDot = boxCenter * pNormalTop + pDistTop;
	if(nearDot + farDot < 0) return false;
	// bottom plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalBottom.x) +
		boxSize.y * fabsf(pNormalBottom.y) + boxSize.z * fabsf(pNormalBottom.z);
	farDot = boxCenter * pNormalBottom + pDistBottom;
	if(nearDot + farDot < 0) return false;
	// good
	return true;
}
*/

bool decCollisionFrustum::FrustumHitsFrustum(decCollisionFrustum *frustum){
	return false;
}



float decCollisionFrustum::SphereMoveHitsFrustum(decCollisionSphere *sphere, const decVector &displacement, decVector *normal){
	return 1;
}

float decCollisionFrustum::CylinderMoveHitsFrustum(decCollisionCylinder *cylinder, const decVector &displacement, decVector *normal){
	return 1;
}

float decCollisionFrustum::CapsuleMoveHitsFrustum(decCollisionCapsule *capsule, const decVector &displacement, decVector *normal){
	return 1;
}

float decCollisionFrustum::BoxMoveHitsFrustum(decCollisionBox *box, const decVector &displacement, decVector *normal){
	return 1;
}

float decCollisionFrustum::TriangleMoveHitsFrustum(decCollisionTriangle *triangle, const decVector &displacement, decVector *normal){
	return 1;
}

float decCollisionFrustum::FrustumMoveHitsFrustum(decCollisionFrustum *frustum, const decVector &displacement, decVector *normal){
	return 1;
}



// management
///////////////

void decCollisionFrustum::SetLeftPlane(const decVector &normal, float dist){
	pNormalLeft = normal;
	pDistLeft = dist;
}

void decCollisionFrustum::SetRightPlane(const decVector &normal, float dist){
	pNormalRight = normal;
	pDistRight = dist;
}

void decCollisionFrustum::SetTopPlane(const decVector &normal, float dist){
	pNormalTop = normal;
	pDistTop = dist;
}

void decCollisionFrustum::SetBottomPlane(const decVector &normal, float dist){
	pNormalBottom = normal;
	pDistBottom = dist;
}

void decCollisionFrustum::SetNearPlane(const decVector &normal, float dist){
	pNormalNear = normal;
	pDistNear = dist;
}

void decCollisionFrustum::SetFarPlane(const decVector &normal, float dist){
	pNormalFar = normal;
	pDistFar = dist;
}

void decCollisionFrustum::SetFrustum(const decDMatrix &mat){
	float len;
	// left clipping plane
	pNormalLeft.Set(mat.a41 + mat.a11, mat.a42 + mat.a12, mat.a43 + mat.a13);
	len = pNormalLeft.Length();
	pNormalLeft /= len;
	pDistLeft = -(mat.a44 + mat.a14) / len;
	// right clipping plane
	pNormalRight.Set(mat.a41 - mat.a11, mat.a42 - mat.a12, mat.a43 - mat.a13);
	len = pNormalRight.Length();
	pNormalRight /= len;
	pDistRight = -(mat.a44 - mat.a14) / len;
	// top clipping plane
	pNormalTop.Set(mat.a41 - mat.a21, mat.a42 - mat.a22, mat.a43 - mat.a23);
	len = pNormalTop.Length();
	pNormalTop /= len;
	pDistTop = -(mat.a44 - mat.a24) / len;
	// bottom clipping plane
	pNormalBottom.Set(mat.a41 + mat.a21, mat.a42 + mat.a22, mat.a43 + mat.a23);
	len = pNormalBottom.Length();
	pNormalBottom /= len;
	pDistBottom = -(mat.a44 + mat.a24) / len;
	// near clipping plane
	pNormalNear.Set(mat.a41 + mat.a31, mat.a42 + mat.a32, mat.a43 + mat.a33);
	len = pNormalNear.Length();
	pNormalNear /= len;
	pDistNear = -(mat.a44 + mat.a34) / len;
	// far clipping plane
	pNormalFar.Set(mat.a41 - mat.a31, mat.a42 - mat.a32, mat.a43 - mat.a33);
	len = pNormalFar.Length();
	pNormalFar /= len;
	pDistFar = -(mat.a44 - mat.a34) / len;
}

void decCollisionFrustum::SetFrustum(const decVector &origin, const decVector &r1, const decVector &r2, const decVector &r3, const decVector &r4, float nearDist){
	pNormalTop = (r1 - origin) % (r1 - r2);
	pNormalTop.Normalize();
	pDistTop = pNormalTop * origin;
	
	pNormalRight = (r2 - origin) % (r2 - r3);
	pNormalRight.Normalize();
	pDistRight = pNormalRight * origin;
	
	pNormalBottom = (r3 - origin) % (r3 - r4);
	pNormalBottom.Normalize();
	pDistBottom = pNormalBottom * origin;
	
	pNormalLeft = (r4 - origin) % (r4 - r1);
	pNormalLeft.Normalize();
	pDistLeft = pNormalLeft * origin;
	
	pNormalFar = (r2 - r1) % (r3 - r2);
	pNormalFar.Normalize();
	pDistFar = pNormalFar * r2;
	
	pNormalNear = -pNormalFar;
	pDistNear = pNormalNear * origin + nearDist;
}

void decCollisionFrustum::SetFrustumBox(const decVector &r1, const decVector &r2, const decVector &r3, const decVector &r4, float dist){
	pNormalTop = r3 - r2;
	pNormalTop.Normalize();
	pDistTop = pNormalTop * r2;
	
	pNormalRight = r4 - r3;
	pNormalRight.Normalize();
	pDistRight = pNormalRight * r3;
	
	pNormalBottom = r1 - r4;
	pNormalBottom.Normalize();
	pDistBottom = pNormalBottom * r4;
	
	pNormalLeft = r2 - r1;
	pNormalLeft.Normalize();
	pDistLeft = pNormalLeft * r1;
	
	pNormalNear = (r3 - r2) % (r2 - r1);
	pNormalNear.Normalize();
	pDistNear = pNormalNear * (r1 - pNormalNear * dist);
	
	pNormalFar = -pNormalNear;
	pDistFar = pNormalFar * (r1 + pNormalNear * dist);
}



// tests
//////////

int decCollisionFrustum::IntersectSphere(decCollisionSphere *sphere){
	decVector center = sphere->GetCenter();
	float radius = sphere->GetRadius();
	int result = eitInside;
	float dist;
	
	dist = pNormalLeft * center - pDistLeft;
	if(dist < -radius) return eitOutside;
	if(fabs(dist) < radius) result = eitIntersect;
	
	dist = pNormalRight * center - pDistRight;
	if(dist < -radius) return eitOutside;
	if(fabs(dist) < radius) result = eitIntersect;
	
	dist = pNormalTop * center - pDistTop;
	if(dist < -radius) return eitOutside;
	if(fabs(dist) < radius) result = eitIntersect;
	
	dist = pNormalBottom * center - pDistBottom;
	if(dist < -radius) return eitOutside;
	if(fabs(dist) < radius) result = eitIntersect;
	
	dist = pNormalNear * center - pDistNear;
	if(dist < -radius) return eitOutside;
	if(fabs(dist) < radius) result = eitIntersect;
	
	dist = pNormalFar * center - pDistFar;
	if(dist < -radius) return eitOutside;
	if(fabs(dist) < radius) result = eitIntersect;
	
	return result;
}
