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
class DE_DLL_EXPORT deInputDeviceButton{
public:
	/** \brief Button types. */
	enum eButtonTypes{
		/**
		 * \brief Generic button.
		 * \version 1.6
		 */
		ebtGeneric,
		
		/**
		 * \brief Action button.
		 * \version 1.6
		 * 
		 * Button with label or symbol like "A", "B", "X", "Y" or "1", "2". These buttons
		 * are used to trigger game actions and are typically grouped together in blocks
		 * of 4 or 6 buttons.
		 */
		ebtAction,
		
		/**
		 * \brief Start/Home button.
		 * \version 1.6
		 * 
		 * Button with label "Start" or "Home" or showing a symbol version thereof.
		 * This button is used to pause a game or returning to a home screen.
		 */
		ebtHome,
		
		/**
		 * \brief Select button.
		 * \version 1.6
		 * 
		 * Button with label "Select" or showing a symbol version thereof. This button
		 * is usually used to trigger in-game or application menus or to alter the
		 * functionality of action buttons.
		 */
		ebtSelect,
		
		/**
		 * \brief Left shoulder button.
		 * \version 1.6
		 * 
		 * Buttons located at the top left side of the device. They are usually labelled
		 * or are color coded to distinguish them. Buttons of this type are ordered from
		 * top to bottom.
		 */
		ebtShoulderLeft,
		
		/**
		 * \brief Right shoulder button.
		 * \version 1.6
		 * 
		 * Buttons located at the top right side of the device. They are usually labelled
		 * or are color coded to distinguish them. Buttons of this type are ordered from
		 * top to bottom.
		 */
		ebtShoulderRight,
		
		/**
		 * \brief Trigger button.
		 * \version 1.6
		 * 
		 * Click point of a trigger.
		 */
		ebtTrigger,
		
		/**
		 * \brief Stick button.
		 * \version 1.6
		 * 
		 * Click point of a stick.
		 */
		ebtStick,
		
		/**
		 * \brief Touch pad tap.
		 * \version 1.6
		 * 
		 * Tap of a touch pad.
		 */
		ebtTouchPad
	};
	
	
	
private:
	decString pID;
	decString pName;
	eButtonTypes pType;
	decString pComponent;
	deImageReference pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	bool pTouchable;
	
	
	
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
	
	/** \brief Button type. */
	inline eButtonTypes GetType() const{ return pType; }
	
	/** \brief Button type. */
	void SetType( eButtonTypes type );
	
	/**
	 * \brief Identifier of component or empty string.
	 * \version 1.6
	 */
	inline const decString &GetComponent() const{ return pComponent; }
	
	/**
	 * \brief Set identifier of component or empty string.
	 * \version 1.6
	 */
	void SetComponent( const char *component );
	
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
	
	/**
	 * \brief Button supports sending touch/untouch events.
	 * \version 1.11.1
	 */
	inline bool GetTouchable() const{ return pTouchable; }
	
	/**
	 * \brief Set if button supports sending touch/untouch events.
	 * \version 1.11.1
	 */
	void SetTouchable( bool touchable );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy input device button. */
	deInputDeviceButton &operator=( const deInputDeviceButton &button );
	/*@}*/
};

#endif
