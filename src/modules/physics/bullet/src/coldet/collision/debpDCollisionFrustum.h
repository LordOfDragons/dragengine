/* 
 * Drag[en]gine Bullet Physics Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEBPDCOLLISIONFRUSTUM_H_
#define _DEBPDCOLLISIONFRUSTUM_H_

#include "debpDCollisionVolume.h"



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
 *          functions for more informations.
 * @todo
 * - Implementing CylinderHitsFrustum
 * - Implementing CapsuleHitsFrustum
 * - Implementing TriangleHitsFrustum
 * - Implementing FrustumHitsFrustum
 */
class debpDCollisionFrustum : public debpDCollisionVolume{
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
	debpDCollisionFrustum();
	/** Cleans up the collision frustum. */
	virtual ~debpDCollisionFrustum();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume(debpDCollisionVolume *volume);
	virtual double VolumeMoveHitsVolume( debpDCollisionVolume *volume, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume(debpDCollisionSphere *sphere);
	virtual bool CylinderHitsVolume(debpDCollisionCylinder *cylinder);
	virtual bool CapsuleHitsVolume(debpDCollisionCapsule *capsule);
	virtual bool BoxHitsVolume(debpDCollisionBox *box);
	virtual bool TriangleHitsVolume( debpDCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( debpDCollisionFrustum *frustum );
	virtual double SphereMoveHitsVolume( debpDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	virtual double CylinderMoveHitsVolume( debpDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal );
	virtual double CapsuleMoveHitsVolume( debpDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal );
	virtual double BoxMoveHitsVolume( debpDCollisionBox *box, const decDVector &displacement, decDVector *normal );
	virtual double TriangleMoveHitsVolume( debpDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal );
	virtual double FrustumMoveHitsVolume( debpDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal );
	virtual double PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere( debpDCollisionSphere *sphere );
	virtual void GetEnclosingBox( debpDCollisionBox *box );
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
	virtual void Visit( debpDCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this frustum. */
	bool SphereHitsFrustum(debpDCollisionSphere *sphere);
	/**
	 * Determines if the given cylinder hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool CylinderHitsFrustum(debpDCollisionCylinder *cylinder);
	/**
	 * Determines if the given capsule hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool CapsuleHitsFrustum(debpDCollisionCapsule *capsule);
	/** Determines if the given box hits this frustum. */
	bool BoxHitsFrustum(debpDCollisionBox *box);
	/**
	 * Determines if the given triangle hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool TriangleHitsFrustum( debpDCollisionTriangle *triangle );
	/**
	 * Determines if the given frustum hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool FrustumHitsFrustum( debpDCollisionFrustum *frustum );
	
	/**
	 * Determines the distance of the given sphere to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double SphereMoveHitsFrustum( debpDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given cylinder to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CylinderMoveHitsFrustum( debpDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given capsule to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CapsuleMoveHitsFrustum( debpDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given box to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double BoxMoveHitsFrustum( debpDCollisionBox *box, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given triangle to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double TriangleMoveHitsFrustum( debpDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given frustum to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double FrustumMoveHitsFrustum( debpDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal );
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
	eIntersectType IntersectSphere(debpDCollisionSphere *sphere);
	/*@}*/
};


// end of include only once
#endif
