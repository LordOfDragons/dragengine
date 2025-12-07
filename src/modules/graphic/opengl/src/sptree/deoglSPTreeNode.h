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
	deoglSPTreeNode(const decVector &center, const decVector &halfSize);
	/** Cleans up the space partition tree node. */
	~deoglSPTreeNode();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the center of the node. */
	inline const decVector &GetCenter() const{return pCenter;}
	/** Retrieves the half size of the node. */
	inline const decVector &GetHalfSize() const{return pHalfSize;}
	/** Retrieves the split state. */
	inline int GetSplitState() const{return pSplitState;}
	/**
	 * Sets the split state. This affects the left and right child if
	 * different than the current split state.
	 */
	void SetSplitState(int splitState);
	/** Retrieves the left child or NULL. */
	inline deoglSPTreeNode *GetLeftChild() const{return pChildLeft;}
	/** Retrieves the right child or NULL. */
	inline deoglSPTreeNode *GetRightChild() const{return pChildRight;}
	/*@}*/
	
private:
};

// end of include only once
#endif
