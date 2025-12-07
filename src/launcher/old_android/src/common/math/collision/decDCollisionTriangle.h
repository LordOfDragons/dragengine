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

#ifndef _DECDCOLLISIONTRIANGLE_H_
#define _DECDCOLLISIONTRIANGLE_H_

#include "decDCollisionVolume.h"



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
class decDCollisionTriangle : public decDCollisionVolume{
private:
	decDVector pCorners[3], pEdges[3];
	decDVector pNormal;
	double pDist;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision triangle with all points 0. */
	decDCollisionTriangle();
	/** Creates a new collision triangle from the given parameters. */
	decDCollisionTriangle(const decDVector &corner1, const decDVector &corner2, const decDVector &corner3);
	/**
	 * Creates a new collision triangle from the given parameters.
	 * Here you can also specify the normal of the triangle. Do not use
	 * this function anymore as it depracted and will be removed soon.
	 */
	decDCollisionTriangle(const decDVector &corner1, const decDVector &corner2, const decDVector &corner3, const decDVector &normal);
	/** Cleans up the collision triangle. */
	~decDCollisionTriangle() override;
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	bool VolumeHitsVolume(decDCollisionVolume *volume) override;
	double VolumeMoveHitsVolume(decDCollisionVolume *volume, const decDVector &displacement, decDVector *normal) override;
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	bool SphereHitsVolume(decDCollisionSphere *sphere) override;
	bool CylinderHitsVolume(decDCollisionCylinder *cylinder) override;
	bool CapsuleHitsVolume(decDCollisionCapsule *capsule) override;
	bool BoxHitsVolume(decDCollisionBox *box) override;
	bool TriangleHitsVolume(decDCollisionTriangle *triangle) override;
	bool FrustumHitsVolume(decDCollisionFrustum *frustum) override;
	double SphereMoveHitsVolume(decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal) override;
	double CylinderMoveHitsVolume(decDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal) override;
	double CapsuleMoveHitsVolume(decDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal) override;
	double BoxMoveHitsVolume(decDCollisionBox *box, const decDVector &displacement, decDVector *normal) override;
	double TriangleMoveHitsVolume(decDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal) override;
	double FrustumMoveHitsVolume(decDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal) override;
	double PointMoveHitsVolume(const decDVector &point, const decDVector &displacement, decDVector *normal) override;
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	void GetEnclosingSphere(decDCollisionSphere *sphere) override;
	void GetEnclosingBox(decDCollisionBox *box) override;
	/*@}*/
	
	/** @name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	bool IsPointInside(const decDVector &point) override;
	/** Retrieves the closest point on the volume. */
	decDVector ClosestPointTo(const decDVector &point) override;
	/*@}*/
	
	/** @name Visiting */
	/*{*/
	void Visit(decDCollisionVolumeVisitor *visitor) override;
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this triangle. */
	bool SphereHitsTriangle(decDCollisionSphere *sphere);
	/**
	 * Determines if the given cylinder hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CylinderHitsTriangle(decDCollisionCylinder *cylinder);
	/**
	 * Determines if the given capsule hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CapsuleHitsTriangle(decDCollisionCapsule *capsule);
	/** Determines if the given box hits this triangle. */
	bool BoxHitsTriangle(decDCollisionBox *box);
	/**
	 * Determines if the given triangle hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool TriangleHitsTriangle(decDCollisionTriangle *triangle);
	
	/** Determines the distance of the given sphere to move until colliding with this triangle. */
	double SphereMoveHitsTriangle(decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal);
	/**
	 * Determines the distance of the given cylinder to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CylinderMoveHitsTriangle(decDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal);
	/**
	 * Determines the distance of the given capsule to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CapsuleMoveHitsTriangle(decDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal);
	/** Determines the distance of the given box to move until colliding with this triangle. */
	double BoxMoveHitsTriangle(decDCollisionBox *box, const decDVector &displacement, decDVector *normal);
	/**
	 * Determines the distance of the given triangle to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double TriangleMoveHitsTriangle(decDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal);
	/*@}*/
	
	/** @name Management */
	/*@{*/
	inline const decDVector &GetCorner1() const{ return pCorners[0]; }
	inline const decDVector &GetCorner2() const{ return pCorners[1]; }
	inline const decDVector &GetCorner3() const{ return pCorners[2]; }
	inline const decDVector &GetNormal() const{ return pNormal; }
	/**
	 * Retrieves the precalculated distance value. The distance
	 * is the dot product between the first corner point and
	 * the triangle normal.
	 */
	inline double GetDistance() const{ return pDist; }
	/** Sets the corners of the triangle and calculates the normal and distance. */
	void SetCorners(const decDVector &corner1, const decDVector &corner2, const decDVector &corner3);
	/**
	 * Sets the corners of the triangle and calculates the distance.
	 * Here you can set the normal yourself. Do not use this function
	 * anymore as it is depracted and will be removed soon.
	 */
	void SetCorners(const decDVector &corner1, const decDVector &corner2, const decDVector &corner3, const decDVector &normal);
	/*@}*/
};


// end of include only once
#endif
