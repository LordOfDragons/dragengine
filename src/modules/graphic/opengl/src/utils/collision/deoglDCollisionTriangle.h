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

#ifndef _DEOGLDCOLLISIONTRIANGLE_H_
#define _DEOGLDCOLLISIONTRIANGLE_H_

#include "deoglDCollisionVolume.h"



/**
 * Triangle collision volume.
 *
 * Defines a collision volume in the shape of a triangle. The
 * triangle is defined by the three corner points. The points
 * should be oriented in counter clockwise order.
 * @warning This class is partially implemented. See the collision
 *          functions for more information.
 * @todo
 * - Implementing CylinderHitsTriangle
 * - Implementing TriangleHitsTriangle
 */
class deoglDCollisionTriangle : public deoglDCollisionVolume{
private:
	decDVector pCorners[ 3 ], pEdges[ 3 ];
	decDVector pNormal;
	double pDist;
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision triangle with all points 0. */
	deoglDCollisionTriangle();
	/** Creates a new collision triangle from the given parameters. */
	deoglDCollisionTriangle( const decDVector &corner1, const decDVector &corner2, const decDVector &corner3 );
	/**
	 * Creates a new collision triangle from the given parameters.
	 * Here you can also specify the normal of the triangle. Do not use
	 * this function anymore as it depracted and will be removed soon.
	 */
	deoglDCollisionTriangle( const decDVector &corner1, const decDVector &corner2, const decDVector &corner3, const decDVector &normal );
	/** Cleans up the collision triangle. */
	virtual ~deoglDCollisionTriangle();
	/*@}*/
	
	/** \name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume( deoglDCollisionVolume *volume );
	virtual double VolumeMoveHitsVolume( deoglDCollisionVolume *volume, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** \name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume( deoglDCollisionSphere *sphere );
	virtual bool CylinderHitsVolume( deoglDCollisionCylinder *cylinder );
	virtual bool CapsuleHitsVolume( deoglDCollisionCapsule *capsule );
	virtual bool BoxHitsVolume( deoglDCollisionBox *box );
	virtual bool TriangleHitsVolume( deoglDCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( deoglDCollisionFrustum *frustum );
	virtual double SphereMoveHitsVolume( deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	virtual double CylinderMoveHitsVolume( deoglDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal );
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
	/*@}*/
	
	/** \name Visiting */
	/*{*/
	virtual void Visit( deoglDCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** \name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this triangle. */
	bool SphereHitsTriangle( deoglDCollisionSphere *sphere );
	/**
	 * Determines if the given cylinder hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CylinderHitsTriangle( deoglDCollisionCylinder *cylinder );
	/**
	 * Determines if the given capsule hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CapsuleHitsTriangle( deoglDCollisionCapsule *capsule );
	/** Determines if the given box hits this triangle. */
	bool BoxHitsTriangle( deoglDCollisionBox *box );
	/**
	 * Determines if the given triangle hits this triangle.
	 * @warning Not implemented yet and returns always false.
	 */
	bool TriangleHitsTriangle( deoglDCollisionTriangle *triangle );
	
	/** Determines the distance of the given sphere to move until colliding with this triangle. */
	double SphereMoveHitsTriangle( deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given cylinder to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CylinderMoveHitsTriangle( deoglDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given capsule to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CapsuleMoveHitsTriangle( deoglDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal );
	/** Determines the distance of the given box to move until colliding with this triangle. */
	double BoxMoveHitsTriangle( deoglDCollisionBox *box, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given triangle to move until colliding with this triangle.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double TriangleMoveHitsTriangle( deoglDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** \name Management */
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
