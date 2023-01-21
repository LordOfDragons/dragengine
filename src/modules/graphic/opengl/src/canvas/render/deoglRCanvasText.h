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
 * Render canvas text.
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
	/** Create peer. */
	deoglRCanvasText( deoglRenderThread &renderThread );
	
	/** Clean up peer. */
	virtual ~deoglRCanvasText();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Font or \em NULL if not set. */
	inline deoglRFont *GetFont() const{ return pFont; }
	
	/** Set font or \em NULL to unset. */
	void SetFont( deoglRFont *font );
	
	/** Font size in canvas units. */
	inline float GetFontSize() const{ return pFontSize; }
	
	/** Set font size in canvas units. */
	void SetFontSize( float size );
	
	/** Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** Set text. */
	void SetText( const char *text );
	
	/** Text color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** Set text color. */
	void SetColor( const decColor &color );
	/*@}*/
	
	
	
	/** Render. */
	virtual void Render( const deoglRenderCanvasContext &context );
	/*@}*/
};

#endif
