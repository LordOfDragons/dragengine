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
