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

#ifndef _FEWPFONT_H_
#define _FEWPFONT_H_

#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/composed/igdeEditPoint.h>

class feFont;
class feWindowProperties;
class feWPFontListener;



/**
 * \brief Font Panel.
 */
class feWPFont : public igdeContainerScroll{
private:
	feWindowProperties &pWindowProperties;
	feFont::Ref pFont;
	feWPFontListener::Ref pListener;
	
	igdeTextField::Ref pEditImagePath;
	igdeButton::Ref pBtnImagePath;
	igdeTextField *pEditLineHeight, pEditBaseLine;
	igdeCheckBox::Ref pChkColorFont;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	feWPFont(feWindowProperties &windowProperties);
	
	/** \brief Clean up panel. */
	virtual ~feWPFont();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline feWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Font. */
	inline feFont *GetFont() const{ return pFont; }
	
	/** \brief Set font. */
	void SetFont(feFont *font);
	
	/** \brief Update font. */
	void UpdateFont();
	/*@}*/
};

#endif
