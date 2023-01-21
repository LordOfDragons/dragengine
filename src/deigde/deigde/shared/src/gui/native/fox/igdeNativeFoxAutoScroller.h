/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	FXDECLARE( igdeNativeFoxAutoScroller )
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
	igdeNativeFoxAutoScroller( FXComposite *parent, int opts = 0 );
	
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
	void SetScrollPosition( int position );
	
	virtual FXint getDefaultWidth();
	virtual FXint getDefaultHeight();
	virtual void layout();
	
	void MoveContent( int x );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onChildMouseMoved( FXObject*, FXSelector, void* );
	/*@}*/
};

#endif
