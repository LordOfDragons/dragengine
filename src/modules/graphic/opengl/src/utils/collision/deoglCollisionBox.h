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

#ifndef _DEOGLCOLLISIONBOX_H_
#define _DEOGLCOLLISIONBOX_H_

#include "deoglCollisionVolume.h"



/**
 * Box Collision Volume.
 * Defines a collision volume in the shape of a box. The box is defined
 * by a position, the length of the extends in each direction ( half-size )
 * and an optional orientation. A box without a rotation is called axis
 * aligned box whereas a box with a rotation is called an oriented box.
 * @warning This class is partially implemented. See the collision
 *          functions for more information.
 * @todo
 * - Implementing CylinderHitsBox
 */
class deoglCollisionBox : public deoglCollisionVolume{
private:
	decVector pCenter, pHalfSize;
	decQuaternion pOrientation;
	decVector pAxisX, pAxisY, pAxisZ;
	bool pOriented;
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision box of 0 size. */
	deoglCollisionBox();
	/**
	 * Creates a new collision box with the given parameters.
	 */
	deoglCollisionBox( const decVector &center, const decVector &halfSize );
	/**
	 * Creates a new collision box with the given parameters.
	 */
	deoglCollisionBox( const decVector &center, const decVector &halfSize, const decQuaternion &orientation );
	/** Cleans up the collision box. */
	virtual ~deoglCollisionBox();
	/*@}*/
	
	/** \name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume( deoglCollisionVolume *volume );
	virtual float VolumeMoveHitsVolume( deoglCollisionVolume *volume, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** \name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume( deoglCollisionSphere *sphere );
	virtual bool CylinderHitsVolume( deoglCollisionCylinder *cylinder );
	virtual bool CapsuleHitsVolume( deoglCollisionCapsule *capsule );
	virtual bool BoxHitsVolume( deoglCollisionBox *box );
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
	
	/** \name Visiting */
	/*{*/
	virtual void Visit( deoglCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** \name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this box. */
	bool SphereHitsBox( deoglCollisionSphere *sphere );
	/**
	 * Determines if the given cylinder hits this box.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CylinderHitsBox( deoglCollisionCylinder *cylinder );
	/** Determines if the given capsule hits this box. */
	bool CapsuleHitsBox( deoglCollisionCapsule *capsule );
	/** Determines if the given box hits this box. */
	bool BoxHitsBox( deoglCollisionBox *box );
	
	/**
	 * Determines the distance of the given sphere to move until colliding with this box.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float SphereMoveHitsBox( deoglCollisionSphere *sphere, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given cylinder to move until colliding with this box.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CylinderMoveHitsBox( deoglCollisionCylinder *cylinder, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given capsule to move until colliding with this box.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CapsuleMoveHitsBox( deoglCollisionCapsule *capsule, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given box to move until colliding with this box.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float BoxMoveHitsBox( deoglCollisionBox *box, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** \name Parameters */
	/*@{*/
	/** Retrieves the center. */
	inline const decVector &GetCenter() const{ return pCenter; }
	/** Retrieves the half sizes. */
	inline const decVector &GetHalfSize() const{ return pHalfSize; }
	/** Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	/** Determines if this box is axis aligned. */
	inline bool GetAxisAligned() const{ return ! pOriented; }
	/** Determines if this box is oriented. */
	inline bool GetOriented() const{ return pOriented; }
	/** Retrieves the x axis. */
	inline const decVector &GetAxisX() const{ return pAxisX; }
	/** Retrieves the y axis. */
	inline const decVector &GetAxisY() const{ return pAxisY; }
	/** Retrieves the z axis. */
	inline const decVector &GetAxisZ() const{ return pAxisZ; }
	/** Sets the center. */
	void SetCenter( const decVector &center );
	/** Sets the half sizes. All components have to be at least 0. */
	void SetHalfSize( const decVector &halfSize );
	/** Sets the orientation. */
	void SetOrientation( const decQuaternion &orientation );
	/** Clears the rotation and turns the box into an axis aligned box. */
	void ClearOrientation();
	/**
	 * Sets the box parameters from the given extends. The extends
	 * define the minimal and maximal values on the approriate axis
	 * the box has to spawn. The resulting box is axis aligned.
	 */
	void SetFromExtends( const decVector &minExtend, const decVector &maxExtend );
	/** Moves the center of the box by the given offset. */
	void MoveBy( const decVector &offset );
	/** Projects the extends of the box to the given axis. */
	inline float ProjectExtends( const decVector &axis ) const{
		return fabsf( axis * pAxisX ) * pHalfSize.x + fabsf( axis * pAxisY ) * pHalfSize.y + fabsf( axis * pAxisZ ) * pHalfSize.z;
	}
	/*@}*/

	decVector WorldToLocal( const decVector &point ) const;
	decVector LocalToWorld( const decVector &point ) const;
	decVector NormalWorldToLocal( const decVector &normal ) const;
	decVector NormalLocalToWorld( const decVector &normal ) const;
private:
	decVector pAxisAlignedCPTo( const decVector &point );
};


// end of include only once
#endif
