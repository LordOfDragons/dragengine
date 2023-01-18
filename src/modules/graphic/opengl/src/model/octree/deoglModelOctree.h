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
