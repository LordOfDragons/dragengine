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

#ifndef _DECDCOLLISIONTRIANGLE_H_
#define _DECDCOLLISIONTRIANGLE_H_

#include "decDCollisionVolume.h"



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
class decDCollisionTriangle : public decDCollisionVolume{
private:
	decDVector pCorners[ 3 ], pEdges[ 3 ];
	decDVector pNormal;
	double pDist;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision triangle with all points 0. */
	decDCollisionTriangle();
	/** Creates a new collision triangle from the given parameters. */
	decDCollisionTriangle( const decDVector &corner1, const decDVector &corner2, const decDVector &corner3 );
	/**
	 * Creates a new collision triangle from the given parameters.
	 * Here you can also specify the normal of the triangle. Do not use
	 * this function anymore as it depracted and will be removed soon.
	 */
	decDCollisionTriangle( const decDVector &corner1, const decDVector &corner2, const decDVector &corner3, const decDVector &normal );
	/** Cleans up the collision triangle. */
	virtual ~decDCollisionTriangle();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume( decDCollisionVolume *volume );
	virtual double VolumeMoveHitsVolume( decDCollisionVolume *volume, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume( decDCollisionSphere *sphere );
	virtual bool CylinderHitsVolume( decDCollisionCylinder *cylinder );
	virtual bool CapsuleHitsVolume( decDCollisionCapsule *capsule );
	virtual bool BoxHitsVolume( decDCollisionBox *box );
	virtual bool TriangleHitsVolume( decDCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( decDCollisionFrustum *frustum );
	virtual double SphereMoveHitsVolume( decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	virtual double CylinderMoveHitsVolume( decDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal );
	virtual double CapsuleMoveHitsVolume( decDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal );
	virtual double BoxMoveHitsVolume( decDCollisionBox *box, const decDVector &displacement, decDVector *normal );
	virtual double TriangleMoveHitsVolume( decDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal );
	virtual double FrustumMoveHitsVolume( decDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal );
	virtual double PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere( decDCollisionSphere *sphere );
	virtual void GetEnclosingBox( decDCollisionBox *box );
	/*@}*/
	
	/** @name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside( const decDVector &point );
	/** Retrieves the closest point on the volume. */
	virtual decDVector ClosestPointTo( const decDVector &point );
	/*@}*/
	
	/** @name Visiting */
	/*{*/
	virtual void Visit( decDCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this triangle. */
	bool SphereHitsTriangle( decDCollisionSphere *sphere );
	/**
	 * Determines if the given cylinder hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CylinderHitsTriangle( decDCollisionCylinder *cylinder );
	/**
	 * Determines if the given capsule hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CapsuleHitsTriangle( decDCollisionCapsule *capsule );
	/** Determines if the given box hits this triangle. */
	bool BoxHitsTriangle( decDCollisionBox *box );
	/**
	 * Determines if the given triangle hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool TriangleHitsTriangle( decDCollisionTriangle *triangle );
	
	/** Determines the distance of the given sphere to move until colliding with this triangle. */
	double SphereMoveHitsTriangle( decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given cylinder to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CylinderMoveHitsTriangle( decDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given capsule to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CapsuleMoveHitsTriangle( decDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal );
	/** Determines the distance of the given box to move until colliding with this triangle. */
	double BoxMoveHitsTriangle( decDCollisionBox *box, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given triangle to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double TriangleMoveHitsTriangle( decDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	inline const decDVector &GetCorner1() const{ return pCorners[ 0 ]; }
	inline const decDVector &GetCorner2() const{ return pCorners[ 1 ]; }
	inline const decDVector &GetCorner3() const{ return pCorners[ 2 ]; }
	inline const decDVector &GetNormal() const{ return pNormal; }
	/**
	 * Retrieves the precalculated distance value. The distance
	 * is the dot product between the first corner point and
	 * the triangle normal.
	 */
	inline double GetDistance() const{ return pDist; }
	/** Sets the corners of the triangle and calculates the normal and distance. */
	void SetCorners( const decDVector &corner1, const decDVector &corner2, const decDVector &corner3 );
	/**
	 * Sets the corners of the triangle and calculates the distance.
	 * Here you can set the normal yourself. Do not use this function
	 * anymore as it is depracted and will be removed soon.
	 */
	void SetCorners( const decDVector &corner1, const decDVector &corner2, const decDVector &corner3, const decDVector &normal );
	/*@}*/
};


// end of include only once
#endif
