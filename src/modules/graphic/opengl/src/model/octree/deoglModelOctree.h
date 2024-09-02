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

#ifndef _DEOGLMODELOCTREE_H_
#define _DEOGLMODELOCTREE_H_

#include "../face/deoglModelFaceList.h"

#include "../../utils/octree/deoglOctree.h"

class deoglModelOctreeVisitor;
class deoglCollisionVolume;



/**
 * Model Octree.
 */
class deoglModelOctree : public deoglOctree{
private:
	deoglModelFaceList pFaces;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new octree. */
	deoglModelOctree( const decVector &center, const decVector &halfExtend );
	/** Cleans up the octree. */
	virtual ~deoglModelOctree();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/**
	 * Creates new octree for the specified octant. Implement this function
	 * to create a new octree of your own type. Do not set the parent of
	 * octree. The caller is responsible for this action if applicable.
	 */
	virtual deoglOctree *CreateOctree( int octant ) const;
	/** Clears the content of this node. */
	virtual void ClearNodeContent();
	/** Clears all faces from the tree. */
	void ClearFaces();
	/** Adds a face into the octree. */
	void InsertFaceIntoTree( deoglModelFace *face, int maxDepth );
	
	/** TEMPORARY HACK!. */
	void VisitNodesCollidingVolume( deoglModelOctreeVisitor &visitor, deoglCollisionVolume &volume );
	
	/** Retrieves the list of faces. */
	inline deoglModelFaceList &GetFaceList(){ return pFaces; }
	inline const deoglModelFaceList &GetFaceList() const{ return pFaces; }
	/*@}*/
	
private:
	deoglModelOctree *pGetNodeFor( const decVector &center, const decVector &halfExtend, int maxDepth );
	deoglModelOctree *pGetNodeFor( const decVector &position, int maxDepth );
};

#endif
