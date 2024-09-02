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
