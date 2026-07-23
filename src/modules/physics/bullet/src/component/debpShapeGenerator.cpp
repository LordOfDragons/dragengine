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

#include "debpShapeGenerator.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/shape/decShapeSphere.h>


// Class debpShapeGenerator
/////////////////////////////

const float vFltMax = std::numeric_limits<float>::max();

debpShapeGenerator::FittingParams::FittingParams(const WeightList &aweights) :
weights(aweights){
}


// Constructor, destructor
////////////////////////////

debpShapeGenerator::debpShapeGenerator() :
pMinRadius(0.005f),
pMinHalfExtents(0.005f),
pMinHalfHeight(0.005f),
pPreferBoxVolRatio(0.75f), //0.5f
pUseTaperedRatio(0.05f),
pUseAxisScalingRatio(0.05f){
}


// Management
///////////////

debpShapeGenerator::ShapeListRef debpShapeGenerator::Create(const WeightList &weights){
	if(weights.GetCount() < 4){
		return {};
	}
	
	FittingParams fparams(weights);
	if(!pPrepareFittingParams(fparams)){
		return {};
	}
	
	const auto capsule = pFitTaperedCapsule(fparams);
	const auto box = pFitBox(fparams);
	const auto convexHull = pFitConvexHull(fparams);
	
	switch(pSelectBestShape(fparams, capsule, box, convexHull)){
	case ShapeType::sphere:
		return deTUniqueReference<decShape::List>::New(devctag,
			decShapeSphere::Ref::New(capsule.radius, capsule.center));
		
	case ShapeType::ellipsoid:
		return deTUniqueReference<decShape::List>::New(devctag,
			decShapeSphere::Ref::New(capsule.radius, capsule.axisScaling,
				capsule.center, capsule.orientation));
		
	case ShapeType::capsule:
		return deTUniqueReference<decShape::List>::New(devctag,
			decShapeCapsule::Ref::New(capsule.halfHeight, capsule.radius, capsule.radius,
				capsule.topAxisScaling, capsule.bottomAxisScaling,
				capsule.center, capsule.orientation));
		
	case ShapeType::taperedCapsule:
		return deTUniqueReference<decShape::List>::New(devctag,
			decShapeCapsule::Ref::New(capsule.halfHeight, capsule.radiusTop,
				capsule.radiusBottom, capsule.topAxisScaling, capsule.bottomAxisScaling,
				capsule.center, capsule.orientation));
		
	case ShapeType::box:
		return deTUniqueReference<decShape::List>::New(devctag,
			decShapeBox::Ref::New(box.halfExtents, box.center, box.orientation));
		
	case ShapeType::taperedBox:
		return deTUniqueReference<decShape::List>::New(devctag,
			decShapeBox::Ref::New(box.halfExtents, box.axisScaling, box.center, box.orientation));
		
	case ShapeType::convexHull:
		return deTUniqueReference<decShape::List>::New(devctag,
			decShapeHull::Ref::New(convexHull.center, convexHull.orientation, convexHull.hullVertices));
		
	case ShapeType::none:
	default:
		break;
	}
	
	return {};
}


// Private Functions
//////////////////////

bool debpShapeGenerator::pPrepareFittingParams(FittingParams &params){
	params.totalWeight = params.weights.Inject(0.0f, [](float acc, const Weight &weight){
		return acc + weight.weight;
	});
	if(params.totalWeight < FLOAT_SAFE_EPSILON){
		return false;
	}
	
	params.center = params.weights.Inject(decVector(), [&](const decVector &acc, const Weight &weight){
		return acc + weight.position * weight.weight;
	}) / params.totalWeight;
	
	params.covariance = pCalcCovariance(params.weights, params.center, params.totalWeight);
	params.eigenvectors = pCalcEigenvectors(params.covariance, params.eigenvalues);
	
	params.majorAxis = params.eigenvectors.TransformRight().Normalized(); // first eigenvector
	params.minorAxis = params.eigenvectors.TransformUp().Normalized(); // second eigenvector
	
	// for analytic shapes the major axis lines up with the up axis
	params.matrix = decMatrix::CreateWorld(params.center, params.minorAxis, params.majorAxis);
	params.invMatrix = params.matrix.QuickInvert();
	
	params.orientation = params.matrix.ToQuaternion();
	params.conjOrientation = params.orientation.Conjugate();
	
	params.weights.Visit([&](const Weight &weight){
		params.vertices.Add(params.invMatrix * weight.position);
	});
	
	params.maxExtents = {-vFltMax, -vFltMax, -vFltMax};
	params.minExtents = { vFltMax,  vFltMax,  vFltMax};
	params.vertices.Visit([&](const decVector &v){
		params.minExtents.SetSmallest(v);
		params.maxExtents.SetLargest(v);
	});
	
	params.extends = params.maxExtents - params.minExtents;
	params.halfExtents = params.extends / 2.0f;
	
	const decVector offset((params.maxExtents + params.minExtents) / 2.0f);
	params.center += params.matrix.TransformNormal(offset);
	
	params.minExtents -= offset;
	params.maxExtents -= offset;
	params.vertices.Visit([&](decVector &v){
		v -= offset;
	});
	
	params.matrix = decMatrix::CreateWorld(params.center, params.minorAxis, params.majorAxis);
	params.invMatrix = params.matrix.QuickInvert();
	
	return true;
}

