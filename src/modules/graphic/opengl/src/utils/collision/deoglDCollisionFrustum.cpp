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
#include "deoglDCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Frustum
////////////////////////////////

// constructors, destructors
//////////////////////////////

deoglDCollisionFrustum::deoglDCollisionFrustum(){
	pPlane[epLeft].normal.Set(1.0, 0.0, 0.0);
	pPlane[epRight].normal.Set(-1.0, 0.0, 0.0);
	pPlane[epTop].normal.Set(0.0, -1.0, 0.0);
	pPlane[epBottom].normal.Set(0.0, 1.0, 0.0);
	pPlane[epNear].normal.Set(0.0, 0.0, 1.0);
	pPlane[epFar].normal.Set(0.0, 0.0, -1.0);
	pPlane[epLeft].absNormal.Set(1.0, 0.0, 0.0);
	pPlane[epRight].absNormal.Set(1.0, 0.0, 0.0);
	pPlane[epTop].absNormal.Set(0.0, 1.0, 0.0);
	pPlane[epBottom].absNormal.Set(0.0, 1.0, 0.0);
	pPlane[epNear].absNormal.Set(0.0, 0.0, 1.0);
	pPlane[epFar].absNormal.Set(0.0, 0.0, 1.0);
	pPlane[epLeft].distance = 0.0;
	pPlane[epRight].distance = 0.0;
	pPlane[epTop].distance = 0.0;
	pPlane[epBottom].distance = 0.0;
	pPlane[epNear].distance = 0.0;
	pPlane[epFar].distance = 0.0;
}

deoglDCollisionFrustum::~deoglDCollisionFrustum(){
}



// double dispatching calls
/////////////////////////////

bool deoglDCollisionFrustum::VolumeHitsVolume(deoglDCollisionVolume *volume){
	return volume->FrustumHitsVolume(this);
}

double deoglDCollisionFrustum::VolumeMoveHitsVolume(deoglDCollisionVolume *volume, const decDVector &displacement, decDVector *normal){
	return volume->FrustumMoveHitsVolume(this, displacement, normal);
}



// first stage calls
//////////////////////

bool deoglDCollisionFrustum::SphereHitsVolume(deoglDCollisionSphere *sphere){
	return SphereHitsFrustum(sphere);
}

bool deoglDCollisionFrustum::CylinderHitsVolume(deoglDCollisionCylinder *cylinder){
	return CylinderHitsFrustum(cylinder);
}

bool deoglDCollisionFrustum::CapsuleHitsVolume(deoglDCollisionCapsule *capsule){
	return CapsuleHitsFrustum(capsule);
}

bool deoglDCollisionFrustum::BoxHitsVolume(deoglDCollisionBox *box){
	return BoxHitsFrustum(box);
}

bool deoglDCollisionFrustum::TriangleHitsVolume(deoglDCollisionTriangle *triangle){
	return TriangleHitsFrustum(triangle);
}

bool deoglDCollisionFrustum::FrustumHitsVolume(deoglDCollisionFrustum *frustum){
	return FrustumHitsFrustum(frustum);
}



double deoglDCollisionFrustum::SphereMoveHitsVolume(deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return SphereMoveHitsFrustum(sphere, displacement, normal);
}

double deoglDCollisionFrustum::CylinderMoveHitsVolume(deoglDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal){
	return CylinderMoveHitsFrustum(cylinder, displacement, normal);
}

double deoglDCollisionFrustum::CapsuleMoveHitsVolume(deoglDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	return CapsuleMoveHitsFrustum(capsule, displacement, normal);
}

double deoglDCollisionFrustum::BoxMoveHitsVolume(deoglDCollisionBox *box, const decDVector &displacement, decDVector *normal){
	return BoxMoveHitsFrustum(box, displacement, normal);
}

double deoglDCollisionFrustum::TriangleMoveHitsVolume(deoglDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal){
	return TriangleMoveHitsFrustum(triangle, displacement, normal);
}

double deoglDCollisionFrustum::FrustumMoveHitsVolume(deoglDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal){
	return FrustumMoveHitsFrustum(frustum, displacement, normal);
}

