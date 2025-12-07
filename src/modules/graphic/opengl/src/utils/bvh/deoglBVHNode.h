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

#ifndef _DEOGLDBVHNODE_H_
#define _DEOGLDBVHNODE_H_

#include <dragengine/common/math/decMath.h>



/**
 * Bounding volume hierarchie node.
 * 
 * Stores either 2 children nodes or a list of primitives. If the node contains child nodes
 * the index of the left nodes is stored. The index of the right node is the index of the
 * left node plus 1. If the node contains primitives the index of the first primitive is
 * stored and the count of primitives. The node is a leaf node if the count of primitives
 * is larger than 0 otherwise it contains child nodes.
 */
class deoglBVHNode{
private:
	decVector pMinExtend;
	decVector pMaxExtend;
	int pFirstIndex;
	int pPrimitiveCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create bvh node. */
	deoglBVHNode();
	
	/** Clean up generic octree object. */
	~deoglBVHNode();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Minimum extend. */
	inline const decVector &GetMinExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decVector &GetMaxExtend() const{ return pMaxExtend; }
	
	/** Set extends. */
	void SetExtends(const decVector &minExtend, const decVector &maxExtend);
	
	/** Index of first child or primitive. */
	inline int GetFirstIndex() const{ return pFirstIndex; }
	
	/** Set index of first child or primitive. */
	void SetFirstIndex(int index);
	
	/** Count of primitives. */
	inline int GetPrimitiveCount() const{ return pPrimitiveCount; }
	
	/** Set count of primitives. */
	void SetPrimitiveCount(int count);
	
	/** Node is leaf node containing primitives. */
	inline bool IsLeaf() const{ return pPrimitiveCount > 0; }
	
	/** Clear node. */
	void Clear();
	/*@}*/
};

#endif