debpShapeGenerator::ShapeParams debpShapeGenerator::pFitSphere(const FittingParams &fparams){
	ShapeParams sparams;
	sparams.type = ShapeType::sphere;
	sparams.center = fparams.center;
	
	sparams.radius = decMath::max(fparams.halfExtents.y, pMinRadius);
	
	const decVector2 axisScaling(
		fparams.halfExtents.x / sparams.radius,
		fparams.halfExtents.z / sparams.radius);
	
	if(!axisScaling.IsEqualTo({1.0f, 1.0f}, pUseAxisScalingRatio)){
		sparams.type = ShapeType::ellipsoid;
		sparams.axisScaling = axisScaling;
		sparams.orientation = fparams.orientation;
		sparams.conjOrientation = fparams.conjOrientation;
	}
	
	return sparams;
}

debpShapeGenerator::ShapeParams debpShapeGenerator::pFitCapsule(const FittingParams &fparams){
	const ShapeParams sparamsSphere(pFitSphere(fparams));
	if(sparamsSphere.type == ShapeType::none){
		return {};
	}
	
	ShapeParams sparams;
	sparams.type = ShapeType::capsule;
	sparams.center = fparams.center;
	sparams.orientation = fparams.orientation;
	sparams.conjOrientation = fparams.conjOrientation;
	
	sparams.radius = decMath::max(fparams.halfExtents.x, fparams.halfExtents.z, pMinRadius);
	
	// half height does not include radius
	sparams.halfHeight = fparams.halfExtents.y - sparams.radius;
	
	if(sparams.halfHeight < pMinHalfHeight){
		return sparamsSphere; // use sphere instead
	}
	
	const decVector2 axisScaling(
		fparams.halfExtents.x / sparams.radius,
		fparams.halfExtents.z / sparams.radius);
	
	if(!axisScaling.IsEqualTo({1.0f, 1.0f}, pUseAxisScalingRatio)){
		sparams.topAxisScaling = sparams.bottomAxisScaling = axisScaling;
	}
	
	return sparams;
}

