/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
