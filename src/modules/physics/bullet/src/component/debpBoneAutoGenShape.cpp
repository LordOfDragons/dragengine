/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include <limits>

#include "debpBoneAutoGenShape.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/shape/decShapeSphere.h>


// Class debpBoneAutoGenShape
///////////////////////////////

const float vFltMax = std::numeric_limits<float>::max();

float debpBoneAutoGenShape::pMinRadius = 0.005f;
float debpBoneAutoGenShape::pMinHalfExtents = 0.005f;
float debpBoneAutoGenShape::pPreferBoxVolRatio = 0.75f; //0.5f
float debpBoneAutoGenShape::pUseTaperedRatio = 0.05f;


// Constructor, destructor
////////////////////////////

debpBoneAutoGenShape::debpBoneAutoGenShape(const ShapeParams &params) :
pParams(params)
{
	DEASSERT_FALSE(params.type == ShapeType::none)
	pCreateShapes();
}


// Management
///////////////

debpBoneAutoGenShape::Ref debpBoneAutoGenShape::Create(const WeightList &weights){
	if(weights.GetCount() < 4){
		return {};
	}
	
	// compute weighted covariance matrix
	/*
	decMatrix covariance;
	covariance.a11 = covariance.a22 = covariance.a33 = covariance.a44 = 0.0f;
	
	weights.Visit([&](const Weight &vertex){
		const decVector diff(vertex.position - center);
		const float w = vertex.weight / totalWeight;
		
		covariance.a11 += diff.x * diff.x * w;
		covariance.a12 += diff.x * diff.y * w;
		covariance.a13 += diff.x * diff.z * w;
		covariance.a21 += diff.y * diff.x * w;
		covariance.a22 += diff.y * diff.y * w;
		covariance.a23 += diff.y * diff.z * w;
		covariance.a31 += diff.z * diff.x * w;
		covariance.a32 += diff.z * diff.y * w;
		covariance.a33 += diff.z * diff.z * w;
	});
	*/
	
	// fit all shape types in bone space
	const auto capsule = pFitTaperedCapsule(weights);
	const auto box = pFitBox(weights);
	const auto convexHull = pFitConvexHull(weights);
	
	// select best shape
	const auto bestType = pSelectBestShape(weights, capsule, box, convexHull);
	
	switch(bestType){
	case ShapeType::sphere:
	case ShapeType::capsule:
	case ShapeType::taperedCapsule:
		return Ref::New(capsule);
		
	case ShapeType::box:
	case ShapeType::taperedBox:
		return Ref::New(box);
		
	case ShapeType::convexHull:
		return Ref::New(convexHull);
		
	case ShapeType::none:
	default:
		return {};
	}
}


// Private Functions
//////////////////////

void debpBoneAutoGenShape::pCreateShapes(){
	switch(pParams.type){
	case ShapeType::sphere:
		pShapes.Add(decShapeSphere::Ref::New(pParams.radius, pParams.center));
		break;
		
	case ShapeType::box:
		pShapes.Add(decShapeBox::Ref::New(pParams.halfExtents, pParams.center, pParams.orientation));
		break;
		
	case ShapeType::capsule:
		pShapes.Add(decShapeCapsule::Ref::New(pParams.halfHeight, pParams.radius,
			pParams.center, pParams.orientation));
		break;
		
	case ShapeType::taperedCapsule:
		pShapes.Add(decShapeCapsule::Ref::New(pParams.halfHeight, pParams.radiusTop,
			pParams.radiusBottom, pParams.center, pParams.orientation));
		break;
		
	case ShapeType::taperedBox:
		pShapes.Add(decShapeHull::Ref::New(pParams.center, pParams.orientation, pParams.hullVertices));
		break;
		
	case ShapeType::convexHull:
		pShapes.Add(decShapeHull::Ref::New(pParams.center, pParams.orientation, pParams.hullVertices));
		break;
		
	case ShapeType::none:
	default:
		break;
	};
}