debpShapeGenerator::ShapeParams debpShapeGenerator::pFitTaperedCapsule(const FittingParams &fparams){
	const ShapeParams sparamsCapsule(pFitCapsule(fparams));
	if(sparamsCapsule.type != ShapeType::capsule){
		return sparamsCapsule;
	}
	
	ShapeParams sparams(sparamsCapsule);
	sparams.type = ShapeType::taperedCapsule;
	
	// radius at bottom 25% and top 25%
	const float bottomY = fparams.minExtents.y + fparams.extends.y * 0.25f;
	const float topY = fparams.maxExtents.y - fparams.extends.y * 0.25f;
	
	decVector minExtTop(vFltMax, vFltMax, vFltMax);
	decVector maxExtTop(-vFltMax, -vFltMax, -vFltMax);
	decVector minExtBottom(vFltMax, vFltMax, vFltMax);
	decVector maxExtBottom(-vFltMax, -vFltMax, -vFltMax);
	float radiusBottomSquared = 0.0f, radiusTopSquared = 0.0f;
	int countBottom = 0, countTop = 0;
	
	fparams.vertices.Visit([&](const decVector &v){
		const float radiusSquared = decVector2(v.x, v.z).LengthSquared();
		if(v.y <= bottomY){
			minExtBottom.SetSmallest(v);
			maxExtBottom.SetLargest(v);
			radiusBottomSquared = decMath::max(radiusBottomSquared, radiusSquared);
			countBottom++;
			
		}else if(v.y >= topY){
			minExtTop.SetSmallest(v);
			maxExtTop.SetLargest(v);
			radiusTopSquared = decMath::max(radiusTopSquared, radiusSquared);
			countTop++;
		}
	});
	
	if(countBottom == 0 || countTop == 0){
		return sparamsCapsule; // not enough points
	}
	
	sparams.radiusBottom = sqrtf(radiusBottomSquared);
	sparams.radiusTop = sqrtf(radiusTopSquared);
	
	// only use tapered if radi difference is large enough
	if(fabsf(sparams.radiusTop - sparams.radiusBottom) < sparams.radius * pUseTaperedRatio){
		return sparamsCapsule;
	}
	
	// ensure minimum radius. bullet requires at least 1cm to not cause problems
	sparams.halfHeight = decMath::max((fparams.extends.y - sparams.radiusTop - sparams.radiusBottom) / 2.0f, pMinHalfHeight);
	
	// adjust center to capsule center
	sparams.center += fparams.matrix.TransformNormal({0.0f,
		(fparams.minExtents.y + sparams.radiusBottom + fparams.maxExtents.y - sparams.radiusTop) / 2.0f,
		0.0f});
	
	// axis scaling
	const decVector topHalfExtents((maxExtTop - minExtTop) / 2.0f);
	const decVector bottomHalfExtents((maxExtBottom - minExtBottom) / 2.0f);
	
	const decVector2 topAxisScaling(
		topHalfExtents.x / sparams.radiusTop,
		topHalfExtents.z / sparams.radiusTop);
	const decVector2 bottomAxisScaling(
		bottomHalfExtents.x / sparams.radiusBottom,
		bottomHalfExtents.z / sparams.radiusBottom);
	
	if(!topAxisScaling.IsEqualTo({1.0f, 1.0f}, pUseAxisScalingRatio)
	|| !bottomAxisScaling.IsEqualTo({1.0f, 1.0f}, pUseAxisScalingRatio)){
		sparams.topAxisScaling = topAxisScaling;
		sparams.bottomAxisScaling = bottomAxisScaling;
	}
	
	return sparams;
}

debpShapeGenerator::ShapeParams debpShapeGenerator::pFitBox(const FittingParams &fparams){
	ShapeParams sparams;
	sparams.type = ShapeType::box;
	sparams.center = fparams.center;
	sparams.orientation = fparams.orientation;
	sparams.conjOrientation = fparams.conjOrientation;
	
	sparams.halfExtents = fparams.halfExtents.Largest(
		{pMinHalfExtents, pMinHalfExtents, pMinHalfExtents});
	
	
	// tapered box
	
	// extents at top and bottom end
	const float bottomY = fparams.minExtents.y + fparams.extends.y * 0.25f;
	const float topY = fparams.maxExtents.y - fparams.extends.y * 0.25f;
	
	decVector minExtBottom(vFltMax, vFltMax, vFltMax);
	decVector maxExtBottom(-vFltMax, -vFltMax, -vFltMax);
	decVector minExtTop(vFltMax, vFltMax, vFltMax);
	decVector maxExtTop(-vFltMax, -vFltMax, -vFltMax);
	
	fparams.vertices.Visit([&](const decVector &v){
		if(v.y < bottomY){
			minExtBottom.SetSmallest(v);
			maxExtBottom.SetLargest(v);
			
		}else if(v.y > topY){
			minExtTop.SetSmallest(v);
			maxExtTop.SetLargest(v);
		}
	});
	
	if(minExtBottom.y == vFltMax || minExtTop.y == vFltMax){
		return sparams; // not enough points. use box
	}
	
	// only use tapered if difference is large enough
	const decVector topHalfExtents((maxExtTop - minExtTop) / 2.0f);
	const decVector bottomHalfExtents((maxExtBottom - minExtBottom) / 2.0f);
	
	const decVector2 checkTopExtents(topHalfExtents.x, topHalfExtents.z);
	const decVector2 checkBottomExtents(bottomHalfExtents.x, bottomHalfExtents.z);
	const float maxExtLen = decMath::max(checkBottomExtents.Length(), checkTopExtents.Length());
	if((checkTopExtents - checkBottomExtents).Length() < maxExtLen * pUseTaperedRatio){
		return sparams; // not enough difference. use box
	}
	
	// ensure the larger extends is at the bottom
	if(topHalfExtents.LengthSquared() < bottomHalfExtents.LengthSquared()){
		sparams.halfExtents.x = decMath::max(bottomHalfExtents.x, pMinHalfExtents);
		sparams.halfExtents.z = decMath::max(bottomHalfExtents.z, pMinHalfExtents);
		sparams.axisScaling.x = topHalfExtents.x / sparams.halfExtents.x;
		sparams.axisScaling.y = topHalfExtents.z / sparams.halfExtents.z;
		
	}else{
		sparams.orientation = decQuaternion::CreateFromEulerX(180.0f * DEG2RAD) * sparams.orientation;
		sparams.conjOrientation = sparams.orientation.Conjugate();
		sparams.halfExtents.x = decMath::max(topHalfExtents.x, pMinHalfExtents);
		sparams.halfExtents.z = decMath::max(topHalfExtents.z, pMinHalfExtents);
		sparams.axisScaling.x = bottomHalfExtents.x / sparams.halfExtents.x;
		sparams.axisScaling.y = bottomHalfExtents.z / sparams.halfExtents.z;
	}
	
	sparams.type = ShapeType::taperedBox;
	return sparams;
}

