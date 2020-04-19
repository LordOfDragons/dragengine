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

#ifndef _DECCOLLISIONTRIANGLE_H_
#define _DECCOLLISIONTRIANGLE_H_

#include "decCollisionVolume.h"



/**
 * @brief Triangle collision volume.
 *
 * Defines a collision volume in the shape of a triangle. The
 * triangle is defined by the three corner points. The points
 * should be oriented in counter clockwise order.
 * @warning This class is partially implemented. See the collision
 *          functions for more informations.
 * @todo
 * - Implementing CylinderHitsTriangle
 * - Implementing TriangleHitsTriangle
 */
class decCollisionTriangle : public decCollisionVolume{
private:
	decVector pCorners[ 3 ], pEdges[ 3 ];
	decVector pNormal;
	float pDist;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision triangle with all points 0. */
	decCollisionTriangle();
	/** Creates a new collision triangle from the given parameters. */
	decCollisionTriangle( const decVector &corner1, const decVector &corner2, const decVector &corner3 );
	/**
	 * Creates a new collision triangle from the given parameters.
	 * Here you can also specify the normal of the triangle. Do not use
	 * this function anymore as it depracted and will be removed soon.
	 */
	decCollisionTriangle( const decVector &corner1, const decVector &corner2, const decVector &corner3, const decVector &normal );
	/** Cleans up the collision triangle. */
	virtual ~decCollisionTriangle();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume( decCollisionVolume *volume );
	virtual float VolumeMoveHitsVolume( decCollisionVolume *volume, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume( decCollisionSphere *sphere );
	virtual bool CylinderHitsVolume( decCollisionCylinder *cylinder );
	virtual bool CapsuleHitsVolume( decCollisionCapsule *capsule );
	virtual bool BoxHitsVolume( decCollisionBox *box );
	virtual bool TriangleHitsVolume( decCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( decCollisionFrustum *frustum );
	virtual float SphereMoveHitsVolume( decCollisionSphere *sphere, const decVector &displacement, decVector *normal );
	virtual float CylinderMoveHitsVolume( decCollisionCylinder *cylinder, const decVector &displacement, decVector *normal );
	virtual float CapsuleMoveHitsVolume( decCollisionCapsule *capsule, const decVector &displacement, decVector *normal );
	virtual float BoxMoveHitsVolume( decCollisionBox *box, const decVector &displacement, decVector *normal );
	virtual float TriangleMoveHitsVolume( decCollisionTriangle *triangle, const decVector &displacement, decVector *normal );
	virtual float FrustumMoveHitsVolume( decCollisionFrustum *frustum, const decVector &displacement, decVector *normal );
	virtual float PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere( decCollisionSphere *sphere );
	virtual void GetEnclosingBox( decCollisionBox *box );
	/*@}*/
	
	/** @name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside( const decVector &point );
	/** Retrieves the closest point on the volume. */
	virtual decVector ClosestPointTo( const decVector &point );
	/*@}*/
	
	/** @name Visiting */
	/*{*/
	virtual void Visit( decCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this triangle. */
	bool SphereHitsTriangle( decCollisionSphere *sphere );
	/**
	 * Determines if the given cylinder hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CylinderHitsTriangle( decCollisionCylinder *cylinder );
	/**
	 * Determines if the given capsule hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CapsuleHitsTriangle( decCollisionCapsule *capsule );
	/** Determines if the given box hits this triangle. */
	bool BoxHitsTriangle( decCollisionBox *box );
	/**
	 * Determines if the given triangle hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool TriangleHitsTriangle( decCollisionTriangle *triangle );
	
	/** Determines the distance of the given sphere to move until colliding with this triangle. */
	float SphereMoveHitsTriangle( decCollisionSphere *sphere, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given cylinder to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CylinderMoveHitsTriangle( decCollisionCylinder *cylinder, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given capsule to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CapsuleMoveHitsTriangle( decCollisionCapsule *capsule, const decVector &displacement, decVector *normal );
	/** Determines the distance of the given box to move until colliding with this triangle. */
	float BoxMoveHitsTriangle( decCollisionBox *box, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given triangle to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float TriangleMoveHitsTriangle( decCollisionTriangle *triangle, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	inline const decVector &GetCorner1() const{ return pCorners[ 0 ]; }
	inline const decVector &GetCorner2() const{ return pCorners[ 1 ]; }
	inline const decVector &GetCorner3() const{ return pCorners[ 2 ]; }
	inline const decVector &GetNormal() const{ return pNormal; }
	/**
	 * Retrieves the precalculated distance value. The distance
	 * is the dot product between the first corner point and
	 * the triangle normal.
	 */
	inline float GetDistance() const{ return pDist; }
	/** Sets the corners of the triangle and calculates the normal and distance. */
	void SetCorners( const decVector &corner1, const decVector &corner2, const decVector &corner3 );
	/**
	 * Sets the corners of the triangle and calculates the distance.
	 * Here you can set the normal yourself. Do not use this function
	 * anymore as it is depracted and will be removed soon.
	 */
	void SetCorners( const decVector &corner1, const decVector &corner2, const decVector &corner3, const decVector &normal );
	/*@}*/
};


// end of include only once
#endif
