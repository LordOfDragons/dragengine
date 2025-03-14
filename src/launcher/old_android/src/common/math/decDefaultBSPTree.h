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
