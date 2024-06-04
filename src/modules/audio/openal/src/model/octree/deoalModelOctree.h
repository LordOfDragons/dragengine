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

#ifndef _DEOALMODELOCTREE_H_
#define _DEOALMODELOCTREE_H_

#include "../../utils/octree/deoalOctree.h"

#include <dragengine/common/collection/decPointerList.h>

class deoalModelOctreeVisitor;
class deoalModelFace;
class deoalCollisionTriangle;


/**
 * \brief Model octree.
 * 
 * Faces can be inserted unique or duplicated. Unique is useful to find faces only once
 * but has worse distribution due to many faces in low depth nodes. Duplicated avoids
 * this problem by inserting faces into all overlapping lowest depth nodes. This works
 * best with ray tracing testing to minimize the testing.
 */
class deoalModelOctree : public deoalOctree{
private:
	decPointerList pFaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create octree. */
	deoalModelOctree( const decVector &center, const decVector &halfExtend );
	
	/** \brief Clean up octree. */
	virtual ~deoalModelOctree();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create octree for octant. */
	virtual deoalOctree *CreateOctree( int octant ) const;
	
	/** \brief Clear content of node. */
	virtual void ClearNodeContent();
	
	/** \brief Clear all faces from tree. */
	void ClearFaces();
	
	/** \brief Add face into octree. */
	void InsertFaceIntoTree( deoalModelFace *face, int maxDepth );
	
	/** \brief Add face into octree. */
	void InsertFaceOverlap( deoalModelFace *face, int depth );
	
	/** \brief Number of faces. */
	int GetFaceCount() const;
	
	/** \brief Face at index. */
	deoalModelFace *GetFaceAt( int index ) const;
	/*@}*/
	
	
	
private:
	deoalModelOctree *pGetNodeFor( const decVector &center, const decVector &halfExtend, int maxDepth );
	deoalModelOctree *pGetNodeFor( const decVector &position, int maxDepth );
	void pInsertFaceOverlap( deoalModelFace *face, deoalCollisionTriangle &triangle, int depth );
};

#endif
