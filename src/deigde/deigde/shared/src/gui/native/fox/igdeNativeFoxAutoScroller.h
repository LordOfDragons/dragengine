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

#ifndef _IGDENATIVEFOXAUTOSCROLLER_H_
#define _IGDENATIVEFOXAUTOSCROLLER_H_

#include "foxtoolkit.h"


class igdeContainerForm;


/**
 * \brief FOX toolkit Native Auto-Scroller.
 * 
 * If mouse pointer is hovering over this widget scrolls the content to match the mouse
 * pointer. The visible range is mapped to the inner widget size. Hence if the mouse pointer
 * is at the left border scrolls the inner widget to the far left. if the mouse pointer is
 * at the right border scrolls the inner widget to the far right. In between the borders
 * scrolls the inner widget relative.
 */
class igdeNativeFoxAutoScroller : public FXComposite{
	FXDECLARE(igdeNativeFoxAutoScroller)
protected:
	   igdeNativeFoxAutoScroller();
	
public:
	enum eFoxIDs{
		ID_SELF = FXComposite::ID_LAST,
		ID_CHILD_MOUSE_MOVED,
		ID_LAST
	};
	
private:
	int pScrollPosition;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeNativeFoxAutoScroller(FXComposite *parent, int opts = 0);
	
	/** \brief Clean up widget. */
	virtual ~igdeNativeFoxAutoScroller();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Content window or NULL. */
	FXWindow *GetContentWindow() const;
	
	/** \brief Content size. */
	decPoint GetContentSize() const;
	
	/** \brief Scroll position. */
	inline int GetScrollPosition() const{ return pScrollPosition; }
	
	/** \brief Set scroll position. */
	void SetScrollPosition(int position);
	
	virtual FXint getDefaultWidth();
	virtual FXint getDefaultHeight();
	virtual void layout();
	
	void MoveContent(int x);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onChildMouseMoved(FXObject*, FXSelector, void*);
	/*@}*/
};

#endif