debpBoneAutoGenShape::ShapeParams debpBoneAutoGenShape::pFitSphere(const WeightList &weights){
	ShapeParams params;
	params.type = ShapeType::sphere;
	
	float totalWeight = 0.0f;
	weights.Visit([&](const Weight &weight){
		params.center += weight.position * weight.weight;
		totalWeight += weight.weight;
	});
	if(totalWeight < FLOAT_SAFE_EPSILON){
		return {};
	}
	params.center /= totalWeight;
	
	float maxDistSquared = 0.0f;
	weights.Visit([&](const Weight &weight){
		const float distSquared = (weight.position - params.center).LengthSquared();
		maxDistSquared = decMath::max(maxDistSquared, distSquared);
	});
	
	params.radius = sqrtf(maxDistSquared);
	
	// ensure minimum radius. bullet requires at least 1cm to not cause problems
	params.radius = decMath::max(params.radius, pMinRadius);
	
	return params;
}

debpBoneAutoGenShape::ShapeParams debpBoneAutoGenShape::pFitCapsule(const WeightList &weights){
	ShapeParams params;
	params.type = ShapeType::capsule;
	
	// simple approach is finding the longest axis from bounding box
	decVector minExt(vFltMax, vFltMax, vFltMax);
	decVector maxExt(-vFltMax, -vFltMax, -vFltMax);
	weights.Visit([&](const Weight &weight){
		minExt.SetSmallest(weight.position);
		maxExt.SetLargest(weight.position);
	});
	
	params.center = (minExt + maxExt) / 2.0f;
	const decVector extents(maxExt - minExt);
	
	int longAxis = 1;
	if(extents.x > extents.y){
		longAxis = 0;
	}
	if(extents.z > (longAxis == 1 ? extents.y : extents.x)){
		longAxis = 2;
	}
	
	// create orientation aligned with longest axis
	decVector axis;
	if(longAxis == 0){
		axis.x = 1.0f;
		
	}else if(longAxis == 1){
		axis.y = 1.0f;
		
	}else{
		axis.z = 1.0f;
	}
	
	// create quaternion that aligns Y axis with the longest axis
	if(longAxis == 0){
		params.orientation.SetFromEuler(0.0f, 0.0f, -HALF_PI);
		
	}else if(longAxis == 2){
		params.orientation.SetFromEuler(HALF_PI, 0.0f, 0.0f);
	}
	params.conjOrientation = params.orientation.Conjugate();
	
	// project vertices onto capsule axis
	const decVector capsuleAxis(params.orientation * decVector(0.0f, 1.0f, 0.0f));
	float maxRadiusSquared = 0.0f;
	float maxProjExt = -vFltMax;
	float minProjExt = vFltMax;
	
	weights.Visit([&](const Weight &weight){
		const decVector diff(weight.position - params.center);
		const float proj = diff * capsuleAxis;
		minProjExt = decMath::min(minProjExt, proj);
		maxProjExt = decMath::max(maxProjExt, proj);
		
		maxRadiusSquared = decMath::max(maxRadiusSquared, (diff - capsuleAxis * proj).LengthSquared());
	});
	
	// ensure minimum radius. bullet requires at least 1cm to not cause problems
	params.radius = decMath::max(sqrtf(maxRadiusSquared), pMinRadius);
	
	// half height does not include radius. if height is too small use a sphere instead
	params.halfHeight = (maxProjExt - minProjExt) / 2.0f - params.radius;
	
	// adjust center to capsule center
	params.center += capsuleAxis * ((maxProjExt + minProjExt) * 0.5f);
	
	// if height is too small use a sphere instead
	const float minHeight = 0.01f;
	if(params.halfHeight < minHeight){
		params.type = ShapeType::sphere;
		params.conjOrientation = params.orientation = {};
	}
	
	return params;
}

