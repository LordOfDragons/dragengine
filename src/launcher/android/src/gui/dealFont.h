/* 
 * Drag[en]gine Android Launcher
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
 
 
