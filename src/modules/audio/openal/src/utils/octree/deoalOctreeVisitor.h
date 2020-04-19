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

#ifndef _DEOALOCTREEVISITOR_H_
#define _DEOALOCTREEVISITOR_H_

class deoalOctree;



/**
 * @brief Generic Octree Node Visitor.
 */
class deoalOctreeVisitor{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new octree node visitor. */
	deoalOctreeVisitor();
	/** Cleans up the octree node visitor. */
	virtual ~deoalOctreeVisitor();
	/*@}*/
	
	/** @name Visiting */
	/*@{*/
	/** Visits an octree node. */
	virtual void VisitNode( deoalOctree *node, int intersection );
	/*@}*/
};

// end of include only once
#endif
