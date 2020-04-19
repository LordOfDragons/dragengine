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

#ifndef _DEOGLDCOLLISIONSPHERE_H_
#define _DEOGLDCOLLISIONSPHERE_H_

#include "deoglDCollisionVolume.h"



/**
 * @brief Sphere collision volume.
 *
 * Defines a collision volume in the shape of a sphere. The
 * sphere is defined by a center point and a radius. The
 * surface of the sphere is considered to be part of the
 * volume.
 */
class deoglDCollisionSphere : public deoglDCollisionVolume{
private:
	decDVector pCenter;
	double pRadius, pSquareRadius;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision sphere with no radius located at the origin. */
	deoglDCollisionSphere();
	/**
	 * Creates a new collision sphere with the given parameters.
	 * @param center Center point of sphere
	 * @param radius Radius of sphere beeing at least 0
	 */
	deoglDCollisionSphere( const decDVector &center, double radius );
	/** Cleans up the collision sphere. */
	~deoglDCollisionSphere();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume( deoglDCollisionVolume *volume );
	virtual double VolumeMoveHitsVolume( deoglDCollisionVolume *volume, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume( deoglDCollisionSphere *sphere );
	virtual bool CylinderHitsVolume( deoglDCollisionCylinder *cylinder );
	virtual bool CapsuleHitsVolume( deoglDCollisionCapsule *capsule );
	virtual bool BoxHitsVolume( deoglDCollisionBox *box );
	virtual bool TriangleHitsVolume( deoglDCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( deoglDCollisionFrustum *frustum );
	virtual double SphereMoveHitsVolume( deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	virtual double CylinderMoveHitsVolume( deoglDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal );
	virtual double CapsuleMoveHitsVolume( deoglDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal );
	virtual double BoxMoveHitsVolume( deoglDCollisionBox *box, const decDVector &displacement, decDVector *normal );
	virtual double TriangleMoveHitsVolume( deoglDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal );
	virtual double FrustumMoveHitsVolume( deoglDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal );
	virtual double PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere( deoglDCollisionSphere *sphere );
	virtual void GetEnclosingBox( deoglDCollisionBox *box );
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
	virtual void Visit( deoglDCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere collides with this sphere. */
	bool SphereHitsSphere( deoglDCollisionSphere *sphere );
	/**
	 * Determines the distance of the given sphere to move until colliding with this sphere.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double SphereMoveHitsSphere( deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
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
