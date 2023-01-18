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

#ifndef _DEOGLDCOLLISIONVOLUME_H_
#define _DEOGLDCOLLISIONVOLUME_H_

#include <dragengine/common/math/decMath.h>

class deoglDCollisionSphere;
class deoglDCollisionCylinder;
class deoglDCollisionCapsule;
class deoglDCollisionBox;
class deoglDCollisionTriangle;
class deoglDCollisionFrustum;
class deoglDCollisionVolumeVisitor;



/**
 * @brief Base class for collision volumes.
 *
 * The collision detection system uses a double dispatch system
 * to determine collisions between various volumes. The first
 * dispatch call is VolumeHitsVolume. Each subclass has to call
 * a left side specialized function on either the caller or
 * itself to trigger the second dispatch. Left side functions
 * are those of the shape  *HitsVolume. Such a left side function
 * then has to call the approriate right side function which is
 * a function of the shape *Hits*. Collision detection is then
 * carried out by such a right side function. A collision check
 * requires though at most 2 dispatch calls in addition to the
 * execution time of the real collision routine.
 *
 * Although the name collision volumes suggests only volumes to
 * be used in collision detection this is not fully true. This
 * collision system also allows a ray as beeing a volume. Also
 * a triangle is strictly not a volume as it is infitesimal
 * small in one direction but is considered a collision volume.
 *
 * @note The Ray as a collision class doesn't really fit in the
 * big picture. A ray is an infinite object and not even a
 * volume. A solution would be to take out the ray class and
 * simply add a collision test call to the collision volume
 * class. This way the important functionality is conserved but
 * the troublesome ray class vanishes.
 */
class deoglDCollisionVolume{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision volume. */
	deoglDCollisionVolume();
	/** Cleans up the collision volume. */
	virtual ~deoglDCollisionVolume();
	/*@}*/
	
	/** \name First Stage Dispatch */
	/*@{*/
	/**
	 * Determines if this volume collides with the given one. The
	 * subclass has to dispatch to a second stage function here.
	 */
	virtual bool VolumeHitsVolume( deoglDCollisionVolume *volume ) = 0;
	/**
	 * Retrieves the distance travelled along a given displacement
	 * vector until this volume hits another one. The returned value
	 * is in the range of 0 to 1. If no collision happens 1 is returned.
	 * In case of collision 'normal' has to be filled in with a normalized
	 * vector designating the plane along which this volume can be
	 * moved without colliding the given volume. Set 'normal' to NULL if
	 * you are not interested in this information.
	 * The subclass has to dispatch to a second stage function here.
	 */
	virtual double VolumeMoveHitsVolume( deoglDCollisionVolume *volume, const decDVector &displacement, decDVector *normal ) = 0;
	/*@}*/
	
	/** \name Second Stage Dispatch */
	/*@{*/
	/**
	 * Determines if the given sphere collides with this volume. The
	 * subclass has to dispatch to a collision function here.
	 */
	virtual bool SphereHitsVolume( deoglDCollisionSphere *sphere ) = 0;
	/**
	 * Determines if the given cylinder collides with this volume. The
	 * subclass has to dispatch to a collision function here.
	 */
	virtual bool CylinderHitsVolume( deoglDCollisionCylinder *cylinder ) = 0;
	/**
	 * Determines if the given capsule collides with this volume. The
	 * subclass has to dispatch to a collision function here.
	 */
	virtual bool CapsuleHitsVolume( deoglDCollisionCapsule *capsule ) = 0;
	/**
	 * Determines if the given box collides with this volume. The
	 * subclass has to dispatch to a collision function here.
	 */
	virtual bool BoxHitsVolume( deoglDCollisionBox *box ) = 0;
	/**
	 * Determines if the given triangle collides with this volume. The
	 * subclass has to dispatch to a collision function here.
	 */
	virtual bool TriangleHitsVolume( deoglDCollisionTriangle *triangle ) = 0;
	/**
	 * Determines if the given frustum collides with this volume. The
	 * subclass has to dispatch to a collision function here.
	 */
	virtual bool FrustumHitsVolume( deoglDCollisionFrustum *frustum ) = 0;
	
