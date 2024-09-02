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

#ifndef _DEALFONT_H_
#define _DEALFONT_H_

#include <GLES/gl.h>
#include <GLES2/gl2.h>

#include "../deObject.h"

class dealDisplay;
class dealImage;
class decXmlElementTag;


/**
 * \brief Font.
 */
class dealFont : public deObject{
public:
	/** \brief Glyph. */
	struct sGlyph{
		int code;
		int u;
		int v;
		int width;
		int bearing;
		int advance;
		
		inline sGlyph() : code(0), u(0), v(0), width(0), bearing(0), advance(0){}
	};
	
	
	
private:
	dealDisplay &pDisplay;
	
	dealImage *pImage;
	
	int pLineHeight;
	sGlyph *pGlyphs;
	sGlyph pDefaultGlyph;
	int pGlyphCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font from XML file. */
	dealFont( dealDisplay &display, const char *filename );
	
	/** \brief Clean up font. */
	virtual ~dealFont();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Display. */
	inline dealDisplay &GetDisplay() const{ return pDisplay; }
	
	/** \brief Image. */
	inline dealImage *GetImage() const{ return pImage; }
	
	/** \brief Line height. */
	inline int GetLineHeight() const{ return pLineHeight; }
	
	/** \brief Number of glyphs. */
	inline int GetGlyphCount() const{ return pGlyphCount; }
	
	/** \brief Glyph for code. */
	const sGlyph &GetGlyphFor( int code ) const;
	/*@}*/
	
private:
	void pCleanUp();
	void pLoadXML( const char *filename );
	void pReadFont( const decXmlElementTag &root );
	sGlyph &pGetGlyphForCreate( int code );
};

#endif
 
 