debpBoneAutoGenShape::ShapeParams debpBoneAutoGenShape::pFitTaperedCapsule(const WeightList &weights){
	ShapeParams params = pFitCapsule(weights);
	if(params.type == ShapeType::none){
		return {};
	}
	
	const decVector capsuleAxis(params.orientation * decVector(0.0f, 1.0f, 0.0f));
	
	// find min/max projection along capsule axis
	float minExt = vFltMax;
	float maxExt = -vFltMax;
	weights.Visit([&](const Weight &weight){
		const decVector diff(weight.position - params.center);
		const float proj = diff * capsuleAxis;
		minExt = decMath::min(minExt, proj);
		maxExt = decMath::max(maxExt, proj);
	});
	
	const float range = maxExt - minExt;
	if(range < 1e-4f){
		return params; // not tapered. use capsule/sphere
	}
	
	// compute radius at bottom 25% and top 25%
	const float bottomThreshold = minExt + range * 0.25f;
	const float topThreshold = maxExt - range * 0.25f;
	
	float maxRadiusBottomSq = 0.0f;
	float maxRadiusTopSq = 0.0f;
	int countBottom = 0;
	int countTop = 0;
	
	weights.Visit([&](const Weight &weight){
		const decVector diff(weight.position - params.center);
		const float proj = diff * capsuleAxis;
		
		const float radiusSquared = (diff - capsuleAxis * proj).LengthSquared();
		
		if(proj <= bottomThreshold){
			maxRadiusBottomSq = decMath::max(maxRadiusBottomSq, radiusSquared);
			countBottom++;
		}
		if(proj >= topThreshold){
			maxRadiusTopSq = decMath::max(maxRadiusTopSq, radiusSquared);
			countTop++;
		}
	});
	
	if(countBottom == 0 || countTop == 0){
		return params; // not enough points. use capsule/sphere
	}
	
	params.radiusBottom = sqrtf(maxRadiusBottomSq);
	params.radiusTop = sqrtf(maxRadiusTopSq);
	
	// ensure minimum radius. bullet requires at least 1cm to not cause problems
	params.halfHeight = (range - params.radiusTop - params.radiusBottom) / 2.0f;
	
	if(params.halfHeight < pMinHalfExtents){
		params.halfHeight = pMinHalfExtents;
	}
	
	// adjust center to capsule center
	params.center += capsuleAxis * ((minExt + params.radiusBottom + maxExt - params.radiusTop) / 2.0f);
	
	// only use tapered if radii difference is large enough
	if(fabsf(params.radiusTop - params.radiusBottom) < params.radius * pUseTaperedRatio){
		return params; // use capsule/sphere
	}
	
	params.type = ShapeType::taperedCapsule;
	return params;
}

debpBoneAutoGenShape::ShapeParams debpBoneAutoGenShape::pFitBox(const WeightList &weights){
	ShapeParams params;
	params.type = ShapeType::box;
	
	decVector minExt(vFltMax, vFltMax, vFltMax);
	decVector maxExt(-vFltMax, -vFltMax, -vFltMax);
	weights.Visit([&](const Weight &weight){
		minExt.SetSmallest(weight.position);
		maxExt.SetLargest(weight.position);
	});
	
	params.center = (minExt + maxExt) / 2.0f;
	params.halfExtents = (maxExt - minExt) / 2.0f;
	
	// ensure minimum extends. bullet requires at least 1cm to not cause problems
	params.halfExtents.SetLargest({pMinHalfExtents, pMinHalfExtents, pMinHalfExtents});
	
	
	// tapered box
	////////////////
	
	// find longest axis
	int longAxis = 1;
	if(params.halfExtents.x > params.halfExtents.y){
		longAxis = 0;
	}
	if(params.halfExtents.z > (longAxis == 1 ? params.halfExtents.y : params.halfExtents.x)){
		longAxis = 2;
	}
	
	decVector axis;
	if(longAxis == 0){
		axis.x = 1.0f;
		
	}else if(longAxis == 1){
		axis.y = 1.0f;
		
	}else{
		axis.z = 1.0f;
	}
	
	// create orientation aligned with longest axis
	if(longAxis == 0){
		params.orientation.SetFromEuler(0.0f, 0.0f, -HALF_PI);
		
	}else if(longAxis == 2){
		params.orientation.SetFromEuler(HALF_PI, 0.0f, 0.0f);
	}
	params.conjOrientation = params.orientation.Conjugate();
	
	// find min/max Y in oriented space
	float minY = vFltMax;
	float maxY = -vFltMax;
	weights.Visit([&](const Weight &weight){
		const decVector diff(params.conjOrientation * (weight.position - params.center));
		minY = decMath::min(minY, diff.y);
		maxY = decMath::max(maxY, diff.y);
	});
	
	const float extentsY = decMath::max(maxY - minY, pMinHalfExtents * 2.0f);
	
	// compute extents at ends in oriented space so y is up
	decVector minExtBottom(vFltMax, vFltMax, vFltMax);
	decVector maxExtBottom(-vFltMax, -vFltMax, -vFltMax);
	decVector minExtTop(vFltMax, vFltMax, vFltMax);
	decVector maxExtTop(-vFltMax, -vFltMax, -vFltMax);
	
	weights.Visit([&](const Weight &weight){
		const decVector diff(params.conjOrientation * (weight.position - params.center));
		const float t = (diff.y - minY) / extentsY;
		
		if(t < 0.33f){
			minExtBottom.SetSmallest(diff);
			maxExtBottom.SetLargest(diff);
			
		}else if(t > 0.66f){
			minExtTop.SetSmallest(diff);
			maxExtTop.SetLargest(diff);
		}
	});
	
	if(minExtBottom.y == vFltMax || minExtTop.y == vFltMax){
		return params; // not enough points. use box
	}
	
	params.halfExtents = (maxExtBottom - minExtBottom) / 2.0f;
	params.halfExtents.y = fabsf(minExtBottom.y);
	
	params.topHalfExtents = (maxExtTop - minExtTop) / 2.0f;
	params.topHalfExtents.y = fabsf(maxExtTop.y);
	
	// only use tapered if difference is large enough
	const decVector2 topExtends(params.topHalfExtents.x, params.topHalfExtents.z);
	const decVector2 bottomExtends(params.halfExtents.x, params.halfExtents.z);
	const float maxExtLen = decMath::max(bottomExtends.Length(), topExtends.Length());
	const decVector2 diff(topExtends - bottomExtends);
	if(diff.Length() < maxExtLen * pUseTaperedRatio){
		// return params; // not enough difference. use box
	}
	
	// generate hull vertices
	params.hullVertices = decShapeHull::PointList(devctag,
		// bottom face (y = -halfHeight)
		decVector(-params.halfExtents.x, -params.halfExtents.y, -params.halfExtents.z),
		decVector( params.halfExtents.x, -params.halfExtents.y, -params.halfExtents.z),
		decVector( params.halfExtents.x, -params.halfExtents.y,  params.halfExtents.z),
		decVector(-params.halfExtents.x, -params.halfExtents.y,  params.halfExtents.z),
		
		// top face (y = +halfHeight)
		decVector(-params.topHalfExtents.x, params.topHalfExtents.y, -params.topHalfExtents.z),
		decVector( params.topHalfExtents.x, params.topHalfExtents.y, -params.topHalfExtents.z),
		decVector( params.topHalfExtents.x, params.topHalfExtents.y,  params.topHalfExtents.z),
		decVector(-params.topHalfExtents.x, params.topHalfExtents.y,  params.topHalfExtents.z)
	);
	
	params.type = ShapeType::taperedBox;
	return params;
}

