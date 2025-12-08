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

#ifndef _FEWPGLYPH_H_
#define _FEWPGLYPH_H_

#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class feFont;
class feFontGlyph;
class feWindowProperties;
class feWPGlyphListener;



/**
 * \brief Glyph panel.
 */
class feWPGlyph : public igdeContainerScroll{
private:
	feWindowProperties &pWindowProperties;
	feFont::Ref pFont;
	feFontGlyph::Ref pGlyph;
	feWPGlyphListener::Ref pListener;
	
	igdeComboBox::Ref pCBGlyph;
	igdeButton *pBtnGlyphAdd, pBtnGlyphDel;
	igdeTextField *pEditU, pEditV, pEditWidth, pEditHeight, pEditBearing,
		pEditBearingY, pEditAdvance;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	feWPGlyph(feWindowProperties &windowProperties);
	
	/** \brief Clean up panel. */
	virtual ~feWPGlyph();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline feWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Font. */
	inline const feFont::Ref &GetFont() const{ return pFont; }
	
	/** \brief Set font. */
	void SetFont(feFont *font);
	
	/** \brief Glyph. */
	inline const feFontGlyph::Ref &GetGlyph() const{ return pGlyph; }
	
	/** \brief Set glyph. */
	void SetGlyph(feFontGlyph *glyph);
	
	/** \brief Update glyph list. */
	void UpdateGlyphList();
	
	/** \brief Update glyph. */
	void UpdateGlyph();
	/*@}*/
};

#endif
