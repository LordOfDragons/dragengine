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

#ifndef _DEOGLMODELOCTREEVISITOR_H_
#define _DEOGLMODELOCTREEVISITOR_H_

#include "../../utils/octree/deoglOctreeVisitor.h"

class deoglModelFace;



/**
 * Model octree visitor.
 */
class deoglModelOctreeVisitor : public deoglOctreeVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new model octree visitor. */
	deoglModelOctreeVisitor();
	/** Cleans up the model octree visitor. */
	virtual ~deoglModelOctreeVisitor();
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/**
	 * Visits an octree node.
	 * \details The default implementation is to visit all faces stored in the node.
	 */
	virtual void VisitNode( deoglOctree *node, int intersection );
	/** Visits a face. */
	virtual void VisitFace( deoglModelFace *face );
	/*@}*/
};

#endif
