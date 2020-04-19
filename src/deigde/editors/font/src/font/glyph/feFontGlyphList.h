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
#ifndef _FEFONTGLYPHLIST_H_
#define _FEFONTGLYPHLIST_H_

// includes
#include <dragengine/common/collection/decObjectOrderedSet.h>

// predefinitions
class feFontGlyph;



/**
 * @brief Font Glyph List.
 * List of font glyph objects.
 */
class feFontGlyphList{
private:
	decObjectOrderedSet pGlyphs;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new font glyph list. */
	feFontGlyphList();
	/** Cleans up the font glyph list. */
	~feFontGlyphList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of glyphs. */
	int GetGlyphCount() const;
	/** Retrieves the glyph at the given position. */
	feFontGlyph *GetGlyphAt( int index ) const;
	/** Retrieves the glyph with the given code or NULL if not found. */
	feFontGlyph *GetGlyphWithCode( int code ) const;
	/** Retrieves the index of the given glyph or -1 if not found. */
	int IndexOfGlyph( feFontGlyph *glyph ) const;
	/** Retrieves the index of the glyph with the given code or -1 if not found. */
	int IndexOfGlyphWithCode( int code ) const;
	/** Determines if a glyph exists. */
	bool HasGlyph( feFontGlyph *glyph ) const;
	/** Determines if a glyph with the given code exists. */
	bool HasGlyphWithCode( int code ) const;
	/** Adds a glyph. */
	void AddGlyph( feFontGlyph *glyph );
	/** Removes a glyph. */
	void RemoveGlyph( feFontGlyph *glyph );
	/** Removes the glyph with the given code if existing. */
	void RemoveGlyphWithCode( int code );
	/** Removes all glyphs. */
	void RemoveAllGlyphs();
	
	/** Sets the list from another list. */
	feFontGlyphList &operator=( const feFontGlyphList &list );
	/*@}*/
};

// end of include only once
#endif
