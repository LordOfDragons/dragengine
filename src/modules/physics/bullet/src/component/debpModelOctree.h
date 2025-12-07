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
	debpModelOctree(const decDVector &center, const decDVector &halfSize);
	debpModelOctree(const decVector &center, const decVector &halfSize);
	/** Cleans up the model object. */
	~debpModelOctree() override;
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Creates new octree for the specified octant. Implement this function
	 * to create a new octree of your own type. Do not set the parent of
	 * octree. The caller is responsible for this action if applicable.
	 */
	debpDOctree *CreateOctree(int octant) const override;
	/** Clears the content of this node. */
	void ClearNodeContent() override;
	
	/** Clears all faces from the entire octree. */
	void ClearFaces();
	/** Insert face into octree. */
	void InsertFaceIntoTree(int face, const decVector &center, const decVector &halfSize, int maxDepth = 8);
	
	/** Retrieves the number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	/** Retrieves the face at the given index. */
	int GetFaceAt(int index) const;
	/** Retrieves the index of the given face or -1 if not found. */
	int IndexOfFace(int face) const;
	/** Adds a face. */
	void AddFace(int face);
	/** Removes a face. */
	void RemoveFace(int face);
	/** Removes all faces. */
	void RemoveAllFaces();
	/*@}*/
	
private:
	debpModelOctree *pGetNodeFor(const decVector &center, const decVector &halfSize, int maxDepth);
};

// end of include only once
#endif
