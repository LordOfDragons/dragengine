/* 
 * Drag[en]gine OpenAL Audio Module
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
