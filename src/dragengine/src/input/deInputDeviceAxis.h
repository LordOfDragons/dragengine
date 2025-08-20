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

#ifndef _INPUTDEVICEAXIS_H_
#define _INPUTDEVICEAXIS_H_

#include "../common/string/decString.h"
#include "../common/collection/decObjectOrderedSet.h"
#include "../resources/image/deImageReference.h"


/**
 * \brief Input device axis.
 * 
 * Axes can be any input control a user can alter between a minimum and maximum range.
 * Axes are always a single value. Input elements moving along more than 1 degree of
 * freedom one individual axis is used for each degree of freedom. A mouse has 2 axes
 * one for X movement and one for Y movement. The same applies to an analog stick on
 * a joystick or gamepad. A 3D mouse or joystick with twist support use 3 axes. Complex
 * input controls like racing wheels can have more than 3 axes.
 * 
 * Dead zone and jitter is handled by the input module. The scripting module does not
 * have to worry about this. The input module adjusts the values to hide these parameters.
 * For example for a gamepad the input value is mapped from the range
 * (realMinimum -> deadZone) to (-1, 0) and (deadZone -> realMaximum) to (0, 1). The
 * entire dead zone shows up as 0 value to the scripting module no matter how the user
 * configurated the dead zone.
 * 
 * The minimum, center and maximum values are fixed to -1, 0 and 1. The input module maps
 * the axis range to this value range to simplify the use for script modules. The input
 * handling used by scripts has to matcht the way axes work. A stick matches the -1 to 1
 * range with 0 value if the stick is not touched. A throttle control on the other hand
 * can have the center position at the minimum position. In this case the game script
 * has to map -1 to 0 to get the correct result. The user is responsible to assign the
 * matching input axes to the appropriate game bindings. The game can use the device
 * type to locate potential problems due to misconfiguration. Relative axes work different
 * in that they have no limited range. The returned value is always the last change in
 * position reported which can be larger than 1.
 */
class DE_DLL_EXPORT deInputDeviceAxis{
public:
	/** \brief Axis types. */
	enum eAxisTypes{
		/**
		 * \brief Mouse.
		 * 
		 * Axis is relative and has no range limitation.
		 */
		eatMouse,
		
		/**
		 * \brief Mouse wheel.
		 * 
		 * Wheel is number of scrolled units.
		 */
		eatMouseWheel,
		
		/** \brief Stick. */
		eatStick,
		
		/** \brief Hat. */
		eatHat,
		
		/** \brief Throttle. */
		eatThrottle,
		
		/** \brief Steering wheel. */
		eatSteeringWheel,
		
		/** \brief Touch pad or touch screen. */
		eatTouchPad,
		
		/** \brief Generic. */
		eatGeneric,
		
		/**
		 * \brief Trigger.
		 * \version 1.6
		 */
		eatTrigger,
		
		/**
		 * \brief Controller grip grabing.
		 * \version 1.6
		 * 
		 * Value 0 indicates open hand up to value 1 indicating closed hand.
		 */
		eatGripGrab,
		
		/**
		 * \brief Controller grip squeezing.
		 * \version 1.6
		 * 
		 * Value 0 indicates no force applied up to value 1 indicating maximum force applied.
		 */
		eatGripSqueeze,
		
		/**
		 * \brief Controller grip pinching.
		 * \version 1.6
		 * 
		 * Value 0 indicates no pinching to value 1 indicating maximum pinching.
		 */
		eatGripPinch,
		
		/**
		 * \brief Finger bending.
		 * \version 1.6
		 * 
		 * If finger bending is supported 5 axes of type eatFingerBend are added in this
		 * order: thumb, index, middle, ring, pinky
		 */
		eatFingerBend,
		
		/**
		 * \brief Finger spreading.
		 * \version 1.6
		 * 
		 * If finger spreading is supported 4 axes of type eatFingerSpread are added in
		 * this order: thumb-index, index-middle, middle-ring, ring-pinky.
		 */
		eatFingerSpread,
		
		/**
		 * \brief Battery level.
		 * \version 1.20
		 * 
		 * Indicates the battery charge level in the range from 0 (discharged) to 1 (fully charged).
		 */
		eatBatteryLevel,
		
		/**
		 * \brief Gesture.
		 * \version 1.23
		 * 
		 * Gesture performed by the user. Usually this is the hand but can be also other body
		 * parts or the entire body itself. Value ranges from 0 (not performing gesture) to
		 * 1 (fully performing gesture).
		 */
		eatGesture,
		
		/**
		 * \brief Force applied to trigger when fully pulled.
		 * \version 1.28
		 */
		eatTriggerForce,
		
		/**
		 * \brief Trigger finger curl.
		 * \version 1.28
		 */
		eatTriggerCurl,
		
		/**
		 * \brief Trigger finger slide.
		 * \version 1.28
		 */
		eatTriggerSlide,
		
		/**
		 * \brief Pressing force applied to thumb rest.
		 * \version 1.28
		 */
		eatThumbrestPress,
		
		/**
		 * \brief Trigger simulated by touching two fingers.
		 * \version 1.28
		 */
		eatTwoFingerTrigger
	};
	
	
	
private:
	/** \brief Axis identifier unique in the parent device. */
	decString pID;
	
	/** \brief Display name. */
	decString pName;
	
	/** \brief Axis type. */
	eAxisTypes pType;
	
	/** \brief Identifier of component or empty string. */
	decString pComponent;
	
	/**
	 * \brief Image to represent the axis in 2D user interfaces or NULL if not set.
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
	/** \brief Create input device axis. */
	deInputDeviceAxis();
	
	/** \brief Create copy of input device axis. */
	deInputDeviceAxis( const deInputDeviceAxis &axis );
	
	/** \brief Clean up input device axis. */
	~deInputDeviceAxis();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Axis identifier.
	 * 
	 * Unique identifier for the axis identifying it inside the parent device. The prefix
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
	 * Dispaly name is human readable like for example 'Axis #1'. The name is unique but
	 * not guaranteed to stay the same across restarting the input module or game engine.
	 */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set Display name. */
	void SetName( const char *name );
	
	/** \brief Axis type. */
	inline eAxisTypes GetType() const{ return pType; }
	
	/** \brief Axis type. */
	void SetType( eAxisTypes type );
	
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
	 * \brief Set image to represent the axis in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	void SetDisplayImage( deImage *image );
	
	/** \brief Count of icons representing the axis in bindings. */
	int GetDisplayIconCount() const;
	
	/**
	 * \brief Icon at index representing the axis in bindings.
	 * 
	 * Icon is of square size and typically has a size of 16, 24, 32 or 64.
	 */
	deImage *GetDisplayIconAt( int index ) const;
	
	/**
	 * \brief Add icon representing the axis in bindings.
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
	/** \brief Copy input device axis. */
	deInputDeviceAxis &operator=( const deInputDeviceAxis &axis );
	/*@}*/
};

#endif
