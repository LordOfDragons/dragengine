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

#ifndef _DEOALDCOLLISIONFRUSTUM_H_
#define _DEOALDCOLLISIONFRUSTUM_H_

#include "deoalDCollisionVolume.h"



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
class deoalDCollisionFrustum : public deoalDCollisionVolume{
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
	decDVector pNormalLeft, pNormalRight;
	decDVector pNormalTop, pNormalBottom;
	decDVector pNormalNear, pNormalFar;
	double pDistLeft, pDistRight;
	double pDistTop, pDistBottom;
	double pDistNear, pDistFar;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creastes a new collision frustum with default parameters. */
	deoalDCollisionFrustum();
	/** Cleans up the collision frustum. */
	virtual ~deoalDCollisionFrustum();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume(deoalDCollisionVolume *volume);
	virtual double VolumeMoveHitsVolume( deoalDCollisionVolume *volume, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume(deoalDCollisionSphere *sphere);
	virtual bool CylinderHitsVolume(deoalDCollisionCylinder *cylinder);
	virtual bool CapsuleHitsVolume(deoalDCollisionCapsule *capsule);
	virtual bool BoxHitsVolume(deoalDCollisionBox *box);
	virtual bool TriangleHitsVolume( deoalDCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( deoalDCollisionFrustum *frustum );
	virtual double SphereMoveHitsVolume( deoalDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	virtual double CylinderMoveHitsVolume( deoalDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal );
	virtual double CapsuleMoveHitsVolume( deoalDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal );
	virtual double BoxMoveHitsVolume( deoalDCollisionBox *box, const decDVector &displacement, decDVector *normal );
	virtual double TriangleMoveHitsVolume( deoalDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal );
	virtual double FrustumMoveHitsVolume( deoalDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal );
	virtual double PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere( deoalDCollisionSphere *sphere );
	virtual void GetEnclosingBox( deoalDCollisionBox *box );
	/*@}*/
	
	/** @name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside( const decDVector &point );
	/** Retrieves the closest point on the volume. */
	virtual decDVector ClosestPointTo( const decDVector &point );
	/*@}*/
	
	/** @name Visiting */
	/*{*/
	virtual void Visit( deoalDCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this frustum. */
	bool SphereHitsFrustum(deoalDCollisionSphere *sphere);
	/**
	 * Determines if the given cylinder hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool CylinderHitsFrustum(deoalDCollisionCylinder *cylinder);
	/**
	 * Determines if the given capsule hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool CapsuleHitsFrustum(deoalDCollisionCapsule *capsule);
	/** Determines if the given box hits this frustum. */
	bool BoxHitsFrustum(deoalDCollisionBox *box);
	/**
	 * Determines if the given triangle hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool TriangleHitsFrustum( deoalDCollisionTriangle *triangle );
	/**
	 * Determines if the given frustum hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool FrustumHitsFrustum( deoalDCollisionFrustum *frustum );
	
	/**
	 * Determines the distance of the given sphere to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double SphereMoveHitsFrustum( deoalDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given cylinder to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CylinderMoveHitsFrustum( deoalDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given capsule to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CapsuleMoveHitsFrustum( deoalDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given box to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double BoxMoveHitsFrustum( deoalDCollisionBox *box, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given triangle to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double TriangleMoveHitsFrustum( deoalDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given frustum to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double FrustumMoveHitsFrustum( deoalDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Collision Routines */
	/*@{*/
	inline decDVector GetLeftNormal() const{ return pNormalLeft; }
	inline decDVector GetRightNormal() const{ return pNormalRight; }
	inline decDVector GetTopNormal() const{ return pNormalTop; }
	inline decDVector GetBottomNormal() const{ return pNormalBottom; }
	inline decDVector GetNearNormal() const{ return pNormalNear; }
	inline decDVector GetFarNormal() const{ return pNormalFar; }
	inline double GetLeftDistance() const{ return pDistLeft; }
	inline double GetRightDistance() const{ return pDistRight; }
	inline double GetTopDistance() const{ return pDistTop; }
	inline double GetBottomDistance() const{ return pDistBottom; }
	inline double GetNearDistance() const{ return pDistNear; }
	inline double GetFarDistance() const{ return pDistFar; }
	void SetLeftPlane(const decDVector &normal, double dist);
	void SetRightPlane(const decDVector &normal, double dist);
	void SetTopPlane(const decDVector &normal, double dist);
	void SetBottomPlane(const decDVector &normal, double dist);
	void SetNearPlane(const decDVector &normal, double dist);
	void SetFarPlane(const decDVector &normal, double dist);
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
	void SetFrustum(const decDVector &origin, const decDVector &r1, const decDVector &r2, const decDVector &r3, const decDVector &r4, double nearDist);
	/**
	 * Sets the frustum from the given parameters. The origin of the
	 * frustum is set to the 0 vector.
	 * @param r1 Top-Left corner part of the left/top/far planes
	 * @param r2 Top-Right corner part of the right/top/far planes
	 * @param r3 Bottom-Right corner part of the right/bottom/far planes
	 * @param r4 Bottom-Left corner part of the left/bottom/far planes
	 * @param nearDist Distance from uncapped frustum pinacle to the near plane
	 */
	void SetFrustumBox(const decDVector &r1, const decDVector &r2, const decDVector &r3, const decDVector &r4, double nearDist);
	/*@}*/
	
	/** @name Intersection Tests */
	/*@{*/
	/**
	 * Determines if the given sphere intersects this frustum.
	 * @return eIntersectType indicating the intersection type
	 */
	eIntersectType IntersectSphere(deoalDCollisionSphere *sphere);
	/*@}*/
};


// end of include only once
#endif
