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

#ifndef _DECDEFAULTOCTREE_H_
#define _DECDEFAULTOCTREE_H_

#include "decOctree.h"

#define DECDO_MAX_DEPTH 8



/**
 * @brief Default Octree Implementation.
 *
 * Default implementation of the octree storing a list of objects.
 */
class decDefaultOctree : public decOctree{
private:
	void **pElements;
	int pElementCount;
	int pElementSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new generic octree object. */
	decDefaultOctree(const decVector &center, const decVector &halfSize);
	/** Cleans up the generic octree object. */
	virtual ~decDefaultOctree();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Creates new octree for the specified octant. Implement this function
	 * to create a new octree of your own type. Do not set the parent of
	 * octree. The caller is responsible for this action if applicable.
	 */
	virtual decOctree *CreateOctree(int octant) const;
	/** Clears the content of this node. */
	virtual void ClearNodeContent();
	/**
	 * Adds a new object to the octree. The object is placed in the node
	 * which hosts the specified box. Returns the node where the element
	 * has been placed into.
	 */
	decDefaultOctree *InsertIntoTree(void *element, const decVector &boxCenter, const decVector &boxHalfSize, int maxDepth = DECDO_MAX_DEPTH);
	/*@}*/
	
	/** @name Elements */
	/*@{*/
	/** Retrieves the number of elements. */
	inline int GetElementCount() const{ return pElementCount; }
	/** Retrieves the element at the given index. */
	void *GetElementAt(int index) const;
	/** Retrieves the index of the given element or -1 if not found. */
	int IndexOfElement(void *element) const;
	/** Adds a element. */
	void AddElement(void *element);
	/** Removes a element. */
	void RemoveElement(void *element);
	/** Removes all elements. */
	void RemoveAllElements();
	/*@}*/
};

// end of include only once
#endif
