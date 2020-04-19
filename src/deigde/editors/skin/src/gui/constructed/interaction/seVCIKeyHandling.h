/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEVCIKEYHANDLING_H_
#define _SEVCIKEYHANDLING_H_

#include <deigde/gui/event/igdeMouseKeyListener.h>

class seViewConstructedView;



/**
 * \brief Key handling listener.
 * 
 * Process single key strokes to edit nodes but only if no mouse interaction is going on.
 */
class seVCIKeyHandling : public igdeMouseKeyListener{
private:
	seViewConstructedView &pView;
	bool pIsLeftMouseButtonPressed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seVCIKeyHandling( seViewConstructedView &view );
	
	/** \brief Clean up listener. */
	virtual ~seVCIKeyHandling();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief User pressed down key while view is focused.
	 */
	virtual void OnKeyPress( igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key );
	
	/**
	 * \brief User released down key while view is focused.
	 */
	virtual void OnKeyRelease( igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key );
	
	/**
	 * \brief User pressed down mouse button.
	 */
	virtual void OnButtonPress( igdeWidget *widget, int button, const decPoint &position, int modifiers );
	
	/**
	 * \brief User released down mouse button.
	 */
	virtual void OnButtonRelease( igdeWidget *widget, int button, const decPoint &position, int modifiers );
	
	/**
	 * \brief User wheeled mouse.
	 */
	virtual void OnMouseWheeled( igdeWidget *widget, const decPoint &position, const decPoint &change, int modifiers );
	
	/**
	 * \brief User double clicked mouse button.
	 */
	virtual void OnDoubleClicked( igdeWidget *widget, int button, const decPoint &position, int modifiers );
	/*@}*/
	
	
	
protected:
	void ShowContextMenu( const decPoint &position );
};

#endif
