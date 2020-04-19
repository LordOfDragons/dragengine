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

#ifndef _DEOGLRRDSRENDERABLECOLOR_H_
#define _DEOGLRRDSRENDERABLECOLOR_H_

#include <dragengine/deObject.h>

#include "deoglRDSRenderable.h"



/**
 * \brief Render dynamic skin color renderable.
 */
class deoglRDSRenderableColor : public deoglRDSRenderable{
private:
	deoglTexture *pTexture;
	decColor pColor;
	bool pDirty;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render dynamic skin value renderable. */
	deoglRDSRenderableColor( deoglRDynamicSkin &dynamicSkin );
	
	/** \brief Clean up render dynamic skin value renderable. */
	virtual ~deoglRDSRenderableColor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set value. */
	void SetColor( const decColor &color );
	
	/** \brief Prepare for render. */
	virtual void PrepareForRender();
	
	/**
	 * \brief Get color if support or default color.
	 * \details Default implementation returns default value.
	 */
	virtual decColor GetRenderColor( const decColor &defaultColor );
	
	/**
	 * \brief Get texture to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglTexture *GetRenderTexture();
	/*@}*/
};

#endif
