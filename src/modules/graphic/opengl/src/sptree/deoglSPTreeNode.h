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
#ifndef _DEOGLSPTREENODE_H_
#define _DEOGLSPTREENODE_H_

// includes
#include "dragengine/common/math/decMath.h"



/**
 * OpenGL Space Partition Tree Node.
 * Node of a space partition tree. A node is a k-Tree like data structure
 * hosting up to 2 child nodes. The node can be split along any of the
 * three major axes in half. Nodes can be either in a split or unsplit
 * state. The left child is always located at the negative side along the
 * split axis and the right child on the positive side.
 */
class deoglSPTreeNode{
public:
	/** Determines along which axis a split happened if any. */
	enum eSplitState{
		/** No splitting present. */
		essNone,
		/** Split along the X axis. */
		essXAxis,
		/** Split along the Y axis. */
		essYAxis,
		/** Split along the Z axis. */
		essZAxis
	};
	
private:
	decVector pCenter;
	decVector pHalfSize;
	deoglSPTreeNode *pChildLeft;
	deoglSPTreeNode *pChildRight;
	int pSplitState;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new space partition tree node. */
	deoglSPTreeNode( const decVector &center, const decVector &halfSize );
	/** Cleans up the space partition tree node. */
	~deoglSPTreeNode();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the center of the node. */
	inline const decVector &GetCenter() const{ return pCenter; }
	/** Retrieves the half size of the node. */
	inline const decVector &GetHalfSize() const{ return pHalfSize; }
	/** Retrieves the split state. */
	inline int GetSplitState() const{ return pSplitState; }
	/**
	 * Sets the split state. This affects the left and right child if
	 * different than the current split state.
	 */
	void SetSplitState( int splitState );
	/** Retrieves the left child or NULL. */
	inline deoglSPTreeNode *GetLeftChild() const{ return pChildLeft; }
	/** Retrieves the right child or NULL. */
	inline deoglSPTreeNode *GetRightChild() const{ return pChildRight; }
	/*@}*/
	
private:
};

// end of include only once
#endif
