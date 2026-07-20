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

#ifndef _DEBPBONEAUTOGENSHAPE_H_
#define _DEBPBONEAUTOGENSHAPE_H_

#include <dragengine/deTUniqueReference.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/resources/model/deModelWeight.h>


/**
 * Automatically generated shape for bone.
 * 
 * Bone collision volume using analytic shapes fitted by bone influenced vertices.
 */
class debpBoneAutoGenShape{
public:
	/** Type holding strong reference. */
	using Ref = deTUniqueReference<debpBoneAutoGenShape>;
	
	/** Weight. */
	struct Weight{
		decVector position;
		float weight;
	};
	using WeightList = decTList<Weight>;
	
	/** Analytic shape types. */
	enum class ShapeType{
		none,
		sphere,
		box,
		capsule,
		taperedCapsule,
		taperedBox,
		convexHull
	};
	
	/** For internal use only. */
	struct ShapeParams{
		ShapeType type = ShapeType::none;
		decVector center;
		decQuaternion orientation, conjOrientation;
		
		float radius = 0.0f; // sphere
		decVector halfExtents; // box
		decVector topHalfExtents; // tapered box
		float halfHeight = 0.0f; // capsule
		float radiusTop = 0.0f; // tapered capsule
		float radiusBottom = 0.0f; // tapered capsule
		decTList<decVector> hullVertices; // convex hull
	};
	
	
private:
	static float pMinRadius, pMinHalfExtents, pPreferBoxVolRatio, pUseTaperedRatio;
	
	ShapeParams pParams;
	decShape::List pShapes;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create automatically generated shape. */
	debpBoneAutoGenShape(const ShapeParams &params);
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Shape parameters. */
	inline const ShapeParams &GetParams() const{ return pParams; }
	
	/** Shape type. */
	inline ShapeType GetShapeType() const{ return pParams.type; }
	
	/** Shapes. */
	inline const decShape::List &GetShapes() const{ return pShapes; }
	
	
	/**
	 * Create best matching analytic shape from bone vertices. Positions are in bone space.
	 * Returns null if bone has too few vertices.
	 */
	static Ref Create(const WeightList &weights);
	/*@}*/
	
	
	
private:
	void pCreateShapes();
	
	static ShapeParams pFitSphere(const WeightList &weights);
	static ShapeParams pFitCapsule(const WeightList &weights);
	static ShapeParams pFitTaperedCapsule(const WeightList &weights);
	static ShapeParams pFitBox(const WeightList &weights);
	static ShapeParams pFitConvexHull(const WeightList &weights);
	
	static ShapeType pSelectBestShape(const WeightList &weights, const ShapeParams &capsule,
		const ShapeParams &box, const ShapeParams &convexHull);
	
	static float pComputeFitError(const ShapeParams &params, const WeightList &weights);
};

#endif