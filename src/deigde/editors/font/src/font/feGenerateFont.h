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

#ifndef _FEGENERATEFONT_H_
#define _FEGENERATEFONT_H_

#include <deigde/gui/resources/igdeFont.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/resources/font/deFont.h>

class feFontImage;
class feFontGlyph;
class feFont;

class igdeEnvironment;



/**
 * \brief Generate Font.
 */
class feGenerateFont{
private:
	igdeEnvironment &pEnvironment;
	
	int pFirstCode;
	int pLastCode;
	int pMinImageWidth;
	int pMaxImageWidth;
	int pBorderSize;
	int pEnlargeGlyph;
	igdeFont::sConfiguration pFontConfig;
	deFont::Ref pSystemFont;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font generator. */
	feGenerateFont(igdeEnvironment &environment);
	
	/** \brief Clean up font generator. */
	~feGenerateFont();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set font configuration to use for generating. */
	void SetFontConfig(const igdeFont::sConfiguration &config);
	
	/** \brief Set range to generate glyphs for. */
	void SetCodeRange(int first, int last);
	
	/** \brief Border size in pixels. */
	inline int GetBorderSize() const{ return pBorderSize; }
	
	/** \brief Set border size in pixels. */
	void SetBorderSize(int borderSize);
	
	/** \brief Amount of pixels to enlarge the glyph box with. */
	inline int GetEnlargeGlyph() const{ return pEnlargeGlyph; }
	
	/** \brief Set amount of pixels to enlarge the glyph box with. */
	void SetEnlargeGlpyh(int enlarge);
	
	/** \brief Generate font with properties. */
	feFont::Ref GenerateFont();
	/*@}*/
	
	
	
private:
	void pAddGlyphs(feFont &font);
	void pCalcLayout(feFont &font);
	void pRenderImage(feFont &font);
};

#endif
