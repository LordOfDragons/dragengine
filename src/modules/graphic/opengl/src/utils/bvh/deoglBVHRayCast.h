/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLBVHRAYCAST_H_
#define _DEOGLBVHRAYCAST_H_

class deoglBVH;
class deoglBVHNode;



/**
 * \brief Bounding volume hierarchie ray cast visitor.
 * 
 * Subclass is responsible to to know how to resolve primitive indices. This class provides
 * basic handling of BVH traversal with the help of the subclass.
 */
class deoglBVHRayCast{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ray caster. */
	deoglBVHRayCast();
	
	/** \brief Clean up ray caster. */
	virtual ~deoglBVHRayCast();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Perform ray cast against BVH. */
	void RayCast( deoglBVH &bvh );
	
	
	
protected:
	/** \brief Ray cast against node. */
	void RayCastNode( deoglBVH &bvh, deoglBVHNode &node );
	/*@}*/
};

#endif
