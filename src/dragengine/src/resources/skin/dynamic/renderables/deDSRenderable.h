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

#ifndef _DEDSRENDERABLE_H_
#define _DEDSRENDERABLE_H_

#include "../../../../common/string/decString.h"

class deDSRenderableVisitor;


/**
 * \brief Dynamic Skin Renderable.
 *
 * Renderables are dynamic texture images the developer can write to
 * during runtime. A renderable can define a texture using various
 * methods to allow for large flexibility. Most of the types stem
 * directly from Skin Resources. For all types the same rules apply
 * as the target Skin Property. For all other cases where this
 * renderable is has not a Skin Property as target the rules of the
 * destination object apply. Renderables have an optional name useful
 * to attach multiple renderables to one target object.
 */
class deDSRenderable{
private:
	decString pName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new renderable. */
	deDSRenderable( const char *name );
	
	/** \brief Clean up renderable. */
	virtual ~deDSRenderable();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the renderable. */
	virtual void Visit( deDSRenderableVisitor &visitor );
};

#endif