debpBoneAutoGenShape::ShapeParams debpBoneAutoGenShape::pFitConvexHull(const WeightList &weights){
	ShapeParams params;
	params.type = ShapeType::convexHull;
	
	// For convex hull, we use the vertices directly as hull vertices
	// We need at least 4 non-coplanar points for a valid 3D convex hull
	if(weights.GetCount() < 4){
		return params; // Not enough points for a convex hull
	}
	
	params.hullVertices.EnlargeCapacityDiscard(weights.GetCount());
	weights.Visit([&](const Weight &weight){
		params.hullVertices.Add(weight.position);
	});
	
	return params;
}

debpBoneAutoGenShape::ShapeType debpBoneAutoGenShape::pSelectBestShape(const WeightList &weights,
const ShapeParams &capsule, const ShapeParams &box, const ShapeParams &convexHull){
	// using simple heuristic based on aspect ratios. for better results computing fit error
	// for each shape is required. but until shown to be a problem this will do. the basic
	// matching rules are like this:
	// - capsule is best for elongated shapes like limbs
	// - box is best for blocky shapes like torso
	// - sphere is fallback
	// - convexHull is best for complex shapes that don't fit simple primitives
	//
	// capsules are a good default shape for bones since most bones are limb-like
	
	ShapeType bestType = capsule.type != ShapeType::none ? capsule.type : box.type;
	
	// if box volume is much smaller than sphere/capsule volume use box
	if(box.type != ShapeType::none && capsule.type != ShapeType::none){
		float capsuleVol = vFltMax;
		float boxVol = vFltMax;
		
		if(box.type == ShapeType::box){
			boxVol = 8.0f * box.halfExtents.x * box.halfExtents.y * box.halfExtents.z;
			
		}else if(box.type == ShapeType::taperedBox){
			// tapered box volume: V = h/3 * (A1 + A2 + sqrt(A1*A2))
			// height: h = halfExtents.y + topHalfExtents.y
			// bottom area: a1 = 4 * halfExtents.x * halfExtents.z
			// top area: a2 = 4 * topHalfExtents.x * topHalfExtents.z
			const float h = box.halfExtents.y + box.topHalfExtents.y;
			const float a1 = 4.0f * box.halfExtents.x * box.halfExtents.z;
			const float a2 = 4.0f * box.topHalfExtents.x * box.topHalfExtents.z;
			boxVol = (h / 3.0f) * (a1 + a2 + sqrtf(a1 * a2));
		}
		
		if(capsule.type == ShapeType::sphere){
			capsuleVol = (4.0f / 3.0f) * PI * capsule.radius * capsule.radius * capsule.radius;
			
		}else if(capsule.type == ShapeType::capsule){
			capsuleVol = (4.0f / 3.0f) * PI * capsule.radius * capsule.radius * capsule.radius
				+ PI * capsule.radius * capsule.radius * capsule.halfHeight * 2.0f;
			
		}else if(capsule.type == ShapeType::taperedCapsule){
			// tapered capsule volume: frustum of cone + two hemispheres
			// frustum height = 2 * halfHeight
			// frustum volume = (1/3) * PI * h * (r1^2 + r2^2 + r1*r2)
			// hemispheres volume = (2/3) * PI * (r1^3 + r2^3)
			const float h = capsule.halfHeight * 2.0f;
			const float r1 = capsule.radiusBottom;
			const float r2 = capsule.radiusTop;
			const float frustumVol = (1.0f / 3.0f) * PI * h * (r1 * r1 + r2 * r2 + r1 * r2);
			const float hemispheresVol = (2.0f / 3.0f) * PI * (r1 * r1 * r1 + r2 * r2 * r2);
			capsuleVol = frustumVol + hemispheresVol;
		}
		
		if(boxVol < capsuleVol * pPreferBoxVolRatio){
			bestType = box.type;
		}
	}
	
	// check if convex hull is significantly better (use fit error)
	/*
	const float hullError = pComputeFitError(convexHull, vertices, weights);
	const float capsuleError = pComputeFitError(capsule, vertices, weights);
	const float boxError = pComputeFitError(box, vertices, weights);
	
	if(hullError < capsuleError * 0.5f && hullError < boxError * 0.5f){
		return ShapeType::convexHull;
	}
	*/
	
	return bestType;
}

