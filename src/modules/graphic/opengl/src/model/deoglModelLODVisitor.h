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

#ifndef _DEOGLMODELLODVISITOR_H_
#define _DEOGLMODELLODVISITOR_H_

#include <dragengine/common/math/decMath.h>

#include "octree/deoglModelOctreeVisitor.h"

class deoglModelLOD;



/**
 * @brief Model LOD Visitor.
 * Model visitor calculating the minimum distance of a vertex faces in a model. Only faces
 * inside a small box around the vertex are tested for speed reasons.
 */
class deoglModelLODVisitor : public deoglModelOctreeVisitor{
private:
	deoglModelLOD &pModelLOD;
	decVector pBoxMinExtend;
	decVector pBoxMaxExtend;
	decVector pLODPoint;
	
	float pMinLODDistance;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new model octree visitor. */
	deoglModelLODVisitor( deoglModelLOD &modelLOD );
	/** Cleans up the model octree visitor. */
	virtual ~deoglModelLODVisitor();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the box minimum extend. */
	inline const decVector &GetBoxMinExtend() const{ return pBoxMinExtend; }
	/** Retrieves the box maximum extend. */
	inline const decVector &GetBoxMaxExtend() const{ return pBoxMaxExtend; }
	/** Sets the box extends. */
	void SetBoxExtends( const decVector &minExtend, const decVector &maxExtend );
	/** Retrieves the lod point. */
	inline const decVector &GetLODPoint() const{ return pLODPoint; }
	/** Sets the lod point. */
	void SetLODPoint( const decVector &lodPoint );
	
	/** Retrieves the minimum lod distance. */
	inline float GetMinLODDistance() const{ return pMinLODDistance; }
	/** Sets the minimum lod distance. */
	void SetMinLODDistance( float distance );
	/*@}*/
	
	/** @name Visiting */
	/*@{*/
	/**
	 * Visits an octree node. The default implementation is to visit all
	 * world elements stored in the node.
	 */
	virtual void VisitNode( deoglOctree *node, int intersection );
	/*@}*/
};

#endif
