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

#ifndef _DESKINPROPERTYCOLOR_H_
#define _DESKINPROPERTYCOLOR_H_

#include "deSkinProperty.h"
#include "../../../common/math/decMath.h"


/**
 * \brief Skin Texture Property Color.
 *
 * Defines a color property of a skin texture.
 */
class deSkinPropertyColor : public deSkinProperty{
private:
	decColor pColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create new named skin property color with the given type.
	 * \param type Type of the skin property obtained from adding a property
	 * name to the texture property map object held by the engine.
	 */
	deSkinPropertyColor( const char *type );
	
	/** \brief Clean up skin property color. */
	virtual ~deSkinPropertyColor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Static color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set static color. */
	void SetColor( const decColor &color );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit property. */
	virtual void Visit( deSkinPropertyVisitor &visitor );
	/*@}*/
};

#endif
