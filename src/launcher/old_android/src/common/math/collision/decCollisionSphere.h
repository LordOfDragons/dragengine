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

#ifndef _DECCOLLISIONSPHERE_H_
#define _DECCOLLISIONSPHERE_H_

#include "decCollisionVolume.h"



/**
 * @brief Sphere collision volume.
 * Defines a collision volume in the shape of a sphere. The
 * sphere is defined by a center point and a radius. The
 * surface of the sphere is considered to be part of the
 * volume.
 */
class decCollisionSphere : public decCollisionVolume{
private:
	decVector pCenter;
	float pRadius, pSquareRadius;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision sphere with no radius located at the origin. */
	decCollisionSphere();
	/**
	 * Creates a new collision sphere with the given parameters.
	 * @param center Center point of sphere
	 * @param radius Radius of sphere beeing at least 0
	 */
	decCollisionSphere(const decVector &center, float radius);
	/** Cleans up the collision sphere. */
	~decCollisionSphere();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume(decCollisionVolume *volume);
	virtual float VolumeMoveHitsVolume(decCollisionVolume *volume, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume(decCollisionSphere *sphere);
	virtual bool CylinderHitsVolume(decCollisionCylinder *cylinder);
	virtual bool CapsuleHitsVolume(decCollisionCapsule *capsule);
	virtual bool BoxHitsVolume(decCollisionBox *box);
	virtual bool TriangleHitsVolume(decCollisionTriangle *triangle);
	virtual bool FrustumHitsVolume(decCollisionFrustum *frustum);
	virtual float SphereMoveHitsVolume(decCollisionSphere *sphere, const decVector &displacement, decVector *normal);
	virtual float CylinderMoveHitsVolume(decCollisionCylinder *cylinder, const decVector &displacement, decVector *normal);
	virtual float CapsuleMoveHitsVolume(decCollisionCapsule *capsule, const decVector &displacement, decVector *normal);
	virtual float BoxMoveHitsVolume(decCollisionBox *box, const decVector &displacement, decVector *normal);
	virtual float TriangleMoveHitsVolume(decCollisionTriangle *triangle, const decVector &displacement, decVector *normal);
	virtual float FrustumMoveHitsVolume(decCollisionFrustum *frustum, const decVector &displacement, decVector *normal);
	virtual float PointMoveHitsVolume(const decVector &point, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere(decCollisionSphere *sphere);
	virtual void GetEnclosingBox(decCollisionBox *box);
	/*@}*/
	
	/** @name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside(const decVector &point);
	/** Retrieves the closest point on the volume. */
	virtual decVector ClosestPointTo(const decVector &point);
	
	/**
	 * Retrieves the surface normal through the given point. The point is either right on the
	 * surface or near to it. This function is relaxing in this case and tries to calculate
	 * the best normal pointing from the volume at the point. For certain points a normal
	 * is impossible to determine. The normal in this case is undefined.
	 * @param point Point to determine the normal for.
	 * @return Surface normal at given point.
	 */
	virtual decVector NormalAtPoint(const decVector &point);
	/**
	 * Determines if a ray hits the volume.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param hitDistance If the ray hits this will be set to the distance from the ray origin
	 * to the collision point.
	 * @return True if the ray hits the volume.
	 */
	virtual bool RayHitsVolume(const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance);
	/*@}*/
	
	/** @name Visiting */
	/*{*/
	virtual void Visit(decCollisionVolumeVisitor *visitor);
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere collides with this sphere. */
	bool SphereHitsSphere(decCollisionSphere *sphere);
	/**
	 * Determines the distance of the given sphere to move until colliding with this sphere.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float SphereMoveHitsSphere(decCollisionSphere *sphere, const decVector &displacement, decVector *normal);
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
	void SetCenter(const decVector &center);
	/**
	 * Sets the radius of the sphere.
	 * @param radius Radius of sphere beeing at least 0
	 */
	void SetRadius(float radius);
	/**
	 * Sets all sphere parameters at once.
	 * @param center Center point of sphere
	 * @param radius Radius of sphere beeing at least 0
	 */
	void SetAll(const decVector &center, float radius);
	/*@}*/
};


// end of include only once
#endif
