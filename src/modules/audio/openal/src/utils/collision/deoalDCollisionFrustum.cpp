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

#include "deoalDCollisionSphere.h"
#include "deoalDCollisionCylinder.h"
#include "deoalDCollisionCapsule.h"
#include "deoalDCollisionBox.h"
#include "deoalDCollisionTriangle.h"
#include "deoalDCollisionFrustum.h"
#include "deoalDCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Frustum
////////////////////////////////

// constructors, destructors
//////////////////////////////

deoalDCollisionFrustum::deoalDCollisionFrustum(){
	pNormalLeft.Set(1.0, 0.0, 0.0);
	pNormalRight.Set(-1.0, 0.0, 0.0);
	pNormalTop.Set(0.0, -1.0, 0.0);
	pNormalBottom.Set(0.0, 1.0, 0.0);
	pNormalNear.Set(0.0, 0.0, 1.0);
	pNormalFar.Set(0.0, 0.0, -1.0);
	pDistLeft = 0.0;
	pDistRight = 0.0;
	pDistTop = 0.0;
	pDistBottom = 0.0;
	pDistNear = 0.0;
	pDistFar = 0.0;
}

deoalDCollisionFrustum::~deoalDCollisionFrustum(){
}



// double dispatching calls
/////////////////////////////

bool deoalDCollisionFrustum::VolumeHitsVolume(deoalDCollisionVolume *volume){
	return volume->FrustumHitsVolume(this);
}

double deoalDCollisionFrustum::VolumeMoveHitsVolume(deoalDCollisionVolume *volume, const decDVector &displacement, decDVector *normal){
	return volume->FrustumMoveHitsVolume(this, displacement, normal);
}



// first stage calls
//////////////////////

bool deoalDCollisionFrustum::SphereHitsVolume(deoalDCollisionSphere *sphere){
	return SphereHitsFrustum(sphere);
}

bool deoalDCollisionFrustum::CylinderHitsVolume(deoalDCollisionCylinder *cylinder){
	return CylinderHitsFrustum(cylinder);
}

bool deoalDCollisionFrustum::CapsuleHitsVolume(deoalDCollisionCapsule *capsule){
	return CapsuleHitsFrustum(capsule);
}

bool deoalDCollisionFrustum::BoxHitsVolume(deoalDCollisionBox *box){
	return BoxHitsFrustum(box);
}

bool deoalDCollisionFrustum::TriangleHitsVolume(deoalDCollisionTriangle *triangle){
	return TriangleHitsFrustum(triangle);
}

bool deoalDCollisionFrustum::FrustumHitsVolume(deoalDCollisionFrustum *frustum){
	return FrustumHitsFrustum(frustum);
}



double deoalDCollisionFrustum::SphereMoveHitsVolume(deoalDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return SphereMoveHitsFrustum(sphere, displacement, normal);
}

double deoalDCollisionFrustum::CylinderMoveHitsVolume(deoalDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal){
	return CylinderMoveHitsFrustum(cylinder, displacement, normal);
}

double deoalDCollisionFrustum::CapsuleMoveHitsVolume(deoalDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	return CapsuleMoveHitsFrustum(capsule, displacement, normal);
}

double deoalDCollisionFrustum::BoxMoveHitsVolume(deoalDCollisionBox *box, const decDVector &displacement, decDVector *normal){
	return BoxMoveHitsFrustum(box, displacement, normal);
}

double deoalDCollisionFrustum::TriangleMoveHitsVolume(deoalDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal){
	return TriangleMoveHitsFrustum(triangle, displacement, normal);
}

double deoalDCollisionFrustum::FrustumMoveHitsVolume(deoalDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal){
	return FrustumMoveHitsFrustum(frustum, displacement, normal);
}

