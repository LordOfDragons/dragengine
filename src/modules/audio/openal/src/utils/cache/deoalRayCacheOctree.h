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

#ifndef _DEOALRAYCACHEOCTREE_H_
#define _DEOALRAYCACHEOCTREE_H_

#include "../octree/deoalOctree.h"

#include <dragengine/common/collection/decPointerList.h>

class deoalRayCacheRay;



/**
 * \brief OpenAL world octree.
 */
class deoalRayCacheOctree : public deoalOctree{
private:
	decPointerList pRays;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create world octree. */
	deoalRayCacheOctree( const decVector &center, const decVector &halfSize );
	
	/** \brief Clean up world octree. */
	virtual ~deoalRayCacheOctree();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create octree for octant. */
	virtual deoalOctree *CreateOctree( int octant ) const;
	
	/** \brief Clear content of node. */
	virtual void ClearNodeContent();
	
	/** \brief Clear all rays from tree. */
	void ClearRays();
	
	/** \brief Add ray into octree. */
	void InsertRayIntoTree( deoalRayCacheRay *ray, int maxDepth );
	/*@}*/
	
	
	
	/** \name Rays */
	/*@{*/
	/** \brief Number of rays. */
	int GetRayCount() const;
	
	/** \brief Ray at index. */
	deoalRayCacheRay *GetRayAt( int index ) const;
	
	/** \brief Index of ray or -1 if absent. */
	int IndexOfRay( deoalRayCacheRay *ray ) const;
	
	/** \brief Add ray. */
	void AddRay( deoalRayCacheRay *ray );
	
	/** \brief Remove ray. */
	void RemoveRay( deoalRayCacheRay *ray );
	
	/** \brief Remove all rays. */
	void RemoveAllRays();
	/*@}*/
	
	
	
private:
	deoalRayCacheOctree *pGetNodeFor( const decVector &position, int maxDepth );
};

#endif
