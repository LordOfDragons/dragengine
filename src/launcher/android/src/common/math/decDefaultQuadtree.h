/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
