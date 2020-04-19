/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALCOLLISIONSPHERE_H_
#define _DEOALCOLLISIONSPHERE_H_

#include "deoalCollisionVolume.h"



/**
 * @brief Sphere collision volume.
 * Defines a collision volume in the shape of a sphere. The
 * sphere is defined by a center point and a radius. The
 * surface of the sphere is considered to be part of the
 * volume.
 */
class deoalCollisionSphere : public deoalCollisionVolume{
private:
	decVector pCenter;
	float pRadius, pSquareRadius;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision sphere with no radius located at the origin. */
	deoalCollisionSphere();
	/**
	 * Creates a new collision sphere with the given parameters.
	 * @param center Center point of sphere
	 * @param radius Radius of sphere beeing at least 0
	 */
	deoalCollisionSphere( const decVector &center, float radius );
	/** Cleans up the collision sphere. */
	~deoalCollisionSphere();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume( deoalCollisionVolume *volume );
	virtual float VolumeMoveHitsVolume( deoalCollisionVolume *volume, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume( deoalCollisionSphere *sphere );
	virtual bool CylinderHitsVolume( deoalCollisionCylinder *cylinder );
	virtual bool CapsuleHitsVolume( deoalCollisionCapsule *capsule );
	virtual bool BoxHitsVolume( deoalCollisionBox *box );
	virtual bool TriangleHitsVolume( deoalCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( deoalCollisionFrustum *frustum );
	virtual float SphereMoveHitsVolume( deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal );
	virtual float CylinderMoveHitsVolume( deoalCollisionCylinder *cylinder, const decVector &displacement, decVector *normal );
	virtual float CapsuleMoveHitsVolume( deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal );
	virtual float BoxMoveHitsVolume( deoalCollisionBox *box, const decVector &displacement, decVector *normal );
	virtual float TriangleMoveHitsVolume( deoalCollisionTriangle *triangle, const decVector &displacement, decVector *normal );
	virtual float FrustumMoveHitsVolume( deoalCollisionFrustum *frustum, const decVector &displacement, decVector *normal );
	virtual float PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere( deoalCollisionSphere *sphere );
	virtual void GetEnclosingBox( deoalCollisionBox *box );
	/*@}*/
	
	/** @name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside( const decVector &point );
	/** Retrieves the closest point on the volume. */
	virtual decVector ClosestPointTo( const decVector &point );
	
	/**
	 * Retrieves the surface normal through the given point. The point is either right on the
	 * surface or near to it. This function is relaxing in this case and tries to calculate
	 * the best normal pointing from the volume at the point. For certain points a normal
	 * is impossible to determine. The normal in this case is undefined.
	 * @param point Point to determine the normal for.
	 * @return Surface normal at given point.
	 */
	virtual decVector NormalAtPoint( const decVector &point );
	/**
	 * Determines if a ray hits the volume.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param hitDistance If the ray hits this will be set to the distance from the ray origin
	 * to the collision point.
	 * @return True if the ray hits the volume.
	 */
	virtual bool RayHitsVolume( const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance );
	/*@}*/
	
	/** @name Visiting */
	/*{*/
	virtual void Visit( deoalCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere collides with this sphere. */
	bool SphereHitsSphere( deoalCollisionSphere *sphere );
	/**
	 * Determines the distance of the given sphere to move until colliding with this sphere.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float SphereMoveHitsSphere( deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** @name Parameters */
	/*@{*/
	/** Retrieves the center point of the sphere. */
	inline const decVector &GetCenter() const{ return pCenter; }
	/** Retrieves the radius of the sphere. */
	inline float GetRadius() const{ return pRadius; }
	/** Retrieves the sqaured radius of the sphere. */
	inline float GetSquareRadius() const{ return pSquareRadius; }
	/** Sets the center point of the sphere. */
	void SetCenter( const decVector &center );
	/**
	 * Sets the radius of the sphere.
	 * @param radius Radius of sphere beeing at least 0
	 */
	void SetRadius( float radius );
	/**
	 * Sets all sphere parameters at once.
	 * @param center Center point of sphere
	 * @param radius Radius of sphere beeing at least 0
	 */
	void SetAll( const decVector &center, float radius );
	/*@}*/
};


// end of include only once
#endif
