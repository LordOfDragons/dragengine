/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLCOLLISIONFRUSTUM_H_
#define _DEOGLCOLLISIONFRUSTUM_H_

#include "deoglCollisionVolume.h"



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
class deoglCollisionFrustum : public deoglCollisionVolume{
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creastes a new collision frustum with default parameters. */
	deoglCollisionFrustum();
	/** Cleans up the collision frustum. */
	virtual ~deoglCollisionFrustum();
	/*@}*/
	
	/** \name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume(deoglCollisionVolume *volume);
	virtual float VolumeMoveHitsVolume( deoglCollisionVolume *volume, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** \name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume(deoglCollisionSphere *sphere);
	virtual bool CylinderHitsVolume(deoglCollisionCylinder *cylinder);
	virtual bool CapsuleHitsVolume(deoglCollisionCapsule *capsule);
	virtual bool BoxHitsVolume(deoglCollisionBox *box);
	virtual bool TriangleHitsVolume( deoglCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( deoglCollisionFrustum *frustum );
	virtual float SphereMoveHitsVolume( deoglCollisionSphere *sphere, const decVector &displacement, decVector *normal );
	virtual float CylinderMoveHitsVolume( deoglCollisionCylinder *cylinder, const decVector &displacement, decVector *normal );
	virtual float CapsuleMoveHitsVolume( deoglCollisionCapsule *capsule, const decVector &displacement, decVector *normal );
	virtual float BoxMoveHitsVolume( deoglCollisionBox *box, const decVector &displacement, decVector *normal );
	virtual float TriangleMoveHitsVolume( deoglCollisionTriangle *triangle, const decVector &displacement, decVector *normal );
	virtual float FrustumMoveHitsVolume( deoglCollisionFrustum *frustum, const decVector &displacement, decVector *normal );
	virtual float PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** \name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere( deoglCollisionSphere *sphere );
	virtual void GetEnclosingBox( deoglCollisionBox *box );
	/*@}*/
	
	/** \name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside( const decVector &point );
	/** Retrieves the closest point on the volume. */
	virtual decVector ClosestPointTo( const decVector &point );
	/*@}*/
	
	/** \name Visiting */
	/*{*/
	virtual void Visit( deoglCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** \name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this frustum. */
	bool SphereHitsFrustum(deoglCollisionSphere *sphere);
	/**
	 * Determines if the given cylinder hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool CylinderHitsFrustum(deoglCollisionCylinder *cylinder);
	/**
	 * Determines if the given capsule hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool CapsuleHitsFrustum(deoglCollisionCapsule *capsule);
	/** Determines if the given box hits this frustum. */
	bool BoxHitsFrustum(deoglCollisionBox *box);
	/**
	 * Determines if the given triangle hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool TriangleHitsFrustum( deoglCollisionTriangle *triangle );
	/**
	 * Determines if the given frustum hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool FrustumHitsFrustum( deoglCollisionFrustum *frustum );
	
	/**
	 * Determines the distance of the given sphere to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float SphereMoveHitsFrustum( deoglCollisionSphere *sphere, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given cylinder to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CylinderMoveHitsFrustum( deoglCollisionCylinder *cylinder, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given capsule to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CapsuleMoveHitsFrustum( deoglCollisionCapsule *capsule, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given box to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float BoxMoveHitsFrustum( deoglCollisionBox *box, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given triangle to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float TriangleMoveHitsFrustum( deoglCollisionTriangle *triangle, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given frustum to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float FrustumMoveHitsFrustum( deoglCollisionFrustum *frustum, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** \name Collision Routines */
	/*@{*/
	inline const decVector &GetLeftNormal() const{ return pNormalLeft; }
	inline const decVector &GetRightNormal() const{ return pNormalRight; }
	inline const decVector &GetTopNormal() const{ return pNormalTop; }
	inline const decVector &GetBottomNormal() const{ return pNormalBottom; }
	inline const decVector &GetNearNormal() const{ return pNormalNear; }
	inline const decVector &GetFarNormal() const{ return pNormalFar; }
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
	void SetFrustum(const decMatrix &mat);
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
	
	/** \name Intersection Tests */
	/*@{*/
	/**
	 * Determines if the given sphere intersects this frustum.
	 * @return eIntersectType indicating the intersection type
	 */
	eIntersectType IntersectSphere(deoglCollisionSphere *sphere);
	/*@}*/
};


// end of include only once
#endif
