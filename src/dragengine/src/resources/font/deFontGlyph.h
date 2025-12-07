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

#ifndef _DEFONTGLYPH_H_
#define _DEFONTGLYPH_H_

#include "../../dragengine_export.h"


/**
 * \brief Text font glyph definition.
 */
class DE_DLL_EXPORT deFontGlyph{
private:
	int pUnicode, pX, pY, pZ, pWidth, pHeight, pBearing, pBearingY, pAdvance;
	
	
	
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
	void SetUnicode(int unicode);
	
	/** \brief Left side of glyph in pixels on image map. */
	inline int GetX() const{ return pX; }
	
	/** \brief Left side of glyph in pixels on image map. */
	void SetX(int x);
	
	/** \brief Top side of glyph in pixels on image map. */
	inline int GetY() const{ return pY; }
	
	/** \brief Top side of glyph in pixels on image map. */
	void SetY(int y);
	
	/** \brief Layer of image map the glyph is located on. */
	inline int GetZ() const{ return pZ; }
	
	/** \brief Layer of image map the glyph is located on. */
	void SetZ(int z);
	
	/** \brief Width of glyph in pixels on image map. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Width of glyph in pixels on image map. */
	void SetWidth(int width);
	
	/** \brief Height of glyph in pixels on image map. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Height of glyph in pixels on image map. */
	void SetHeight(int height);
	
	/** \brief Horizontal bearing in pixels. */
	inline int GetBearing() const{ return pBearing; }
	
	/** \brief Horizontal bearing in pixels. */
	void SetBearing(int bearing);
	
	/** \brief Vertical bearing in pixels. */
	inline int GetBearingY() const{ return pBearingY; }
	
	/** \brief Vertical bearing in pixels. */
	void SetBearingY(int bearing);
	
	/** \brief Horizontal advance in pixels. */
	inline int GetAdvance() const{ return pAdvance; }
	
	/** \brief Horizontal advance in pixels. */
	void SetAdvance(int advance);
};

#endif
