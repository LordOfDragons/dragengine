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

#ifndef _DEDSRENDERABLECOLOR_H_
#define _DEDSRENDERABLECOLOR_H_

#include "deDSRenderable.h"
#include "../../../../common/math/decMath.h"


/**
 * \brief Renderable Color.
 *
 * The entire texture is defined by one single color for all pixels.
 * Use this type if the renderable is used as the source of a Color
 * or Image Type Skin Property. If used for an Image Type Skin
 * Property the dimension of the texture is undefined but usually
 * very small ( possibly down to 1x1 ).
 */
class DE_DLL_EXPORT deDSRenderableColor : public deDSRenderable{
private:
	decColor pColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new renderable. */
	deDSRenderableColor( const char *name );
	
	/** \brief Clean up renderable. */
	virtual ~deDSRenderableColor();
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
	/** \brief Visits the renderable. */
	virtual void Visit( deDSRenderableVisitor &visitor );
};

#endif
