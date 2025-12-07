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

#ifndef _DEOALCOLLISIONFRUSTUM_H_
#define _DEOALCOLLISIONFRUSTUM_H_

#include "deoalCollisionVolume.h"



/**
 * @brief Frustum collision volume.
 *
 * Defines a collision volume in the shape of a view frustum.
 * A view frustum is a pyramid with capped top. Such frustums
 * are used mainly for culling objects inside a camera view.
 * The frustum is defined by the six planes forming the sides
 * of the pyramid. The planes are defined by a normal and the
 * distance to the origin. A frustum is best set from a
 * projection matrix. The normals of the planes are considered
 * to point inside the frustum.
 *
 * The planes in this frustum are labeled like the pyramid is
 * laying on the ground. Hence left, right, top and bottom
 * refer to the sides of the frustum surrounding it. Near and
 * far refer to the capped pinacle and the base of the frustum.
 * @warning This class is partially implemented. See collision
 *          functions for more information.
 * @todo
 * - Implementing CylinderHitsFrustum
 * - Implementing CapsuleHitsFrustum
 * - Implementing TriangleHitsFrustum
 * - Implementing FrustumHitsFrustum
 */
class deoalCollisionFrustum : public deoalCollisionVolume{
public:
	/** Enumerator for collision type. */
	enum eIntersectType{
		/** Does not intersect frustum */
		eitOutside = -1,
		/** Intersects frustum */
		eitIntersect = 0,
		/** Is fully inside frustum */
		eitInside = 1
	};
private:
	decVector pNormalLeft, pNormalRight;
	decVector pNormalTop, pNormalBottom;
	decVector pNormalNear, pNormalFar;
	float pDistLeft, pDistRight;
	float pDistTop, pDistBottom;
	float pDistNear, pDistFar;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creastes a new collision frustum with default parameters. */
	deoalCollisionFrustum();
	/** Cleans up the collision frustum. */
	virtual ~deoalCollisionFrustum();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume(deoalCollisionVolume *volume);
	virtual float VolumeMoveHitsVolume(deoalCollisionVolume *volume, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume(deoalCollisionSphere *sphere);
	virtual bool CylinderHitsVolume(deoalCollisionCylinder *cylinder);
	virtual bool CapsuleHitsVolume(deoalCollisionCapsule *capsule);
	virtual bool BoxHitsVolume(deoalCollisionBox *box);
	virtual bool TriangleHitsVolume(deoalCollisionTriangle *triangle);
	virtual bool FrustumHitsVolume(deoalCollisionFrustum *frustum);
	virtual float SphereMoveHitsVolume(deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal);
	virtual float CylinderMoveHitsVolume(deoalCollisionCylinder *cylinder, const decVector &displacement, decVector *normal);
	virtual float CapsuleMoveHitsVolume(deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal);
	virtual float BoxMoveHitsVolume(deoalCollisionBox *box, const decVector &displacement, decVector *normal);
	virtual float TriangleMoveHitsVolume(deoalCollisionTriangle *triangle, const decVector &displacement, decVector *normal);
	virtual float FrustumMoveHitsVolume(deoalCollisionFrustum *frustum, const decVector &displacement, decVector *normal);
	virtual float PointMoveHitsVolume(const decVector &point, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere(deoalCollisionSphere *sphere);
	virtual void GetEnclosingBox(deoalCollisionBox *box);
	/*@}*/
	
	/** @name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside(const decVector &point);
	/** Retrieves the closest point on the volume. */
	virtual decVector ClosestPointTo(const decVector &point);
	/*@}*/
	
	/** @name Visiting */
	/*{*/
	virtual void Visit(deoalCollisionVolumeVisitor *visitor);
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this frustum. */
	bool SphereHitsFrustum(deoalCollisionSphere *sphere);
	/**
	 * Determines if the given cylinder hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool CylinderHitsFrustum(deoalCollisionCylinder *cylinder);
	/**
	 * Determines if the given capsule hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool CapsuleHitsFrustum(deoalCollisionCapsule *capsule);
	/** Determines if the given box hits this frustum. */
	bool BoxHitsFrustum(deoalCollisionBox *box);
	/**
	 * Determines if the given triangle hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool TriangleHitsFrustum(deoalCollisionTriangle *triangle);
	/**
	 * Determines if the given frustum hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool FrustumHitsFrustum(deoalCollisionFrustum *frustum);
	
	/**
	 * Determines the distance of the given sphere to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float SphereMoveHitsFrustum(deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given cylinder to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CylinderMoveHitsFrustum(deoalCollisionCylinder *cylinder, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given capsule to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CapsuleMoveHitsFrustum(deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given box to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float BoxMoveHitsFrustum(deoalCollisionBox *box, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given triangle to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float TriangleMoveHitsFrustum(deoalCollisionTriangle *triangle, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given frustum to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float FrustumMoveHitsFrustum(deoalCollisionFrustum *frustum, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** @name Collision Routines */
	/*@{*/
	inline decVector GetLeftNormal() const{ return pNormalLeft; }
	inline decVector GetRightNormal() const{ return pNormalRight; }
	inline decVector GetTopNormal() const{ return pNormalTop; }
	inline decVector GetBottomNormal() const{ return pNormalBottom; }
	inline decVector GetNearNormal() const{ return pNormalNear; }
	inline decVector GetFarNormal() const{ return pNormalFar; }
	inline float GetLeftDistance() const{ return pDistLeft; }
	inline float GetRightDistance() const{ return pDistRight; }
	inline float GetTopDistance() const{ return pDistTop; }
	inline float GetBottomDistance() const{ return pDistBottom; }
	inline float GetNearDistance() const{ return pDistNear; }
	inline float GetFarDistance() const{ return pDistFar; }
	void SetLeftPlane(const decVector &normal, float dist);
	void SetRightPlane(const decVector &normal, float dist);
	void SetTopPlane(const decVector &normal, float dist);
	void SetBottomPlane(const decVector &normal, float dist);
	void SetNearPlane(const decVector &normal, float dist);
	void SetFarPlane(const decVector &normal, float dist);
	/** Sets the frustm from the given projection matrix. */
	void SetFrustum(const decDMatrix &mat);
	/**
	 * Sets the frustum from the given parameters.
	 * @param origin Pinacle of the uncapped frustum.
	 * @param r1 Top-Left corner part of the left/top/far planes
	 * @param r2 Top-Right corner part of the right/top/far planes
	 * @param r3 Bottom-Right corner part of the right/bottom/far planes
	 * @param r4 Bottom-Left corner part of the left/bottom/far planes
	 * @param nearDist Distance from uncapped frustum pinacle to the near plane
	 */
	void SetFrustum(const decVector &origin, const decVector &r1, const decVector &r2, const decVector &r3, const decVector &r4, float nearDist);
	/**
	 * Sets the frustum from the given parameters. The origin of the
	 * frustum is set to the 0 vector.
	 * @param r1 Top-Left corner part of the left/top/far planes
	 * @param r2 Top-Right corner part of the right/top/far planes
	 * @param r3 Bottom-Right corner part of the right/bottom/far planes
	 * @param r4 Bottom-Left corner part of the left/bottom/far planes
	 * @param nearDist Distance from uncapped frustum pinacle to the near plane
	 */
	void SetFrustumBox(const decVector &r1, const decVector &r2, const decVector &r3, const decVector &r4, float nearDist);
	/*@}*/
	
	/** @name Intersection Tests */
	/*@{*/
	/**
	 * Determines if the given sphere intersects this frustum.
	 * @return eIntersectType indicating the intersection type
	 */
	eIntersectType IntersectSphere(deoalCollisionSphere *sphere);
	/*@}*/
};


// end of include only once
#endif
