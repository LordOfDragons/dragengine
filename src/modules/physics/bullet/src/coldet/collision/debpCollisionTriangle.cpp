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



// Collision Detection Triangle
/////////////////////////////////

// constructors, destructors
//////////////////////////////

debpCollisionTriangle::debpCollisionTriangle(){
}

debpCollisionTriangle::debpCollisionTriangle(const decVector &corner1, const decVector &corner2, const decVector &corner3){
	SetCorners(corner1, corner2, corner3);
}

debpCollisionTriangle::debpCollisionTriangle(const decVector &corner1, const decVector &corner2, const decVector &corner3, const decVector &normal){
	SetCorners(corner1, corner2, corner3, normal);
}

debpCollisionTriangle::~debpCollisionTriangle(){
}



// float dispatching calls
/////////////////////////////

bool debpCollisionTriangle::VolumeHitsVolume(debpCollisionVolume *volume){
	return volume->TriangleHitsVolume(this);
}

float debpCollisionTriangle::VolumeMoveHitsVolume(debpCollisionVolume *volume, const decVector &displacement, decVector *normal){
	return volume->TriangleMoveHitsVolume(this, displacement, normal);
}



// first stage calls
//////////////////////

bool debpCollisionTriangle::SphereHitsVolume(debpCollisionSphere *sphere){
	return SphereHitsTriangle(sphere);
}

bool debpCollisionTriangle::CylinderHitsVolume(debpCollisionCylinder *cylinder){
	return CylinderHitsTriangle(cylinder);
}

bool debpCollisionTriangle::CapsuleHitsVolume(debpCollisionCapsule *capsule){
	return CapsuleHitsTriangle(capsule);
}

bool debpCollisionTriangle::BoxHitsVolume(debpCollisionBox *box){
	return BoxHitsTriangle(box);
}

bool debpCollisionTriangle::TriangleHitsVolume(debpCollisionTriangle *triangle){
	return TriangleHitsTriangle(triangle);
}

bool debpCollisionTriangle::FrustumHitsVolume(debpCollisionFrustum *frustum){
	return frustum->TriangleHitsFrustum(this);
}



float debpCollisionTriangle::SphereMoveHitsVolume(debpCollisionSphere *sphere, const decVector &displacement, decVector *normal){
	return SphereMoveHitsTriangle(sphere, displacement, normal);
}

float debpCollisionTriangle::CylinderMoveHitsVolume(debpCollisionCylinder *cylinder, const decVector &displacement, decVector *normal){
	return CylinderMoveHitsTriangle(cylinder, displacement, normal);
}

float debpCollisionTriangle::CapsuleMoveHitsVolume(debpCollisionCapsule *capsule, const decVector &displacement, decVector *normal){
	return CapsuleMoveHitsTriangle(capsule, displacement, normal);
}

float debpCollisionTriangle::BoxMoveHitsVolume(debpCollisionBox *box, const decVector &displacement, decVector *normal){
	return BoxMoveHitsTriangle(box, displacement, normal);
}

float debpCollisionTriangle::TriangleMoveHitsVolume(debpCollisionTriangle *triangle, const decVector &displacement, decVector *normal){
	return TriangleMoveHitsTriangle(triangle, displacement, normal);
}

