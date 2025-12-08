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

#ifndef _CETEXTBOXTEXT_H_
#define _CETEXTBOXTEXT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

#include <dragengine/resources/canvas/deCanvasView.h>
class ceTextBox;
class deGraphicContext;



/**
 * \brief Text Box Text.
 */
class ceTextBoxText : public deObject{
private:
	decUnicodeString pName;
	decUnicodeString pText;
	
	deCanvasView::Ref pCanvasView;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceTextBoxText> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text box text. */
	ceTextBoxText();
	
	/** \brief Clean up text box text. */
	virtual ~ceTextBoxText();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decUnicodeString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const decUnicodeString &name);
	
	/** \brief Text. */
	inline const decUnicodeString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText(const decUnicodeString &text);
	
	/** \brief Canvas view. */
	inline const deCanvasView::Ref &GetCanvasView() const{ return pCanvasView; }
	
	/** \brief Layout text. Create canvas if not existing. */
	void Layout(const ceTextBox &textBox);
	/*@}*/
};

#endif
