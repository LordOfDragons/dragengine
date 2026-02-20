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



// Definitions
////////////////

#define NO_COLLISION 1.0f
//#define NO_COLLISION 2.0f



// Inlines
////////////

inline bool SATMoveHitTest(float speed, float min1, float max1, float min2, float max2, float &first, float &last){
	float t;
	
	if(max2 < min1){ // smaller side
		if(speed <= 0.0f){
			return false;
		}
		
		t = (min1 - max2) / speed;
		if(t > 1.0f){
			return false;
		}
		if(t > first){
			first = t;
		}
		
		t = (max1 - min2) / speed;
		if(t < last){
			if(first > t){
				return false;
			}
			last = t;
		}
		
	}else if(max1 < min2){ // larger side
		if(speed >= 0.0f){
			return false;
		}
		
		t = (max1 - min2) / speed;
		if(t > 1.0f){
			return false;
		}
		if(t > first){
			first = t;
		}
		
		t = (min1 - max2) / speed;
		if(t < last){
			if(first > t){
				return false;
			}
			last = t;
		}
		
	}else{ // overlapping
		if(speed > 0.0f){
			t = (max1 - min2) / speed;
			
			if(t < last){
				if(first > t){
					return false;
				}
				last = t;
			}
			
		}else if(speed < 0.0f){
			t = (min1 - max2) / speed;
			
			if(t < last){
				if(first > t){
					return false;
				}
				last = t;
			}
		}
	}
	
	return true;
}



// Collision Detection Box
////////////////////////////

// constructors, destructors
//////////////////////////////

debpCollisionBox::debpCollisionBox(){
	pHalfSize.Set(1.0f, 1.0f, 1.0f);
	ClearOrientation();
}

debpCollisionBox::debpCollisionBox(const decVector &center, const decVector &halfSize) :
pCenter(center),
pHalfSize(halfSize)
{
	ClearOrientation();
}

debpCollisionBox::debpCollisionBox(const decVector &center, const decVector &halfSize,
const decQuaternion &orientation) :
pCenter(center),
pHalfSize(halfSize)
{
	SetOrientation(orientation);
}

debpCollisionBox::~debpCollisionBox(){
}



// float dispatching calls
/////////////////////////////

bool debpCollisionBox::VolumeHitsVolume(debpCollisionVolume *volume){
	return volume->BoxHitsVolume(this);
}

float debpCollisionBox::VolumeMoveHitsVolume(debpCollisionVolume *volume, const decVector &displacement, decVector *normal){
	return volume->BoxMoveHitsVolume(this, displacement, normal);
}



// first stage calls
//////////////////////

bool debpCollisionBox::SphereHitsVolume(debpCollisionSphere *sphere){
	return SphereHitsBox(sphere);
}

bool debpCollisionBox::CylinderHitsVolume(debpCollisionCylinder *cylinder){
	return CylinderHitsBox(cylinder);
}

bool debpCollisionBox::CapsuleHitsVolume(debpCollisionCapsule *capsule){
	return CapsuleHitsBox(capsule);
}

bool debpCollisionBox::BoxHitsVolume(debpCollisionBox *box){
	return BoxHitsBox(box);
}

bool debpCollisionBox::TriangleHitsVolume(debpCollisionTriangle *triangle){
	return triangle->BoxHitsTriangle(this);
}

bool debpCollisionBox::FrustumHitsVolume(debpCollisionFrustum *frustum){
	return frustum->BoxHitsFrustum(this);
}



float debpCollisionBox::SphereMoveHitsVolume(debpCollisionSphere *sphere, const decVector &displacement, decVector *normal){
	return SphereMoveHitsBox(sphere, displacement, normal);
}

float debpCollisionBox::CylinderMoveHitsVolume(debpCollisionCylinder *cylinder, const decVector &displacement, decVector *normal){
	return CylinderMoveHitsBox(cylinder, displacement, normal);
}

float debpCollisionBox::CapsuleMoveHitsVolume(debpCollisionCapsule *capsule, const decVector &displacement, decVector *normal){
	return CapsuleMoveHitsBox(capsule, displacement, normal);
}

float debpCollisionBox::BoxMoveHitsVolume(debpCollisionBox *box, const decVector &displacement, decVector *normal){
	return BoxMoveHitsBox(box, displacement, normal);
}

