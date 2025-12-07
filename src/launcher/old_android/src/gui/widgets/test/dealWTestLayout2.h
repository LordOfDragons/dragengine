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

#ifndef _DEALWTESTLAYOUT2_H_
#define _DEALWTESTLAYOUT2_H_

#include "../dealWidgetLayout.h"
#include "../dealWidgetLayoutFlow.h"
#include "../dealWidgetLabel.h"



/**
 * \brief Layout test widget.
 */
class dealWTestLayout2 : public dealWidgetLayoutFlow{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWTestLayout2(dealDisplay &display);
	
	/** \brief Clean up widget. */
	virtual ~dealWTestLayout2();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Add layout of three sample texts. */
	void AddLayout(dealWidgetLabel::eAlignments verticalAlign);
	
	/** \brief Add layout of three sample texts. */
	void AddLayout2(dealWidgetLabel::eAlignments verticalAlign);
	
	/** \brief Add layout of a long sample text. */
	void AddLayout3(dealWidgetLabel::eAlignments align);
	/*@}*/
};

#endif
