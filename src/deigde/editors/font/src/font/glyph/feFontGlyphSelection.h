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
#ifndef _FEFONTGLYPHSELECTION_H_
#define _FEFONTGLYPHSELECTION_H_

// includes
#include "feFontGlyphList.h"

// predefinitions
class feFont;
class feFontGlyph;



/**
 * @brief Font Glyph Selection.
 * Stores a selection of glyphs as well as the active glyph if any.
 */
class feFontGlyphSelection{
private:
	feFont *pParentFont;
	
	feFontGlyphList pSelected;
	feFontGlyph *pActive;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new font. */
	feFontGlyphSelection( feFont *parentFont );
	/** Cleans up the font. */
	~feFontGlyphSelection();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of selected glyphs. */
	int GetSelectedGlyphCount() const;
	/** Retrieves the n-thed selected glyph. */
	feFontGlyph *GetSelectedGlyphAt( int index ) const;
	/** Determines if a glyph is in the list of selected glyphs. */
	bool IsGlypthSelected( feFontGlyph *glyph ) const;
	/** Retrieves the index of the glyph if in the list of selected glyphs. */
	int IndexOfGlyph( feFontGlyph *glyph ) const;
	/** Adds a glyph to the selection if not there already. */
	void AddGlyphToSelection( feFontGlyph *glyph );
	/** Removes a glyph from the selection if selected. */
	void RemoveGlyphFromSelection( feFontGlyph *glyph );
	/** Removes all glyph from the selection. */
	void RemoveAllGlyphsFromSelection();
	
	/** Retrieves the list of selected glyphs. */
	void GetSelectedList( feFontGlyphList &list ) const;
	
	/** Retrieves the active glyph or NULL if none is active. */
	inline feFontGlyph *GetActiveGlyph() const{ return pActive; }
	/** Determines if there is an active glyph or not. */
	bool HasActiveGlyph() const;
	/** Sets the active glyph or NULL if none is active. */
	void SetActiveGlyph( feFontGlyph *glyph );
	/**
	 * Sets the next possible glyph from the selection as the active one or
	 * NULL if the selection is empty.
	 */
	void ActivateNextGlyph();
	
	/** Removes all glyph from the selection and sets the active glyph to NULL. */
	void Reset();
};

// end of include only once
#endif
