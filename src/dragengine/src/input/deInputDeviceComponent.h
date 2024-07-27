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

#ifndef _INPUTDEVICECOMPONENT_H_
#define _INPUTDEVICECOMPONENT_H_

#include "../common/string/decString.h"
#include "../common/collection/decObjectOrderedSet.h"
#include "../resources/image/deImageReference.h"


/**
 * \brief Input device component.
 * \version 1.6
 * 
 * Components group buttons, axes and feedbacks into a single unit. For example a joystick
 * on a gamepad typically composes of 2 axes and optionally buttons (for example press
 * down) and feedbacks (rumbling). Using components enables the user to relate these
 * input features.
 */
class DE_DLL_EXPORT deInputDeviceComponent{
public:
	/** \brief Component types. */
	enum eComponentTypes{
		/** \brief Generic component. */
		ectGeneric,
		
		/** \brief Button. */
		ectButton,
		
		/** \brief Trigger. */
		ectTrigger,
		
		/** \brief Joystick. */
		ectJoystick,
		
		/** \brief Touch pad. */
		ectTouchPad,
		
		/** \brief Gesture. */
		ectGesture
	};
	
	
	
private:
	/** \brief Component identifier unique in the parent device. */
	decString pID;
	
	/** \brief Display name. */
	decString pName;
	
	/** \brief Component type. */
	eComponentTypes pType;
	
	/**
	 * \brief Image to represent the component in 2D user interfaces or NULL if not set.
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
	/** \brief Create input device component. */
	deInputDeviceComponent();
	
	/** \brief Create input device component. */
	deInputDeviceComponent( const deInputDeviceComponent &component );
	
	/** \brief Clean up input device component. */
	~deInputDeviceComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Component identifier.
	 * 
	 * Unique identifier for the component identifying it inside the parent device. The prefix
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
	 * Dispaly name is human readable like for example 'Component #1'. The name is unique but
	 * not guaranteed to stay the same across restarting the input module or game engine.
	 */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set Display name. */
	void SetName( const char *name );
	
	/** \brief Component type. */
	inline eComponentTypes GetType() const{ return pType; }
	
	/** \brief Component type. */
	void SetType( eComponentTypes type );
	
	/**
	 * \brief Image to represent the device in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/**
	 * \brief Set image to represent the component in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	void SetDisplayImage( deImage *image );
	
	/** \brief Count of icons representing the component in bindings. */
	int GetDisplayIconCount() const;
	
	/**
	 * \brief Icon at index representing the component in bindings.
	 * 
	 * Icon is of square size and typically has a size of 16, 24, 32 or 64.
	 */
	deImage *GetDisplayIconAt( int index ) const;
	
	/**
	 * \brief Add icon representing the component in bindings.
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
	/** \brief Copy input device component. */
	deInputDeviceComponent &operator=( const deInputDeviceComponent &component );
	/*@}*/
};

#endif
