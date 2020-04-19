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

// include only once
#ifndef _DEOGLSPTREE_H_
#define _DEOGLSPTREE_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefintions
class deoglSPTreeNode;



/**
 * @brief OpenGL Space Partition Tree.
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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new space partition tree. */
	deoglSPTree( const decVector &minExtend, const decVector &maxExtend, const decPoint3 &nodeCount );
	/** Cleans up the space partition tree. */
	~deoglSPTree();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the minimum extend of the boundary box. */
	inline const decVector &GetMinimumExtend() const{ return pMinExtend; }
	/** Retrieves the maximum extend of the boundary box. */
	inline const decVector &GetMaximumExtend() const{ return pMaxExtend; }
	/** Retrieves the node size. */
	inline const decVector &GetNodeSize() const{ return pNodeSize; }
	/** Retrieves the number of nodes. */
	inline const decPoint3 &GetNodeCount() const{ return pNodeCount; }
	/** Determines if the index is inside the valid range. */
	bool IsIndexValid( const decPoint3 &index ) const;
	/**
	 * Retrieves the index of the node containing the given point in space.
	 * If the point is located outside the tree boundary box all components
	 * of index are set to -1.
	 */
	void IndexOfNodeAt( decPoint3 &index, const decVector &position ) const;
	/** Retrieves the node at the given index or NULL if empty. */
	deoglSPTreeNode *GetNodeAt( const decPoint3 &index ) const;
	/** Sets the node at the given index or NULL to set it empty. */
	void SetNodeAt( const decPoint3 &index, deoglSPTreeNode *node );
	/** Clears all nodes. */
	void ClearAllNodes();
	/*@}*/
	
private:
};

// end of include only once
#endif
