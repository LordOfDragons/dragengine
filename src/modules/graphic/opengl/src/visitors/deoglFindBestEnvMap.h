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

#ifndef _DEOGLFINDBESTENVMAP_H_
#define _DEOGLFINDBESTENVMAP_H_

#include <dragengine/common/math/decMath.h>

#include "../world/deoglWorldOctreeVisitor.h"

class deoglEnvironmentMap;
class deoglEnvironmentMapList;



/**
 * Find best environment map world octree visitor.
 * Uses as input the position to search the best environment map for. The environment map with the
 * smallest distance to the target position is considered the best. After visiting the found
 * environment map with stored or NULL otherwise if no result has been found.
 */
class deoglFindBestEnvMap : public deoglWorldOctreeVisitor{
private:
	decDVector pPosition;
	deoglEnvironmentMap *pEnvMap; ///< weak reference
	double pDistance;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	deoglFindBestEnvMap();
	/** Cleans up the visitor. */
	virtual ~deoglFindBestEnvMap();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the target position. */
	inline const decDVector &GetPosition() const{
		return pPosition;
	}
	/** Sets the target position. */
	void SetPosition( const decDVector &position );
	
	/** Retrieves the found environment map or NULL if no environment map could be found. */
	inline deoglEnvironmentMap *GetEnvMap() const{
		return pEnvMap;
	}
	/** Retrieves the distance of the found environment map to the target position or 0 if no environment map could be found. */
	inline double GetDistance() const{
		return pDistance;
	}
	
	/** Resets the visitor setting the found environment map to NULL. */
	void Reset();
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/** Visits an octree node. */
	virtual void VisitNode( deoglDOctree *node, int intersection );
	
	/** Test all environment maps in a list of environment maps. */
	void VisitList( const deoglEnvironmentMapList &list );
	/*@}*/
};

#endif
