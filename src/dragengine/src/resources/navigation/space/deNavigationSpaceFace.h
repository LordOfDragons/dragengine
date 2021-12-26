/* 
 * Drag[en]gine Game Engine
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

#ifndef _DENAVIGATIONSPACEFACE_H_
#define _DENAVIGATIONSPACEFACE_H_

#include "../../../dragengine_export.h"


/**
 * \brief Navigation Space Face.
 * Face in a navigation space. A valid face composes of 3 or more vertices.
 * All vertices have to be co-planar and the face has to be convex.
 */
class DE_DLL_EXPORT deNavigationSpaceFace{
private:
	unsigned short pCornerCount;
	unsigned short pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new navigation space face. */
	deNavigationSpaceFace();
	
	/** \brief Clean up navigation space face. */
	~deNavigationSpaceFace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of corners. */
	inline unsigned short GetCornerCount() const{ return pCornerCount; }
	
	/** \brief Set number of corners. */
	void SetCornerCount( unsigned short count );
	
	/** \brief Type number for crossing this face. */
	inline unsigned short GetType() const{ return pType; }
	
	/** \brief Set type number for crossing this face. */
	void SetType( unsigned short type );
	/*@}*/
};

#endif
