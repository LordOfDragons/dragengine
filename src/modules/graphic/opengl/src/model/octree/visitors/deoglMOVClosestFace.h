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

#ifndef _DEOGLMOVCLOSESTFACE_H_
#define _DEOGLMOVCLOSESTFACE_H_

#include <dragengine/common/math/decMath.h>

#include "../deoglModelOctreeVisitor.h"

class deoglModelLOD;


/**
 * Model octree visitor retrieving the closest face to a point.
 * 
 * Visits the octree with a box enclosing a maximum search radius. The result
 * is the index of the face or -1 if no face could be found with a distance
 * less then or equal to the search radius.
 * 
 * \note Before using the visitor you have to call deoglModelLOD::PrepareOctree()
 *       to ensure the octree is valid before visiting.
 */
class deoglMOVClosestFace : public deoglModelOctreeVisitor{
private:
	const deoglModelLOD &pModelLOD;
	decVector pTestPoint;
	float pMaxRadius;
	
	int pFaceIndex;
	float pFaceDistanceSquared;
	
	decVector pTestBoxMin;
	decVector pTestBoxMax;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new model octree visitor. */
	deoglMOVClosestFace( const deoglModelLOD &modelLOD );
	/** Cleans up the model octree visitor. */
	virtual ~deoglMOVClosestFace();
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/** Retrieves the test point. */
	inline const decVector &GetTestPoint() const{ return pTestPoint; }
	/** Retrieves the maximum search distance. */
	inline float GetMaxRadius() const{ return pMaxRadius; }
	/** Sets the test point and maximum search radius. */
	void SetTestPoint( const decVector &testPoint, float maxRadius );
	
	/**
	 * Resets the visitor.
	 * \details Sets the found face index to -1.
	 */
	void Reset();
	
	/** Retrieves the face index or -1 if not found. */
	inline int GetFaceIndex() const{ return pFaceIndex; }
	
	/** Convenience method to visit stored model lod returning the found face or -1 if not found. */
	int FindClosestFace( const decVector &testPoint, float maxRadius );
	
	/** Visits an octree node. */
	virtual void VisitNode( deoglOctree *node, int intersection );
	/*@}*/
};

#endif
