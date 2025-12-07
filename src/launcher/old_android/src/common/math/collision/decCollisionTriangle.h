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

#ifndef _DECCOLLISIONTRIANGLE_H_
#define _DECCOLLISIONTRIANGLE_H_

#include "decCollisionVolume.h"



/**
 * @brief Triangle collision volume.
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
class decCollisionTriangle : public decCollisionVolume{
private:
	decVector pCorners[3], pEdges[3];
	decVector pNormal;
	float pDist;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision triangle with all points 0. */
	decCollisionTriangle();
	/** Creates a new collision triangle from the given parameters. */
	decCollisionTriangle(const decVector &corner1, const decVector &corner2, const decVector &corner3);
	/**
	 * Creates a new collision triangle from the given parameters.
	 * Here you can also specify the normal of the triangle. Do not use
	 * this function anymore as it depracted and will be removed soon.
	 */
	decCollisionTriangle(const decVector &corner1, const decVector &corner2, const decVector &corner3, const decVector &normal);
	/** Cleans up the collision triangle. */
	~decCollisionTriangle() override;
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	bool VolumeHitsVolume(decCollisionVolume *volume) override;
	float VolumeMoveHitsVolume(decCollisionVolume *volume, const decVector &displacement, decVector *normal) override;
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	bool SphereHitsVolume(decCollisionSphere *sphere) override;
	bool CylinderHitsVolume(decCollisionCylinder *cylinder) override;
	bool CapsuleHitsVolume(decCollisionCapsule *capsule) override;
	bool BoxHitsVolume(decCollisionBox *box) override;
	bool TriangleHitsVolume(decCollisionTriangle *triangle) override;
	bool FrustumHitsVolume(decCollisionFrustum *frustum) override;
	float SphereMoveHitsVolume(decCollisionSphere *sphere, const decVector &displacement, decVector *normal) override;
	float CylinderMoveHitsVolume(decCollisionCylinder *cylinder, const decVector &displacement, decVector *normal) override;
	float CapsuleMoveHitsVolume(decCollisionCapsule *capsule, const decVector &displacement, decVector *normal) override;
	float BoxMoveHitsVolume(decCollisionBox *box, const decVector &displacement, decVector *normal) override;
	float TriangleMoveHitsVolume(decCollisionTriangle *triangle, const decVector &displacement, decVector *normal) override;
	float FrustumMoveHitsVolume(decCollisionFrustum *frustum, const decVector &displacement, decVector *normal) override;
	float PointMoveHitsVolume(const decVector &point, const decVector &displacement, decVector *normal) override;
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	void GetEnclosingSphere(decCollisionSphere *sphere) override;
	void GetEnclosingBox(decCollisionBox *box) override;
	/*@}*/
	
	/** @name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	bool IsPointInside(const decVector &point) override;
	/** Retrieves the closest point on the volume. */
	decVector ClosestPointTo(const decVector &point) override;
	/*@}*/
	
	/** @name Visiting */
	/*{*/
	void Visit(decCollisionVolumeVisitor *visitor) override;
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this triangle. */
	bool SphereHitsTriangle(decCollisionSphere *sphere);
	/**
	 * Determines if the given cylinder hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CylinderHitsTriangle(decCollisionCylinder *cylinder);
	/**
	 * Determines if the given capsule hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CapsuleHitsTriangle(decCollisionCapsule *capsule);
	/** Determines if the given box hits this triangle. */
	bool BoxHitsTriangle(decCollisionBox *box);
	/**
	 * Determines if the given triangle hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool TriangleHitsTriangle(decCollisionTriangle *triangle);
	
	/** Determines the distance of the given sphere to move until colliding with this triangle. */
	float SphereMoveHitsTriangle(decCollisionSphere *sphere, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given cylinder to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CylinderMoveHitsTriangle(decCollisionCylinder *cylinder, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given capsule to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CapsuleMoveHitsTriangle(decCollisionCapsule *capsule, const decVector &displacement, decVector *normal);
	/** Determines the distance of the given box to move until colliding with this triangle. */
	float BoxMoveHitsTriangle(decCollisionBox *box, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given triangle to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float TriangleMoveHitsTriangle(decCollisionTriangle *triangle, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** @name Management */
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
