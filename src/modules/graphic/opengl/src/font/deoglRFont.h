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
 * \brief Render font.
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
	/** \brief Create render font. */
	deoglRFont( deoglRenderThread &renderThread, const deFont &font );
	
	/** \brief Clean up render font. */
	virtual ~deoglRFont();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** \brief Finalize after asynchronous resource loading. */
	void FinalizeAsyncResLoading();
	
	
	
	/** \brief Undefined glyph. */
	inline const sGlyph &GetUndefinedGlyph() const{ return pUndefinedGlyph; }
	
	/** \brief Glyphs. */
	inline const sGlyph *GetGlyphs() const{ return pGlyphs; }
	
	/** \brief Line height. */
	inline int GetLineHeight() const{ return pLineHeight; }
	
	/** \brief Font is a colorable font. */
	inline bool GetIsColorFont() const{ return pIsColorFont; }
	
	/** \brief Render image or \em NULL if not existing. */
	inline deoglRImage *GetImage() const{ return pImage; }
	/*@}*/
	
private:
	void pCleanUp();
	void pBuildGlyphs( const deFont &font );
	void pSetGlyph( sGlyph &target, const deFontGlyph &source );
};

#endif
