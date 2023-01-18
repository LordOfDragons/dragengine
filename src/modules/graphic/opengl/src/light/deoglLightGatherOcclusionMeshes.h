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

#ifndef _DEOGLLIGHTGATHEROCCLUSIONMESHES_H_
#define _DEOGLLIGHTGATHEROCCLUSIONMESHES_H_

#include "../world/deoglWorldOctreeVisitor.h"
#include "../triangles/deoglTriangleSorter.h"

class deoglRLight;
class deoglRComponent;



/**
 * @brief Light Gather Occlusion Meshes Visitor.
 * Visits components in the world containing occlusion meshes adding them to a triangle sorter
 * for later cropping light volumes with it.
 */
class deoglLightGatherOcclusionMeshes : public deoglWorldOctreeVisitor{
private:
	deoglRLight &pLight;
	deoglTriangleSorter pTriangleSorter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	deoglLightGatherOcclusionMeshes( deoglRLight &light );
	/** Cleans up the visitor. */
	virtual ~deoglLightGatherOcclusionMeshes();
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/** Retrieves the triangle sorter. */
	inline deoglTriangleSorter &GetTriangleSorter(){ return pTriangleSorter; }
	inline const deoglTriangleSorter &GetTriangleSorter() const{ return pTriangleSorter; }
	
	/** Reset the visitor clearing the triangle sorter. */
	void Reset();
	
	/** Visits an octree node. */
	virtual void VisitNode( deoglDOctree *node, int intersection );
	
	/** Add occlusion mesh to triangle sorter. */
	void AddOcclusionMesh( deoglRComponent &component );
	/*@}*/
};

#endif