float debpBoneAutoGenShape::pComputeFitError(const ShapeParams &params, const WeightList &weights){
	float totalError = 0.0f;
	float totalWeight = 0.0f;
	
	weights.Visit([&](const Weight &weight){
		const decVector local(params.conjOrientation * (weight.position - params.center));
		float dist = 0.0f;
		
		switch(params.type){
		case ShapeType::sphere:
			dist = fabsf(local.Length() - params.radius);
			break;
			
		case ShapeType::box:
			dist = decMath::max(
				fabsf(local.x) - params.halfExtents.x,
				fabsf(local.y) - params.halfExtents.y,
				fabsf(local.z) - params.halfExtents.z,
				0.0f);
			break;
			
		case ShapeType::capsule:
		case ShapeType::taperedCapsule:{
			// distance to capsule
			const float hh = params.halfHeight;
			float r = params.radius;
			
			if(params.type == ShapeType::taperedCapsule){
				// interpolate radius
				const float t = (local.z + hh) / (2.0f * hh);
				r = params.radiusBottom * (1.0f - t) + params.radiusTop * t;
			}
			const float dxy = sqrtf(local.x * local.x + local.y * local.y);
			const float dz = fabsf(local.z) - hh;
			if(dz > 0){
				dist = sqrtf(dxy * dxy + dz * dz) - r;
				
			}else{
				dist = dxy - r;
			}
			dist = decMath::max(dist, 0.0f);
			}break;
			
		case ShapeType::taperedBox:{
			const decVector avgExtents((params.halfExtents + params.topHalfExtents) / 2.0f);
			dist = decMath::max(
				fabsf(local.x) - avgExtents.x,
				fabsf(local.y) - avgExtents.y,
				fabsf(local.z) - avgExtents.z,
				0.0f);
			}break;
			
		default:
			dist = 0.0f;
		}
		
		totalError += dist * weight.weight;
		totalWeight += weight.weight;
	});
	
	return totalWeight > 0 ? totalError / totalWeight : 0.0f;
}