debpShapeGenerator::ShapeParams debpShapeGenerator::pFitConvexHull(const FittingParams &fparams){
	ShapeParams sparams;
	sparams.type = ShapeType::convexHull;
	
	// For convex hull, we use the vertices directly as hull vertices
	// We need at least 4 non-coplanar points for a valid 3D convex hull
	if(fparams.weights.GetCount() < 4){
		return sparams; // Not enough points for a convex hull
	}
	
	sparams.hullVertices.EnlargeCapacityDiscard(fparams.weights.GetCount());
	fparams.weights.Visit([&](const Weight &weight){
		sparams.hullVertices.Add(weight.position);
	});
	
	return sparams;
}

debpShapeGenerator::ShapeType debpShapeGenerator::pSelectBestShape(const FittingParams &fparams,
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
			const decVector topHalfExtents(box.halfExtents.
				Multiply({box.axisScaling.x, 1.0f, box.axisScaling.y}));
			const float h = box.halfExtents.y * 2.0f;
			const float a1 = 4.0f * box.halfExtents.x * box.halfExtents.z;
			const float a2 = 4.0f * topHalfExtents.x * topHalfExtents.z;
			boxVol = (h / 3.0f) * (a1 + a2 + sqrtf(a1 * a2));
		}
		
		if(capsule.type == ShapeType::sphere){
			capsuleVol = (4.0f / 3.0f) * PI * capsule.radius * capsule.radius * capsule.radius;
			
		}else if(capsule.type == ShapeType::ellipsoid){
			// ellipsoid volume: V = 4/3 * PI * a * b * c
			const float a = capsule.radius;
			const float b = capsule.radius * capsule.axisScaling.x;
			const float c = capsule.radius * capsule.axisScaling.y;
			capsuleVol = (4.0f / 3.0f) * PI * a * b * c;
			
		}else if(capsule.type == ShapeType::capsule){
			// capsule volume: ellipsoid (two hemispheres) + cylinder
			// ellipsoid: V = 4/3 * PI * a * b * c where a=radius, b=radius*tx, c=radius*ty
			// cylinder: V = PI * a * b * h where a=radius*tx, b=radius*ty, h=2*halfHeight
			const float rx = capsule.topAxisScaling.x;
			const float ry = capsule.topAxisScaling.y;
			const float ellipsoidVol = (4.0f / 3.0f) * PI * capsule.radius * capsule.radius * capsule.radius * rx * ry;
			const float cylinderVol = PI * capsule.radius * capsule.radius * rx * ry * capsule.halfHeight * 2.0f;
			capsuleVol = ellipsoidVol + cylinderVol;
			
		}else if(capsule.type == ShapeType::taperedCapsule){
			// tapered capsule volume: frustum of elliptical cone + two half-ellipsoids
			// frustum height = 2 * halfHeight
			// frustum volume = (h/3) * (A1 + A2 + sqrt(A1*A2))
			// where A1 = PI * r1^2 * bx * by, A2 = PI * r2^2 * tx * ty
			// half-ellipsoid volume = (2/3) * PI * a * b * c
			// bottom: a = r1 * bx, b = r1, c = r1 * by
			// top: a = r2 * tx, b = r2, c = r2 * ty
			const float h = capsule.halfHeight * 2.0f;
			const float r1 = capsule.radiusBottom;
			const float r2 = capsule.radiusTop;
			const float bx = capsule.bottomAxisScaling.x;
			const float by = capsule.bottomAxisScaling.y;
			const float tx = capsule.topAxisScaling.x;
			const float ty = capsule.topAxisScaling.y;
			
			const float a1 = PI * r1 * r1 * bx * by;
			const float a2 = PI * r2 * r2 * tx * ty;
			const float frustumVol = (h / 3.0f) * (a1 + a2 + sqrtf(a1 * a2));
			
			const float bottomHemiVol = (2.0f / 3.0f) * PI * r1 * r1 * r1 * bx * by;
			const float topHemiVol = (2.0f / 3.0f) * PI * r2 * r2 * r2 * tx * ty;
			const float hemispheresVol = bottomHemiVol + topHemiVol;
			
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

float debpShapeGenerator::pComputeFitError(const ShapeParams &params, const FittingParams &fparams){
	float totalError = 0.0f;
	float totalWeight = 0.0f;
	
	fparams.weights.Visit([&](const Weight &weight){
		const decVector local(params.conjOrientation * (weight.position - params.center));
		float dist = 0.0f;
		
		switch(params.type){
		case ShapeType::sphere:
			dist = fabsf(local.Length() - params.radius);
			break;
			
		case ShapeType::ellipsoid:{
			// distance to ellipsoid: sqrt((x/a)^2 + (y/b)^2 + (z/c)^2) - 1
			// where a, b, c are the ellipsoid radii along local axes
			const float a = decMath::max(params.radius, 0.001f);
			const float b = decMath::max(params.radius * params.axisScaling.x, 0.001f);
			const float c = decMath::max(params.radius * params.axisScaling.y, 0.001f);
			const float termX = (local.x / a) * (local.x / a);
			const float termY = (local.y / b) * (local.y / b);
			const float termZ = (local.z / c) * (local.z / c);
			const float ellipsoidValue = sqrtf(termX + termY + termZ);
			dist = ellipsoidValue - 1.0f;
			}break;
			
		case ShapeType::box:{
			const decVector diff(local.Absolute() - params.halfExtents);
			dist = decMath::max(diff.x, diff.y, diff.z, 0.0f);
			}break;
			
		case ShapeType::capsule:
		case ShapeType::taperedCapsule:{
			// distance to capsule
			const float hh = params.halfHeight;
			float r = params.radius;
			float rx = 1.0f;
			float ry = 1.0f;
			
			if(params.type == ShapeType::taperedCapsule){
				// interpolate radius and axis scaling
				const float t = (local.z + hh) / (2.0f * hh);
				r = params.radiusBottom * (1.0f - t) + params.radiusTop * t;
				rx = params.bottomAxisScaling.x * (1.0f - t) + params.topAxisScaling.x * t;
				ry = params.bottomAxisScaling.y * (1.0f - t) + params.topAxisScaling.y * t;
				
			}else{
				rx = params.topAxisScaling.x;
				ry = params.topAxisScaling.y;
			}
			
			// distance to elliptical cross-section
			const float dxy = sqrtf((local.x / rx) * (local.x / rx) + (local.y / ry) * (local.y / ry));
			const float dz = fabsf(local.z) - hh;
			if(dz > 0){
				dist = sqrtf(dxy * dxy + dz * dz) - r;
				
			}else{
				dist = dxy - r;
			}
			dist = decMath::max(dist, 0.0f);
			}break;
			
		case ShapeType::taperedBox:{
			const decVector topHalfExtends(params.halfExtents.Multiply(
				{params.axisScaling.x, 1.0f, params.axisScaling.y}));
			const decVector avgHalfExtends((params.halfExtents + topHalfExtends) / 2.0f);
			const decVector diff(local.Absolute() - avgHalfExtends);
			dist = decMath::max(diff.x, diff.y, diff.z, 0.0f);
			}break;
			
		default:
			dist = 0.0f;
		}
		
		totalError += dist * weight.weight;
		totalWeight += weight.weight;
	});
	
	return totalWeight > 0 ? totalError / totalWeight : 0.0f;
}

decMatrix debpShapeGenerator::pCalcCovariance(const WeightList &weights,
const decVector &center, float totalWeight){
	decMatrix covariance;
	covariance.a11 = covariance.a22 = covariance.a33 = 0.0f;
	
	weights.Visit([&](const Weight &weight){
		const decVector diff(weight.position - center);
		const float w = weight.weight / totalWeight;
		
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
	
	// symmetrize covariance matrix
	covariance.a12 = covariance.a21;
	covariance.a13 = covariance.a31;
	covariance.a23 = covariance.a32;
	
	return covariance;
}

decMatrix debpShapeGenerator::pCalcEigenvectors(const decMatrix &matrix, decVector &eigenvalues){
	// Jacobi eigenvalue decomposition for symmetric 3x3 matrix
	// A = V * D * V^T where V is eigenvector matrix, D is diagonal eigenvalues
	decMatrix working(matrix);
	decMatrix eigenvectors;
	
	float * const valWorking = &working.a11;
	float * const valEigenvectors = &eigenvectors.a11;
	
	eigenvalues = {working.a11, working.a22, working.a33};
	
	// jacobi iteration for symmetric matrix
	for(int iter=0; iter<50; iter++){
		const float maxOffDiag = fabs(working.a12) + fabs(working.a13) + fabs(working.a23);
		if(maxOffDiag < FLOAT_SAFE_EPSILON){
			break; // converged
		}
		
		// find largest off-diagonal element
		int p = 0, q = 1;
		float maxVal = fabs(working.a12);
		if(fabs(working.a13) > maxVal){
			p = 0; q = 2;
			maxVal = fabs(working.a13);
		}
		if(fabs(working.a23) > maxVal){
			p = 1; q = 2;
		}
		
		// rotation angle
		const float app = valWorking[p * 4 + p];
		const float aqq = valWorking[q * 4 + q];
		const float apq = valWorking[p * 4 + q];
		const float theta = 0.5f * atan2f(2.0f * apq, aqq - app);
		
		// apply rotation to working matrix
		const float c = cosf(theta);
		const float s = sinf(theta);
		
		for(int i=0; i<3; i++){
			if(i == p || i == q){
				continue;
			}
			
			const float aip = valWorking[i * 4 + p];
			const float aiq = valWorking[i * 4 + q];
			valWorking[i * 4 + p] = c * aip - s * aiq;
			valWorking[i * 4 + q] = s * aip + c * aiq;
			valWorking[p * 4 + i] = valWorking[i * 4 + p];
			valWorking[q * 4 + i] = valWorking[i * 4 + q];
		}
		
		// update diagonal elements of working matrix
		const float c2 = c * c;
		const float s2 = s * s;
		const float cs2 = 2.0f * c * s;
		
		const float newApp = c2 * app + s2 * aqq - cs2 * apq;
		const float newAqq = s2 * app + c2 * aqq + cs2 * apq;
		
		valWorking[p * 4 + p] = newApp;
		valWorking[q * 4 + q] = newAqq;
		valWorking[p * 4 + q] = 0.0f;
		valWorking[q * 4 + p] = 0.0f;
		
		// accumulate rotation in eigenvector matrix: V = V * R
		for(int i=0; i<3; i++){
			const float vip = valEigenvectors[i * 4 + p];
			const float viq = valEigenvectors[i * 4 + q];
			valEigenvectors[i * 4 + p] = c * vip - s * viq;
			valEigenvectors[i * 4 + q] = s * vip + c * viq;
		}
		
		// update eigenvalues from diagonal of working matrix
		eigenvalues.x = valWorking[0 * 4 + 0];
		eigenvalues.y = valWorking[1 * 4 + 1];
		eigenvalues.z = valWorking[2 * 4 + 2];
	}
	
	// sort eigenvalues and eigenvectors in descending order
	float * const valEigenvalues = &eigenvalues.x;
	
	for(int i=0; i<2; i++){
		for(int j=i+1; j<3; j++){
			if(valEigenvalues[i] < valEigenvalues[j]){
				std::swap(valEigenvalues[i], valEigenvalues[j]);
				for(int k=0; k<3; k++){
					std::swap(valEigenvectors[k * 4 + i], valEigenvectors[k * 4 + j]);
				}
			}
		}
	}
	
	return eigenvectors;
}
