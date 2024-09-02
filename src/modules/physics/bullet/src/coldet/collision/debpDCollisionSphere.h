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

#ifndef _DEBPDCOLLISIONSPHERE_H_
#define _DEBPDCOLLISIONSPHERE_H_

#include "debpDCollisionVolume.h"



/**
 * @brief Sphere collision volume.
 *
 * Defines a collision volume in the shape of a sphere. The
 * sphere is defined by a center point and a radius. The
 * surface of the sphere is considered to be part of the
 * volume.
 */
class debpDCollisionSphere : public debpDCollisionVolume{
private:
	decDVector pCenter;
	double pRadius, pSquareRadius;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision sphere with no radius located at the origin. */
	debpDCollisionSphere();
	/**
	 * Creates a new collision sphere with the given parameters.
	 * @param center Center point of sphere
	 * @param radius Radius of sphere beeing at least 0
	 */
	debpDCollisionSphere( const decDVector &center, double radius );
	/** Cleans up the collision sphere. */
	~debpDCollisionSphere();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume( debpDCollisionVolume *volume );
	virtual double VolumeMoveHitsVolume( debpDCollisionVolume *volume, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume( debpDCollisionSphere *sphere );
	virtual bool CylinderHitsVolume( debpDCollisionCylinder *cylinder );
	virtual bool CapsuleHitsVolume( debpDCollisionCapsule *capsule );
	virtual bool BoxHitsVolume( debpDCollisionBox *box );
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
	virtual void Visit( debpDCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere collides with this sphere. */
	bool SphereHitsSphere( debpDCollisionSphere *sphere );
	/**
	 * Determines the distance of the given sphere to move until colliding with this sphere.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double SphereMoveHitsSphere( debpDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
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