double deoalDCollisionFrustum::PointMoveHitsVolume(const decDVector &point, const decDVector &displacement, decDVector *normal){
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void deoalDCollisionFrustum::GetEnclosingSphere(deoalDCollisionSphere *sphere){
	// not possible without complex plane intersection calculations
}

void deoalDCollisionFrustum::GetEnclosingBox(deoalDCollisionBox *box){
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

bool deoalDCollisionFrustum::IsPointInside(const decDVector &point){
	return (pNormalLeft * point >= pDistLeft) && (pNormalTop * point >= pDistTop)
		&& (pNormalRight * point >= pDistRight) && (pNormalBottom * point >= pDistBottom)
		&& (pNormalNear * point >= pDistNear) && (pNormalFar * point >= pDistFar);
}

decDVector deoalDCollisionFrustum::ClosestPointTo(const decDVector &point){
	decDVector result = point;
	double dot;
	
	dot = pNormalLeft * result - pDistLeft;
	if(dot < 0.0){
		result -= pNormalLeft * dot;
	}
	
	dot = pNormalTop * result - pDistTop;
	if(dot < 0.0){
		result -= pNormalTop * dot;
	}
	
	dot = pNormalRight * result - pDistRight;
	if(dot < 0.0){
		result -= pNormalRight * dot;
	}
	
	dot = pNormalBottom * result - pDistBottom;
	if(dot < 0.0){
		result -= pNormalBottom * dot;
	}
	
	dot = pNormalNear * result - pDistNear;
	if(dot < 0.0){
		result -= pNormalNear * dot;
	}
	
	dot = pNormalFar * result - pDistFar;
	if(dot < 0.0){
		result -= pNormalFar * dot;
	}
	
	return result;
}



// Visiting
/////////////

void deoalDCollisionFrustum::Visit(deoalDCollisionVolumeVisitor *visitor){
	if(! visitor) DETHROW(deeInvalidParam);
	visitor->VisitFrustum(this);
}



// collision routines
///////////////////////

bool deoalDCollisionFrustum::SphereHitsFrustum(deoalDCollisionSphere *sphere){
	const decDVector &center = sphere->GetCenter();
	const decDVector &diff = center - ClosestPointTo(center);
	const double radius = sphere->GetRadius();
	
	return diff * diff < radius * radius;
	
	//return IntersectSphere(sphere) != eitOutside;
}

bool deoalDCollisionFrustum::CylinderHitsFrustum(deoalDCollisionCylinder *cylinder){
	return false;
}

bool deoalDCollisionFrustum::CapsuleHitsFrustum(deoalDCollisionCapsule *capsule){
	return false;
}

bool deoalDCollisionFrustum::BoxHitsFrustum(deoalDCollisionBox *box){
	const decDVector &boxCenter = box->GetCenter();
	const decDVector &boxSize = box->GetHalfSize();
	double nearDot, farDot;
	
	// near plane
	nearDot = boxSize.x * fabs(pNormalNear.x) + boxSize.y * fabs(pNormalNear.y) + boxSize.z * fabs(pNormalNear.z);
	farDot = pDistNear - boxCenter * pNormalNear;
	if(farDot > nearDot){
		return false;
	}
	
	// far plane
	nearDot = boxSize.x * fabs(pNormalFar.x) + boxSize.y * fabs(pNormalFar.y) + boxSize.z * fabs(pNormalFar.z);
	farDot = pDistFar - boxCenter * pNormalFar;
	if(farDot > nearDot){
		return false;
	}
	
	// left plane
	nearDot = boxSize.x * fabs(pNormalLeft.x) + boxSize.y * fabs(pNormalLeft.y) + boxSize.z * fabs(pNormalLeft.z);
	farDot = pDistLeft - boxCenter * pNormalLeft;
	if(farDot > nearDot){
		return false;
	}
	
	// right plane
	nearDot = boxSize.x * fabs(pNormalRight.x) + boxSize.y * fabs(pNormalRight.y) + boxSize.z * fabs(pNormalRight.z);
	farDot = pDistRight - boxCenter * pNormalRight;
	if(farDot > nearDot){
		return false;
	}
	
	// top plane
	nearDot = boxSize.x * fabs(pNormalTop.x) + boxSize.y * fabs(pNormalTop.y) + boxSize.z * fabs(pNormalTop.z);
	farDot = pDistTop - boxCenter * pNormalTop;
	if(farDot > nearDot){
		return false;
	}
	
	// bottom plane
	nearDot = boxSize.x * fabs(pNormalBottom.x) + boxSize.y * fabs(pNormalBottom.y) + boxSize.z * fabs(pNormalBottom.z);
	farDot = pDistBottom - boxCenter * pNormalBottom;
	if(farDot > nearDot){
		return false;
	}
	
	return true;
}

bool deoalDCollisionFrustum::TriangleHitsFrustum(deoalDCollisionTriangle *triangle){
	return false;
}
/*
bool deoalDCollisionFrustum::OrientedBoxHitsFrustum(deoalDCollisionOrientedBox *box){
	// NOT WORKING
	decDVector boxCenter = box->GetCenter();
	decDVector boxSize = box->GetHalfSize();
	double nearDot, farDot;
	// near plane (wrong)
	nearDot = boxSize.x * fabs(pNormalNear.x) +
		boxSize.y * fabs(pNormalNear.y) + boxSize.z * fabs(pNormalNear.z);
	farDot = boxCenter * pNormalNear + pDistNear;
	if(nearDot + farDot < 0) return false;
	// far plane (ok)
	nearDot = boxSize.x * fabs(pNormalFar.x) +
		boxSize.y * fabs(pNormalFar.y) + boxSize.z * fabs(pNormalFar.z);
	farDot = boxCenter * pNormalFar + pDistFar;
	if(nearDot + farDot < 0) return false;
	// left plane (wrong)
	nearDot = boxSize.x * fabs(pNormalLeft.x) +
		boxSize.y * fabs(pNormalLeft.y) + boxSize.z * fabs(pNormalLeft.z);
	farDot = boxCenter * pNormalLeft + pDistLeft;
	if(nearDot + farDot < 0) return false;
	// right plane (wrong)
	nearDot = boxSize.x * fabs(pNormalRight.x) +
		boxSize.y * fabs(pNormalRight.y) + boxSize.z * fabs(pNormalRight.z);
	farDot = boxCenter * pNormalRight + pDistRight;
	if(nearDot + farDot < 0) return false;
	// top plane (wrong)
	nearDot = boxSize.x * fabs(pNormalTop.x) +
		boxSize.y * fabs(pNormalTop.y) + boxSize.z * fabs(pNormalTop.z);
	farDot = boxCenter * pNormalTop + pDistTop;
	if(nearDot + farDot < 0) return false;
	// bottom plane (wrong)
	nearDot = boxSize.x * fabs(pNormalBottom.x) +
		boxSize.y * fabs(pNormalBottom.y) + boxSize.z * fabs(pNormalBottom.z);
	farDot = boxCenter * pNormalBottom + pDistBottom;
	if(nearDot + farDot < 0) return false;
	// good
	return true;
}
*/

bool deoalDCollisionFrustum::FrustumHitsFrustum(deoalDCollisionFrustum *frustum){
	return false;
}



double deoalDCollisionFrustum::SphereMoveHitsFrustum(deoalDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoalDCollisionFrustum::CylinderMoveHitsFrustum(deoalDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoalDCollisionFrustum::CapsuleMoveHitsFrustum(deoalDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoalDCollisionFrustum::BoxMoveHitsFrustum(deoalDCollisionBox *box, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoalDCollisionFrustum::TriangleMoveHitsFrustum(deoalDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoalDCollisionFrustum::FrustumMoveHitsFrustum(deoalDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal){
	return 1;
}



// management
///////////////

void deoalDCollisionFrustum::SetLeftPlane(const decDVector &normal, double dist){
	pNormalLeft = normal;
	pDistLeft = dist;
}

void deoalDCollisionFrustum::SetRightPlane(const decDVector &normal, double dist){
	pNormalRight = normal;
	pDistRight = dist;
}

void deoalDCollisionFrustum::SetTopPlane(const decDVector &normal, double dist){
	pNormalTop = normal;
	pDistTop = dist;
}

void deoalDCollisionFrustum::SetBottomPlane(const decDVector &normal, double dist){
	pNormalBottom = normal;
	pDistBottom = dist;
}

void deoalDCollisionFrustum::SetNearPlane(const decDVector &normal, double dist){
	pNormalNear = normal;
	pDistNear = dist;
}

void deoalDCollisionFrustum::SetFarPlane(const decDVector &normal, double dist){
	pNormalFar = normal;
	pDistFar = dist;
}

void deoalDCollisionFrustum::SetFrustum(const decDMatrix &mat){
	double len;
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

void deoalDCollisionFrustum::SetFrustum(const decDVector &origin, const decDVector &r1, const decDVector &r2, const decDVector &r3, const decDVector &r4, double nearDist){
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

void deoalDCollisionFrustum::SetFrustumBox(const decDVector &r1, const decDVector &r2, const decDVector &r3, const decDVector &r4, double dist){
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

deoalDCollisionFrustum::eIntersectType deoalDCollisionFrustum::IntersectSphere(deoalDCollisionSphere *sphere){
	decDVector center = sphere->GetCenter();
	double radius = sphere->GetRadius();
	double dist;
	eIntersectType result = eitInside;
	
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
