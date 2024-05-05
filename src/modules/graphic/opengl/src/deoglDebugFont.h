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

#ifndef _DEOGLDEBUGFONT_H_
#define _DEOGLDEBUGFONT_H_

#include "deoglBasics.h"

class deoglRenderThread;
class deoglTexture;



/**
 * Debug Font.
 * OpenGL internal font for debuging purpose. Does not use engine resources except loading an image
 * from file for the time being.
 */
class deoglDebugFont{
public:
	struct sGlyph{
		float x1, y1, x2, y2;
		int width, height, advance;
	};
	
private:
	deoglRenderThread &pRenderThread;
	sGlyph *pGlyphs;
	int pFontHeight;
	deoglTexture *pTexture;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new debug font. */
	deoglDebugFont( deoglRenderThread &renderThread );
	/** Cleans up the shared vbo. */
	~deoglDebugFont();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the glyphs array. */
	inline sGlyph *GetGlyphs() const{ return pGlyphs; }
	/** Retrieves the texture. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	/** Retrieves the font height. */
	inline int GetFontHeight() const{ return pFontHeight; }
	/*@}*/
	
private:
	void pCleanUp();
	void pBuildGlyphs();
	void pBuildTexture();
};

#endif
