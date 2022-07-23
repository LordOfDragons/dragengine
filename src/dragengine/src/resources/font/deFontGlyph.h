/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEFONTGLYPH_H_
#define _DEFONTGLYPH_H_

#include "../../dragengine_export.h"


/**
 * \brief Text font glyph definition.
 */
class DE_DLL_EXPORT deFontGlyph{
private:
	int pUnicode;
	int pX;
	int pY;
	int pZ;
	int pWidth;
	int pBearing;
	int pAdvance;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font glyph. */
	deFontGlyph();
	
	/** \brief Clean up font glyph. */
	~deFontGlyph();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Unicode. */
	inline int GetUnicode() const{ return pUnicode; }
	
	/** \brief Set unicode. */
	void SetUnicode( int unicode );
	
	/** \brief Left side of glyph in pixels on image map. */
	inline int GetX() const{ return pX; }
	
	/** \brief Left side of glyph in pixels on image map. */
	void SetX( int x );
	
	/** \brief Top side of glyph in pixels on image map. */
	inline int GetY() const{ return pY; }
	
	/** \brief Top side of glyph in pixels on image map. */
	void SetY( int y );
	
	/** \brief Layer of image map the glyph is located on. */
	inline int GetZ() const{ return pZ; }
	
	/** \brief Layer of image map the glyph is located on. */
	void SetZ( int z );
	
	/** \brief Width of glyph in pixels on image map. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Width of glyph in pixels on image map. */
	void SetWidth( int width );
	
	/** \brief Horizontal bearing in pixels. */
	inline int GetBearing() const{ return pBearing; }
	
	/** \brief Horizontal bearing in pixels. */
	void SetBearing( int bearing );
	
	/** \brief Horizontal advance in pixels. */
	inline int GetAdvance() const{ return pAdvance; }
	
	/** \brief Horizontal advance in pixels. */
	void SetAdvance( int advance );
};

#endif
