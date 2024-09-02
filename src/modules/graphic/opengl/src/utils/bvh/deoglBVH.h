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

#ifndef _DEOGLDBVH_H_
#define _DEOGLDBVH_H_

#include <dragengine/common/math/decMath.h>

class deoglBVHNode;


/**
 * Bounding volume hierarchie.
 * 
 * Stores the entire BVH as a list of nodes. Content stored in the nodes is stored elsewhere
 * and referenced by indices. This allows storing the BVH to cache and upload to GPU.
 */
class deoglBVH{
public:
	/** Primitive boundary used for building BVH. */
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
	/** Create bvh. */
	deoglBVH();
	
	/** Clean up bvh. */
	~deoglBVH();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of nodes. */
	inline int GetNodeCount() const{ return pNodeCount; }
	
	/** Node at index. */
	deoglBVHNode &GetNodeAt( int index ) const;
	
	/** Direct access to nodes array. */
	inline deoglBVHNode *GetNodes() const{ return pNodes; }
	
	
	
	/** Count of primitives. */
	inline int GetPrimitiveCount() const{ return pPrimitiveCount; }
	
	/** Primitive at index. */
	int GetPrimitiveAt( int index ) const;
	
	/** Direct access to primitives array. */
	inline int *GetPrimitives() const{ return pPrimitives; }
	
	
	
	
	/** Get root node or NULL if empty. */
	deoglBVHNode *GetRootNode() const;
	
	/** Clear tree. */
	void Clear();
	
	/**
	 * Build tree. List of build primitives has to contain the boundary information
	 * for each primitive in the same order the primitives are indexed.
	 * The array can be deleted after build.
	 */
	void Build( const sBuildPrimitive *primitives, int primitiveCount, int maxDepth = 12 );
	/*@}*/
	
	
	
protected:
	void pAddNode();
	void pInitPrimitives( int primitiveCount );
	void pBuildNode( const sBuildPrimitive *primitives, int primitiveCount, int node, int maxDepth );
};

#endif