	/**
	 * Retrieves the distance travelled along a given displacement
	 * vector until this volume hits a sphere. The returned value
	 * is in the range of 0 to 1. If no collision happens 1 is returned.
	 * In case of collision 'normal' has to be filled in with a normalized
	 * vector designating the plane along which this volume can be
	 * moved without colliding the given volume. Set 'normal' to NULL if
	 * you are not interested in this information.
	 * The subclass has to dispatch to a collision function here.
	 */
	virtual double SphereMoveHitsVolume( deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ) = 0;
	/**
	 * Retrieves the distance travelled along a given displacement
	 * vector until this volume hits a cylinder. The returned value
	 * is in the range of 0 to 1. If no collision happens 1 is returned.
	 * In case of collision 'normal' has to be filled in with a normalized
	 * vector designating the plane along which this volume can be
	 * moved without colliding the given volume. Set 'normal' to NULL if
	 * you are not interested in this information.
	 * The subclass has to dispatch to a collision function here.
	 */
	virtual double CylinderMoveHitsVolume( deoglDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal ) = 0;
	/**
	 * Retrieves the distance travelled along a given displacement
	 * vector until this volume hits a capsule. The returned value
	 * is in the range of 0 to 1. If no collision happens 1 is returned.
	 * In case of collision 'normal' has to be filled in with a normalized
	 * vector designating the plane along which this volume can be
	 * moved without colliding the given volume. Set 'normal' to NULL if
	 * you are not interested in this information.
	 * The subclass has to dispatch to a collision function here.
	 */
	virtual double CapsuleMoveHitsVolume( deoglDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal ) = 0;
	/**
	 * Retrieves the distance travelled along a given displacement
	 * vector until this volume hits a box. The returned value
	 * is in the range of 0 to 1. If no collision happens 1 is returned.
	 * In case of collision 'normal' has to be filled in with a normalized
	 * vector designating the plane along which this volume can be
	 * moved without colliding the given volume. Set 'normal' to NULL if
	 * you are not interested in this information.
	 * The subclass has to dispatch to a collision function here.
	 */
	virtual double BoxMoveHitsVolume( deoglDCollisionBox *box, const decDVector &displacement, decDVector *normal ) = 0;
	/**
	 * Retrieves the distance travelled along a given displacement
	 * vector until this volume hits a triangle. The returned value
	 * is in the range of 0 to 1. If no collision happens 1 is returned.
	 * In case of collision 'normal' has to be filled in with a normalized
	 * vector designating the plane along which this volume can be
	 * moved without colliding the given volume. Set 'normal' to NULL if
	 * you are not interested in this information.
	 * The subclass has to dispatch to a collision function here.
	 */
	virtual double TriangleMoveHitsVolume( deoglDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal ) = 0;
	/**
	 * Retrieves the distance travelled along a given displacement
	 * vector until this volume hits a frustum. The returned value
	 * is in the range of 0 to 1. If no collision happens 1 is returned.
	 * In case of collision 'normal' has to be filled in with a normalized
	 * vector designating the plane along which this volume can be
	 * moved without colliding the given volume. Set 'normal' to NULL if
	 * you are not interested in this information.
	 * The subclass has to dispatch to a collision function here.
	 */
	virtual double FrustumMoveHitsVolume( deoglDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal ) = 0;

	/**
	 * Retrieves the distance travelled along a given displacement
	 * vector until the given point hits this volume. The returned value
	 * is in the range of 0 to 1. If no collision happens 1 is returned.
	 * In case of collision 'normal' has to be filled in with a normalized
	 * vector designating the plane along which this volume can be
	 * moved without colliding the given volume. Set 'normal' to NULL if
	 * you are not interested in this information.
	 * The subclass has to dispatch to a collision function here.
	 */
	virtual double PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal ) = 0;
	/*@}*/
	
	/** \name Enclosing Volumes */
	/*@{*/
	/**
	 * Retrieves the sphere volume best enclosing this volume.
	 */
	virtual void GetEnclosingSphere( deoglDCollisionSphere *sphere ) = 0;
	/**
	 * Retrieves the box volume best enclosing this volume.
	 */
	virtual void GetEnclosingBox( deoglDCollisionBox *box ) = 0;
	/*@}*/

	/** \name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside( const decDVector &point ) = 0;
	/** Retrieves the closest point on the volume. */
	virtual decDVector ClosestPointTo( const decDVector &point ) = 0;
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
	
	/** \name Visiting */
	/*{*/
	/** Visits the collision volume. */
	virtual void Visit( deoglDCollisionVolumeVisitor *visitor ) = 0;
	/*}*/
};


// end of include only once
#endif
