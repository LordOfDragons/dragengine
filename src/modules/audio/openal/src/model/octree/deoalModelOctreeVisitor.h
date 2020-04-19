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

#ifndef _DEOGLMODELOCTREEVISITOR_H_
#define _DEOGLMODELOCTREEVISITOR_H_

#include "../../utils/octree/deoalOctreeVisitor.h"

class deoalModelFace;
class deoalModelOctree;



/**
 * \brief Model octree visitor.
 */
class deoalModelOctreeVisitor : public deoalOctreeVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create model octree visitor. */
	deoalModelOctreeVisitor();
	
	/** \brief Clean up model octree visitor. */
	virtual ~deoalModelOctreeVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/**
	 * \brief Visit octree node.
	 * 
	 * Visitor is responsible to check if the node and children thereof have to be
	 * visited. This allows to apply special checks not handled by deoalOctree.
	 * 
	 * To visit model octree call VisitNode(*model.GetOctree()).
	 * 
	 * The default implementation calls VisitNode(deoalOctree*,int) then visits child nodes.
	 */
	virtual void VisitNode( deoalModelOctree &node );
	
	/**
	 * \brief Visit octree node.
	 * 
	 * Default implementation visits all faces stored in node.
	 */
	virtual void VisitNode( deoalOctree *node, int intersection );
	
	/** \brief Visit face. */
	virtual void VisitFace( deoalModelFace *face );
	/*@}*/
};

#endif
