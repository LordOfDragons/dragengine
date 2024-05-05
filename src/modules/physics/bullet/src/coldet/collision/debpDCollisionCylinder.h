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

#ifndef _DEBPDCOLLISIONCYLINDER_H_
#define _DEBPDCOLLISIONCYLINDER_H_

#include "debpDCollisionVolume.h"



/**
 * @brief Cylinder Collision Volume.
 *
 * Defines a collision volumed in the shape of a cylinder.
 * The cylinder is defined by a half height and two radi
 * one for the top circle and one for the bottom circle.
 * The cylinder is oriented along the Y-Axis. A cylinder
 * with different radi is a tapered cylinder while a
 * cylinder with two identical radi is a common cylinder.
 * Common cylinders are usually faster.
 * @warning Partially implemented, see function notes
 */
class debpDCollisionCylinder : public debpDCollisionVolume{
private:
	decDVector pPosition;
	decQuaternion pOrientation;
	double pTopRadius;
	double pBottomRadius;
	double pHalfHeight;
	
	decDVector pAxisX, pAxisY, pAxisZ;
	bool pOriented;
	bool pTapered;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/**
	 * Creates a new unoriented cylinder. The half height is set to 0.5, the top
	 * and bottom radius are set both to 0.5 too.
	 */
	debpDCollisionCylinder();
	/**
	 * Creates a new unoriented cylinder.
	 * @param position Center of the cylinder.
	 * @param halfHeight Half the height of the cylinder. Has to be greater than 0.
	 * @param topRadius Radius of the top cap. Has to be greater than 0.
	 * @param bottomRadius Radius of the bottom cap. Has to be greater than 0.
	 */
	debpDCollisionCylinder( const decDVector &position, double halfHeight, double topRadius, double bottomRadius );
	/**
	 * Creates a new oriented cylinder.
	 * @param position Center of the cylinder.
	 * @param halfHeight Half the height of the cylinder. Has to be greater than 0.
	 * @param topRadius Radius of the top cap. Has to be greater than 0.
	 * @param bottomRadius Radius of the bottom cap. Has to be greater than 0.
	 * @param orientation Orientation of the cylinder as normalized quaternion.
	 */
	debpDCollisionCylinder( const decDVector &position, double halfHeight, double topRadius, double bottomRadius, const decQuaternion &orientation );
	/** Cleans up the collision Cylinder. */
	virtual ~debpDCollisionCylinder();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume( debpDCollisionVolume *volume );
	virtual double VolumeMoveHitsVolume( debpDCollisionVolume *volume, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume( debpDCollisionSphere *sphere );
	virtual bool CylinderHitsVolume( debpDCollisionCylinder *Cylinder );
	virtual bool CapsuleHitsVolume( debpDCollisionCapsule *capsule );
	virtual bool BoxHitsVolume( debpDCollisionBox *box );
	virtual bool TriangleHitsVolume( debpDCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( debpDCollisionFrustum *frustum );
	virtual double SphereMoveHitsVolume( debpDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	virtual double CylinderMoveHitsVolume( debpDCollisionCylinder *Cylinder, const decDVector &displacement, decDVector *normal );
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
	/**
	 * Determines if the given sphere collides with this Cylinder.
	 * @warning Function is not implemented yet and always returns false.
	 */
	bool SphereHitsCylinder( debpDCollisionSphere *sphere );
	/**
	 * Determines if the given Cylinder collides with this Cylinder.
	 * @warning Function is not implemented yet and always returns false.
	 */
	bool CylinderHitsCylinder( debpDCollisionCylinder *Cylinder );
	/**
	 * Determines the distance of the given sphere to move until colliding with this Cylinder.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double SphereMoveHitsCylinder( debpDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given Cylinder to move until colliding with this Cylinder.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CylinderMoveHitsCylinder( debpDCollisionCylinder *Cylinder, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Retrieves the cylinder center.
	 */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/**
	 * Sets the position.
	 * @param position Center of cylinder.
	 */
	void SetPosition( const decDVector &position );
	/** Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	/**
	 * Sets the orientation.
	 * @param orientation Normalized rotation quaternion.
	 */
	void SetOrientation( const decQuaternion &orientation );
	/** Clears the orientation of the cylinder setting it to the identity quaternion. */
	void ClearOrientation();
	/** Retrieves the half height. */
	inline double GetHalfHeight() const{ return pHalfHeight; }
	/**
	 * Sets the half height.
	 * @param halfHeight Half the height of the cylinder. Has to be greater than 0.
	 */
	void SetHalfHeight( double halfHeight );
	/** Retrieves the top radius. */
	inline double GetTopRadius() const{ return pTopRadius; }
	/**
	 * Sets the top radius.
	 * @param radius Radius of the top cylinder cap. Has to be greater than 0.
	 */
	void SetTopRadius( double radius );
	/** Retrieves the bottom radius. */
	inline double GetBottomRadius() const{ return pBottomRadius; }
	/**
	 * Sets the bottom radius.
	 * @param radius Radius of the bottom cylinder cap. Has to be greater than 0.
	 */
	void SetBottomRadius( double radius );
	/**
	 * Sets both radi to the same value.
	 * @param radius Radius of both the top and bottom cap. Has to be greater than 0.
	 */
	void SetRadius( double radius );
	
	/**
	 * Determines if this cylinder is oriented.
	 * @return True if the cylinder has an orientation other than the identity quaternion.
	 */
	inline bool GetOriented() const{ return pOriented; }
	/**
	 * Determines if this cylinder is axis aligned.
	 * @return True if the cylinder has an orientation equal to the identity quaternion.
	 */
	inline bool GetAxisAligned() const{ return ! pOriented; }
	/**
	 * Determines if this cylinder is tapered.
	 * @return True if the top and bottom radius are not identical.
	 */
	inline bool GetTapered() const{ return pTapered; }
	
	/**
	 * Retrieves the X-Axis.
	 * @return Normalized vector along the X-Axis.
	 */
	inline const decDVector &GetAxisX() const{ return pAxisX; }
	/**
	 * Retrieves the Y-Axis.
	 * @return Normalized vector along the Y-Axis.
	 */
	inline const decDVector &GetAxisY() const{ return pAxisY; }
	/**
	 * Retrieves the Z-Axis.
	 * @return Normalized vector along the Z-Axis.
	 */
	inline const decDVector &GetAxisZ() const{ return pAxisZ; }
	
	/** Transforms a point from world coordinates into local coordinates. */
	decDVector WorldToLocal( const decDVector &point ) const;
	/** Transforms a point from local coordinates into world coordinates. */
	decDVector LocalToWorld( const decDVector &point ) const;
	/** Transforms a normal from world coordinates into local coordinates. */
	decDVector NormalWorldToLocal( const decDVector &normal ) const;
	/** Transforms a normal from local coordinates into world coordinates. */
	decDVector NormalLocalToWorld( const decDVector &normal ) const;
	/*@}*/
};


// end of include only once
#endif
