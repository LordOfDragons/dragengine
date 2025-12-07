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

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "deoalDCollisionDetection.h"
#include "deoalDCollisionSphere.h"
#include "deoalDCollisionCylinder.h"
#include "deoalDCollisionCapsule.h"
#include "deoalDCollisionBox.h"
#include "deoalDCollisionTriangle.h"
#include "deoalDCollisionFrustum.h"
#include "deoalDCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define NO_COLLISION 1.0
//#define NO_COLLISION 2.0



// Inlines
////////////

inline bool SATMoveHitTest(double speed, double min1, double max1, double min2, double max2, double &first, double &last){
	double t;
	
	if(max2 < min1){ // smaller side
		if(speed <= 0.0){
			return false;
		}
		
		t = (min1 - max2) / speed;
		if(t > 1.0){
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
		if(speed >= 0.0){
			return false;
		}
		
		t = (max1 - min2) / speed;
		if(t > 1.0){
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
		if(speed > 0.0){
			t = (max1 - min2) / speed;
			
			if(t < last){
				if(first > t){
					return false;
				}
				last = t;
			}
			
		}else if(speed < 0.0){
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

deoalDCollisionBox::deoalDCollisionBox(){
	pHalfSize.Set(1.0, 1.0, 1.0);
	ClearOrientation();
}

deoalDCollisionBox::deoalDCollisionBox(const decDVector &center, const decDVector &halfSize) :
pCenter(center),
pHalfSize(halfSize)
{
	ClearOrientation();
}

deoalDCollisionBox::deoalDCollisionBox(const decDVector &center, const decDVector &halfSize,
const decQuaternion &orientation) :
pCenter(center),
pHalfSize(halfSize)
{
	SetOrientation(orientation);
}

deoalDCollisionBox::~deoalDCollisionBox(){
}



// double dispatching calls
/////////////////////////////

bool deoalDCollisionBox::VolumeHitsVolume(deoalDCollisionVolume *volume){
	return volume->BoxHitsVolume(this);
}

double deoalDCollisionBox::VolumeMoveHitsVolume(deoalDCollisionVolume *volume, const decDVector &displacement, decDVector *normal){
	return volume->BoxMoveHitsVolume(this, displacement, normal);
}



// first stage calls
//////////////////////

bool deoalDCollisionBox::SphereHitsVolume(deoalDCollisionSphere *sphere){
	return SphereHitsBox(sphere);
}

bool deoalDCollisionBox::CylinderHitsVolume(deoalDCollisionCylinder *cylinder){
	return CylinderHitsBox(cylinder);
}

bool deoalDCollisionBox::CapsuleHitsVolume(deoalDCollisionCapsule *capsule){
	return CapsuleHitsBox(capsule);
}

bool deoalDCollisionBox::BoxHitsVolume(deoalDCollisionBox *box){
	return BoxHitsBox(box);
}

bool deoalDCollisionBox::TriangleHitsVolume(deoalDCollisionTriangle *triangle){
	return triangle->BoxHitsTriangle(this);
}

bool deoalDCollisionBox::FrustumHitsVolume(deoalDCollisionFrustum *frustum){
	return frustum->BoxHitsFrustum(this);
}



double deoalDCollisionBox::SphereMoveHitsVolume(deoalDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return SphereMoveHitsBox(sphere, displacement, normal);
}

double deoalDCollisionBox::CylinderMoveHitsVolume(deoalDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal){
	return CylinderMoveHitsBox(cylinder, displacement, normal);
}

double deoalDCollisionBox::CapsuleMoveHitsVolume(deoalDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	return CapsuleMoveHitsBox(capsule, displacement, normal);
}

double deoalDCollisionBox::BoxMoveHitsVolume(deoalDCollisionBox *box, const decDVector &displacement, decDVector *normal){
	return BoxMoveHitsBox(box, displacement, normal);
}

double deoalDCollisionBox::TriangleMoveHitsVolume(deoalDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal){
	double distance = triangle->BoxMoveHitsTriangle(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double deoalDCollisionBox::FrustumMoveHitsVolume(deoalDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal){
	double distance = frustum->BoxMoveHitsFrustum(this, -displacement, normal);
	if(normal) normal->Negate();
	return distance;
}

double deoalDCollisionBox::PointMoveHitsVolume(const decDVector &point, const decDVector &displacement, decDVector *normal){
	double lambda, hp, factor;
	double px, py, pz;
	double fpx, fpy, fpz;
	double dx, dy, dz;
	// transform values if required
	if(pOriented){
		decDVector transformedPoint = WorldToLocal(point);
		decDVector transformedDisplacement = NormalWorldToLocal(displacement);
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
	fpx = fabs(px);
	fpy = fabs(py);
	fpz = fabs(pz);
	if(fpx <= pHalfSize.x && fpy <= pHalfSize.y && fpz <= pHalfSize.z){
		if(normal){
			if(fpx > fpy){
				if(fpx > fpz){
					if(px > 0.0){
						normal->Set(1.0, 0.0, 0.0);
					}else{
						normal->Set(-1.0, 0.0, 0.0);
					}
				}else{
					if(pz > 0.0){
						normal->Set(0.0, 0.0, 1.0);
					}else{
						normal->Set(0.0, 0.0, -1.0);
					}
				}
			}else{
				if(fpy > fpz){
					if(py > 0.0){
						normal->Set(0.0, 1.0, 0.0);
					}else{
						normal->Set(0.0, -1.0, 0.0);
					}
				}else{
					if(pz > 0.0){
						normal->Set(0.0, 0.0, 1.0);
					}else{
						normal->Set(0.0, 0.0, -1.0);
					}
				}
			}
		}
		return 0.0;
	}
	// test x-axis
	if(dx < -1e-10 || dx > 1e-10){
		factor = 1.0 / dx;
		// bf = pHalfSize.x, normal = ( 1, 0, 0 )
		lambda = (pHalfSize.x - px) * factor;
		if(lambda >= 0.0 && lambda < 1.0){
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
		if(lambda >= 0.0 && lambda < 1.0){
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
	if(dy < -1e-10 || dy > 1e-10){
		factor = 1.0 / dy;
		// bf = pHalfSize.y, normal = ( 1, 0, 0 )
		lambda = (pHalfSize.y - py) * factor;
		if(lambda >= 0.0 && lambda < 1.0){
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
		if(lambda >= 0.0 && lambda < 1.0){
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
	if(dz < -1e-10 || dz > 1e-10){
		factor = 1.0 / dz;
		// bf = pHalfSize.z, normal = ( 1, 0, 0 )
		lambda = (pHalfSize.z - pz) * factor;
		if(lambda >= 0.0 && lambda < 1.0){
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
		if(lambda >= 0.0 && lambda < 1.0){
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
	return 1.0;
/*
// TODO: returning distance travelled, not simply if hit or not
	double lambda, hp, bf, factor;
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

bool deoalDCollisionBox::SphereHitsBox(deoalDCollisionSphere *sphere){
	double temp, dist = 0.0;
	double sx, sy, sz;
	// transform values if required
	if(pOriented){
		decDVector transformedSphere = WorldToLocal(sphere->GetCenter());
		sx = fabs(transformedSphere.x);
		sy = fabs(transformedSphere.y);
		sz = fabs(transformedSphere.z);
	}else{
		const decDVector &sphereCenter = sphere->GetCenter();
		sx = fabs(sphereCenter.x - pCenter.x);
		sy = fabs(sphereCenter.y - pCenter.y);
		sz = fabs(sphereCenter.z - pCenter.z);
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
	decDVector sphereCenter = sphere->GetCenter();
	decDVector boxMin = pCenter - pHalfSize;
	decDVector boxMax = pCenter + pHalfSize;
	double squareRadius = sphere->GetSquareRadius();
	double temp, dist=0;
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

bool deoalDCollisionBox::CylinderHitsBox(deoalDCollisionCylinder *cylinder){
	return false;
}

bool deoalDCollisionBox::CapsuleHitsBox(deoalDCollisionCapsule *capsule){
	/*
	double radius = capsule->GetRadius();
	decDVector bmin = pCenter - pHalfSize;
	decDVector bmax = pCenter + pHalfSize;
	decDVector cdiff, hp, corners[8];
	double lambda, factor;
	decDVector cs, ce;
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
	corners[0] = decDVector(bmin.x, bmax.y, bmin.z);
	corners[1] = decDVector(bmax.x, bmax.y, bmin.z);
	corners[2] = decDVector(bmax.x, bmin.y, bmin.z);
	corners[3] = decDVector(bmin.x, bmin.y, bmin.z);
	corners[4] = decDVector(bmin.x, bmax.y, bmax.z);
	corners[5] = decDVector(bmax.x, bmax.y, bmax.z);
	corners[6] = decDVector(bmax.x, bmin.y, bmax.z);
	corners[7] = decDVector(bmin.x, bmin.y, bmax.z);
	// front 4 edges
	if(deoalDCollisionDetection::SegmentSegmentDistance(cs, ce, corners[0], corners[1]) <= radius) return true;
	if(deoalDCollisionDetection::SegmentSegmentDistance(cs, ce, corners[1], corners[2]) <= radius) return true;
	if(deoalDCollisionDetection::SegmentSegmentDistance(cs, ce, corners[2], corners[3]) <= radius) return true;
	if(deoalDCollisionDetection::SegmentSegmentDistance(cs, ce, corners[3], corners[0]) <= radius) return true;
	// back 4 edges
	if(deoalDCollisionDetection::SegmentSegmentDistance(cs, ce, corners[4], corners[5]) <= radius) return true;
	if(deoalDCollisionDetection::SegmentSegmentDistance(cs, ce, corners[5], corners[6]) <= radius) return true;
	if(deoalDCollisionDetection::SegmentSegmentDistance(cs, ce, corners[6], corners[7]) <= radius) return true;
	if(deoalDCollisionDetection::SegmentSegmentDistance(cs, ce, corners[7], corners[4]) <= radius) return true;
	// connection 4 edges
	if(deoalDCollisionDetection::SegmentSegmentDistance(cs, ce, corners[0], corners[4]) <= radius) return true;
	if(deoalDCollisionDetection::SegmentSegmentDistance(cs, ce, corners[1], corners[5]) <= radius) return true;
	if(deoalDCollisionDetection::SegmentSegmentDistance(cs, ce, corners[2], corners[6]) <= radius) return true;
	if(deoalDCollisionDetection::SegmentSegmentDistance(cs, ce, corners[3], corners[7]) <= radius) return true;
	// it has to miss
	*/
	return false;
}

bool deoalDCollisionBox::BoxHitsBox(deoalDCollisionBox *box){
	// if one of the boxes is not axis aligned we have to use the heavier algorithm
	if(pOriented || box->GetOriented()){
		const decDVector centerDiff = box->GetCenter() - pCenter;
		const decDVector &bax = box->GetAxisX();
		const decDVector &bay = box->GetAxisY();
		const decDVector &baz = box->GetAxisZ();
		const decDVector &bhs = box->GetHalfSize();
		decDVector axis;
		// axes of box 1
		if(fabs(pAxisX * centerDiff) - box->ProjectExtends(pAxisX) > pHalfSize.x) return false;
		if(fabs(pAxisY * centerDiff) - box->ProjectExtends(pAxisY) > pHalfSize.y) return false;
		if(fabs(pAxisZ * centerDiff) - box->ProjectExtends(pAxisZ) > pHalfSize.z) return false;
		// axes of box 2
		if(fabs(bax * centerDiff) - bhs.x > ProjectExtends(bax)) return false;
		if(fabs(bay * centerDiff) - bhs.y > ProjectExtends(bay)) return false;
		if(fabs(baz * centerDiff) - bhs.z > ProjectExtends(baz)) return false;
		// axis x of box 1 cross axes of box 2
		axis = pAxisX % bax;
		if(fabs(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		axis = pAxisX % bay;
		if(fabs(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		axis = pAxisX % baz;
		if(fabs(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		// axis y of box 1 cross axes of box 2
		axis = pAxisY % bax;
		if(fabs(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		axis = pAxisY % bay;
		if(fabs(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		axis = pAxisY % baz;
		if(fabs(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		// axis z of box 1 cross axes of box 2
		axis = pAxisZ % bax;
		if(fabs(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		axis = pAxisZ % bay;
		if(fabs(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		axis = pAxisZ % baz;
		if(fabs(axis * centerDiff) - box->ProjectExtends(axis) > ProjectExtends(axis)) return false;
		// we do intersect
		return true;
	
	// otherwise a simple test does the trick
	}else{
		const decDVector &hs = box->GetHalfSize();
		decDVector dist = box->GetCenter() - pCenter;
		return fabs(dist.x) <= (pHalfSize.x + hs.x) &&
			fabs(dist.y) <= (pHalfSize.y + hs.y) &&
			fabs(dist.z) <= (pHalfSize.z + hs.z);
	}
}



double deoalDCollisionBox::SphereMoveHitsBox(deoalDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	const double r = sphere->GetRadius();
	const double rsquared = r * r;
	decDVector so, d;
	
	// transform if required
	if(pOriented){
		so = WorldToLocal(sphere->GetCenter());
		d = NormalWorldToLocal(displacement);
		
	}else{
		so = sphere->GetCenter() - pCenter;
		d = displacement;
	}
	
	// test if the sphere is inside the box
	if((so - pAxisAlignedCPTo(so)).LengthSquared() <= rsquared){
		// the center of the sphere is inside the box. this is a stupid situation
		// as determining a plane normal for this case is difficult to do. what we
		// do here to solve this situation is to determine the coordinate axis in
		// which direction the sphere penetrates the box the least. this normal is
		// more coherent with the normal of a sphere hitting the box at this location
		if(normal){
			const double dx = pHalfSize.x - fabs(so.x) + r; // halfSize - (sphereCenter - sphereRadius)
			const double dy = pHalfSize.y - fabs(so.y) + r;
			const double dz = pHalfSize.z - fabs(so.z) + r;
			
			if(dx < dy){
				if(dx < dz){ // x smallest penetration
					*normal = so.x > 0.0 ? pAxisX : -pAxisX;
					
				}else{ // z smallest penetration
					*normal = so.z > 0.0 ? pAxisZ : -pAxisZ;
				}
				
			}else{
				if(dy < dz){ // y smallest penetration
					*normal = so.y > 0.0 ? pAxisY : -pAxisY;
					
				}else{ // z smallest penentration
					*normal = so.z > 0.0 ? pAxisZ : -pAxisZ;
				}
			}
		}
		
		return 0.0;
	}
	
	// flip all axes to positive so the number of tests can be reduced
	decDVector st(so);
	
	if(st.x < 0.0){
		st.x = -st.x;
		d.x = -d.x;
	}
	if(st.y < 0.0){
		st.y = -st.y;
		d.y = -d.y;
	}
	if(st.z < 0.0){
		st.z = -st.z;
		d.z = -d.z;
	}
	
	// in the case of faces an early exit can be done if a result is found. this is though only
	// valid if the sphere center at the collision point is located inside the volume formed by
	// the tested box face extruded along the face normal. if the sphere i outside this volume
	// during the contact another face, edge or corner yields a closer hit distance
	
	// test x axis
	if(st.x > pHalfSize.x + r){
		if(d.x >= -1e-9){
			return NO_COLLISION;
		}
		const double lambda = (pHalfSize.x - st.x + r) / d.x;
		if(lambda > 1.0){
			return NO_COLLISION;
		}
		double hpsc = st.y + d.y * lambda; // hit point sphere center
		if(hpsc >= -pHalfSize.y && hpsc <= pHalfSize.y){
			hpsc = st.z + d.z * lambda;
			if(hpsc >= -pHalfSize.z && hpsc <= pHalfSize.z){
				if(normal){
					*normal = so.x > 0.0 ? pAxisX : -pAxisX;
				}
				return lambda;
			}
		}
	}
	
	// test y axis
	if(st.y > pHalfSize.y + r){
		if(d.y >= -1e-9){
			return NO_COLLISION;
		}
		const double lambda = (pHalfSize.y - st.y + r) / d.y;
		if(lambda > 1.0){
			return NO_COLLISION;
		}
		double hpsc = st.x + d.x * lambda; // hit point sphere center
		if(hpsc >= -pHalfSize.x && hpsc <= pHalfSize.x){
			hpsc = st.z + d.z * lambda;
			if(hpsc >= -pHalfSize.z && hpsc <= pHalfSize.z){
				if(normal){
					*normal = so.y > 0.0 ? pAxisY : -pAxisY;
				}
				return lambda;
			}
		}
	}
	
	// test z axis
	if(st.z > pHalfSize.z + r){
		if(d.z >= -1e-9){
			return NO_COLLISION;
		}
		const double lambda = (pHalfSize.z - st.z + r) / d.z;
		if(lambda > 1.0){
			return NO_COLLISION;
		}
		double hpsc = st.x + d.x * lambda; // hit point sphere center
		if(hpsc >= -pHalfSize.x && hpsc <= pHalfSize.x){
			hpsc = st.y + d.y * lambda;
			if(hpsc >= -pHalfSize.y && hpsc <= pHalfSize.y){
				if(normal){
					*normal = so.z > 0.0 ? pAxisZ : -pAxisZ;
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
	// 
	// todo: check evaluation order. is it possible a suboptimal edge/corner is returned? is it
	// required to check all solutions and pick the best one or is this version here enough?
	decDVector n;
	
	// for edges the early exit is correct if the sphere is located in the volume starting at the
	// edge expanding along the two face axes connected to the edge but not above and beyond. this
	// covers the area outside the face volume belonging to the two faces connected to the edge
	// without the corner area. the early exit due to lambda >= 1 is not correct for edges.
	
	// test x edge
	double denom = d.y * d.y + d.z * d.z;
	if(denom != 0.0){
		denom = 1.0 / denom;
		const double c1a = pHalfSize.y - st.y;
		const double c1b = pHalfSize.z - st.z;
		const double p = -2.0 * (c1a * d.y + c1b * d.z) * denom;
		const double q = (c1a * c1a + c1b * c1b - rsquared) * denom;
		denom = p * p * 0.25 - q;
		if(denom >= 0.0){
			double lambda = -0.5 * p;
			if(denom > 0.0){
				denom = sqrt(denom);
				lambda -= denom;
				if(lambda < 0.0){
					lambda += denom * 2.0;
				}
			}
			if(lambda >= 0.0 && lambda < 1.0){
				double hpsc = st.x + d.x * lambda; // hit point sphere center
				if(hpsc >= -pHalfSize.x && hpsc <= pHalfSize.x){
					hpsc = st.y + d.y * lambda;
					if(hpsc >= pHalfSize.y){
						hpsc = st.z + d.z * lambda;
						if(hpsc >= pHalfSize.z){
							const decDVector sn(so + d * lambda);
							n = sn - pAxisAlignedCPTo(sn);
							const double len = n.Length();
							if(len > DOUBLE_EPSILON){
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
	if(denom != 0.0){
		denom = 1.0 / denom;
		const double c1a = pHalfSize.x - st.x;
		const double c1b = pHalfSize.z - st.z;
		const double p = -2.0 * (c1a * d.x + c1b * d.z) * denom;
		const double q = (c1a * c1a + c1b * c1b - rsquared) * denom;
		denom = p * p * 0.25 - q;
		if(denom >= 0.0){
			double lambda = -0.5 * p;
			if(denom > 0.0){
				denom = sqrt(denom);
				lambda -= denom;
				if(lambda < 0.0){
					lambda += denom * 2.0;
				}
			}
			if(lambda >= 0.0 && lambda < 1.0){
				double hpsc = st.y + d.y * lambda; // hit point sphere center
				if(hpsc >= -pHalfSize.y && hpsc <= pHalfSize.y){
					hpsc = st.x + d.x * lambda;
					if(hpsc >= pHalfSize.x){
						hpsc = st.z + d.z * lambda;
						if(hpsc >= pHalfSize.z){
							const decDVector sn(so + d * lambda);
							n = sn - pAxisAlignedCPTo(sn);
							const double len = n.Length();
							if(len > DOUBLE_EPSILON){
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
	if(denom != 0.0){
		denom = 1.0 / denom;
		const double c1a = pHalfSize.x - st.x;
		const double c1b = pHalfSize.y - st.y;
		const double p = -2.0 * (c1a * d.x + c1b * d.y) * denom;
		const double q = (c1a * c1a + c1b * c1b - rsquared) * denom;
		denom = p * p * 0.25 - q;
		if(denom >= 0.0){
			double lambda = -0.5 * p;
			if(denom > 0.0){
				denom = sqrt(denom);
				lambda -= denom;
				if(lambda < 0.0){
					lambda += denom * 2.0;
				}
			}
			if(lambda >= 0.0 && lambda < 1.0){
				double hpsc = st.z + d.z * lambda; // hit point sphere center
				if(hpsc >= -pHalfSize.z && hpsc <= pHalfSize.z){
					hpsc = st.x + d.x * lambda;
					if(hpsc >= pHalfSize.x){
						hpsc = st.y + d.y * lambda;
						if(hpsc >= pHalfSize.y){
							const decDVector sn(so + d * lambda);
							n = sn - pAxisAlignedCPTo(sn);
							const double len = n.Length();
							if(len > DOUBLE_EPSILON){
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
	const decDVector a(st - pHalfSize);
	denom = d * d;
	if(denom != 0.0){
		denom = 1.0 / denom;
		const double p = (a * d) * 2.0 * denom;
		const double q = (a * a - rsquared) * denom;
		denom = p * p * 0.25 - q;
		if(denom >= 0.0){
			double lambda = -0.5 * p;
			if(denom > 0.0){
				denom = sqrt(denom);
				lambda -= denom;
				if(lambda < 0.0){
					lambda += denom * 2.0;
				}
			}
			
			if(lambda >= 0.0f && lambda < 1.0f){
				const decDVector sn(so + d * lambda);
				n = sn - pAxisAlignedCPTo(sn);
				const double len = n.Length();
				if(len > DOUBLE_EPSILON){
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

double deoalDCollisionBox::CylinderMoveHitsBox(deoalDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoalDCollisionBox::CapsuleMoveHitsBox(deoalDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	return 1;
}

double deoalDCollisionBox::BoxMoveHitsBox(deoalDCollisionBox *box, const decDVector &displacement, decDVector *normal){
	double curDist, dispDist;
	double bestDist = 0.0;
	decDVector bestNormal;
	double normalValue;
	
	// if one of the boxes is not axis aligned we have to use the heavier algorithm
	if(pOriented || box->GetOriented()){
		const decDVector centerDiff(box->GetCenter() - pCenter);
		const decDVector &bax = box->GetAxisX();
		const decDVector &bay = box->GetAxisY();
		const decDVector &baz = box->GetAxisZ();
		const decDVector &bhs = box->GetHalfSize();
		decDVector axis;
		
		// axes of box 1
		curDist = centerDiff * pAxisX;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= box->ProjectExtends(pAxisX) + pHalfSize.x;
		if(curDist > 0.0){
			dispDist = -((displacement * pAxisX) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(pAxisX * normalValue);
			}
		}
		
		curDist = centerDiff * pAxisY;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= box->ProjectExtends(pAxisY) + pHalfSize.y;
		if(curDist > 0.0){
			dispDist = -((displacement * pAxisY) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(pAxisY * normalValue);
			}
		}
		
		curDist = centerDiff * pAxisZ;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= box->ProjectExtends(pAxisZ) + pHalfSize.z;
		if(curDist > 0.0){
			dispDist = -((displacement * pAxisZ) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(pAxisZ * normalValue);
			}
		}
		
		// axes of box 2
		curDist = bax * centerDiff;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= bhs.x + ProjectExtends(bax);
		if(curDist > 0.0){
			dispDist = -((displacement * bax) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(bax * normalValue);
			}
		}
		
		curDist = bay * centerDiff;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= bhs.y + ProjectExtends(bay);
		if(curDist > 0.0){
			dispDist = -((displacement * bay) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(bay * normalValue);
			}
		}
		
		curDist = baz * centerDiff;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= bhs.z + ProjectExtends(baz);
		if(curDist > 0.0){
			dispDist = -((displacement * baz) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(baz * normalValue);
			}
		}
		
		// axis x of box 1 cross axes of box 2
		axis = pAxisX % bax;
		curDist = axis * centerDiff;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		axis = pAxisX % bay;
		curDist = axis * centerDiff;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		axis = pAxisX % baz;
		curDist = axis * centerDiff;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		// axis y of box 1 cross axes of box 2
		axis = pAxisY % bax;
		curDist = axis * centerDiff;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		axis = pAxisY % bay;
		curDist = axis * centerDiff;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		axis = pAxisY % baz;
		curDist = axis * centerDiff;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		// axis z of box 1 cross axes of box 2
		axis = pAxisZ % bax;
		curDist = axis * centerDiff;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		axis = pAxisZ % bay;
		curDist = axis * centerDiff;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
		
		axis = pAxisZ % baz;
		curDist = axis * centerDiff;
		if(curDist > 0.0){
			normalValue = 1.0;
		}else{
			curDist = -curDist;
			normalValue = -1.0;
		}
		curDist -= box->ProjectExtends(axis) + ProjectExtends(axis);
		if(curDist > 0.0){
			dispDist = -((displacement * axis) * normalValue);
			if(dispDist <= curDist) return 1.0;
			curDist /= dispDist;
			if(curDist > bestDist){
				bestDist = curDist;
				if(normal) bestNormal.Set(axis * normalValue);
			}
		}
	
	// otherwise a simple test does the trick
	}else{
		// flip problem space into positive x,y,z
		decDVector cdiff = box->GetCenter() - pCenter;
		decDVector disp = displacement;
		bool flip[3] = {false, false, false};
		
		if(cdiff.x < 0.0){
			cdiff.x = -cdiff.x;
			disp.x = -disp.x;
			flip[0] = true;
		}
		if(cdiff.y < 0.0){
			cdiff.y = -cdiff.y;
			disp.y = -disp.y;
			flip[1] = true;
		}
		if(cdiff.z < 0.0){
			cdiff.z = -cdiff.z;
			disp.z = -disp.z;
			flip[2] = true;
		}
		
		// calculate parameters
		const double cextx = box->GetHalfSize().x + pHalfSize.x;
		const double cexty = box->GetHalfSize().y + pHalfSize.y;
		const double cextz = box->GetHalfSize().z + pHalfSize.z;
		const double gapx = cdiff.x - cextx;
		const double gapy = cdiff.y - cexty;
		const double gapz = cdiff.z - cextz;
		
		// if in contact check if we are moving away. in this case consider no collision.
		// this is less error prone allowing for objects to free themselves better
		if(gapx <= 0.0 && gapy <= 0.0 && gapz <= 0.0){
			// if in contact check if we are moving away. in this case consider no collision.
			// this is less error prone allowing for objects to free themselves better
			
			// if in contact at 0 distance use the normal separating the objects the fastest
			if(normal){
				// gap is negative while penetrating so checking for the smallest
				// penetration means checking for the largest gap
				if(gapx > gapy && gapx > gapz){
					normal->Set(flip[0] ? -1.0 : 1.0, 0.0, 0.0);
					
				}else if(gapy > gapz){
					normal->Set(0.0, flip[1] ? -1.0 : 1.0, 0.0);
					
				}else{
					normal->Set(0.0, 0.0, flip[2] ? -1.0 : 1.0);
				}
			}
			return 0.0;
		}
		
		// quick exit test. if either axis is a separation axis no collision can happen
		if(gapx > 0.0 && gapx + disp.x >= 0.0){
			return 1.0;
		}
		if(gapy > 0.0 && gapy + disp.y >= 0.0){
			return 1.0;
		}
		if(gapz > 0.0 && gapz + disp.z >= 0.0){
			return 1.0;
		}
		
		// check if x axis face of box is hit
		if(gapx > 0.0){
			curDist = gapx / -disp.x;
			if(fabs(cdiff.y + disp.y * curDist) <= cexty && fabs(cdiff.z + disp.z * curDist) <= cextz){
				if(normal){
					normal->Set(flip[0] ? -1.0 : 1.0, 0.0, 0.0);
				}
				return curDist;
			}
		}
		
		// check if y axis face of box is hit
		if(gapy > 0.0){
			curDist = gapy / -disp.y;
			if(fabs(cdiff.x + disp.x * curDist) <= cextx && fabs(cdiff.z + disp.z * curDist) <= cextz){
				if(normal){
					normal->Set(0.0, flip[0] ? -1.0 : 1.0, 0.0);
				}
				return curDist;
			}
		}
		
		// check if z axis face of box is hit
		if(gapz > 0.0){
			curDist = gapz / -disp.z;
			if(fabs(cdiff.x + disp.x * curDist) <= cextx && fabs(cdiff.y + disp.y * curDist) <= cexty){
				if(normal){
					normal->Set(0.0, 0.0, flip[0] ? -1.0 : 1.0);
				}
				return curDist;
			}
		}
		
		// no collision found
		return 1.0;
	}
	
	// otherwise return the found result
	if(normal) normal->Set(bestNormal);
	return bestDist;
}



// Enclosing Volumes
//////////////////////

void deoalDCollisionBox::GetEnclosingSphere(deoalDCollisionSphere *sphere){
	if(!sphere) DETHROW(deeInvalidParam);
	sphere->SetAll(pCenter, pHalfSize.Length());
}

void deoalDCollisionBox::GetEnclosingBox(deoalDCollisionBox *box){
	if(!box){
		DETHROW(deeInvalidParam);
	}
	
	box->SetCenter(pCenter);
	
	if(pOriented){
		/*
		box->SetHalfSize(decDVector(ProjectExtends(decDVector(1.0, 0.0, 0.0)),
			ProjectExtends(decDVector(0.0, 1.0, 0.0)),
			ProjectExtends(decDVector(0.0, 0.0, 1.0))));
		*/
		box->SetHalfSize(decDVector(			fabs(pAxisX.x) * pHalfSize.x + fabs(pAxisY.x) * pHalfSize.y + fabs(pAxisZ.x) * pHalfSize.z,
			fabs(pAxisX.y) * pHalfSize.x + fabs(pAxisY.y) * pHalfSize.y + fabs(pAxisZ.y) * pHalfSize.z,
			fabs(pAxisX.z) * pHalfSize.x + fabs(pAxisY.z) * pHalfSize.y + fabs(pAxisZ.z) * pHalfSize.z
));
		
	}else{
		box->SetHalfSize(pHalfSize);
	}
	
	box->ClearOrientation();
}



// Miscelanous Functions
//////////////////////////

decDVector deoalDCollisionBox::ClosestPointTo(const decDVector &point){
	if(pOriented){
		return LocalToWorld(pAxisAlignedCPTo(WorldToLocal(point)));
	}else{
		return pCenter + pAxisAlignedCPTo(point - pCenter);
	}
}

bool deoalDCollisionBox::IsPointInside(const decDVector &point){
	const decDVector localPoint = WorldToLocal(point);
	
	return localPoint >= -pHalfSize && localPoint <= pHalfSize;
}

decDVector deoalDCollisionBox::NormalAtPoint(const decDVector &point){
	decDVector localPoint = WorldToLocal(point);
	bool sideXPos = (localPoint.x - pHalfSize.x >= -1e-6);
	bool sideXNeg = (localPoint.x + pHalfSize.x <= 1e-6);
	bool sideYPos = (localPoint.y - pHalfSize.y >= -1e-6);
	bool sideYNeg = (localPoint.y + pHalfSize.y <= 1e-6);
	bool sideZPos = (localPoint.z - pHalfSize.z >= -1e-6);
	bool sideZNeg = (localPoint.z + pHalfSize.z <= 1e-6);
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
		double fx = fabs(localPoint.x);
		double fy = fabs(localPoint.y);
		double fz = fabs(localPoint.z);
		
		if(fx > fy && fx > fz){
			if(localPoint.x > 0.0){
				return pAxisX;
				
			}else{
				return -pAxisX;
			}
			
		}else if(fy > fx && fy > fz){
			if(localPoint.y > 0.0){
				return pAxisY;
				
			}else{
				return -pAxisY;
			}
			
		}else{
			if(localPoint.z > 0.0){
				return pAxisZ;
				
			}else{
				return -pAxisZ;
			}
		}
	}
	
	// normal along an edge or corner
	decDVector normal = localPoint - pAxisAlignedCPTo(localPoint);
	double length = normal.Length();
	
	if(length < 1e-6){
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

bool deoalDCollisionBox::RayHitsVolume(const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance){
	if(pOriented){
		return deoalDCollisionDetection::RayHitsBox(WorldToLocal(rayOrigin), NormalWorldToLocal(rayDirection),
			decDVector(), pHalfSize, hitDistance);
		
	}else{
		return deoalDCollisionDetection::RayHitsBox(rayOrigin, rayDirection, pCenter, pHalfSize, hitDistance);
	}
}



// Visiting
/////////////

void deoalDCollisionBox::Visit(deoalDCollisionVolumeVisitor *visitor){
	if(!visitor) DETHROW(deeInvalidParam);
	visitor->VisitBox(this);
}



// management
///////////////

void deoalDCollisionBox::SetCenter(const decDVector &center){
	pCenter = center;
}

void deoalDCollisionBox::SetHalfSize(const decDVector &halfSize){
	pHalfSize = decDVector().Largest(halfSize);
}

void deoalDCollisionBox::SetOrientation(const decQuaternion &orientation){
	const decDMatrix matrix = decDMatrix::CreateFromQuaternion(orientation);
	pOrientation = orientation;
	pAxisX.Set(matrix.TransformRight());
	pAxisY.Set(matrix.TransformUp());
	pAxisZ.Set(matrix.TransformView());
	pOriented = true;
}

void deoalDCollisionBox::ClearOrientation(){
	pOrientation.SetZero();
	pAxisX.Set(1.0, 0.0, 0.0);
	pAxisY.Set(0.0, 1.0, 0.0);
	pAxisZ.Set(0.0, 0.0, 1.0);
	pOriented = false;
}

void deoalDCollisionBox::SetFromExtends(const decDVector &minExtend, const decDVector &maxExtend){
	pHalfSize = decDVector().Largest((maxExtend - minExtend) * 0.5);
	pCenter = minExtend + pHalfSize;
	ClearOrientation();
}

void deoalDCollisionBox::MoveBy(const decDVector &offset){
	pCenter += offset;
}



// private functions
//////////////////////

decDVector deoalDCollisionBox::WorldToLocal(const decDVector &point) const{
	decDVector tp = point - pCenter;
	return decDVector(pAxisX * tp, pAxisY * tp, pAxisZ * tp);
}

decDVector deoalDCollisionBox::LocalToWorld(const decDVector &point) const{
	return pCenter + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decDVector deoalDCollisionBox::NormalWorldToLocal(const decDVector &normal) const{
	return decDVector(pAxisX * normal, pAxisY * normal, pAxisZ * normal);
}

decDVector deoalDCollisionBox::NormalLocalToWorld(const decDVector &normal) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}

decDVector deoalDCollisionBox::pAxisAlignedCPTo(const decDVector &point){
	return point.Clamped(-pHalfSize, pHalfSize);
}
