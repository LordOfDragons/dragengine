/* 
 * Drag[en]gine Bullet Physics Module
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

// include only once
#ifndef _DEBPHEIGHTTERRAINSHAPE_H_
#define _DEBPHEIGHTTERRAINSHAPE_H_

// includes
#include "dragengine/common/math/decMath.h"
#include "BulletCollision/CollisionShapes/btConcaveShape.h"

// predefinitions
class debpHTSector;



/**
 * @brief Height Terrain Bullet Shape.
 *
 * Provides a height terrain bullet shape. This class is a reworked version
 * of the bullet btHeightfieldTerrainShape class. The original class provides
 * already height terrain support but has a couple of problems which is why
 * this class has been created to be used instead. On one side the original
 * class uses a variable array of height data using a bullet internal system.
 * There is already a system for handling these values in the bullet module
 * where it makes more sense to modify the class to adapt instead of rewriting
 * the data to a special memory location. Furthermore the original class does
 * not support visibility of faces which this class takes into account. In
 * general a specific implementation is used to avoid possible speed loss due
 * to wrapping the data to fit the original class.
 */
class debpHeightTerrainShape : public btConcaveShape{
private:
	debpHTSector *pSector;
	
	int pPointCount;
	
	btScalar pOffset;
	btScalar pInvScale;
	btScalar pStep;
	
	int pClusterCountX;
	int pClusterCountZ;
	btScalar pClusterInvScaleX;
	btScalar pClusterInvScaleZ;
	
	btVector3 pLocalScaling;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new bullet shape. */
	debpHeightTerrainShape( debpHTSector *sector );
	/** Cleans up the bullet shape. */
	virtual ~debpHeightTerrainShape();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * getAabb returns the axis aligned bounding box in the coordinate frame of the given transform t.
	 */
	virtual void getAabb( const btTransform &trans, btVector3 &aabbMin, btVector3 &aabbMax ) const;
	
	/**
	 * \brief Process all triangles for ray casting.
	 * \details This call is more intelligent version of processAllTriangles able to
	 *          optimize which groups of triangles are tested.
	 */
	virtual void processRaycastAllTriangles( btTriangleRaycastCallback *callback,
	const btVector3 &raySource, const btVector3 &rayTarget );
	
	/**
	 * \brief Process all triangles for convex shape casting.
	 * \details This call is more intelligent version of processAllTriangles able to
	 *          optimize which groups of triangles are tested.
	 */
	virtual void processConvexcastAllTriangles( btTriangleConvexcastCallback *callback,
	const btVector3 &boxSource, const btVector3 &boxTarget,
	const btVector3 &boxMin, const btVector3 &boxMax );
	
	/** \brief Processes all triangles which. */
	virtual void processAllTriangles( btTriangleCallback *callback, const btVector3 &aabbMin, const btVector3 &aabbMax ) const;
	
	/** \brief Process all triangles inside a box area. */
	void ProcessAllTrianglesIn( btTriangleCallback &callback, int fromX, int toX, int fromZ, int toZ ) const;
	
	/** \brief Calculates the local inertia. */
	virtual void calculateLocalInertia( btScalar mass, btVector3 &inertia ) const;
	/** \brief Sets the local scaling. Empty since the scaling is always 1. */
	virtual void setLocalScaling( const btVector3 &scaling );
	/** \brief Retrieves the local scaling. Always 1. */
	virtual const btVector3 &getLocalScaling() const;
	/** \brief Retrieves the debugging name. */
	virtual const char *getName() const;
	
	/** \brief Calculate the point indices for a box. */
	void CalculateBoxPointIndices( const btVector3 &boxMin, const btVector3 boxMax,
	int &pointMinX, int &pointMaxX, int &pointMinZ, int &pointMaxZ ) const;
	/** \brief Calculate the cluster indices for a box. */
	void CalculateBoxClusterIndices( const btVector3 &boxMin, const btVector3 boxMax,
	int &clusterMinX, int &clusterMaxX, int &clusterMinZ, int &clusterMaxZ ) const;
	/*@}*/
};

#endif
