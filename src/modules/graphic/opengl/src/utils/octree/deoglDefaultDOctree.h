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

#ifndef _DEOGLDEFAULTDOCTREE_H_
#define _DEOGLDEFAULTDOCTREE_H_

#include "deoglDOctree.h"

#define deoglDO_MAX_DEPTH 8



/**
 * @brief Default Octree Implementation.
 * Default implementation of the octree storing a list of objects.
 */
class deoglDefaultDOctree : public deoglDOctree{
private:
	void **pElements;
	int pElementCount;
	int pElementSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new generic octree object. */
	deoglDefaultDOctree( const decDVector &center, const decDVector &halfSize );
	/** Cleans up the generic octree object. */
	virtual ~deoglDefaultDOctree();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Creates new octree for the specified octant. Implement this function
	 * to create a new octree of your own type. Do not set the parent of
	 * octree. The caller is responsible for this action if applicable.
	 */
	virtual deoglDOctree *CreateOctree( int octant ) const;
	/** Clears the content of this node. */
	virtual void ClearNodeContent();
	/**
	 * Adds a new object to the octree. The object is placed in the node
	 * which hosts the specified box. Returns the node where the element
	 * has been placed into.
	 */
	deoglDefaultDOctree *InsertIntoTree( void *element, const decDVector &boxCenter, const decDVector &boxHalfSize, int maxDepth = deoglDO_MAX_DEPTH );
	/*@}*/
	
	/** @name Elements */
	/*@{*/
	/** Retrieves the number of elements. */
	inline int GetElementCount() const{ return pElementCount; }
	/** Retrieves the element at the given index. */
	void *GetElementAt( int index ) const;
	/** Retrieves the index of the given element or -1 if not found. */
	int IndexOfElement( void *element ) const;
	/** Adds a element. */
	void AddElement( void *element );
	/** Removes a element. */
	void RemoveElement( void *element );
	/** Removes all elements. */
	void RemoveAllElements();
	/*@}*/
};

// end of include only once
#endif
