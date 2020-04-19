/* 
 * Drag[en]gine Bullet Physics Module
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
#ifndef _DEBPMODELOCTREE_H_
#define _DEBPMODELOCTREE_H_

// includes
#include "../coldet/octree/debpDOctree.h"



/**
 * @brief Model Octree.
 * Octree storing indices to faces of a model.
 */
class debpModelOctree : public debpDOctree{
private:
	int *pFaces;
	int pFaceCount;
	int pFaceSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new model octree. */
	debpModelOctree( const decDVector &center, const decDVector &halfSize );
	debpModelOctree( const decVector &center, const decVector &halfSize );
	/** Cleans up the model object. */
	virtual ~debpModelOctree();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Creates new octree for the specified octant. Implement this function
	 * to create a new octree of your own type. Do not set the parent of
	 * octree. The caller is responsible for this action if applicable.
	 */
	virtual debpDOctree *CreateOctree( int octant ) const;
	/** Clears the content of this node. */
	virtual void ClearNodeContent();
	
	/** Clears all faces from the entire octree. */
	void ClearFaces();
	/** Insert face into octree. */
	void InsertFaceIntoTree( int face, const decVector &center, const decVector &halfSize, int maxDepth = 8 );
	
	/** Retrieves the number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	/** Retrieves the face at the given index. */
	int GetFaceAt( int index ) const;
	/** Retrieves the index of the given face or -1 if not found. */
	int IndexOfFace( int face ) const;
	/** Adds a face. */
	void AddFace( int face );
	/** Removes a face. */
	void RemoveFace( int face );
	/** Removes all faces. */
	void RemoveAllFaces();
	/*@}*/
	
private:
	debpModelOctree *pGetNodeFor( const decVector &center, const decVector &halfSize, int maxDepth );
};

// end of include only once
#endif
