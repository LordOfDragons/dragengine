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

#ifndef _DECDCOLLISIONVOLUME_H_
#define _DECDCOLLISIONVOLUME_H_

#include "../decMath.h"

class decDCollisionSphere;
class decDCollisionCylinder;
class decDCollisionCapsule;
class decDCollisionBox;
class decDCollisionTriangle;
class decDCollisionFrustum;
class decDCollisionVolumeVisitor;



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
class decDCollisionVolume{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision volume. */
	decDCollisionVolume();
	/** Cleans up the collision volume. */
	virtual ~decDCollisionVolume();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	/**
	 * Determines if this volume collides with the given one. The
	 * subclass has to dispatch to a second stage function here.
	 */
	virtual bool VolumeHitsVolume(decDCollisionVolume *volume) = 0;
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
	virtual double VolumeMoveHitsVolume(decDCollisionVolume *volume, const decDVector &displacement, decDVector *normal) = 0;
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	/**
	 * Determines if the given sphere collides with this volume. The
	 * subclass has to dispatch to a collision function here.
	 */
	virtual bool SphereHitsVolume(decDCollisionSphere *sphere) = 0;
	/**
	 * Determines if the given cylinder collides with this volume. The
	 * subclass has to dispatch to a collision function here.
	 */
	virtual bool CylinderHitsVolume(decDCollisionCylinder *cylinder) = 0;
	/**
	 * Determines if the given capsule collides with this volume. The
	 * subclass has to dispatch to a collision function here.
	 */
	virtual bool CapsuleHitsVolume(decDCollisionCapsule *capsule) = 0;
	/**
	 * Determines if the given box collides with this volume. The
	 * subclass has to dispatch to a collision function here.
	 */
	virtual bool BoxHitsVolume(decDCollisionBox *box) = 0;
	/**
	 * Determines if the given triangle collides with this volume. The
	 * subclass has to dispatch to a collision function here.
	 */
	virtual bool TriangleHitsVolume(decDCollisionTriangle *triangle) = 0;
	/**
	 * Determines if the given frustum collides with this volume. The
	 * subclass has to dispatch to a collision function here.
	 */
	virtual bool FrustumHitsVolume(decDCollisionFrustum *frustum) = 0;
	
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
	virtual double SphereMoveHitsVolume(decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal) = 0;
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
	virtual double CylinderMoveHitsVolume(decDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal) = 0;
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
	virtual double CapsuleMoveHitsVolume(decDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal) = 0;
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
	virtual double BoxMoveHitsVolume(decDCollisionBox *box, const decDVector &displacement, decDVector *normal) = 0;
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
	virtual double TriangleMoveHitsVolume(decDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal) = 0;
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
	virtual double FrustumMoveHitsVolume(decDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal) = 0;

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
	virtual double PointMoveHitsVolume(const decDVector &point, const decDVector &displacement, decDVector *normal) = 0;
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	/**
	 * Retrieves the sphere volume best enclosing this volume.
	 */
	virtual void GetEnclosingSphere(decDCollisionSphere *sphere) = 0;
	/**
	 * Retrieves the box volume best enclosing this volume.
	 */
	virtual void GetEnclosingBox(decDCollisionBox *box) = 0;
	/*@}*/

	/** @name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside(const decDVector &point) = 0;
	/** Retrieves the closest point on the volume. */
	virtual decDVector ClosestPointTo(const decDVector &point) = 0;
	/**
	 * Retrieves the surface normal through the given point. The point is either right on the
	 * surface or near to it. This function is relaxing in this case and tries to calculate
	 * the best normal pointing from the volume at the point. For certain points a normal
	 * is impossible to determine. The normal in this case is undefined.
	 * @param point Point to determine the normal for.
	 * @return Surface normal at given point.
	 */
	virtual decDVector NormalAtPoint(const decDVector &point);
	/**
	 * Determines if a ray hits the volume.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param hitDistance If the ray hits this will be set to the distance from the ray origin
	 * to the collision point.
	 * @return True if the ray hits the volume.
	 */
	virtual bool RayHitsVolume(const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance);
	/*@}*/
	
	/** @name Visiting */
	/*{*/
	/** Visits the collision volume. */
	virtual void Visit(decDCollisionVolumeVisitor *visitor) = 0;
	/*}*/
};


// end of include only once
#endif
