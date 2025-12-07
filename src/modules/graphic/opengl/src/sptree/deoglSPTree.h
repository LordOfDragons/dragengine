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
#ifndef _DEOGLSPTREE_H_
#define _DEOGLSPTREE_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefintions
class deoglSPTreeNode;



/**
 * OpenGL Space Partition Tree.
 * Provides a data structure to partition space into cubes of variable
 * size including connectivity data. For speed and memory reasons the
 * space partition tree is organized in a grid fashion on the base
 * level. The grid is of equal size in all directions providing for
 * fast locating cubes given a position or a collision volume as well
 * as providing a good basic partition of space. Each cube is set to
 * NULL if it is empty space reducing memory consumption to a minimum.
 * Nodes containing a shift from empty space to solid space are filled
 * with a space partition tree node object which is the root of a k-Tree
 * breaking space down more fine grained where required.
 */
class deoglSPTree{
private:
	decVector pMinExtend;
	decVector pMaxExtend;
	decVector pNodeSize;
	decPoint3 pNodeCount;
	deoglSPTreeNode **pNodes;
	int pTotalNodeCount;
	int pStride;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new space partition tree. */
	deoglSPTree(const decVector &minExtend, const decVector &maxExtend, const decPoint3 &nodeCount);
	/** Cleans up the space partition tree. */
	~deoglSPTree();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the minimum extend of the boundary box. */
	inline const decVector &GetMinimumExtend() const{return pMinExtend;}
	/** Retrieves the maximum extend of the boundary box. */
	inline const decVector &GetMaximumExtend() const{return pMaxExtend;}
	/** Retrieves the node size. */
	inline const decVector &GetNodeSize() const{return pNodeSize;}
	/** Retrieves the number of nodes. */
	inline const decPoint3 &GetNodeCount() const{return pNodeCount;}
	/** Determines if the index is inside the valid range. */
	bool IsIndexValid(const decPoint3 &index) const;
	/**
	 * Retrieves the index of the node containing the given point in space.
	 * If the point is located outside the tree boundary box all components
	 * of index are set to -1.
	 */
	void IndexOfNodeAt(decPoint3 &index, const decVector &position) const;
	/** Retrieves the node at the given index or NULL if empty. */
	deoglSPTreeNode *GetNodeAt(const decPoint3 &index) const;
	/** Sets the node at the given index or NULL to set it empty. */
	void SetNodeAt(const decPoint3 &index, deoglSPTreeNode *node);
	/** Clears all nodes. */
	void ClearAllNodes();
	/*@}*/
	
private:
};

// end of include only once
#endif
