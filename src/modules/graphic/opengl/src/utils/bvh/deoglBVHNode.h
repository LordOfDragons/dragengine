/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
	void SetExtends( const decVector &minExtend, const decVector &maxExtend );
	
	/** Index of first child or primitive. */
	inline int GetFirstIndex() const{ return pFirstIndex; }
	
	/** Set index of first child or primitive. */
	void SetFirstIndex( int index );
	
	/** Count of primitives. */
	inline int GetPrimitiveCount() const{ return pPrimitiveCount; }
	
	/** Set count of primitives. */
	void SetPrimitiveCount( int count );
	
	/** Node is leaf node containing primitives. */
	inline bool IsLeaf() const{ return pPrimitiveCount > 0; }
	
	/** Clear node. */
	void Clear();
	/*@}*/
};

#endif
