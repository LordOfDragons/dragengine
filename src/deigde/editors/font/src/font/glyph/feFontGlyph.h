/* 
 * Drag[en]gine IGDE Font Editor
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

// include only once
#ifndef _FEFONTGLYPH_H_
#define _FEFONTGLYPH_H_

// includes
#include <dragengine/deObject.h>

#include <dragengine/common/string/decString.h>

// predefinitions
class feFont;



/**
 * @brief Font Glyph.
 * Geometric definition of a glyph in a font.
 */
class feFontGlyph : public deObject{
private:
	feFont *pParentFont;
	
	int pCode;
	int pU;
	int pV;
	int pWidth;
	int pBearing;
	int pAdvance;
	
	bool pSelected;
	bool pActive;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new font glyph. */
	feFontGlyph();
	/** Cleans up the font glyph. */
	virtual ~feFontGlyph();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent font or NULL if not assigned. */
	inline feFont *GetParentFont() const{ return pParentFont; }
	/** Sets the parent font or NULL it not assigned. */
	void SetParentFont( feFont *font );
	
	/** Retrieves the code. */
	inline int GetCode() const{ return pCode; }
	/** Sets the code. */
	void SetCode( int code );
	/** Retrieves the u position. */
	inline int GetU() const{ return pU; }
	/** Sets the u position. */
	void SetU( int u );
	/** Retrieves the v position. */
	inline int GetV() const{ return pV; }
	/** Sets the v position. */
	void SetV( int v );
	/** Retrieves the width. */
	inline int GetWidth() const{ return pWidth; }
	/** Sets the width. */
	void SetWidth( int width );
	/** Retrieves the bearing in x direction. */
	inline int GetBearing() const{ return pBearing; }
	/** Sets the bearing in x direction. */
	void SetBearing( int bearing );
	/** Retrieves the advance in x direction. */
	inline int GetAdvance() const{ return pAdvance; }
	/** Sets the advance in x direction. */
	void SetAdvance( int advance );
	
	/** Determines if the glyph is the active one. */
	inline bool GetActive() const{ return pActive; }
	/** Sets if the glyph is the active one. */
	void SetActive( bool active );
	/** Determines if the glyph is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** Sets if the glyph is selected. */
	void SetSelected( bool selected );
	
	/** Creates a copy of this glyph. */
	feFontGlyph *Copy() const;
	
	/** Notifies the parent if assigned that the glyph changed. */
    void NotifyGlyphChanged();
	/*@}*/
};

// end of include only once
#endif
