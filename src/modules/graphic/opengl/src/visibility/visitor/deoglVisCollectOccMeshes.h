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

#ifndef _DEOGLVISCOLLECTOCCMESHES_H_
#define _DEOGLVISCOLLECTOCCMESHES_H_

#include <dragengine/common/math/decMath.h>

#include "../../world/deoglWorldOctreeVisitor.h"

class deoglTriangleSorter;
class deoglRComponent;



/**
 * Visibility Collect Occlusion Meshes Visitor.
 * Visits components in the world containing occlusion meshes adding them to a triangle sorter
 * for later cropping convex volume lists with it.
 */
class deoglVisCollectOccMeshes : public deoglWorldOctreeVisitor{
private:
	decDVector pVisitMinExtend;
	decDVector pVisitMaxExtend;
	decDMatrix pMatrixInvHull;
	deoglTriangleSorter *pTriangleSorter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	deoglVisCollectOccMeshes();
	/** Cleans up the visitor. */
	virtual ~deoglVisCollectOccMeshes();
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/** Retrieves the minimum extend of the box used to visit the occlusion meshes. */
	inline const decDVector &GetVisitMinExtend() const{ return pVisitMinExtend; }
	/** Retrieves the maximum extend of the box used to visit the occlusion meshes. */
	inline const decDVector &GetVisitMaxExtend() const{ return pVisitMaxExtend; }
	/** Sets the extends of the box used to visit the occlusion meshes. */
	void SetVisitExtends( const decDVector &minExtend, const decDVector &maxExtend );
	/** Retrieves the inverse matrix hull to transfor the occlusion meshes into the hull space. */
	inline const decDMatrix &GetMatrixInvHull() const{ return pMatrixInvHull; }
	/** Sets the inverse matrix hull to transfor the occlusion meshes into the hull space. */
	void SetMatrixInvHull( const decDMatrix &matrixInvHull );
	
	/** Retrieves the triangle sorter. */
	inline deoglTriangleSorter *GetTriangleSorter() const{ return pTriangleSorter; }
	
	/** Reset the visitor clearing the triangle sorter. */
	void Reset();
	
	/** Visits an octree node. */
	virtual void VisitNode( deoglDOctree *node, int intersection );
	
	/** Add occlusion mesh to triangle sorter. */
	void AddOcclusionMesh( deoglRComponent &component );
	/*@}*/
};

#endif