float debpCollisionBox::TriangleMoveHitsVolume(debpCollisionTriangle *triangle, const decVector &displacement, decVector *normal){
	float distance = triangle->BoxMoveHitsTriangle(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

float debpCollisionBox::FrustumMoveHitsVolume(debpCollisionFrustum *frustum, const decVector &displacement, decVector *normal){
	float distance = frustum->BoxMoveHitsFrustum(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

float debpCollisionBox::PointMoveHitsVolume(const decVector &point, const decVector &displacement, decVector *normal){
	float lambda, hp, factor;
	float px, py, pz;
	float fpx, fpy, fpz;
	float dx, dy, dz;
	// transform values if required
	if(pOriented){
		decVector transformedPoint = WorldToLocal(point);
		decVector transformedDisplacement = NormalWorldToLocal(displacement);
		px = transformedPoint.x;
		py = transformedPoint.y;
		pz = transformedPoint.z;
		dx = transformedDisplacement.x;
		dy = transformedDisplacement.y;
		dz = transformedDisplacement.z;
	}else{
		px = point.x;
		py = point.y;
		pz = point.z;
		dx = displacement.x;
		dy = displacement.y;
		dz = displacement.z;
	}
	// test if inside
	fpx = fabsf(px);
	fpy = fabsf(py);
	fpz = fabsf(pz);
	if(fpx <= pHalfSize.x && fpy <= pHalfSize.y && fpz <= pHalfSize.z){
		if(normal){
			if(fpx > fpy){
				if(fpx > fpz){
					if(px > 0.0f){
						normal->Set(1.0f, 0.0f, 0.0f);
					}else{
						normal->Set(-1.0f, 0.0f, 0.0f);
					}
				}else{
					if(pz > 0.0f){
						normal->Set(0.0f, 0.0f, 1.0f);
					}else{
						normal->Set(0.0f, 0.0f, -1.0f);
					}
				}
			}else{
				if(fpy > fpz){
					if(py > 0.0f){
						normal->Set(0.0f, 1.0f, 0.0f);
					}else{
						normal->Set(0.0f, -1.0f, 0.0f);
					}
				}else{
					if(pz > 0.0f){
						normal->Set(0.0f, 0.0f, 1.0f);
					}else{
						normal->Set(0.0f, 0.0f, -1.0f);
					}
				}
			}
		}
		return 0.0f;
	}
	// test x-axis
	if(dx < -1e-6f || dx > 1e-6f){
		factor = 1.0f / dx;
		// bf = pHalfSize.x, normal = ( 1, 0, 0 )
		lambda = (pHalfSize.x - px) * factor;
		if(lambda >= 0.0f && lambda < 1.0f){
			hp = py + dy * lambda;
			if(hp >= -pHalfSize.y && hp <= pHalfSize.y){
				hp = pz + dz * lambda;
				if(hp >= -pHalfSize.z && hp <= pHalfSize.z){
					if(normal) normal->Set(pAxisX);
					return lambda;
				}
			}
		}
		// bf = -pHalfSize.x, normal = ( -1, 0, 0 )
		lambda = -(pHalfSize.x + px) * factor;
		if(lambda >= 0.0f && lambda < 1.0f){
			hp = py + dy * lambda;
			if(hp >= -pHalfSize.y && hp <= pHalfSize.y){
				hp = pz + dz * lambda;
				if(hp >= -pHalfSize.z && hp <= pHalfSize.z){
					if(normal) normal->Set(-pAxisX);
					return lambda;
				}
			}
		}
	}
	// test y-axis
	if(dy < -1e-6f || dy > 1e-6f){
		factor = 1.0f / dy;
		// bf = pHalfSize.y, normal = ( 1, 0, 0 )
		lambda = (pHalfSize.y - py) * factor;
		if(lambda >= 0.0f && lambda < 1.0f){
			hp = px + dx * lambda;
			if(hp >= -pHalfSize.x && hp <= pHalfSize.x){
				hp = pz + dz * lambda;
				if(hp >= -pHalfSize.z && hp <= pHalfSize.z){
					if(normal) normal->Set(pAxisY);
					return lambda;
				}
			}
		}
		// bf = -pHalfSize.y, normal = ( -1, 0, 0 )
		lambda = -(pHalfSize.y + py) * factor;
		if(lambda >= 0.0f && lambda < 1.0f){
			hp = px + dx * lambda;
			if(hp >= -pHalfSize.x && hp <= pHalfSize.x){
				hp = pz + dz * lambda;
				if(hp >= -pHalfSize.z && hp <= pHalfSize.z){
					if(normal) normal->Set(-pAxisY);
					return lambda;
				}
			}
		}
	}
	// test z-axis
	if(dz < -1e-6f || dz > 1e-6f){
		factor = 1.0f / dz;
		// bf = pHalfSize.z, normal = ( 1, 0, 0 )
		lambda = (pHalfSize.z - pz) * factor;
		if(lambda >= 0.0f && lambda < 1.0f){
			hp = px + dx * lambda;
			if(hp >= -pHalfSize.x && hp <= pHalfSize.x){
				hp = py + dy * lambda;
				if(hp >= -pHalfSize.y && hp <= pHalfSize.y){
					if(normal) normal->Set(pAxisZ);
					return lambda;
				}
			}
		}
		// bf = pCenter.z - pHalfSize.z, normal = ( -1, 0, 0 )
		lambda = -(pHalfSize.z + pz) * factor;
		if(lambda >= 0.0f && lambda < 1.0f){
			hp = px + dx * lambda;
			if(hp >= -pHalfSize.x && hp <= pHalfSize.x){
				hp = py + dy * lambda;
				if(hp >= -pHalfSize.y && hp <= pHalfSize.y){
					if(normal) normal->Set(-pAxisZ);
					return lambda;
				}
			}
		}
	}
	// no collision
	return 1.0f;
/*
// TODO: returning distance travelled, not simply if hit or not
	float lambda, hp, bf, factor;
	// test x-axis
	if(displacement.x < -0.00001 || displacement.x > 0.00001){
		factor = 1.0 / displacement.x;
		// bf = pCenter.x + pHalfSize.x, normal = ( 1, 0, 0 )
		lambda = (pCenter.x + pHalfSize.x - point.x) * factor;
		if(lambda >= 0){
			hp = pCenter.y + displacement.y * lambda;
			if(hp >= pCenter.y - pHalfSize.y && hp <= pCenter.y + pHalfSize.y){
				hp = pCenter.z + displacement.z * lambda;
				if(hp >= pCenter.z - pHalfSize.z && hp <= pCenter.z + pHalfSize.z){
					return 0.0;
				}
			}
		}
		// bf = pCenter.x - pHalfSize.x, normal = ( -1, 0, 0 )
		lambda = (pCenter.x - pHalfSize.x - pCenter.x) * factor;
		if(lambda >= 0){
			hp = pCenter.y + displacement.y * lambda;
			if(hp >= pCenter.y - pHalfSize.y && hp <= pCenter.y + pHalfSize.y){
				hp = pCenter.z + displacement.z * lambda;
				if(hp >= pCenter.z - pHalfSize.z && hp <= pCenter.z + pHalfSize.z) return 0.0;
			}
		}
	}
	// test y-axis
	if(displacement.y < -0.00001 || displacement.y > 0.00001){
		factor = 1.0 / displacement.y;
		// bf = pCenter.y + pHalfSize.y, normal = ( 1, 0, 0 )
		lambda = (pCenter.y + pHalfSize.y - pCenter.y) * factor;
		if(lambda >= 0){
			hp = pCenter.x + displacement.x * lambda;
			if(hp >= pCenter.x - pHalfSize.x && hp <= pCenter.x + pHalfSize.x){
				hp = pCenter.z + displacement.z * lambda;
				if(hp >= pCenter.z - pHalfSize.z && hp <= pCenter.z + pHalfSize.z) return 0.0;
			}
		}
		// bf = pCenter.y - pHalfSize.y, normal = ( -1, 0, 0 )
		lambda = (pCenter.y - pHalfSize.y - pCenter.y) * factor;
		if(lambda >= 0){
			hp = pCenter.x + displacement.x * lambda;
			if(hp >= pCenter.x - pHalfSize.x && hp <= pCenter.x + pHalfSize.x){
				hp = pCenter.z + displacement.z * lambda;
				if(hp >= pCenter.z - pHalfSize.z && hp <= pCenter.z + pHalfSize.z) return 0.0;
			}
		}
	}
	// test z-axis
	if(displacement.z < -0.00001 || displacement.z > 0.00001){
		factor = 1.0 / displacement.z;
		// bf = pCenter.z + pHalfSize.z, normal = ( 1, 0, 0 )
		lambda = (pCenter.z + pHalfSize.z - pCenter.z) * factor;
		if(lambda >= 0){
			hp = pCenter.x + displacement.x * lambda;
			if(hp >= pCenter.x - pHalfSize.x && hp <= pCenter.x + pHalfSize.x){
				hp = pCenter.y + displacement.y * lambda;
				if(hp >= pCenter.y - pHalfSize.y && hp <= pCenter.y + pHalfSize.y) return 0.0;
			}
		}
		// bf = pCenter.z - pHalfSize.z, normal = ( -1, 0, 0 )
		lambda = (pCenter.z - pHalfSize.z - pCenter.z) * factor;
		if(lambda >= 0){
			hp = pCenter.x + displacement.x * lambda;
			if(hp >= pCenter.x - pHalfSize.x && hp <= pCenter.x + pHalfSize.x){
				hp = pCenter.y + displacement.y * lambda;
				if(hp >= pCenter.y - pHalfSize.y && hp <= pCenter.y + pHalfSize.y) return 0.0;
			}
		}
	}
	// no collision
	return 1.0;
}
*/
}



// collision routines
///////////////////////

bool debpCollisionBox::SphereHitsBox(debpCollisionSphere *sphere){
	float temp, dist = 0.0f;
	float sx, sy, sz;
	// transform values if required
	if(pOriented){
		decVector transformedSphere = WorldToLocal(sphere->GetCenter());
		sx = fabsf(transformedSphere.x);
		sy = fabsf(transformedSphere.y);
		sz = fabsf(transformedSphere.z);
	}else{
		const decVector &sphereCenter = sphere->GetCenter();
		sx = fabsf(sphereCenter.x - pCenter.x);
		sy = fabsf(sphereCenter.y - pCenter.y);
		sz = fabsf(sphereCenter.z - pCenter.z);
	}
	// determine square distance of sphere center to box outline
	if(sx > pHalfSize.x){
		temp = sx - pHalfSize.x;
		dist += temp * temp;
	}
	if(sy > pHalfSize.y){
		temp = sy - pHalfSize.y;
		dist += temp * temp;
	}
	if(sz > pHalfSize.z){
		temp = sz - pHalfSize.z;
		dist += temp * temp;
	}
	// return result
	return dist <= sphere->GetSquareRadius();
	
	/*
	decVector sphereCenter = sphere->GetCenter();
	decVector boxMin = pCenter - pHalfSize;
	decVector boxMax = pCenter + pHalfSize;
	float squareRadius = sphere->GetSquareRadius();
	float temp, dist=0;
	// determine square distance of sphere center to box outline
	// x-axis
	if(sphereCenter.x < boxMin.x){
		temp = sphereCenter.x - boxMin.x;
		dist += temp * temp;
	}
	if(sphereCenter.x > boxMax.x){
		temp = sphereCenter.x - boxMax.x;
		dist += temp * temp;
	}
	// y-axis
	if(sphereCenter.y < boxMin.y){
		temp = sphereCenter.y - boxMin.y;
		dist += temp * temp;
	}
	if(sphereCenter.y > boxMax.y){
		temp = sphereCenter.y - boxMax.y;
		dist += temp * temp;
	}
	// z-axis
	if(sphereCenter.z < boxMin.z){
		temp = sphereCenter.z - boxMin.z;
		dist += temp * temp;
	}
	if(sphereCenter.z > boxMax.z){
		temp = sphereCenter.z - boxMax.z;
		dist += temp * temp;
	}
	// return result
	return dist <= squareRadius;
	*/
}

bool debpCollisionBox::CylinderHitsBox(debpCollisionCylinder *cylinder){
	return false;
}

bool debpCollisionBox::CapsuleHitsBox(debpCollisionCapsule *capsule){
	/*
	float radius = capsule->GetRadius();
	decVector bmin = pCenter - pHalfSize;
	decVector bmax = pCenter + pHalfSize;
	decVector cdiff, hp, corners[8];
	float lambda, factor;
	decVector cs, ce;
	// transform if required
	if(pOriented){
		cs = WorldToLocal(capsule->GetStartPoint());
		ce = WorldToLocal(capsule->GetEndPoint());
	}else{
		cs = capsule->GetStartPoint();
		ce = capsule->GetEndPoint();
	}
	// test if one of the segment entpoints is inside the aabb
	if(cs.x >= bmin.x && cs.x <= bmax.x &&
		cs.y >= bmin.y && cs.y <= bmax.y &&
		cs.z >= bmin.z && cs.z <= bmax.z) return true;
	if(ce.x >= bmin.x && ce.x <= bmax.x &&
		ce.y >= bmin.y && ce.y <= bmax.y &&
		ce.z >= bmin.z && ce.z <= bmax.z) return true;
	// test extended faces
	cdiff = ce - cs;
	if(cdiff.x > 0.00001 || cdiff.x < -0.00001){
		factor = 1.0 / cdiff.x;
		lambda = (bmin.x - radius - cs.x) * factor;
		if(lambda >= 0 && lambda <= 1){
			hp = cs + cdiff * lambda;
			if(hp.y >= bmin.y && hp.y <= bmax.y && hp.z >= bmin.z && hp.z <= bmax.z) return true;
		}
		lambda = (bmax.x + radius - cs.x) * factor;
		if(lambda >= 0 && lambda <= 1){
			hp = cs + cdiff * lambda;
			if(hp.y >= bmin.y && hp.y <= bmax.y && hp.z >= bmin.z && hp.z <= bmax.z) return true;
		}
	}
	if(cdiff.y > 0.00001 || cdiff.y < -0.00001){
		factor = 1.0 / cdiff.y;
		lambda = (bmin.y - radius - cs.y) * factor;
		if(lambda >= 0 && lambda <= 1){
			hp = cs + cdiff * lambda;
			if(hp.x >= bmin.x && hp.x <= bmax.x && hp.z >= bmin.z && hp.z <= bmax.z) return true;
		}
		lambda = (bmax.y + radius - cs.y) * factor;
		if(lambda >= 0 && lambda <= 1){
			hp = cs + cdiff * lambda;
			if(hp.x >= bmin.x && hp.x <= bmax.x && hp.z >= bmin.z && hp.z <= bmax.z) return true;
		}
	}
	if(cdiff.z > 0.00001 || cdiff.z < -0.00001){
		factor = 1.0 / cdiff.z;
		lambda = (bmin.z - radius - cs.z) * factor;
		if(lambda >= 0 && lambda <= 1){
			hp = cs + cdiff * lambda;
			if(hp.x >= bmin.x && hp.x <= bmax.x && hp.y >= bmin.y && hp.y <= bmax.y) return true;
		}
		lambda = (bmax.z + radius - cs.z) * factor;
		if(lambda >= 0 && lambda <= 1){
			hp = cs + cdiff * lambda;
			if(hp.x >= bmin.x && hp.x <= bmax.x && hp.y >= bmin.y && hp.y <= bmax.y) return true;
		}
	}
	// fat edges tests
	corners[0] = decVector(bmin.x, bmax.y, bmin.z);
	corners[1] = decVector(bmax.x, bmax.y, bmin.z);
	corners[2] = decVector(bmax.x, bmin.y, bmin.z);
	corners[3] = decVector(bmin.x, bmin.y, bmin.z);
	corners[4] = decVector(bmin.x, bmax.y, bmax.z);
	corners[5] = decVector(bmax.x, bmax.y, bmax.z);
	corners[6] = decVector(bmax.x, bmin.y, bmax.z);
	corners[7] = decVector(bmin.x, bmin.y, bmax.z);
	// front 4 edges
	if(debpDECollisionDetection::SegmentSegmentDistance(cs, ce, corners[0], corners[1]) <= radius) return true;
	if(debpDECollisionDetection::SegmentSegmentDistance(cs, ce, corners[1], corners[2]) <= radius) return true;
	if(debpDECollisionDetection::SegmentSegmentDistance(cs, ce, corners[2], corners[3]) <= radius) return true;
	if(debpDECollisionDetection::SegmentSegmentDistance(cs, ce, corners[3], corners[0]) <= radius) return true;
	// back 4 edges
	if(debpDECollisionDetection::SegmentSegmentDistance(cs, ce, corners[4], corners[5]) <= radius) return true;
	if(debpDECollisionDetection::SegmentSegmentDistance(cs, ce, corners[5], corners[6]) <= radius) return true;
	if(debpDECollisionDetection::SegmentSegmentDistance(cs, ce, corners[6], corners[7]) <= radius) return true;
	if(debpDECollisionDetection::SegmentSegmentDistance(cs, ce, corners[7], corners[4]) <= radius) return true;
	// connection 4 edges
	if(debpDECollisionDetection::SegmentSegmentDistance(cs, ce, corners[0], corners[4]) <= radius) return true;
	if(debpDECollisionDetection::SegmentSegmentDistance(cs, ce, corners[1], corners[5]) <= radius) return true;
	if(debpDECollisionDetection::SegmentSegmentDistance(cs, ce, corners[2], corners[6]) <= radius) return true;
	if(debpDECollisionDetection::SegmentSegmentDistance(cs, ce, corners[3], corners[7]) <= radius) return true;
	// it has to miss
	*/
	return false;
}

bool debpCollisionBox::BoxHitsBox(debpCollisionBox *box){
	// if one of the boxes is not axis aligned we have to use the heavier algorithm
	if(pOriented || box->GetOriented()){
		const decVector centerDiff = box->GetCenter() - pCenter;
		const decVector &bax = box->GetAxisX();
		const decVector &bay = box->GetAxisY();
		const decVector &baz = box->GetAxisZ();
		const decVector &bhs = box->GetHalfSize();
		decVector axis;
		// axes of box 1
		if(fabsf(pAxisX * centerDiff) - box->ProjectExtends(pAxisX) > pHalfSize.x) return false;
		if(fabsf(pAxisY * centerDiff) - box->ProjectExtends(pAxisY) > pHalfSize.y) return false;
		if(fabsf(pAxisZ * centerDiff) - box->ProjectExtends(pAxisZ) > pHalfSize.z) return false;
		// axes of box 2
		if(fabsf(bax * centerDiff) - bhs.x > ProjectExtends(bax)) return false;
		if(fabsf(bay * centerDiff) - bhs.y > ProjectExtends(bay)) return false;
		if(fabsf(baz * centerDiff) - bhs.z > ProjectExtends(baz)) return false;
		// axis x of box 1 cross axes of box 2
		axis = pAxisX % bax;
		if(fabsf(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		axis = pAxisX % bay;
		if(fabsf(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		axis = pAxisX % baz;
		if(fabsf(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		// axis y of box 1 cross axes of box 2
		axis = pAxisY % bax;
		if(fabsf(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		axis = pAxisY % bay;
		if(fabsf(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		axis = pAxisY % baz;
		if(fabsf(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		// axis z of box 1 cross axes of box 2
		axis = pAxisZ % bax;
		if(fabsf(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		axis = pAxisZ % bay;
		if(fabsf(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		axis = pAxisZ % baz;
		if(fabsf(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		// we do intersect
		return true;
	
	// otherwise a simple test does the trick
	}else{
		const decVector &hs = box->GetHalfSize();
		decVector dist = box->GetCenter() - pCenter;
		return fabsf(dist.x) <= (pHalfSize.x + hs.x) &&
			fabsf(dist.y) <= (pHalfSize.y + hs.y) &&
			fabsf(dist.z) <= (pHalfSize.z + hs.z);
	}
}



float debpCollisionBox::SphereMoveHitsBox(debpCollisionSphere *sphere, const decVector &displacement, decVector *normal){
	const float r = sphere->GetRadius();
	const float rsquared = r * r;
	decVector so, d;
	
	// transform if required
	if(pOriented){
		so = WorldToLocal(sphere->GetCenter());
		d = NormalWorldToLocal(displacement);
		
	}else{
		so = sphere->GetCenter() - pCenter;
		d = displacement;
	}
	
	// test if the sphere is inside the box
	if((so - pAxisAlignedCPTo(so)).LengthSquared() <= r){
		// the center of the sphere is inside the box. this is a stupid situation
		// as determining a plane normal for this case is difficult to do. what we
		// do here to solve this situation is to determine the coordinate axis in
		// which direction there is the most amount we have to travel until we
		// hit the border. this way sliding along the plane gets us out of the box
		// the quickest.
		if(normal){
			const float dx = pHalfSize.x - fabsf(so.x);
			const float dy = pHalfSize.y - fabsf(so.y);
			const float dz = pHalfSize.z - fabsf(so.z);
			
			if(dx > dy){
				if(dx > dz){
					*normal = so.x > 0.0f ? pAxisX : -pAxisX;
					
				}else{
					*normal = so.z > 0.0f ? pAxisY : -pAxisY;
				}
				
			}else{
				if(dy > dz){
					*normal = so.y > 0.0f ? pAxisY : -pAxisY;
					
				}else{
					*normal = so.z > 0.0f ? pAxisZ : -pAxisZ;
				}
			}
		}
		
		return 0.0f;
	}
	
	// flip all axes to positive so the number of tests can be reduced
	decVector st(so);
	
	if(st.x < 0.0f){
		st.x = -st.x;
		d.x = -d.x;
	}
	if(st.y < 0.0f){
		st.y = -st.y;
		d.y = -d.y;
	}
	if(st.z < 0.0f){
		st.z = -st.z;
		d.z = -d.z;
	}
	
	// in the case of faces an early exit can be done if a result is found. this is though only
	// valid if the sphere center at the collision point is located inside the volume formed by
	// the tested box face extruded along the face normal. if the sphere i outside this volume
	// during the contact another face, edge or corner yields a closer hit distance
	
	// test x axis
	if(st.x > pHalfSize.x + r){
		if(d.x >= -1e-9f){
			return NO_COLLISION;
		}
		const float lambda = (pHalfSize.x - st.x + r) / d.x;
		if(lambda > 1.0f){
			return NO_COLLISION;
		}
		float hpsc = st.y + d.y * lambda; // hit point sphere center
		if(hpsc >= -pHalfSize.y && hpsc <= pHalfSize.y){
			hpsc = st.z + d.z * lambda;
			if(hpsc >= -pHalfSize.z && hpsc <= pHalfSize.z){
				if(normal){
					*normal = so.x > 0.0f ? pAxisX : -pAxisX;
				}
				return lambda;
			}
		}
	}
	
	// test y axis
	if(st.y > pHalfSize.y + r){
		if(d.y >= -1e-9f){
			return NO_COLLISION;
		}
		const float lambda = (pHalfSize.y - st.y + r) / d.y;
		if(lambda > 1.0f){
			return NO_COLLISION;
		}
		float hpsc = st.x + d.x * lambda; // hit point sphere center
		if(hpsc >= -pHalfSize.x && hpsc <= pHalfSize.x){
			hpsc = st.z + d.z * lambda;
			if(hpsc >= -pHalfSize.z && hpsc <= pHalfSize.z){
				if(normal){
					*normal = so.y > 0.0f ? pAxisY : -pAxisY;
				}
				return lambda;
			}
		}
	}
	
	// test z axis
	if(st.z > pHalfSize.z + r){
		if(d.z >= -1e-9f){
			return NO_COLLISION;
		}
		const float lambda = (pHalfSize.z - st.z + r) / d.z;
		if(lambda > 1.0f){
			return NO_COLLISION;
		}
		float hpsc = st.x + d.x * lambda; // hit point sphere center
		if(hpsc >= -pHalfSize.x && hpsc <= pHalfSize.x){
			hpsc = st.y + d.y * lambda;
			if(hpsc >= -pHalfSize.y && hpsc <= pHalfSize.y){
				if(normal){
					*normal = so.z > 0.0f ? pAxisZ : -pAxisZ;
				}
				return lambda;
			}
		}
	}
	
	// test edges. the tests below are optimized versions of this general version
	// (with the edge defined by the vertices a and b):
	// 
	// vector e, c1, c2
	// double p, q, denom
	// 
	// e = ( b - a ) / abs( b - a )
	// c1 = a + e * ( ( s - a ) dot e ) - s
	// c2 = e * ( d dot e ) - d
	// denom = c2 dot c2;
	// if denom == 0: no-hit
	// p = 2 * ( c1 dot c2 ) / denom
	// q = ( ( c1 dot c1 ) - r*r ) / denom
	// denom = p * p * 0.25 - q
	// if denom < 0: no-hit
	// if denom > 0:
	//    lambda = -p * 0.5 - sqrt( denom )
	//    if lambda < 0:
	//       lambda = -p * 0.5 + sqrt( denom )
	// else:
	//    lambda = -p * 0.5
	// if lambda > 1: no-hit
	// test hit point if it is on the edge. hit point is defined as:
	// hp = a + e * ( ( s - a + d * lambda ) dot e )
	// 
	// note about the length test for normalization. edges are also hit if they are not the best edge.
	// if the normal becomes 0 the sphere ends up in the box and thus the tested edge can never be
	// the best edge. it is skipped to avoid normalization issues as well as delivering a bad normal
	decVector n;
	
	// for edges the early exit is correct if the sphere is located in the volume starting at the
	// edge expanding along the two face axes connected to the edge but not above and beyond. this
	// covers the area outside the face volume belonging to the two faces connected to the edge
	// without the corner area. the early exit due to lambda >= 1 is not correct for edges.
	
	// test x edge
	float denom = d.y * d.y + d.z * d.z;
	if(denom != 0.0f){
		denom = 1.0f / denom;
		const float c1a = pHalfSize.y - st.y;
		const float c1b = pHalfSize.z - st.z;
		const float p = -2.0f * (c1a * d.y + c1b * d.z) * denom;
		const float q = (c1a * c1a + c1b * c1b - rsquared) * denom;
		denom = p * p * 0.25f - q;
		if(denom >= 0.0f){
			float lambda = -0.5f * p;
			if(denom > 0.0f){
				denom = sqrt(denom);
				lambda -= denom;
				if(lambda < 0.0f) lambda += denom * 2.0f;
			}
			if(lambda >= 0.0f && lambda < 1.0){
				float hpsc = st.x + d.x * lambda; // hit point sphere center
				if(hpsc >= -pHalfSize.x && hpsc <= pHalfSize.x){
					hpsc = st.y + d.y * lambda;
					if(hpsc >= pHalfSize.y){
						hpsc = st.z + d.z * lambda;
						if(hpsc >= pHalfSize.z){
							const decVector sn(so + d * lambda);
							n = sn - pAxisAlignedCPTo(sn);
							const float len = n.Length();
							if(len > FLOAT_EPSILON){
								if(normal){
									n /= len;
									*normal = pOriented ? NormalLocalToWorld( n ) : n;
								}
								return lambda;
							}
						}
					}
				}
			}
		}
	}
	
	// test y edge
	denom = d.x * d.x + d.z * d.z;
	if(denom != 0.0f){
		denom = 1.0f / denom;
		const float c1a = pHalfSize.x - st.x;
		const float c1b = pHalfSize.z - st.z;
		const float p = -2.0f * (c1a * d.x + c1b * d.z) * denom;
		const float q = (c1a * c1a + c1b * c1b - rsquared) * denom;
		denom = p * p * 0.25f - q;
		if(denom >= 0.0f){
			float lambda = -0.5f * p;
			if(denom > 0.0f){
				denom = sqrt(denom);
				lambda -= denom;
				if(lambda < 0.0f) lambda += denom * 2.0f;
			}
			if(lambda >= 0.0f && lambda < 1.0){
				float hpsc = st.y + d.y * lambda; // hit point sphere center
				if(hpsc >= -pHalfSize.y && hpsc <= pHalfSize.y){
					hpsc = st.x + d.x * lambda;
					if(hpsc >= pHalfSize.x){
						hpsc = st.z + d.z * lambda;
						if(hpsc >= pHalfSize.z){
							const decVector sn(so + d * lambda);
							n = sn - pAxisAlignedCPTo(sn);
							const float len = n.Length();
							if(len > FLOAT_EPSILON){
								if(normal){
									n /= len;
									*normal = pOriented ? NormalLocalToWorld( n ) : n;
								}
								return lambda;
							}
						}
					}
				}
			}
		}
	}
	
	// test z edge
	denom = d.x * d.x + d.y * d.y;
	if(denom != 0.0f){
		denom = 1.0f / denom;
		const float c1a = pHalfSize.x - st.x;
		const float c1b = pHalfSize.y - st.y;
		const float p = -2.0f * (c1a * d.x + c1b * d.y) * denom;
		const float q = (c1a * c1a + c1b * c1b - rsquared) * denom;
		denom = p * p * 0.25f - q;
		if(denom >= 0.0f){
			float lambda = -0.5f * p;
			if(denom > 0.0f){
				denom = sqrt(denom);
				lambda -= denom;
				if(lambda < 0.0f) lambda += denom * 2.0f;
			}
			if(lambda >= 0.0f && lambda < 1.0){
				float hpsc = st.z + d.z * lambda; // hit point sphere center
				if(hpsc >= -pHalfSize.z && hpsc <= pHalfSize.z){
					hpsc = st.x + d.x * lambda;
					if(hpsc >= pHalfSize.x){
						hpsc = st.y + d.y * lambda;
						if(hpsc >= pHalfSize.y){
							const decVector sn(so + d * lambda);
							n = sn - pAxisAlignedCPTo(sn);
							const float len = n.Length();
							if(len > FLOAT_EPSILON){
								if(normal){
									n /= len;
									*normal = pOriented ? NormalLocalToWorld( n ) : n;
								}
								return lambda;
							}
						}
					}
				}
			}
		}
	}
	
	// test corner
	const decVector a(st - pHalfSize);
	denom = d * d;
	if(denom != 0.0f){
		denom = 1.0f / denom;
		const float p = (a * d) * 2.0f * denom;
		const float q = (a.LengthSquared() - rsquared) * denom;
		denom = p * p * 0.25f - q;
		if(denom >= 0.0f){
			float lambda = -0.5f * p;
			if(denom > 0.0f){
				denom = sqrt(denom);
				lambda -= denom;
				if(lambda < 0.0f) lambda += denom * 2.0f;
			}
			
			if(lambda >= 0.0f && lambda < 1.0f){
				const decVector sn(so + d * lambda);
				n = sn - pAxisAlignedCPTo(sn);
				const float len = n.Length();
				if(len > FLOAT_EPSILON){
					if(normal){
						n /= len;
						*normal = pOriented ? NormalLocalToWorld( n ) : n;
					}
					return lambda;
				}
			}
		}
	}
	
	// if we go here there has been no hit
	return NO_COLLISION;
}

float debpCollisionBox::CylinderMoveHitsBox(debpCollisionCylinder *cylinder, const decVector &displacement, decVector *normal){
	return 1;
}

float debpCollisionBox::CapsuleMoveHitsBox(debpCollisionCapsule *capsule, const decVector &displacement, decVector *normal){
	return 1;
}

float debpCollisionBox::BoxMoveHitsBox(debpCollisionBox *box, const decVector &displacement, decVector *normal){
	float curDist, dispDist;
	float bestDist = 0.0f;
	decVector bestNormal;
	float normalValue;
	
	// if one of the boxes is not axis aligned we have to use the heavier algorithm
	if(pOriented || box->GetOriented()){
		const decVector centerDiff(box->GetCenter() - pCenter);
		const decVector &bax = box->GetAxisX();
		const decVector &bay = box->GetAxisY();
		const decVector &baz = box->GetAxisZ();
		const decVector &bhs = box->GetHalfSize();
		decVector axis;
		
		// axes of box 1
		curDist = centerDiff * pAxisX;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= box->ProjectExtends(pAxisX) + pHalfSize.x;
		if(curDist > 0.0f){
			dispDist = -((displacement * pAxisX) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(pAxisX * normalValue);
			}
		}
		
		curDist = centerDiff * pAxisY;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= box->ProjectExtends(pAxisY) + pHalfSize.y;
		if(curDist > 0.0f){
			dispDist = -((displacement * pAxisY) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(pAxisY * normalValue);
			}
		}
		
		curDist = centerDiff * pAxisZ;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= box->ProjectExtends(pAxisZ) + pHalfSize.z;
		if(curDist > 0.0f){
			dispDist = -((displacement * pAxisZ) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(pAxisZ * normalValue);
			}
		}
		
		// axes of box 2
		curDist = bax * centerDiff;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= bhs.x + ProjectExtends(bax);
		if(curDist > 0.0f){
			dispDist = -((displacement * bax) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(bax * normalValue);
			}
		}
		
		curDist = bay * centerDiff;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= bhs.y + ProjectExtends(bay);
		if(curDist > 0.0f){
			dispDist = -((displacement * bay) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(bay * normalValue);
			}
		}
		
		curDist = baz * centerDiff;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= bhs.z + ProjectExtends(baz);
		if(curDist > 0.0f){
			dispDist = -((displacement * baz) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(baz * normalValue);
			}
		}
		
		// axis x of box 1 cross axes of box 2
		axis = pAxisX % bax;
		curDist = axis * centerDiff;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0f){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		axis = pAxisX % bay;
		curDist = axis * centerDiff;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0f){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		axis = pAxisX % baz;
		curDist = axis * centerDiff;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0f){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		// axis y of box 1 cross axes of box 2
		axis = pAxisY % bax;
		curDist = axis * centerDiff;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0f){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		axis = pAxisY % bay;
		curDist = axis * centerDiff;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0f){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		axis = pAxisY % baz;
		curDist = axis * centerDiff;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0f){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		// axis z of box 1 cross axes of box 2
		axis = pAxisZ % bax;
		curDist = axis * centerDiff;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0f){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		axis = pAxisZ % bay;
		curDist = axis * centerDiff;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0f){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		axis = pAxisZ % baz;
		curDist = axis * centerDiff;
		if(curDist > 0.0f){
			normalValue = 1.0f;
		}else{
			curDist = -curDist;
			normalValue = -1.0f;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0f){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0f;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
	
	// otherwise a simple test does the trick
	}else{
		// flip problem space into positive x,y,z
		decVector cdiff = box->GetCenter() - pCenter;
		decVector disp = displacement;
		bool flip[3] = {false, false, false};
		
		if(cdiff.x < 0.0f){
			cdiff.x = -cdiff.x;
			disp.x = -disp.x;
			flip[0] = true;
		}
		if(cdiff.y < 0.0f){
			cdiff.y = -cdiff.y;
			disp.y = -disp.y;
			flip[1] = true;
		}
		if(cdiff.z < 0.0f){
			cdiff.z = -cdiff.z;
			disp.z = -disp.z;
			flip[2] = true;
		}
		
		// calculate parameters
		const float cextx = box->GetHalfSize().x + pHalfSize.x;
		const float cexty = box->GetHalfSize().y + pHalfSize.y;
		const float cextz = box->GetHalfSize().z + pHalfSize.z;
		const float gapx = cdiff.x - cextx;
		const float gapy = cdiff.y - cexty;
		const float gapz = cdiff.z - cextz;
		
		// if in contact at 0 distance use the normal separating the objects the fastest
		if(gapx <= 0.0f && gapy <= 0.0f && gapz <= 0.0f){
			if(normal){
				// gap is negative while penetrating so checking for the smallest
				// penetration means checking for the largest gap
				if(gapx > gapy && gapx > gapz){
					normal->Set(flip[0] ? -1.0f : 1.0f, 0.0f, 0.0f);
					
				}else if(gapy > gapz){
					normal->Set(0.0f, flip[1] ? -1.0f : 1.0f, 0.0f);
					
				}else{
					normal->Set(0.0f, 0.0f, flip[2] ? -1.0f : 1.0f);
				}
			}
			return 0.0f;
		}
		
		// quick exit test. if either axis is a separation axis no collision can happen
		if(gapx > 0.0f && gapx + disp.x >= 0.0f){
			return 1.0f;
		}
		if(gapy > 0.0f && gapy + disp.y >= 0.0f){
			return 1.0f;
		}
		if(gapz > 0.0f && gapz + disp.z >= 0.0f){
			return 1.0f;
		}
		
		// check if x axis face of box is hit
		if(gapx > 0.0f){
			curDist = gapx / -disp.x;
			if(fabsf(cdiff.y + disp.y * curDist) <= cexty && fabsf(cdiff.z + disp.z * curDist) <= cextz){
				if(normal){
					normal->Set(flip[0] ? -1.0f : 1.0f, 0.0f, 0.0f);
				}
				return curDist;
			}
		}
		
		// check if y axis face of box is hit
		if(gapy > 0.0f){
			curDist = gapy / -disp.y;
			if(fabsf(cdiff.x + disp.x * curDist) <= cextx && fabsf(cdiff.z + disp.z * curDist) <= cextz){
				if(normal){
					normal->Set(0.0f, flip[0] ? -1.0f : 1.0f, 0.0f);
				}
				return curDist;
			}
		}
		
		// check if z axis face of box is hit
		if(gapz > 0.0f){
			curDist = gapz / -disp.z;
			if(fabsf(cdiff.x + disp.x * curDist) <= cextx && fabsf(cdiff.y + disp.y * curDist) <= cexty){
				if(normal){
					normal->Set(0.0f, 0.0f, flip[0] ? -1.0f : 1.0f);
				}
				return curDist;
			}
		}
		
		// no collision found
		return 1.0f;
	}
	
	// otherwise return the found result
	if(normal) normal->Set(bestNormal);
	return bestDist;
}



// Enclosing Volumes
//////////////////////

void debpCollisionBox::GetEnclosingSphere(debpCollisionSphere *sphere){
	if(!sphere) DETHROW(deeInvalidParam);
	sphere->SetAll(pCenter, pHalfSize.Length());
}

void debpCollisionBox::GetEnclosingBox(debpCollisionBox *box){
	if(!box){
		DETHROW(deeInvalidParam);
	}
	
	box->SetCenter(pCenter);
	
	if(pOriented){
		/*
		box->SetHalfSize(decVector(ProjectExtends(decVector(1.0f, 0.0f, 0.0f)),
			ProjectExtends(decVector(0.0f, 1.0f, 0.0f)),
			ProjectExtends(decVector(0.0f, 0.0f, 1.0f))));
		*/
		box->SetHalfSize(decVector(			fabsf(pAxisX.x) * pHalfSize.x + fabsf(pAxisY.x) * pHalfSize.y + fabsf(pAxisZ.x) * pHalfSize.z,
			fabsf(pAxisX.y) * pHalfSize.x + fabsf(pAxisY.y) * pHalfSize.y + fabsf(pAxisZ.y) * pHalfSize.z,
			fabsf(pAxisX.z) * pHalfSize.x + fabsf(pAxisY.z) * pHalfSize.y + fabsf(pAxisZ.z) * pHalfSize.z
));
		
	}else{
		box->SetHalfSize(pHalfSize);
	}
	
	box->ClearOrientation();
}



// Miscelanous Functions
//////////////////////////

bool debpCollisionBox::IsPointInside(const decVector &point){
	const decVector localPoint = WorldToLocal(point);
	
	return localPoint >= -pHalfSize && localPoint <= pHalfSize;
}

decVector debpCollisionBox::ClosestPointTo(const decVector &point){
	if(pOriented){
		return LocalToWorld(pAxisAlignedCPTo(WorldToLocal(point)));
	}else{
		return pCenter + pAxisAlignedCPTo(point - pCenter);
	}
}

decVector debpCollisionBox::NormalAtPoint(const decVector &point){
	decVector localPoint = WorldToLocal(point);
	bool sideXPos = (localPoint.x - pHalfSize.x >= -1e-6f);
	bool sideXNeg = (localPoint.x + pHalfSize.x <= 1e-6f);
	bool sideYPos = (localPoint.y - pHalfSize.y >= -1e-6f);
	bool sideYNeg = (localPoint.y + pHalfSize.y <= 1e-6f);
	bool sideZPos = (localPoint.z - pHalfSize.z >= -1e-6f);
	bool sideZNeg = (localPoint.z + pHalfSize.z <= 1e-6f);
	bool sideX = (sideXPos || sideXNeg);
	bool sideY = (sideYPos || sideYNeg);
	bool sideZ = (sideZPos || sideZNeg);
	
	// normal along x axis
	if(sideX && !sideY && !sideZ){
		if(sideXPos){
			return pAxisX;
			
		}else{
			return -pAxisX;
		}
	}
	
	// normal along y axis
	if(sideY && !sideX && !sideZ){
		if(sideYPos){
			return pAxisY;
			
		}else{
			return -pAxisY;
		}
	}
	
	// normal along z axis
	if(sideZ && !sideX && !sideY){
		if(sideZPos){
			return pAxisZ;
			
		}else{
			return -pAxisZ;
		}
	}
	
	// normal inside
	if(!sideX && !sideY && !sideZ){
		float fx = fabsf(localPoint.x);
		float fy = fabsf(localPoint.y);
		float fz = fabsf(localPoint.z);
		
		if(fx > fy && fx > fz){
			if(localPoint.x > 0.0f){
				return pAxisX;
				
			}else{
				return -pAxisX;
			}
			
		}else if(fy > fx && fy > fz){
			if(localPoint.y > 0.0f){
				return pAxisY;
				
			}else{
				return -pAxisY;
			}
			
		}else{
			if(localPoint.z > 0.0f){
				return pAxisZ;
				
			}else{
				return -pAxisZ;
			}
		}
	}
	
	// normal along an edge or corner
	decVector normal = localPoint - pAxisAlignedCPTo(localPoint);
	float length = normal.Length();
	
	if(length < 1e-6f){
		normal.SetZero();
		
		if(sideXPos){
			normal += pAxisX;
			
		}else if(sideXNeg){
			normal -= pAxisX;
		}
		
		if(sideYPos){
			normal += pAxisY;
			
		}else if(sideYNeg){
			normal -= pAxisY;
		}
		
		if(sideZPos){
			normal += pAxisZ;
			
		}else if(sideZNeg){
			normal -= pAxisZ;
		}
		normal.Normalize();
		return normal;
		
	}else{
		return NormalLocalToWorld(normal / length);
	}
}

bool debpCollisionBox::RayHitsVolume(const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance){
	if(pOriented){
		return debpDECollisionDetection::RayHitsBox(WorldToLocal(rayOrigin), NormalWorldToLocal(rayDirection),
			decVector(), pHalfSize, hitDistance);
		
	}else{
		return debpDECollisionDetection::RayHitsBox(rayOrigin, rayDirection, pCenter, pHalfSize, hitDistance);
	}
}



// Visiting
/////////////

void debpCollisionBox::Visit(debpCollisionVolumeVisitor *visitor){
	if(!visitor) DETHROW(deeInvalidParam);
	visitor->VisitBox(this);
}



// management
///////////////

void debpCollisionBox::SetCenter(const decVector &center){
	pCenter = center;
}

void debpCollisionBox::SetHalfSize(const decVector &halfSize){
	if(halfSize.x < -1e-6f || halfSize.y < -1e-6f || halfSize.z < -1e-6f) DETHROW(deeInvalidParam);
	pHalfSize = halfSize;
}

void debpCollisionBox::SetOrientation(const decQuaternion &orientation){
	decMatrix matrix = decMatrix::CreateFromQuaternion(orientation);
	pOrientation = orientation;
	pAxisX.Set(matrix.TransformRight());
	pAxisY.Set(matrix.TransformUp());
	pAxisZ.Set(matrix.TransformView());
	pOriented = true;
}

void debpCollisionBox::ClearOrientation(){
	pOrientation = decQuaternion();
	pAxisX.Set(1.0f, 0.0f, 0.0f);
	pAxisY.Set(0.0f, 1.0f, 0.0f);
	pAxisZ.Set(0.0f, 0.0f, 1.0f);
	pOriented = false;
}

void debpCollisionBox::SetFromExtends(const decVector &minExtend, const decVector &maxExtend){
	pHalfSize = (maxExtend - minExtend) * 0.5f;
	pCenter = minExtend + pHalfSize;
	ClearOrientation();
}

void debpCollisionBox::MoveBy(const decVector &offset){
	pCenter += offset;
}



// private functions
//////////////////////

decVector debpCollisionBox::WorldToLocal(const decVector &point) const{
	const decVector tp = point - pCenter;
	return decVector(pAxisX * tp, pAxisY * tp, pAxisZ * tp);
}

decVector debpCollisionBox::LocalToWorld(const decVector &point) const{
	return pCenter + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decVector debpCollisionBox::NormalWorldToLocal(const decVector &normal) const{
	return decVector(pAxisX * normal, pAxisY * normal, pAxisZ * normal);
}

decVector debpCollisionBox::NormalLocalToWorld(const decVector &normal) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}

decVector debpCollisionBox::pAxisAlignedCPTo(const decVector &point){
	return point.Clamped(-pHalfSize, pHalfSize);
}