float debpCollisionTriangle::FrustumMoveHitsVolume(debpCollisionFrustum *frustum, const decVector &displacement, decVector *normal){
	float distance = frustum->TriangleMoveHitsFrustum(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

float debpCollisionTriangle::PointMoveHitsVolume(const decVector &point, const decVector &displacement, decVector *normal){
	decVector hitPoint;
	float dot = displacement * pNormal, lambda;
	if(dot < -0.00001 || dot > 0.00001f){
		lambda = ((pCorners[0] - point) * pNormal) / dot;
		if(lambda < 0.0f || lambda > 1.0f){
			return 1.0f;
		}else{
			hitPoint = point + displacement * lambda;
			if(debpDECollisionDetection::PointInTriangle(pCorners[0], pCorners[1],
			pCorners[2], hitPoint)){
				if(normal){
					normal->Set(point - hitPoint);
					normal->Normalize();
				}
				return lambda;
			}else{
				return 1;
			}
		}
	}
	return 1;
}



// Enclosing Volumes
//////////////////////

void debpCollisionTriangle::GetEnclosingSphere(debpCollisionSphere *sphere){
	if(!sphere) DETHROW(deeInvalidParam);
	decVector center = (pCorners[0] + pCorners[1] + pCorners[2]) / 3.0f;
	decVector distVector = pCorners[0] - center;
	float radiusSquared = distVector * distVector;
	distVector = pCorners[1] - center;
	float distSquared = distVector * distVector;
	if(distSquared > radiusSquared) radiusSquared = distSquared;
	distVector = pCorners[2] - center;
	distSquared = distVector * distVector;
	if(distSquared > radiusSquared) radiusSquared = distSquared;
	sphere->SetAll(center, sqrt(radiusSquared));
}

void debpCollisionTriangle::GetEnclosingBox(debpCollisionBox *box){
	if(!box) DETHROW(deeInvalidParam);
	decVector minExtend = pCorners[0];
	decVector maxExtend = minExtend;
	if(pCorners[1].x < minExtend.x){
		minExtend.x = pCorners[1].x;
	}else if(pCorners[1].x > maxExtend.x){
		maxExtend.x = pCorners[1].x;
	}
	if(pCorners[1].y < minExtend.y){
		minExtend.y = pCorners[1].y;
	}else if(pCorners[1].y > maxExtend.y){
		maxExtend.y = pCorners[1].y;
	}
	if(pCorners[1].z < minExtend.z){
		minExtend.z = pCorners[1].z;
	}else if(pCorners[1].z > maxExtend.z){
		maxExtend.z = pCorners[1].z;
	}
	if(pCorners[2].x < minExtend.x){
		minExtend.x = pCorners[2].x;
	}else if(pCorners[2].x > maxExtend.x){
		maxExtend.x = pCorners[2].x;
	}
	if(pCorners[2].y < minExtend.y){
		minExtend.y = pCorners[2].y;
	}else if(pCorners[2].y > maxExtend.y){
		maxExtend.y = pCorners[2].y;
	}
	if(pCorners[2].z < minExtend.z){
		minExtend.z = pCorners[2].z;
	}else if(pCorners[2].z > maxExtend.z){
		maxExtend.z = pCorners[2].z;
	}
	box->SetFromExtends(minExtend, maxExtend);
}



// Miscelanous Functions
//////////////////////////

bool debpCollisionTriangle::IsPointInside(const decVector &point){
	return false; // triangles are infinitesimally thin
}

decVector debpCollisionTriangle::ClosestPointTo(const decVector &point){
	decVector hitPoint = point - pNormal * (point * pNormal - pDist);
	if(debpDECollisionDetection::PointInTriangle(pCorners[0], pCorners[1], pCorners[2], hitPoint)){
		return hitPoint;
	}else{
		return debpDECollisionDetection::ClosestPointOnTriangleEdge(pCorners[0], pCorners[1], pCorners[2], hitPoint);
	}
}



// Visiting
/////////////

void debpCollisionTriangle::Visit(debpCollisionVolumeVisitor *visitor){
	if(!visitor) DETHROW(deeInvalidParam);
	visitor->VisitTriangle(this);
}



// collision routines
///////////////////////

bool debpCollisionTriangle::SphereHitsTriangle(debpCollisionSphere *sphere){
	const decVector &sc = sphere->GetCenter();
	float lambda, squareRadius = sphere->GetSquareRadius();
	decVector hitPoint;
	// hit point of sphere center projected onto plane
	lambda = pDist - sc * pNormal;
	hitPoint = sc + pNormal * lambda;
	// if the point is not inside the triangle calculate the point on the
	// triangle closest to the hitpoint
	if(!debpDECollisionDetection::PointInTriangle(pCorners[0], pCorners[1], pCorners[2], hitPoint)){
		hitPoint = debpDECollisionDetection::ClosestPointOnTriangleEdge(pCorners[0], pCorners[1], pCorners[2], hitPoint);
	}
	// we need to compare the distance of the hitpoint to the sphere center
	hitPoint -= sc;
	// check if squared-distance is less than the square-radius
	return hitPoint * hitPoint <= squareRadius;
}

bool debpCollisionTriangle::CylinderHitsTriangle(debpCollisionCylinder *cylinder){
	return false;
}

bool debpCollisionTriangle::CapsuleHitsTriangle(debpCollisionCapsule *capsule){
	return false;
}

bool debpCollisionTriangle::BoxHitsTriangle(debpCollisionBox *box){
	const decVector &bc = box->GetCenter();
	const decVector &bhs = box->GetHalfSize();
	decVector normal, corners[3];
	float projBox, projTri[3];
	int i, id1, id2;
	
	// adjust corners if required
	corners[0] = pCorners[0] - bc;
	corners[1] = pCorners[1] - bc;
	corners[2] = pCorners[2] - bc;
	
	// test normal of triangle as separation-axis
	if(fabs(corners[0] * pNormal) > box->ProjectExtends(pNormal)) return false;
	
	// test box axes as separation-axis
	const decVector &bax = box->GetAxisX();
	if(corners[0] * bax > bhs.x && corners[1] * bax > bhs.x && corners[2] * bax > bhs.x) return false;
	if(corners[0] * bax < -bhs.x && corners[1] * bax < -bhs.x && corners[2] * bax < -bhs.x) return false;
	
	const decVector &bay = box->GetAxisY();
	if(corners[0] * bay > bhs.y && corners[1] * bay > bhs.y && corners[2] * bay > bhs.y) return false;
	if(corners[0] * bay < -bhs.y && corners[1] * bay < -bhs.y && corners[2] * bay < -bhs.y) return false;
	
	const decVector &baz = box->GetAxisZ();
	if(corners[0] * baz > bhs.z && corners[1] * baz > bhs.z && corners[2] * baz > bhs.z) return false;
	if(corners[0] * baz < -bhs.z && corners[1] * baz < -bhs.z && corners[2] * baz < -bhs.z) return false;
	
	// test triangle axes as separation-axis
	for(i=0; i<3; i++){
		projBox = box->ProjectExtends(pEdges[i]);
		
		// get projecton of corner points.
		projTri[0] = corners[0] * pEdges[i];
		projTri[1] = corners[1] * pEdges[i];
		projTri[2] = corners[2] * pEdges[i];
		
		// test if projection of box and triangle do overlap
		if(projTri[0] > projBox && projTri[1] > projBox && projTri[2] > projBox) return false;
		if(-projTri[0] > projBox && -projTri[1] > projBox && -projTri[2] > projBox) return false;
	}
	
	// test triangle edges crossed with box axes as separation-axis
	for(i=0; i<3; i++){
		id1 = (i + 1) % 3;
		id2 = (i + 2) % 3;
		
		// cross axes[ i ] with box x-axis
		normal = pEdges[i] % bax;
		if(!normal.IsZero()){
			normal.Normalize();
			projBox = box->ProjectExtends(normal);
			projTri[0] = corners[id1] * normal;
			projTri[1] = corners[id2] * normal;
			if(projTri[0] > projBox && projTri[1] > projBox) return false;
			if(-projTri[0] > projBox && -projTri[1] > projBox) return false;
		}
		
		// cross axes[ i ] with box y-axis
		normal = pEdges[i] % bay;
		if(!normal.IsZero()){
			normal.Normalize();
			projBox = box->ProjectExtends(normal);
			projTri[0] = corners[id1] * normal;
			projTri[1] = corners[id2] * normal;
			if(projTri[0] > projBox && projTri[1] > projBox) return false;
			if(-projTri[0] > projBox && -projTri[1] > projBox) return false;
		}
		
		// cross axes[ i ] with box z-axis
		normal = pEdges[i] % baz;
		if(!normal.IsZero()){
			normal.Normalize();
			projBox = box->ProjectExtends(normal);
			projTri[0] = corners[id1] * normal;
			projTri[1] = corners[id2] * normal;
			if(projTri[0] > projBox && projTri[1] > projBox) return false;
			if(-projTri[0] > projBox && -projTri[1] > projBox) return false;
		}
	}
	
	// we do collide
	return true;
}

bool debpCollisionTriangle::TriangleHitsTriangle(debpCollisionTriangle *triangle){
	return false;
}



float debpCollisionTriangle::SphereMoveHitsTriangle(debpCollisionSphere *sphere, const decVector &displacement, decVector *normal){
	const decVector &sc = sphere->GetCenter();
	float sr2 = sphere->GetSquareRadius();
	float sr = sphere->GetRadius();
	float dot, lambda;
	decVector hitPoint, hpt;
	
	// determine which hit point to use. if the sphere already intersects the
	// triangle plane we use the sphere center projected onto the triangle
	// plane using the triangle normal. otherwise we use the sphere center
	// moved by the negated triangle normal times sphere radius projected
	// onto the triangle plane using the displacement vector.
	lambda = sc * pNormal - pDist;
	if(fabs(lambda) - sr > 0.0001f){
		dot = pNormal * displacement;
		if(lambda > 0.0f){
			if(dot > -0.0001f) return 1.0f;
			hitPoint = sc - pNormal * sr;
		}else{
			if(dot < 0.0001f) return 1.0f;
			hitPoint = sc + pNormal * sr;
		}
		hitPoint -= displacement * ((pNormal * hitPoint - pDist) / dot);
	}else{
		hitPoint = sc - pNormal * lambda;
	}
	
	// if the point is not inside the triangle calculate the point on the triangle closest to the hitpoint
	if(!debpDECollisionDetection::PointInTriangle(pCorners[0], pCorners[1], pCorners[2], hitPoint)){
		hitPoint = debpDECollisionDetection::ClosestPointOnTriangleEdge(pCorners[0], pCorners[1], pCorners[2], hitPoint);
	}
	
	// go on depending if the point is inside the sphere or not
	hpt = hitPoint - sc;
	if(normal){
		normal->Set(-hpt);
		normal->Normalize();
	}
	if(hpt * hpt > sr2){
		// project hitpoint by inverse direction back to sphere and check for collision
		float a = displacement * displacement;
		float b = hpt * displacement; // left out -
		float disc = b * b - a * (hpt * hpt - sr2);
		if(disc < 0.0f){
			lambda = 1.0f;
		}else{
			lambda = (b - sqrt(disc)) / a; // payed respect to left out -
		}
		if(lambda < 0.0f || lambda > 1.0f) lambda = 1.0f;
		return lambda;
	}else{
		return 0.0f;
	}
}

float debpCollisionTriangle::CylinderMoveHitsTriangle(debpCollisionCylinder *cylinder, const decVector &displacement, decVector *normal){
	return 1.0f;
}

float debpCollisionTriangle::CapsuleMoveHitsTriangle(debpCollisionCapsule *capsule, const decVector &displacement, decVector *normal){
	return 1.0f;
}

// this function still seems troublesome. in some situations the result is wrong,
// especially the returned normal is sometimes off. furthermore the function seems
// to detect a collision with the ground where there is none. further investigation
// is require. for the time beeing the sphere test is used as this one is rather
// robust and producing no wrong hits so far.
float debpCollisionTriangle::BoxMoveHitsTriangle(debpCollisionBox *box, const decVector &displacement, decVector *normal){
	const decVector &bhs = box->GetHalfSize();
	const decVector &bc = box->GetCenter();
	float testDist, dispDist;
	decVector tnormal, corners[3];
	float projBox;
	decVector bestNormal;
	float bestDist = 0.0f;
	bool hitAtStart = true;
	int i, id1, id2;
	float dot1, dot2, dot3;
	// init
	corners[0] = pCorners[0] - bc;
	corners[1] = pCorners[1] - bc;
	corners[2] = pCorners[2] - bc;
	
	// test normal of triangle as separation-axis
	testDist = pNormal * corners[0];
	if(testDist > 0.0f){
		testDist -= box->ProjectExtends(pNormal);
		if(testDist > 0.0f){
			dispDist = displacement * pNormal;
			if(dispDist <= testDist) return 1.0f;
			testDist /= dispDist;
			if(testDist > bestDist){
				bestDist = testDist;
				if(normal) bestNormal.Set(-pNormal);
				hitAtStart = false;
			}
		}
	}else{
		testDist = -(testDist + box->ProjectExtends(pNormal));
		if(testDist > 0.0f){
			dispDist = -(displacement * pNormal);
			if(dispDist <= testDist) return 1.0f;
			testDist /= dispDist;
			if(testDist > bestDist){
				bestDist = testDist;
				if(normal) bestNormal.Set(pNormal);
				hitAtStart = false;
			}
		}
	}
	
	// test box axes as separation-axis

	// x axis
	const decVector &bax = box->GetAxisX();
	dot1 = corners[0] * bax;
	if(dot1 > 0.0f){
		dot2 = corners[1] * bax - bhs.x;
		dot3 = corners[2] * bax - bhs.x;
		if(dot2 > 0.0f && dot3 > 0.0f){
			testDist = dot1 - bhs.x;
			if(dot2 < testDist) testDist = dot2;
			if(dot3 < testDist) testDist = dot3;
			if(testDist > 0.0f){
				dispDist = displacement * bax;
				if(dispDist <= testDist) return 1.0f;
				testDist /= dispDist;
				if(testDist > bestDist){
					bestDist = testDist;
					if(normal) bestNormal = -bax;
					hitAtStart = false;
				}
			}
		}
	}else{
		dot2 = -(corners[1] * bax + bhs.x);
		dot3 = -(corners[2] * bax + bhs.x);
		if(dot2 > 0.0f && dot3 > 0.0f){
			testDist = -(dot1 + bhs.x);
			if(dot2 < testDist) testDist = dot2;
			if(dot3 < testDist) testDist = dot3;
			if(testDist > 0.0f){
				dispDist = -(displacement * bax);
				if(dispDist <= testDist) return 1.0f;
				testDist /= dispDist;
				if(testDist > bestDist){
					bestDist = testDist;
					if(normal) bestNormal = bax;
					hitAtStart = false;
				}
			}
		}
	}
	
	// y axis
	const decVector &bay = box->GetAxisY();
	dot1 = corners[0] * bay;
	if(dot1 > 0.0f){
		dot2 = corners[1] * bay - bhs.y;
		dot3 = corners[2] * bay - bhs.y;
		if(dot2 > 0.0f && dot3 > 0.0f){
			testDist = dot1 - bhs.y;
			if(dot2 < testDist) testDist = dot2;
			if(dot3 < testDist) testDist = dot3;
			if(testDist > 0.0f){
				dispDist = displacement * bay;
				if(dispDist <= testDist) return 1.0f;
				testDist /= dispDist;
				if(testDist > bestDist){
					bestDist = testDist;
					if(normal) bestNormal = -bay;
					hitAtStart = false;
				}
			}
		}
	}else{
		dot2 = -(corners[1] * bay + bhs.y);
		dot3 = -(corners[2] * bay + bhs.y);
		if(dot2 > 0.0f && dot3 > 0.0f){
			testDist = -(dot1 + bhs.y);
			if(dot2 < testDist) testDist = dot2;
			if(dot3 < testDist) testDist = dot3;
			if(testDist > 0.0f){
				dispDist = -(displacement * bay);
				if(dispDist <= testDist) return 1.0f;
				testDist /= dispDist;
				if(testDist > bestDist){
					bestDist = testDist;
					if(normal) bestNormal = bay;
					hitAtStart = false;
				}
			}
		}
	}
	
	// z axis
	const decVector &baz = box->GetAxisZ();
	dot1 = corners[0] * baz;
	if(dot1 > 0.0f){
		dot2 = corners[1] * baz - bhs.z;
		dot3 = corners[2] * baz - bhs.z;
		if(dot2 > 0.0f && dot3 > 0.0f){
			testDist = dot1 - bhs.z;
			if(dot2 < testDist) testDist = dot2;
			if(dot3 < testDist) testDist = dot3;
			if(testDist > 0.0f){
				dispDist = displacement * baz;
				if(dispDist <= testDist) return 1.0f;
				testDist /= dispDist;
				if(testDist > bestDist){
					bestDist = testDist;
					if(normal) bestNormal = -baz;
					hitAtStart = false;
				}
			}
		}
	}else{
		dot2 = -(corners[1] * baz + bhs.z);
		dot3 = -(corners[2] * baz + bhs.z);
		if(dot2 > 0.0f && dot3 > 0.0f){
			testDist = -(dot1 + bhs.z);
			if(dot2 < testDist) testDist = dot2;
			if(dot3 < testDist) testDist = dot3;
			if(testDist > 0.0f){
				dispDist = -(displacement * baz);
				if(dispDist <= testDist) return 1.0f;
				testDist /= dispDist;
				if(testDist > bestDist){
					bestDist = testDist;
					if(normal) bestNormal = baz;
					hitAtStart = false;
				}
			}
		}
	}
	
	// test triangle axes as separation-axis
	for(i=0; i<3; i++){
		tnormal = pEdges[i];
		projBox = box->ProjectExtends(tnormal);
		dot1 = corners[0] * tnormal;
		if(dot1 > 0.0f){
			dot2 = corners[1] * tnormal - projBox;
			dot3 = corners[2] * tnormal - projBox;
			if(dot2 > 0.0f && dot3 > 0.0f){
				testDist = dot1 - projBox;
				if(dot2 < testDist) testDist = dot2;
				if(dot3 < testDist) testDist = dot3;
				if(testDist > 0.0f){
					dispDist = displacement * tnormal;
					if(dispDist <= testDist) return 1.0f;
					testDist /= dispDist;
					if(testDist > bestDist){
						bestDist = testDist;
						if(normal) bestNormal = -tnormal;
						hitAtStart = false;
					}
				}
			}
		}else{
			dot2 = -(corners[1] * tnormal + projBox);
			dot3 = -(corners[2] * tnormal + projBox);
			if(dot2 > 0.0f && dot3 > 0.0f){
				testDist = -(dot1 + projBox);
				if(dot2 < testDist) testDist = dot2;
				if(dot3 < testDist) testDist = dot3;
				if(testDist > 0.0f){
					dispDist = -(displacement * tnormal);
					if(dispDist <= testDist) return 1.0f;
					testDist /= dispDist;
					if(testDist > bestDist){
						bestDist = testDist;
						if(normal) bestNormal = tnormal;
						hitAtStart = false;
					}
				}
			}
		}
	}
	
	// test triangle edges crossed with box axes as separation-axis
	for(i=0; i<3; i++){
		id1 = (i + 1) % 3;
		id2 = (i + 2) % 3;
		
		// cross axes[ i ] with box x-axis
		tnormal = pEdges[i] % bax;
		if(!tnormal.IsZero()){
			tnormal.Normalize();
			projBox = fabs(tnormal * bay) * bhs.y + fabs(tnormal * baz) * bhs.z;
			dot1 = corners[id1] * tnormal;
			if(dot1 > 0.0f){
				dot2 = corners[id2] * tnormal - projBox;
				if(dot2 > 0.0f){
					testDist = dot1 - projBox;
					if(dot2 < testDist) testDist = dot2;
					if(testDist > 0.0f){
						dispDist = displacement * tnormal;
						if(dispDist <= testDist) return 1.0f;
						testDist /= dispDist;
						if(testDist > bestDist){
							bestDist = testDist;
							if(normal) bestNormal = -tnormal;
							hitAtStart = false;
						}
					}
				}
			}else{
				dot2 = -(corners[id2] * tnormal + projBox);
				if(dot2 > 0.0f){
					testDist = -(dot1 + projBox);
					if(dot2 < testDist) testDist = dot2;
					if(testDist > 0.0f){
						dispDist = -(displacement * tnormal);
						if(dispDist <= testDist) return 1.0f;
						testDist /= dispDist;
						if(testDist > bestDist){
							bestDist = testDist;
							if(normal) bestNormal = tnormal;
							hitAtStart = false;
						}
					}
				}
			}
		}

		// cross axes[ i ] with box y-axis
		tnormal = pEdges[i] % bay;
		if(!tnormal.IsZero()){
			tnormal.Normalize();
			projBox = fabs(tnormal * bax) * bhs.x + fabs(tnormal * baz) * bhs.z;
			dot1 = corners[id1] * tnormal;
			if(dot1 > 0.0f){
				dot2 = corners[id2] * tnormal - projBox;
				if(dot2 > 0.0f){
					testDist = dot1 - projBox;
					if(dot2 < testDist) testDist = dot2;
					if(testDist > 0.0f){
						dispDist = displacement * tnormal;
						if(dispDist <= testDist) return 1.0f;
						testDist /= dispDist;
						if(testDist > bestDist){
							bestDist = testDist;
							if(normal) bestNormal = -tnormal;
							hitAtStart = false;
						}
					}
				}
			}else{
				dot2 = -(corners[id2] * tnormal + projBox);
				if(dot2 > 0.0f){
					testDist = -(dot1 + projBox);
					if(dot2 < testDist) testDist = dot2;
					if(testDist > 0.0f){
						dispDist = -(displacement * tnormal);
						if(dispDist <= testDist) return 1.0f;
						testDist /= dispDist;
						if(testDist > bestDist){
							bestDist = testDist;
							if(normal) bestNormal = tnormal;
							hitAtStart = false;
						}
					}
				}
			}
		}

		// cross axes[ i ] with box z-axis
		tnormal = pEdges[i] % baz;
		if(!tnormal.IsZero()){
			tnormal.Normalize();
			projBox = fabs(tnormal * bax) * bhs.x + fabs(tnormal * bay) * bhs.y;
			dot1 = corners[id1] * tnormal;
			if(dot1 > 0.0f){
				dot2 = corners[id2] * tnormal - projBox;
				if(dot2 > 0.0f){
					testDist = dot1 - projBox;
					if(dot2 < testDist) testDist = dot2;
					if(testDist > 0.0f){
						dispDist = displacement * tnormal;
						if(dispDist <= testDist) return 1.0f;
						testDist /= dispDist;
						if(testDist > bestDist){
							bestDist = testDist;
							if(normal) bestNormal = -tnormal;
							hitAtStart = false;
						}
					}
				}
			}else{
				dot2 = -(corners[id2] * tnormal + projBox);
				if(dot2 > 0.0f){
					testDist = -(dot1 + projBox);
					if(dot2 < testDist) testDist = dot2;
					if(testDist > 0.0f){
						dispDist = -(displacement * tnormal);
						if(dispDist <= testDist) return 1.0f;
						testDist /= dispDist;
						if(testDist > bestDist){
							bestDist = testDist;
							if(normal) bestNormal = tnormal;
							hitAtStart = false;
						}
					}
				}
			}
		}
	}
	
	// set normal if required
	if(normal){
		// if there is a collision at point 0 try to calculate a reasonable normal
		if(hitAtStart){
			decVector projPoint = bc - pNormal * (bc * pNormal - pDist);
			if(debpDECollisionDetection::PointInTriangle(pCorners[0], pCorners[1], pCorners[2], projPoint)){
				normal->Set(pNormal);
			}else{
				projPoint = debpDECollisionDetection::ClosestPointOnTriangleEdge(pCorners[0],
					pCorners[1], pCorners[2], projPoint) - bc;
				float fx = fabs(projPoint.x);
				float fy = fabs(projPoint.y);
				float fz = fabs(projPoint.z);
				if(fx > fy){
					if(fx > fz){
						if(projPoint.x > 0.0f){
							normal->Set(-1.0f, 0.0f, 0.0f);
						}else{
							normal->Set(1.0f, 0.0f, 0.0f);
						}
					}else{
						if(projPoint.z > 0.0f){
							normal->Set(0.0f, 0.0f, -1.0f);
						}else{
							normal->Set(0.0f, 0.0f, 1.0f);
						}
					}
				}else{
					if(fy > fz){
						if(projPoint.y > 0.0f){
							normal->Set(0.0f, -1.0f, 0.0f);
						}else{
							normal->Set(0.0f, 1.0f, 0.0f);
						}
					}else{
						if(projPoint.z > 0.0f){
							normal->Set(0.0f, 0.0f, -1.0f);
						}else{
							normal->Set(0.0f, 0.0f, 1.0f);
						}
					}
				}
			}
		// set found normal but normalize first
		}else{
			normal->Set(bestNormal);
			normal->Normalize();
		}
	}
	// return found distance
	return bestDist;
}

float debpCollisionTriangle::TriangleMoveHitsTriangle(debpCollisionTriangle *triangle, const decVector &displacement, decVector *normal){
	return 1;
}



// management
///////////////

void debpCollisionTriangle::SetCorners(const decVector &corner1, const decVector &corner2, const decVector &corner3){
	SetCorners(corner1, corner2, corner3, ((corner2 - corner1) % (corner3 - corner2)).Normalized());
}

void debpCollisionTriangle::SetCorners(const decVector &corner1, const decVector &corner2, const decVector &corner3, const decVector &normal){
	pCorners[0] = corner1;
	pCorners[1] = corner2;
	pCorners[2] = corner3;
	pEdges[0] = (corner2 - corner1).Normalized();
	pEdges[1] = (corner3 - corner2).Normalized();
	pEdges[2] = (corner1 - corner3).Normalized();
	pNormal = normal;
	pDist = corner1 * pNormal;
}
