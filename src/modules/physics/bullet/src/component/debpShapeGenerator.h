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

#ifndef _DEBPSHAPEGENERATOR_H_
#define _DEBPSHAPEGENERATOR_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/resources/model/deModelWeight.h>


/**
 * Generate analytic collision volumes fitted to vertex clusters.
 */
class debpShapeGenerator{
public:
	/** Type holding strong reference. */
	using Ref = deTUniqueReference<debpShapeGenerator>;
	
	/** Weight. */
	struct Weight{
		decVector position;
		float weight;
	};
	using WeightList = decTList<Weight>;
	
	/** Shape list reference. */
	using ShapeListRef = deTUniqueReference<decShape::List>;
	
private:
	enum class ShapeType{
		sphere,
		ellipsoid,
		box,
		capsule,
		taperedBox,
		taperedCapsule,
		cylinder,
		taperedCylinder,
		convexHull
	};
	
	class FittingParams : public deObject{
	public:
		using Ref = deTObjectReference<FittingParams>;
		
		const WeightList &weights;
		float convexHullThreshold;
		float totalWeight;
		float volume;
		decVector center;
		decMatrix covariance;
		decMatrix eigenvectors;
		decVector eigenvalues;
		decVector majorAxis, minorAxis;
		decQuaternion orientation, conjOrientation;
		decMatrix matrix, invMatrix;
		decTList<decVector> vertices;
		decVector minExtents, maxExtents, extents, halfExtents;
		
		explicit FittingParams(const WeightList &weights);
		FittingParams(const FittingParams &fparams);
		
	protected:
		~FittingParams() override;
	};
	
	class ShapeParams : public deObject{
	public:
		using Ref = deTObjectReference<ShapeParams>;
		
		FittingParams::Ref fparams;
		ShapeType type = ShapeType::sphere;
		decVector center;
		decQuaternion orientation, conjOrientation;
		
		float radius = 0.0f; // sphere
		decVector2 axisScaling{1.0f, 1.0f}; // tapered sphere, tapered box
		decVector halfExtents; // box
		float halfHeight = 0.0f; // capsule
		float radiusTop = 0.0f; // tapered capsule
		float radiusBottom = 0.0f; // tapered capsule
		decVector2 topAxisScaling{1.0f, 1.0f}; // tapered capsule
		decVector2 bottomAxisScaling{1.0f, 1.0f}; // tapered capsule
		decTList<decVector> hullVertices; // convex hull
		
	protected:
		~ShapeParams() override;
	};
	
	struct SelectShapeResult{
		ShapeParams::Ref best;
		ShapeParams::Ref fallback;
		float maxError = 0.0f;
		float avgError = 0.0f;
	};
	
	struct FitErrorResult{
		float maxError = 0.0f;
		float avgError = 0.0f;
	};
	
	
	float pMinRadius;
	float pMinHalfExtents;
	float pMinHalfHeight;
	float pPreferBoxVolRatio;
	float pUseTaperedRatio;
	float pUseAxisScalingRatio;
	float pUseConvexHullThreshold;
	float pUseConvexHullThresholdPower;
	float pMinUseConvexHullThreshold;
	bool pTestCardinalAxes;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create automatically generated shape. */
	debpShapeGenerator();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Create best matching analytic shape from bone vertices. Positions are in bone space.
	 * Error margin indicates the error in meters to use to choose between analytic and convex
	 * hull shapes. Returns nullptr if bone has too few vertices.
	 */
	ShapeListRef Create(const WeightList &weights, float convexHullThreshold);
	/*@}*/
	
	
	
private:
	void pPrepareFittingParamsBasic(FittingParams &fparams);
	void pPrepareFittingParamsAxis(FittingParams &fparams);
	void pPrepareFittingParamsEigen(FittingParams &fparams);
	void pPrepareFittingParamsShared(FittingParams &fparams);
	
	ShapeParams::Ref pFitSphere(const FittingParams::Ref &fparams);
	ShapeParams::Ref pFitCapsule(const FittingParams::Ref &fparams);
	ShapeParams::Ref pFitTaperedCapsule(const FittingParams::Ref &fparams);
	ShapeParams::Ref pFitCylinder(const FittingParams::Ref &fparams);
	ShapeParams::Ref pFitTaperedCylinder(const FittingParams::Ref &fparams);
	ShapeParams::Ref pFitBox(const FittingParams::Ref &fparams);
	ShapeParams::Ref pFitConvexHull(const FittingParams::Ref &fparams);
	
	ShapeParams::Ref pUseBetter(const ShapeParams::Ref &params1, const ShapeParams::Ref &params2);
	
	SelectShapeResult pSelectBestShape(const ShapeParams::Ref &capsule,
		const ShapeParams::Ref &cylinder, const ShapeParams::Ref &box);
	
	FitErrorResult pComputeFitError(const ShapeParams &params);
	
	decMatrix pCalcCovariance(const WeightList &weights, const decVector &center, float totalWeight);
	decMatrix pCalcEigenvectors(const decMatrix &matrix, decVector &eigenvalues);
};

#endif