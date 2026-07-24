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

#include <dragengine/deTUniqueReference.h>
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
		none,
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
	
	class FittingParams{
	public:
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
	};
	
	struct ShapeParams{
		const FittingParams *fparams = nullptr;
		ShapeType type = ShapeType::none;
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
	};
	
	struct SelectShapeResult{
		ShapeType type = ShapeType::none;
		ShapeType fallbackType = ShapeType::none;
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
	
	ShapeParams pFitSphere(const FittingParams &fparams);
	ShapeParams pFitCapsule(const FittingParams &fparams);
	ShapeParams pFitTaperedCapsule(const FittingParams &fparams);
	ShapeParams pFitCylinder(const FittingParams &fparams);
	ShapeParams pFitTaperedCylinder(const FittingParams &fparams);
	ShapeParams pFitBox(const FittingParams &fparams);
	ShapeParams pFitConvexHull(const FittingParams &fparams);
	
	ShapeParams pUseBetter(const ShapeParams &params1, const ShapeParams &params2);
	
	SelectShapeResult pSelectBestShape(const ShapeParams &capsule,
		const ShapeParams &cylinder, const ShapeParams &box);
	
	FitErrorResult pComputeFitError(const ShapeParams &params);
	
	decMatrix pCalcCovariance(const WeightList &weights, const decVector &center, float totalWeight);
	decMatrix pCalcEigenvectors(const decMatrix &matrix, decVector &eigenvalues);
};

#endif