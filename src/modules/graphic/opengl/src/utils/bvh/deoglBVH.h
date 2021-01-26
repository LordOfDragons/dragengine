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

#ifndef _DEOGLDBVH_H_
#define _DEOGLDBVH_H_

#include <dragengine/common/math/decMath.h>

class deoglBVHNode;


/**
 * \brief Bounding volume hierarchie.
 * 
 * Stores the entire BVH as a list of nodes. Content stored in the nodes is stored elsewhere
 * and referenced by indices. This allows storing the BVH to cache and upload to GPU.
 */
class deoglBVH{
public:
	/** \brief Primitive boundary used for building BVH. */
	struct sBuildPrimitive{
		decVector minExtend;
		decVector maxExtend;
		decVector center;
	};
	
	
	
private:
	deoglBVHNode *pNodes;
	int pNodeCount;
	int pNodeSize;
	
	int *pPrimitives;
	int pPrimitiveCount;
	int pPrimitiveSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create bvh. */
	deoglBVH();
	
	/** \brief Clean up bvh. */
	~deoglBVH();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of nodes. */
	inline int GetNodeCount() const{ return pNodeCount; }
	
	/** \brief Node at index. */
	deoglBVHNode &GetNodeAt( int index ) const;
	
	/** \brief Direct access to nodes array. */
	inline deoglBVHNode *GetNodes() const{ return pNodes; }
	
	
	
	/** \brief Count of primitives. */
	inline int GetPrimitiveCount() const{ return pPrimitiveCount; }
	
	/** \brief Primitive at index. */
	int GetPrimitiveAt( int index ) const;
	
	/** \brief Direct access to primitives array. */
	inline int *GetPrimitives() const{ return pPrimitives; }
	
	
	
	
	/** \brief Get root node or NULL if empty. */
	deoglBVHNode *GetRootNode() const;
	
	/** \brief Clear tree. */
	void Clear();
	
	/**
	 * \brief Build tree.
	 * 
	 * List of build primitives has to contain the boundary information for each primitive
	 * in the same order the primitives are indexed. The array can be deleted after build.
	 */
	void Build( const sBuildPrimitive *primitives, int primitiveCount, int maxDepth = 12 );
	/*@}*/
	
	
	
protected:
	void pAddNode();
	void pInitPrimitives( int primitiveCount );
	void pBuildNode( const sBuildPrimitive *primitives, int primitiveCount, int node, int maxDepth );
};

#endif
