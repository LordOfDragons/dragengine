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

#ifndef _DEOGLRCANVASTEXT_H_
#define _DEOGLRCANVASTEXT_H_

#include "deoglRCanvas.h"

#include <dragengine/common/string/decString.h>

class deoglRFont;


/**
 * \brief Render canvas text.
 */
class deoglRCanvasText : public deoglRCanvas{
private:
	deoglRFont *pFont;
	float pFontSize;
	decString pText;
	decColor pColor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglRCanvasText( deoglRenderThread &renderThread );
	
	/** \brief Clean up peer. */
	virtual ~deoglRCanvasText();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Font or \em NULL if not set. */
	inline deoglRFont *GetFont() const{ return pFont; }
	
	/** \brief Set font or \em NULL to unset. */
	void SetFont( deoglRFont *font );
	
	/** \brief Font size in canvas units. */
	inline float GetFontSize() const{ return pFontSize; }
	
	/** \brief Set font size in canvas units. */
	void SetFontSize( float size );
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const char *text );
	
	/** \brief Text color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set text color. */
	void SetColor( const decColor &color );
	/*@}*/
	
	
	
	/** \brief Render. */
	virtual void Render( const deoglRenderCanvasContext &context );
	/*@}*/
};

#endif
