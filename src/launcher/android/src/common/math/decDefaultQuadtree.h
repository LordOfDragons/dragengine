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

// include only once
#ifndef _DECDEFAULTQUADTREE_H_
#define _DECDEFAULTQUADTREE_H_

// includes
#include "decQuadtree.h"

// definitions
#define DECDO_MAX_DEPTH 8



/**
 * @brief Default Quadtree Implementation.
 *
 * Default implementation of the quadtree storing a list of objects.
 */
class decDefaultQuadtree : public decQuadtree{
private:
	void **pElements;
	int pElementCount;
	int pElementSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new generic quadtree object. */
	decDefaultQuadtree( const decVector2 &center, const decVector2 &halfSize );
	/** Cleans up the generic quadtree object. */
	virtual ~decDefaultQuadtree();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Creates new quadtree for the specified quadrant. Implement this function
	 * to create a new quadtree of your own type. Do not set the parent of
	 * quadtree. The caller is responsible for this action if applicable.
	 */
	virtual decQuadtree *CreateQuadtree( int quadrant ) const;
	/** Clears the content of this node. */
	virtual void ClearNodeContent();
	/**
	 * Adds a new object to the quadtree. The object is placed in the node
	 * which hosts the specified box. Returns the node where the element
	 * has been placed into.
	 */
	decDefaultQuadtree *InsertIntoTree( void *element, const decVector2 &boxCenter, const decVector2 &boxHalfSize, int maxDepth = DECDO_MAX_DEPTH );
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
