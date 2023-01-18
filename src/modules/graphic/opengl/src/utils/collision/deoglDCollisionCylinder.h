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

#ifndef _DEOGLDCOLLISIONCYLINDER_H_
#define _DEOGLDCOLLISIONCYLINDER_H_

#include "deoglDCollisionVolume.h"



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
class deoglDCollisionCylinder : public deoglDCollisionVolume{
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
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * Creates a new unoriented cylinder. The half height is set to 0.5, the top
	 * and bottom radius are set both to 0.5 too.
	 */
	deoglDCollisionCylinder();
	/**
	 * Creates a new unoriented cylinder.
	 * @param position Center of the cylinder.
	 * @param halfHeight Half the height of the cylinder. Has to be greater than 0.
	 * @param topRadius Radius of the top cap. Has to be greater than 0.
	 * @param bottomRadius Radius of the bottom cap. Has to be greater than 0.
	 */
	deoglDCollisionCylinder( const decDVector &position, double halfHeight, double topRadius, double bottomRadius );
	/**
	 * Creates a new oriented cylinder.
	 * @param position Center of the cylinder.
	 * @param halfHeight Half the height of the cylinder. Has to be greater than 0.
	 * @param topRadius Radius of the top cap. Has to be greater than 0.
	 * @param bottomRadius Radius of the bottom cap. Has to be greater than 0.
	 * @param orientation Orientation of the cylinder as normalized quaternion.
	 */
	deoglDCollisionCylinder( const decDVector &position, double halfHeight, double topRadius, double bottomRadius, const decQuaternion &orientation );
	/** Cleans up the collision Cylinder. */
	virtual ~deoglDCollisionCylinder();
	/*@}*/
	
	/** \name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume( deoglDCollisionVolume *volume );
	virtual double VolumeMoveHitsVolume( deoglDCollisionVolume *volume, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** \name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume( deoglDCollisionSphere *sphere );
	virtual bool CylinderHitsVolume( deoglDCollisionCylinder *Cylinder );
	virtual bool CapsuleHitsVolume( deoglDCollisionCapsule *capsule );
	virtual bool BoxHitsVolume( deoglDCollisionBox *box );
	virtual bool TriangleHitsVolume( deoglDCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( deoglDCollisionFrustum *frustum );
	virtual double SphereMoveHitsVolume( deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	virtual double CylinderMoveHitsVolume( deoglDCollisionCylinder *Cylinder, const decDVector &displacement, decDVector *normal );
	virtual double CapsuleMoveHitsVolume( deoglDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal );
	virtual double BoxMoveHitsVolume( deoglDCollisionBox *box, const decDVector &displacement, decDVector *normal );
	virtual double TriangleMoveHitsVolume( deoglDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal );
	virtual double FrustumMoveHitsVolume( deoglDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal );
	virtual double PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** \name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere( deoglDCollisionSphere *sphere );
	virtual void GetEnclosingBox( deoglDCollisionBox *box );
	/*@}*/
	
	/** \name Miscelanous Functions */
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
	
	/** \name Visiting */
	/*{*/
	virtual void Visit( deoglDCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** \name Collision Routines */
	/*@{*/
	/**
	 * Determines if the given sphere collides with this Cylinder.
	 * @warning Function is not implemented yet and always returns false.
	 */
	bool SphereHitsCylinder( deoglDCollisionSphere *sphere );
	/**
	 * Determines if the given Cylinder collides with this Cylinder.
	 * @warning Function is not implemented yet and always returns false.
	 */
	bool CylinderHitsCylinder( deoglDCollisionCylinder *Cylinder );
	/**
	 * Determines the distance of the given sphere to move until colliding with this Cylinder.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double SphereMoveHitsCylinder( deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given Cylinder to move until colliding with this Cylinder.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CylinderMoveHitsCylinder( deoglDCollisionCylinder *Cylinder, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** \name Management */
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
