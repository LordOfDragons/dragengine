/* 
 * Drag[en]gine Android Launcher
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
#ifndef _DECDEFAULTBSPTREE_H_
#define _DECDEFAULTBSPTREE_H_

// includes
#include "decBSPTree.h"

// predefinitions



/**
 * @brief Default BSP Tree Implementation.
 *
 * Default implementation of the bsp tree storing a list of objects.
 */
class decDefaultBSPTree : public decBSPTree{
private:
	decBSPTreeFace **pFaces;
	int pFaceCount;
	int pFaceSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new default bsp tree object. */
	decDefaultBSPTree( const decVector &normal, float distance );
	/** Cleans up the default bsp tree object. */
	virtual ~decDefaultBSPTree();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Creates new bsp tree for the specified octant. Implement this function
	 * to create a new bsp tree of your own type. Do not set the parent of
	 * bsp tree. The caller is responsible for this action if applicable.
	 */
	virtual decBSPTree *CreateBSPTree( const decVector &normal, float distance ) const;
	/** Clears the content of this node. */
	virtual void ClearNodeContent();
	/** Add face as a coplanar face for this node. */
	virtual void AddCoplanarFace( const decBSPTreeFace &face );
	/*@}*/
	
	/** @name Faces */
	/*@{*/
	/** Retrieves the number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	/** Retrieves the face at the given index. */
	decBSPTreeFace *GetFaceAt( int index ) const;
	/** Retrieves the index of the given face or -1 if not found. */
	int IndexOfFace( decBSPTreeFace *face ) const;
	/** Adds a face. */
	void AddFace( decBSPTreeFace *face );
	/** Removes a face. */
	void RemoveFace( decBSPTreeFace *face );
	/** Removes all faces. */
	void RemoveAllFaces();
	/*@}*/
};

// end of include only once
#endif
