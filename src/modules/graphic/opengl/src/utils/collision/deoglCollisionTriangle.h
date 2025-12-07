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

#ifndef _DEOGLCOLLISIONTRIANGLE_H_
#define _DEOGLCOLLISIONTRIANGLE_H_

#include "deoglCollisionVolume.h"



/**
 * Triangle collision volume.
 *
 * Defines a collision volume in the shape of a triangle. The
 * triangle is defined by the three corner points. The points
 * should be oriented in counter clockwise order.
 * @warning This class is partially implemented. See the collision
 *          functions for more information.
 * @todo
 * - Implementing CylinderHitsTriangle
 * - Implementing TriangleHitsTriangle
 */
class deoglCollisionTriangle : public deoglCollisionVolume{
private:
	decVector pCorners[3], pEdges[3];
	decVector pNormal;
	float pDist;
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision triangle with all points 0. */
	deoglCollisionTriangle();
	/** Creates a new collision triangle from the given parameters. */
	deoglCollisionTriangle(const decVector &corner1, const decVector &corner2, const decVector &corner3);
	/**
	 * Creates a new collision triangle from the given parameters.
	 * Here you can also specify the normal of the triangle. Do not use
	 * this function anymore as it depracted and will be removed soon.
	 */
	deoglCollisionTriangle(const decVector &corner1, const decVector &corner2, const decVector &corner3, const decVector &normal);
	/** Cleans up the collision triangle. */
	~deoglCollisionTriangle() override;
	/*@}*/
	
	/** \name First Stage Dispatch */
	/*@{*/
	bool VolumeHitsVolume(deoglCollisionVolume *volume) override;
	virtual float VolumeMoveHitsVolume(deoglCollisionVolume *volume, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** \name Second Stage Dispatch */
	/*@{*/
	bool SphereHitsVolume(deoglCollisionSphere *sphere) override;
	bool CylinderHitsVolume(deoglCollisionCylinder *cylinder) override;
	bool CapsuleHitsVolume(deoglCollisionCapsule *capsule) override;
	bool BoxHitsVolume(deoglCollisionBox *box) override;
	bool TriangleHitsVolume(deoglCollisionTriangle *triangle) override;
	bool FrustumHitsVolume(deoglCollisionFrustum *frustum) override;
	virtual float SphereMoveHitsVolume(deoglCollisionSphere *sphere, const decVector &displacement, decVector *normal);
	virtual float CylinderMoveHitsVolume(deoglCollisionCylinder *cylinder, const decVector &displacement, decVector *normal);
	virtual float CapsuleMoveHitsVolume(deoglCollisionCapsule *capsule, const decVector &displacement, decVector *normal);
	virtual float BoxMoveHitsVolume(deoglCollisionBox *box, const decVector &displacement, decVector *normal);
	virtual float TriangleMoveHitsVolume(deoglCollisionTriangle *triangle, const decVector &displacement, decVector *normal);
	virtual float FrustumMoveHitsVolume(deoglCollisionFrustum *frustum, const decVector &displacement, decVector *normal);
	virtual float PointMoveHitsVolume(const decVector &point, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** \name Enclosing Volumes */
	/*@{*/
	void GetEnclosingSphere(deoglCollisionSphere *sphere) override;
	void GetEnclosingBox(deoglCollisionBox *box) override;
	/*@}*/
	
	/** \name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside(const decVector &point);
	/** Retrieves the closest point on the volume. */
	virtual decVector ClosestPointTo(const decVector &point);
	/*@}*/
	
	/** \name Visiting */
	/*{*/
	void Visit(deoglCollisionVolumeVisitor *visitor) override;
	/*}*/
	
	/** \name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this triangle. */
	bool SphereHitsTriangle(deoglCollisionSphere *sphere);
	/**
	 * Determines if the given cylinder hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CylinderHitsTriangle(deoglCollisionCylinder *cylinder);
	/**
	 * Determines if the given capsule hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CapsuleHitsTriangle(deoglCollisionCapsule *capsule);
	/** Determines if the given box hits this triangle. */
	bool BoxHitsTriangle(deoglCollisionBox *box);
	/**
	 * Determines if the given triangle hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool TriangleHitsTriangle(deoglCollisionTriangle *triangle);
	
	/** Determines the distance of the given sphere to move until colliding with this triangle. */
	float SphereMoveHitsTriangle(deoglCollisionSphere *sphere, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given cylinder to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CylinderMoveHitsTriangle(deoglCollisionCylinder *cylinder, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given capsule to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CapsuleMoveHitsTriangle(deoglCollisionCapsule *capsule, const decVector &displacement, decVector *normal);
	/** Determines the distance of the given box to move until colliding with this triangle. */
	float BoxMoveHitsTriangle(deoglCollisionBox *box, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given triangle to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float TriangleMoveHitsTriangle(deoglCollisionTriangle *triangle, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** \name Management */
	/*@{*/
	inline const decVector &GetCorner1() const{ return pCorners[0]; }
	inline const decVector &GetCorner2() const{ return pCorners[1]; }
	inline const decVector &GetCorner3() const{ return pCorners[2]; }
	inline const decVector &GetNormal() const{ return pNormal; }
	/**
	 * Retrieves the precalculated distance value. The distance
	 * is the dot product between the first corner point and
	 * the triangle normal.
	 */
	inline float GetDistance() const{ return pDist; }
	/** Sets the corners of the triangle and calculates the normal and distance. */
	void SetCorners(const decVector &corner1, const decVector &corner2, const decVector &corner3);
	/**
	 * Sets the corners of the triangle and calculates the distance.
	 * Here you can set the normal yourself. Do not use this function
	 * anymore as it is depracted and will be removed soon.
	 */
	void SetCorners(const decVector &corner1, const decVector &corner2, const decVector &corner3, const decVector &normal);
	/*@}*/
};


// end of include only once
#endif
