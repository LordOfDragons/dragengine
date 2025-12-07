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

#ifndef _DEALWIDGETLAYOUTSTACK_H_
#define _DEALWIDGETLAYOUTSTACK_H_

#include "dealWidgetLayout.h"



/**
 * \brief Stack widget layout.
 * 
 * Overlay widgets ontop of each other either at their minimal size or content area size.
 * 
 */
class dealWidgetLayoutStack : public dealWidgetLayout{
private:
	bool pFullSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetLayoutStack(dealDisplay &display);
	
	/** \brief Create widget. */
	dealWidgetLayoutStack(dealDisplay &display, bool fullSize);
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetLayoutStack();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Resize widgets to content area size. */
	inline bool GetFullSize() const{ return pFullSize; }
	
	/** \brief Set if widgets are resized to content area size. */
	void SetFullSize(bool fullSize);
	
	
	
	/** \brief Minimum size of widget. */
	virtual decPoint GetMinimumSize();
	
	/** \brief Layout child widgets. */
	virtual void LayoutWidgets();
	
	
	
	/** \brief Real minimum size of widget. */
	decPoint GetRealMinimumSize();
	/*@}*/
};

#endif
