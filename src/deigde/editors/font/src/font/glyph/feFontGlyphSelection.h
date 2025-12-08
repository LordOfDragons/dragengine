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

// include only once
#ifndef _FEFONTGLYPHSELECTION_H_
#define _FEFONTGLYPHSELECTION_H_

// includes
#include "feFontGlyphList.h"

// predefinitions
class feFont;
#include "feFontGlyph.h"



/**
 * @brief Font Glyph Selection.
 * Stores a selection of glyphs as well as the active glyph if any.
 */
class feFontGlyphSelection{
private:
	feFont *pParentFont;
	
	feFontGlyphList pSelected;
	feFontGlyph::Ref pActive;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new font. */
	feFontGlyphSelection(feFont *parentFont);
	/** Cleans up the font. */
	~feFontGlyphSelection();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of selected glyphs. */
	int GetSelectedGlyphCount() const;
	/** Retrieves the n-thed selected glyph. */
	feFontGlyph *GetSelectedGlyphAt(int index) const;
	/** Determines if a glyph is in the list of selected glyphs. */
	bool IsGlypthSelected(feFontGlyph *glyph) const;
	/** Retrieves the index of the glyph if in the list of selected glyphs. */
	int IndexOfGlyph(feFontGlyph *glyph) const;
	/** Adds a glyph to the selection if not there already. */
	void AddGlyphToSelection(feFontGlyph *glyph);
	/** Removes a glyph from the selection if selected. */
	void RemoveGlyphFromSelection(feFontGlyph *glyph);
	/** Removes all glyph from the selection. */
	void RemoveAllGlyphsFromSelection();
	
	/** Retrieves the list of selected glyphs. */
	void GetSelectedList(feFontGlyphList &list) const;
	
	/** Retrieves the active glyph or NULL if none is active. */
	inline const feFontGlyph::Ref &GetActiveGlyph() const{ return pActive; }
	/** Determines if there is an active glyph or not. */
	bool HasActiveGlyph() const;
	/** Sets the active glyph or NULL if none is active. */
	void SetActiveGlyph(feFontGlyph *glyph);
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
