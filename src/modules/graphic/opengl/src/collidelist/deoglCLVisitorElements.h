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

#ifndef _DEOGLCLVISITORELEMENTS_H_
#define _DEOGLCLVISITORELEMENTS_H_

#include "../world/deoglDefaultWorldOctreeVisitor.h"

class deoglCollideList;
class deoglDCollisionVolume;



/**
 * Collide List Element Visitor.
 * Visits scene elements colliding with a given volume and adds them to the given
 * collide list.
 */
class deoglCLVisitorElements : public deoglDefaultWorldOctreeVisitor{
private:
	deoglCollideList *pCollideList;
	deoglDCollisionVolume *pVolume;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	deoglCLVisitorElements( deoglCollideList *collideList, deoglDCollisionVolume *volume );
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/**
	 * Visits an octree node. The default implementation is to visit all
	 * world elements stored in the node.
	 */
	virtual void VisitNode( deoglDOctree *node, int intersection );
	/*@}*/
};

// end of include only once
#endif
