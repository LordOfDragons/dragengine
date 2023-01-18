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

#ifndef _DEOGLRFONT_H_
#define _DEOGLRFONT_H_

#include <dragengine/deObject.h>

class deoglImage;
class deoglRenderThread;
class deFont;
class deFontGlyph;
class deoglRImage;



/**
 * Render font.
 */
class deoglRFont : public deObject{
public:
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
	
	deoglRImage *pImage;
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
	
	/** Render image or \em NULL if not existing. */
	inline deoglRImage *GetImage() const{ return pImage; }
	/*@}*/
	
private:
	void pCleanUp();
	void pBuildGlyphs( const deFont &font );
	void pSetGlyph( sGlyph &target, const deFontGlyph &source );
};

#endif
