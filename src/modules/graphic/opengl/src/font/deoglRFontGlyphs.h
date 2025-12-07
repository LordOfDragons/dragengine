/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEOGLRFONTGLYPHS_H_
#define _DEOGLRFONTGLYPHS_H_

#include "../texture/deoglRImage.h"

class deoglImage;
class deoglRenderThread;
class deoglRFont;
class deFont;
class deFontSize;
class deFontGlyph;
class deImage;


/**
 * Render font glyphs.
 */
class deoglRFontGlyphs{
public:
	struct sGlyph{
		float x1, y1, x2, y2;
		int z, bearing, bearingY, width, height, advance;
	};
	
	
private:
	deoglRenderThread &pRenderThread;
	
	sGlyph pUndefinedGlyph, *pGlyphs;
	const sGlyph **pGlyphMap;
	int pGlyphCount, pGlyphMapCount;
	
	int pLineHeight;
	
	deoglRImage::Ref pImage;
	deoglImage *pDelayedImage;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render font size. */
	deoglRFontGlyphs(deoglRenderThread &renderThread, const deFont &font);
	deoglRFontGlyphs(deoglRenderThread &renderThread, const deFontSize &size);
	
	/** Clean up render font. */
	~deoglRFontGlyphs();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	/** Finalize after asynchronous resource loading. */
	void FinalizeAsyncResLoading();
	
	
	/** Undefined glyph. */
	inline const sGlyph &GetUndefinedGlyph() const{ return pUndefinedGlyph; }
	
	/** Glyphs. */
	inline const sGlyph *GetGlyphs() const{ return pGlyphs; }
	
	/** Glyph for unicode. */
	const sGlyph &GetGlyphFor(int unicode) const;
	
	/** Line height. */
	inline int GetLineHeight() const{ return pLineHeight; }
	
	/** Render image or nullptr. */
	inline const deoglRImage::Ref &GetImage() const{ return pImage; }
	
	/** Set render image or nullptr. */
	void SetImage(const deImage *image);
	
	/** Debug print. */
	void DebugPrint() const;
	/*@}*/
	
	
private:
	void pCleanUp();
	void pBuildGlyphs(const deFont &font);
	void pBuildGlyphs(const deFontSize &size);
	void pSetGlyph(sGlyph &target, const deFontGlyph &source);
};

#endif
