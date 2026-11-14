/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEFTRENDERER_H_
#define _DEFTRENDERER_H_

#include <ft2build.h>
#include FT_FREETYPE_H

class deftLayout;
class deftModule;
class deFontSize;
class deImage;


/**
 * Render glyph map.
 */
class deftRenderer{
private:
	deftModule &pModule;
	FT_Face pFace;
	FT_Int32 pLoadFlags;
	int pComponentCount;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deftRenderer(deftModule &module, FT_Face face);
	
	/** Clean up renderer. */
	~deftRenderer();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Load flags for FreeType glyph loading. */
	inline FT_Int32 GetLoadFlags() const{ return pLoadFlags; }
	
	/** Set load flags for FreeType glyph loading. */
	void SetLoadFlags(FT_Int32 loadFlags);
	
	/** Component count for the image. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** Set component count for the image. */
	void SetComponentCount(int componentCount);
	
	
	/** Render glyph map. */
	void Render(const deftLayout &layout, deFontSize &fontSize);
	/*@}*/
	
	
private:
	void pRenderNonColor(const deftLayout &layout, deImage &image);
	void pRenderColor(const deftLayout &layout, deImage &image);
};

#endif
