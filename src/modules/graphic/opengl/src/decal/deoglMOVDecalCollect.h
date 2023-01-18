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

#ifndef _DEOGLMOVDECALCOLLECT_H_
#define _DEOGLMOVDECALCOLLECT_H_

#include <dragengine/common/math/decMath.h>

#include "../model/octree/deoglModelOctreeVisitor.h"

class deoglModelLOD;
class deoglTriangleSorter;



/**
 * Model octree visitor collecting triangles touching decal projection.
 * 
 * \note Before using the visitor you have to call deoglModelLOD::PrepareOctree()
 *       to ensure the octree is valid before visiting.
 */
class deoglMOVDecalCollect : public deoglModelOctreeVisitor{
private:
	const deoglModelLOD &pModelLOD;
	deoglTriangleSorter &pTriangleSorter;
	
	decVector pPlaneNormal[ 6 ];
	float pPlaneDistance[ 6 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create model octree visitor. */
	deoglMOVDecalCollect( const deoglModelLOD &modelLOD, deoglTriangleSorter &triangleSorter );
	
	/** Clean up model octree visitor. */
	virtual ~deoglMOVDecalCollect();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/**
	 * Visit model octree storing list in triangle sorter.
	 * 
	 * The matrix is the decal matrix (position and orientation) in model coordinate system.
	 * Faces are clipped against the range (size.x*-0.5, size.y*-0.5, -size.z) to
	 * (size.x*0.5, size.y*0.5, 0).
	 * 
	 * Faces are added to triangle sorter and sorted front to back.
	 */
	void CollectFaces( const decMatrix &matrix, const decVector &size );
	
	/** Visits an octree node. */
	virtual void VisitNode( deoglOctree *node, int intersection );
	/*@}*/
};

#endif
