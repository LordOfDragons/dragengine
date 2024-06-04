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

#ifndef _DEOGLRFONT_H_
#define _DEOGLRFONT_H_

#include "../texture/deoglRImage.h"

#include <dragengine/deObject.h>

class deoglImage;
class deoglRenderThread;
class deFont;
class deFontGlyph;



/**
 * Render font.
 */
class deoglRFont : public deObject{
public:
	typedef deTObjectReference<deoglRFont> Ref;
	
	struct sGlyph{
		float x1;
		float y1;
		float x2;
		float y2;
		int bearing;
		int width;
		int height;
		int advance;
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	sGlyph pUndefinedGlyph;
	sGlyph *pGlyphs;
	int pLineHeight;
	bool pIsColorFont;
	
	deoglRImage::Ref pImage;
	deoglImage *pDelayedImage;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render font. */
	deoglRFont( deoglRenderThread &renderThread, const deFont &font );
	
	/** Clean up render font. */
	virtual ~deoglRFont();
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
	
	/** Line height. */
	inline int GetLineHeight() const{ return pLineHeight; }
	
	/** Font is a colorable font. */
	inline bool GetIsColorFont() const{ return pIsColorFont; }
	
	/** Render image or nullptr if not existing. */
	inline const deoglRImage::Ref &GetImage() const{ return pImage; }
	/*@}*/
	
private:
	void pCleanUp();
	void pBuildGlyphs( const deFont &font );
	void pSetGlyph( sGlyph &target, const deFontGlyph &source );
};

#endif