double deoglDCollisionFrustum::PointMoveHitsVolume(const decDVector &point, const decDVector &displacement, decDVector *normal){
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void deoglDCollisionFrustum::GetEnclosingSphere(deoglDCollisionSphere *sphere){
	// not possible without complex plane intersection calculations
}

void deoglDCollisionFrustum::GetEnclosingBox(deoglDCollisionBox *box){
	// not possible without complex plane intersection calculations
	/*
	p1:
	p1 * normalRight = distanceRight
	p1 * normalTop = distanceTop
	p1 * normalFar = distanceFar
	
	x*nrx + y*nry + z*nrz = dr
	x*ntx + y*nty + z*ntz = dt
	x*nfx + y*nfy + z*nfz = df
	
	
	
	x*a + y*b + z*c = d
	x*e + y*f + z*g = h
	x*i + y*j + z*k = l
	
	x*a + y*b + z*c = d
	y*(b*e - f*a) + z*(c*e - g*a) = d*e - h*a
	x*i + y*j + z*k = l
	
	x*a + y*b + z*c = d
	y*(b*e-f*a) + z*(c*e-g*a) = d*e-h*a
	y*(b*i-j*a) + z*(c*i-k*a) = d*i-l*a
	
	xa + yb + zc = d
	y(be-fa) + z(ce-ga) = de-ha
	z((ce-ga)(bi-ja) - (ci-ka)(be-fa)) = (de-ha)(bi-ja) - (di-la)(be-fa)
	
	z = [(de-ha)(bi-ja) - (di-la)(be-fa)] / [(ce-ga)(bi-ja) - (ci-ka)(be-fa)]
	y = [de-ha - z(ce-ga)] / [be-fa]
	x = [d - yb - zc] / a
*/
}



// Miscelanous Functions
//////////////////////////

bool deoglDCollisionFrustum::IsPointInside(const decDVector &point){
	int i;
	for(i=0; i<6; i++){
		if(pPlane[i].normal * point < pPlane[i].distance){
			return false;
		}
	}
	return true;
}

decDVector deoglDCollisionFrustum::ClosestPointTo(const decDVector &point){
	decDVector result(point);
	int i;
	
	for(i=0; i<6; i++){
		const double dot = pPlane[i].normal * result - pPlane[i].distance;
		if(dot < 0.0){
			result -= pPlane[i].normal * dot;
		}
	}
	
	return result;
}



// Visiting
/////////////

void deoglDCollisionFrustum::Visit(deoglDCollisionVolumeVisitor *visitor){
	if(! visitor) DETHROW(deeInvalidParam);
	visitor->VisitFrustum(this);
}



// collision routines
///////////////////////

bool deoglDCollisionFrustum::SphereHitsFrustum(deoglDCollisionSphere *sphere){
	const decDVector &center = sphere->GetCenter();
	const decDVector &diff = center - ClosestPointTo(center);
	const double radius = sphere->GetRadius();
	
	return diff * diff < radius * radius;
	
	//return IntersectSphere(sphere) != eitOutside;
}

bool deoglDCollisionFrustum::CylinderHitsFrustum(deoglDCollisionCylinder *cylinder){
	return false;
}

bool deoglDCollisionFrustum::CapsuleHitsFrustum(deoglDCollisionCapsule *capsule){
	return false;
}

bool deoglDCollisionFrustum::BoxHitsFrustum(deoglDCollisionBox *box){
	return BoxHits(box->GetCenter() - box->GetHalfSize(), box->GetCenter() + box->GetHalfSize());
}

bool deoglDCollisionFrustum::BoxHits(const decDVector &minExtend, const decDVector &maxExtend) const{
	int i;
	
	for(i=0; i<6; i++){ 
		const decDVector vp(
			pPlane[i].normal.x > 0.0 ? maxExtend.x : minExtend.x,
			pPlane[i].normal.y > 0.0 ? maxExtend.y : minExtend.y,
			pPlane[i].normal.z > 0.0 ? maxExtend.z : minExtend.z);
		
		if(pPlane[i].normal * vp < pPlane[i].distance){
			return false;
		}
	} 
	
	return true;
}

deoglDCollisionFrustum::eIntersectType deoglDCollisionFrustum::BoxIntersect(
const decDVector &minExtend, const decDVector &maxExtend) const{
	eIntersectType intersect = eitInside;
	decDVector vn, vp;
	int i;
	
	for(i=0; i<6; i++){
		if(pPlane[i].normal.x > 0.0){
			vn.x = minExtend.x;
			vp.x = maxExtend.x;
			
		}else{
			vn.x = maxExtend.x;
			vp.x = minExtend.x;
		}
		
		if(pPlane[i].normal.y > 0.0){
			vn.y = minExtend.y;
			vp.y = maxExtend.y;
			
		}else{ 
			vn.y = maxExtend.y;
			vp.y = minExtend.y;
		}
		
		if(pPlane[i].normal.z > 0.0){
			vn.z = minExtend.z;
			vp.z = maxExtend.z;
			
		}else{
			vn.z = maxExtend.z;
			vp.z = minExtend.z;
		}
		
		if(pPlane[i].normal * vp < pPlane[i].distance){
			return eitOutside;
			
		}else if(pPlane[i].normal * vn < pPlane[i].distance){
			intersect = eitIntersect;
		}
	}
	
	return intersect;
}

bool deoglDCollisionFrustum::TriangleHitsFrustum(deoglDCollisionTriangle *triangle){
	return false;
}
/*
bool deoglDCollisionFrustum::OrientedBoxHitsFrustum(deoglDCollisionOrientedBox *box){
	// NOT WORKING
	decDVector boxCenter = box->GetCenter();
	decDVector boxSize = box->GetHalfSize();
	double nearDot, farDot;
	// near plane (wrong)
	nearDot = boxSize.x * fabs(pPlane[epNear].normal.x) +
		boxSize.y * fabs(pPlane[epNear].normal.y) + boxSize.z * fabs(pPlane[epNear].normal.z);
	farDot = boxCenter * pPlane[epNear].normal + pPlane[epNear].distance;
	if(nearDot + farDot < 0) return false;
	// far plane (ok)
	nearDot = boxSize.x * fabs(pPlane[epFar].normal.x) +
		boxSize.y * fabs(pPlane[epFar].normal.y) + boxSize.z * fabs(pPlane[epFar].normal.z);
	farDot = boxCenter * pPlane[epFar].normal + pPlane[epFar].distance;
	if(nearDot + farDot < 0) return false;
	// left plane (wrong)
	nearDot = boxSize.x * fabs(pPlane[epLeft].normal.x) +
		boxSize.y * fabs(pPlane[epLeft].normal.y) + boxSize.z * fabs(pPlane[epLeft].normal.z);
	farDot = boxCenter * pPlane[epLeft].normal + pPlane[epLeft].distance;
	if(nearDot + farDot < 0) return false;
	// right plane (wrong)
	nearDot = boxSize.x * fabs(pPlane[epRight].normal.x) +
		boxSize.y * fabs(pPlane[epRight].normal.y) + boxSize.z * fabs(pPlane[epRight].normal.z);
	farDot = boxCenter * pPlane[epRight].normal + pPlane[epRight].distance;
	if(nearDot + farDot < 0) return false;
	// top plane (wrong)
	nearDot = boxSize.x * fabs(pPlane[epTop].normal.x) +
		boxSize.y * fabs(pPlane[epTop].normal.y) + boxSize.z * fabs(pPlane[epTop].normal.z);
	farDot = boxCenter * pPlane[epTop].normal + pPlane[epTop].distance;
	if(nearDot + farDot < 0) return false;
	// bottom plane (wrong)
	nearDot = boxSize.x * fabs(pPlane[epBottom].normal.x) +
		boxSize.y * fabs(pPlane[epBottom].normal.y) + boxSize.z * fabs(pPlane[epBottom].normal.z);
	farDot = boxCenter * pPlane[epBottom].normal + pPlane[epBottom].distance;
	if(nearDot + farDot < 0) return false;
	// good
	return true;
}
*/

bool deoglDCollisionFrustum::FrustumHitsFrustum(deoglDCollisionFrustum *frustum){
	return false;
}



double deoglDCollisionFrustum::SphereMoveHitsFrustum(deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoglDCollisionFrustum::CylinderMoveHitsFrustum(deoglDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoglDCollisionFrustum::CapsuleMoveHitsFrustum(deoglDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoglDCollisionFrustum::BoxMoveHitsFrustum(deoglDCollisionBox *box, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoglDCollisionFrustum::TriangleMoveHitsFrustum(deoglDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoglDCollisionFrustum::FrustumMoveHitsFrustum(deoglDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal){
	return 1;
}



// management
///////////////

void deoglDCollisionFrustum::SetLeftPlane(const decDVector &normal, double dist){
	pPlane[epLeft].normal = normal;
	pPlane[epLeft].absNormal = normal.Absolute();
	pPlane[epLeft].distance = dist;
}

void deoglDCollisionFrustum::SetRightPlane(const decDVector &normal, double dist){
	pPlane[epRight].normal = normal;
	pPlane[epRight].absNormal = normal.Absolute();
	pPlane[epRight].distance = dist;
}

void deoglDCollisionFrustum::SetTopPlane(const decDVector &normal, double dist){
	pPlane[epTop].normal = normal;
	pPlane[epTop].absNormal = normal.Absolute();
	pPlane[epTop].distance = dist;
}

void deoglDCollisionFrustum::SetBottomPlane(const decDVector &normal, double dist){
	pPlane[epBottom].normal = normal;
	pPlane[epBottom].absNormal = normal.Absolute();
	pPlane[epBottom].distance = dist;
}

void deoglDCollisionFrustum::SetNearPlane(const decDVector &normal, double dist){
	pPlane[epNear].normal = normal;
	pPlane[epNear].absNormal = normal.Absolute();
	pPlane[epNear].distance = dist;
}

void deoglDCollisionFrustum::SetFarPlane(const decDVector &normal, double dist){
	pPlane[epFar].normal = normal;
	pPlane[epFar].absNormal = normal.Absolute();
	pPlane[epFar].distance = dist;
}

void deoglDCollisionFrustum::SetFrustum(const decDMatrix &mat){
	double len;
	// left clipping plane
	pPlane[epLeft].normal.Set(mat.a41 + mat.a11, mat.a42 + mat.a12, mat.a43 + mat.a13);
	len = pPlane[epLeft].normal.Length();
	pPlane[epLeft].normal /= len;
	pPlane[epLeft].absNormal = pPlane[epLeft].normal.Absolute();
	pPlane[epLeft].distance = -(mat.a44 + mat.a14) / len;
	// right clipping plane
	pPlane[epRight].normal.Set(mat.a41 - mat.a11, mat.a42 - mat.a12, mat.a43 - mat.a13);
	len = pPlane[epRight].normal.Length();
	pPlane[epRight].normal /= len;
	pPlane[epRight].absNormal = pPlane[epRight].normal.Absolute();
	pPlane[epRight].distance = -(mat.a44 - mat.a14) / len;
	// top clipping plane
	pPlane[epTop].normal.Set(mat.a41 - mat.a21, mat.a42 - mat.a22, mat.a43 - mat.a23);
	len = pPlane[epTop].normal.Length();
	pPlane[epTop].normal /= len;
	pPlane[epTop].absNormal = pPlane[epTop].normal.Absolute();
	pPlane[epTop].distance = -(mat.a44 - mat.a24) / len;
	// bottom clipping plane
	pPlane[epBottom].normal.Set(mat.a41 + mat.a21, mat.a42 + mat.a22, mat.a43 + mat.a23);
	len = pPlane[epBottom].normal.Length();
	pPlane[epBottom].normal /= len;
	pPlane[epBottom].absNormal = pPlane[epBottom].normal.Absolute();
	pPlane[epBottom].distance = -(mat.a44 + mat.a24) / len;
	// near clipping plane
	pPlane[epNear].normal.Set(mat.a41 + mat.a31, mat.a42 + mat.a32, mat.a43 + mat.a33);
	len = pPlane[epNear].normal.Length();
	pPlane[epNear].normal /= len;
	pPlane[epNear].absNormal = pPlane[epNear].normal.Absolute();
	pPlane[epNear].distance = -(mat.a44 + mat.a34) / len;
	// far clipping plane
	pPlane[epFar].normal.Set(mat.a41 - mat.a31, mat.a42 - mat.a32, mat.a43 - mat.a33);
	len = pPlane[epFar].normal.Length();
	pPlane[epFar].normal /= len;
	pPlane[epFar].absNormal = pPlane[epFar].normal.Absolute();
	pPlane[epFar].distance = -(mat.a44 - mat.a34) / len;
}

void deoglDCollisionFrustum::SetFrustum(const decDVector &origin, const decDVector &r1, const decDVector &r2, const decDVector &r3, const decDVector &r4, double nearDist){
	pPlane[epTop].normal = (r1 - origin) % (r1 - r2);
	pPlane[epTop].normal.Normalize();
	pPlane[epTop].absNormal = pPlane[epTop].normal.Absolute();
	pPlane[epTop].distance = pPlane[epTop].normal * origin;
	
	pPlane[epRight].normal = (r2 - origin) % (r2 - r3);
	pPlane[epRight].normal.Normalize();
	pPlane[epRight].absNormal = pPlane[epRight].normal.Absolute();
	pPlane[epRight].distance = pPlane[epRight].normal * origin;
	
	pPlane[epBottom].normal = (r3 - origin) % (r3 - r4);
	pPlane[epBottom].normal.Normalize();
	pPlane[epBottom].absNormal = pPlane[epBottom].normal.Absolute();
	pPlane[epBottom].distance = pPlane[epBottom].normal * origin;
	
	pPlane[epLeft].normal = (r4 - origin) % (r4 - r1);
	pPlane[epLeft].normal.Normalize();
	pPlane[epLeft].absNormal = pPlane[epLeft].normal.Absolute();
	pPlane[epLeft].distance = pPlane[epLeft].normal * origin;
	
	pPlane[epFar].normal = (r2 - r1) % (r3 - r2);
	pPlane[epFar].normal.Normalize();
	pPlane[epFar].absNormal = pPlane[epFar].normal.Absolute();
	pPlane[epFar].distance = pPlane[epFar].normal * r2;
	
	pPlane[epNear].normal = -pPlane[epFar].normal;
	pPlane[epNear].absNormal = pPlane[epNear].normal.Absolute();
	pPlane[epNear].distance = pPlane[epNear].normal * origin + nearDist;
}

void deoglDCollisionFrustum::SetFrustumBox(const decDVector &r1, const decDVector &r2, const decDVector &r3, const decDVector &r4, double dist){
	pPlane[epTop].normal = r3 - r2;
	pPlane[epTop].normal.Normalize();
	pPlane[epTop].absNormal = pPlane[epTop].normal.Absolute();
	pPlane[epTop].distance = pPlane[epTop].normal * r2;
	
	pPlane[epRight].normal = r4 - r3;
	pPlane[epRight].normal.Normalize();
	pPlane[epRight].absNormal = pPlane[epRight].normal.Absolute();
	pPlane[epRight].distance = pPlane[epRight].normal * r3;
	
	pPlane[epBottom].normal = r1 - r4;
	pPlane[epBottom].normal.Normalize();
	pPlane[epBottom].absNormal = pPlane[epBottom].normal.Absolute();
	pPlane[epBottom].distance = pPlane[epBottom].normal * r4;
	
	pPlane[epLeft].normal = r2 - r1;
	pPlane[epLeft].normal.Normalize();
	pPlane[epLeft].absNormal = pPlane[epLeft].normal.Absolute();
	pPlane[epLeft].distance = pPlane[epLeft].normal * r1;
	
	pPlane[epNear].normal = (r3 - r2) % (r2 - r1);
	pPlane[epNear].normal.Normalize();
	pPlane[epNear].absNormal = pPlane[epNear].normal.Absolute();
	pPlane[epNear].distance = pPlane[epNear].normal * (r1 - pPlane[epNear].normal * dist);
	
	pPlane[epFar].normal = -pPlane[epNear].normal;
	pPlane[epFar].absNormal = pPlane[epFar].normal.Absolute();
	pPlane[epFar].distance = pPlane[epFar].normal * (r1 + pPlane[epNear].normal * dist);
}



// tests
//////////

deoglDCollisionFrustum::eIntersectType deoglDCollisionFrustum::IntersectSphere(deoglDCollisionSphere *sphere){
	decDVector center = sphere->GetCenter();
	double radius = sphere->GetRadius();
	double dist;
	eIntersectType result = eitInside;
	
	dist = pPlane[epLeft].normal * center - pPlane[epLeft].distance;
	if(dist < -radius) return eitOutside;
	if(fabs(dist) < radius) result = eitIntersect;
	
	dist = pPlane[epRight].normal * center - pPlane[epRight].distance;
	if(dist < -radius) return eitOutside;
	if(fabs(dist) < radius) result = eitIntersect;
	
	dist = pPlane[epTop].normal * center - pPlane[epTop].distance;
	if(dist < -radius) return eitOutside;
	if(fabs(dist) < radius) result = eitIntersect;
	
	dist = pPlane[epBottom].normal * center - pPlane[epBottom].distance;
	if(dist < -radius) return eitOutside;
	if(fabs(dist) < radius) result = eitIntersect;
	
	dist = pPlane[epNear].normal * center - pPlane[epNear].distance;
	if(dist < -radius) return eitOutside;
	if(fabs(dist) < radius) result = eitIntersect;
	
	dist = pPlane[epFar].normal * center - pPlane[epFar].distance;
	if(dist < -radius) return eitOutside;
	if(fabs(dist) < radius) result = eitIntersect;
	
	return result;
}
