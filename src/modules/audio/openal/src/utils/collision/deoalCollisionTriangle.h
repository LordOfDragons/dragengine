/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALCOLLISIONTRIANGLE_H_
#define _DEOALCOLLISIONTRIANGLE_H_

#include "deoalCollisionVolume.h"



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
class deoalCollisionTriangle : public deoalCollisionVolume{
private:
	decVector pCorners[ 3 ], pEdges[ 3 ];
	decVector pNormal;
	float pDist;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision triangle with all points 0. */
	deoalCollisionTriangle();
	/** Creates a new collision triangle from the given parameters. */
	deoalCollisionTriangle( const decVector &corner1, const decVector &corner2, const decVector &corner3 );
	/**
	 * Creates a new collision triangle from the given parameters.
	 * Here you can also specify the normal of the triangle. Do not use
	 * this function anymore as it depracted and will be removed soon.
	 */
	deoalCollisionTriangle( const decVector &corner1, const decVector &corner2, const decVector &corner3, const decVector &normal );
	/** Cleans up the collision triangle. */
	virtual ~deoalCollisionTriangle();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume( deoalCollisionVolume *volume );
	virtual float VolumeMoveHitsVolume( deoalCollisionVolume *volume, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume( deoalCollisionSphere *sphere );
	virtual bool CylinderHitsVolume( deoalCollisionCylinder *cylinder );
	virtual bool CapsuleHitsVolume( deoalCollisionCapsule *capsule );
	virtual bool BoxHitsVolume( deoalCollisionBox *box );
	virtual bool TriangleHitsVolume( deoalCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( deoalCollisionFrustum *frustum );
	virtual float SphereMoveHitsVolume( deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal );
	virtual float CylinderMoveHitsVolume( deoalCollisionCylinder *cylinder, const decVector &displacement, decVector *normal );
	virtual float CapsuleMoveHitsVolume( deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal );
	virtual float BoxMoveHitsVolume( deoalCollisionBox *box, const decVector &displacement, decVector *normal );
	virtual float TriangleMoveHitsVolume( deoalCollisionTriangle *triangle, const decVector &displacement, decVector *normal );
	virtual float FrustumMoveHitsVolume( deoalCollisionFrustum *frustum, const decVector &displacement, decVector *normal );
	virtual float PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal );
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere( deoalCollisionSphere *sphere );
	virtual void GetEnclosingBox( deoalCollisionBox *box );
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
	virtual void Visit( deoalCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this triangle. */
	bool SphereHitsTriangle( deoalCollisionSphere *sphere );
	/**
	 * Determines if the given cylinder hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CylinderHitsTriangle( deoalCollisionCylinder *cylinder );
	/**
	 * Determines if the given capsule hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CapsuleHitsTriangle( deoalCollisionCapsule *capsule );
	/** Determines if the given box hits this triangle. */
	bool BoxHitsTriangle( deoalCollisionBox *box );
	/**
	 * Determines if the given triangle hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool TriangleHitsTriangle( deoalCollisionTriangle *triangle );
	
	/** Determines the distance of the given sphere to move until colliding with this triangle. */
	float SphereMoveHitsTriangle( deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given cylinder to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CylinderMoveHitsTriangle( deoalCollisionCylinder *cylinder, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given capsule to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CapsuleMoveHitsTriangle( deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal );
	/** Determines the distance of the given box to move until colliding with this triangle. */
	float BoxMoveHitsTriangle( deoalCollisionBox *box, const decVector &displacement, decVector *normal );
	/**
	 * Determines the distance of the given triangle to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float TriangleMoveHitsTriangle( deoalCollisionTriangle *triangle, const decVector &displacement, decVector *normal );
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
