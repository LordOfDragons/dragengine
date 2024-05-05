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

#ifndef _CEPCBOPTION_H_
#define _CEPCBOPTION_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class deCanvasView;
class cePlayerChoiceBox;
class deGraphicContext;
class ceCAPlayerChoice;
class ceCAPlayerChoiceOption;



/**
 * \brief Player Choice Box Option.
 */
class cePCBOption : public deObject{
private:
	decUnicodeString pText;
	ceCAPlayerChoice *pAction;
	ceCAPlayerChoiceOption *pActionOption;
	
	deCanvasView *pCanvasView;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create player choice box option. */
	cePCBOption();
	
	/** \brief Clean up player choice box option. */
	virtual ~cePCBOption();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Text. */
	inline const decUnicodeString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const decUnicodeString &text );
	
	/** \brief Action. */
	inline ceCAPlayerChoice *GetAction() const{ return pAction; }
	
	/** \brief Action option. */
	inline ceCAPlayerChoiceOption *GetActionOption() const{ return pActionOption; }
	
	/** \brief Set action option. */
	void SetActionOption( ceCAPlayerChoice *action, ceCAPlayerChoiceOption *actionOption );
	
	/** \brief Canvas view. */
	inline deCanvasView *GetCanvasView() const{ return pCanvasView; }
	
	/** \brief Layout text. Create canvas if not existing. */
	void Layout( const cePlayerChoiceBox &pcbox, bool selected );
	/*@}*/
};

#endif
