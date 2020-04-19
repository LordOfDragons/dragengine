/* 
 * Drag[en]gine Game Engine
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

#ifndef _INPUTDEVICEBUTTON_H_
#define _INPUTDEVICEBUTTON_H_

#include "../common/string/decString.h"
#include "../common/collection/decObjectOrderedSet.h"
#include "../resources/image/deImageReference.h"


/**
 * \brief Input device button.
 * 
 * Buttons can be anything a user can press. This can be a mouse button,
 * buttons on keypads, keys on keyboards or touch fields on screens.
 * Buttons have a pressed or not pressed state. Pressure sensitive input
 * elements are handled as axes not buttons.
 */
class deInputDeviceButton{
private:
	/** \brief Button identifier unique in the parent device. */
	decString pID;
	
	/** \brief Display name. */
	decString pName;
	
	/**
	 * \brief Image to represent the button in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	deImageReference pDisplayImage;
	
	/** \brief List of small icons of different size for use in binding displays. */
	decObjectOrderedSet pDisplayIcons;
	
	/** \brief Text to display centered across display image or icon. */
	decString pDisplayText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create input device button. */
	deInputDeviceButton();
	
	/** \brief Create input device button. */
	deInputDeviceButton( const deInputDeviceButton &button );
	
	/** \brief Clean up input device button. */
	~deInputDeviceButton();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Button identifier.
	 * 
	 * Unique identifier for the button identifying it inside the parent device. The prefix
	 * is a normalized string that contains only letters, numbers and underscores. It is
	 * suitable to be combined with the device identifier to store them in config files as
	 * key bindings.
	 */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID( const char *id );
	
	/**
	 * \brief Display name.
	 * 
	 * Dispaly name is human readable like for example 'Button #1'. The name is unique but
	 * not guaranteed to stay the same across restarting the input module or game engine.
	 */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set Display name. */
	void SetName( const char *name );
	
	/**
	 * \brief Image to represent the device in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/**
	 * \brief Set image to represent the button in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	void SetDisplayImage( deImage *image );
	
	/** \brief Count of icons representing the button in bindings. */
	int GetDisplayIconCount() const;
	
	/**
	 * \brief Icon at index representing the button in bindings.
	 * 
	 * Icon is of square size and typically has a size of 16, 24, 32 or 64.
	 */
	deImage *GetDisplayIconAt( int index ) const;
	
	/**
	 * \brief Add icon representing the button in bindings.
	 * 
	 * Icon is of square size and typically has a size of 16, 24, 32 or 64.
	 */
	void AddDisplayIcon( deImage *image );
	
	/** \brief Text to display centered across display image or icon. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** \brief Set text to display centered across display image or icon. */
	void SetDisplayText( const char *text );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy input device button. */
	deInputDeviceButton &operator=( const deInputDeviceButton &button );
	/*@}*/
};

#endif
