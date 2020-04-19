/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DECDCOLLISIONSPHERE_H_
#define _DECDCOLLISIONSPHERE_H_

#include "decDCollisionVolume.h"



/**
 * @brief Sphere collision volume.
 *
 * Defines a collision volume in the shape of a sphere. The
 * sphere is defined by a center point and a radius. The
 * surface of the sphere is considered to be part of the
 * volume.
 */
class decDCollisionSphere : public decDCollisionVolume{
private:
	decDVector pCenter;
	double pRadius, pSquareRadius;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision sphere with no radius located at the origin. */
	decDCollisionSphere();
	/**
	 * Creates a new collision sphere with the given parameters.
	 * @param center Center point of sphere
	 * @param radius Radius of sphere beeing at least 0
	 */
	decDCollisionSphere( const decDVector &center, double radius );
	/** Cleans up the collision sphere. */
	~decDCollisionSphere();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume( decDCollisionVolume *volume );
	virtual double VolumeMoveHitsVolume( decDCollisionVolume *volume, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume( decDCollisionSphere *sphere );
	virtual bool CylinderHitsVolume( decDCollisionCylinder *cylinder );
	virtual bool CapsuleHitsVolume( decDCollisionCapsule *capsule );
	virtual bool BoxHitsVolume( decDCollisionBox *box );
	virtual bool TriangleHitsVolume( decDCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( decDCollisionFrustum *frustum );
	virtual double SphereMoveHitsVolume( decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	virtual double CylinderMoveHitsVolume( decDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal );
	virtual double CapsuleMoveHitsVolume( decDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal );
	virtual double BoxMoveHitsVolume( decDCollisionBox *box, const decDVector &displacement, decDVector *normal );
	virtual double TriangleMoveHitsVolume( decDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal );
	virtual double FrustumMoveHitsVolume( decDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal );
	virtual double PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere( decDCollisionSphere *sphere );
	virtual void GetEnclosingBox( decDCollisionBox *box );
	/*@}*/
	
	/** @name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside( const decDVector &point );
	/** Retrieves the closest point on the volume. */
	virtual decDVector ClosestPointTo( const decDVector &point );
	
	/**
	 * Retrieves the surface normal through the given point. The point is either right on the
	 * surface or near to it. This function is relaxing in this case and tries to calculate
	 * the best normal pointing from the volume at the point. For certain points a normal
	 * is impossible to determine. The normal in this case is undefined.
	 * @param point Point to determine the normal for.
	 * @return Surface normal at given point.
	 */
	virtual decDVector NormalAtPoint( const decDVector &point );
	/**
	 * Determines if a ray hits the volume.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param hitDistance If the ray hits this will be set to the distance from the ray origin
	 * to the collision point.
	 * @return True if the ray hits the volume.
	 */
	virtual bool RayHitsVolume( const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance );
	/*@}*/
	
	/** @name Visiting */
	/*{*/
	virtual void Visit( decDCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere collides with this sphere. */
	bool SphereHitsSphere( decDCollisionSphere *sphere );
	/**
	 * Determines the distance of the given sphere to move until colliding with this sphere.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double SphereMoveHitsSphere( decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Parameters */
	/*@{*/
	/** Retrieves the center point of the sphere. */
	inline const decDVector &GetCenter() const{ return pCenter; }
	/** Retrieves the radius of the sphere. */
	inline double GetRadius() const{ return pRadius; }
	/** Retrieves the sqaured radius of the sphere. */
	inline double GetSquareRadius() const{ return pSquareRadius; }
	/** Sets the center point of the sphere. */
	void SetCenter( const decDVector &center );
	/**
	 * Sets the radius of the sphere.
	 * @param radius Radius of sphere beeing at least 0
	 */
	void SetRadius( double radius );
	/**
	 * Sets all sphere parameters at once.
	 * @param center Center point of sphere
	 * @param radius Radius of sphere beeing at least 0
	 */
	void SetAll( const decDVector &center, double radius );
	/*@}*/
};


// end of include only once
#endif
