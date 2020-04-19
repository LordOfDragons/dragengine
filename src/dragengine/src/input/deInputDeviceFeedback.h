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

#ifndef _INPUTDEVICEFEEDBACK_H_
#define _INPUTDEVICEFEEDBACK_H_

#include "../common/string/decString.h"
#include "../common/collection/decObjectOrderedSet.h"
#include "../resources/image/deImageReference.h"


/**
 * \brief Input device feedback.
 * 
 * Feedback provides support for games to give a feedback in the form of lights, vibration
 * or other haptic feeling. The maximum is fixed to 1 to make it simpler for scripts to work
 * with. The null point is 0 where the feedback is switched off. Using the input module the
 * value  of feedback can be requested to be changed. The input module itself decides if the
 * request is honored or not. The current value of the feedback can be read any time.
 */
class deInputDeviceFeedback{
public:
	/** \brief Feedback types. */
	enum eFeedbackTypes{
		/** \brief Vibration. */
		eftVibration,
		
		/** \brief Light. */
		eftLight,
		
		/** \brief Generic. */
		eftGeneric
	};
	
	
	
private:
	/** \brief Feedback identifier unique in the parent device. */
	decString pID;
	
	/** \brief Display name. */
	decString pName;
	
	/** \brief Feedback type. */
	eFeedbackTypes pType;
	
	/**
	 * \brief Image to represent the feedback in 2D user interfaces or NULL if not set.
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
	/** \brief Create input device feedback. */
	deInputDeviceFeedback();
	
	/** \brief Create copy of input device feedback. */
	deInputDeviceFeedback( const deInputDeviceFeedback &feedback );
	
	/** \brief Clean up input device feedback. */
	~deInputDeviceFeedback();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Feedback identifier.
	 * 
	 * Unique identifier for the feedback identifying it inside the parent device. The prefix
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
	 * Dispaly name is human readable like for example 'Feedback #1'. The name is unique but
	 * not guaranteed to stay the same across restarting the input module or game engine.
	 */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set Display name. */
	void SetName( const char *name );
	
	/** \brief Feedback type. */
	inline eFeedbackTypes GetType() const{ return pType; }
	
	/** \brief Feedback type. */
	void SetType( eFeedbackTypes type );
	
	/**
	 * \brief Image to represent the device in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/**
	 * \brief Set image to represent the feedback in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	void SetDisplayImage( deImage *image );
	
	/** \brief Count of icons representing the feedback in bindings. */
	int GetDisplayIconCount() const;
	
	/**
	 * \brief Icon at index representing the feedback in bindings.
	 * 
	 * Icon is of square size and typically has a size of 16, 24, 32 or 64.
	 */
	deImage *GetDisplayIconAt( int index ) const;
	
	/**
	 * \brief Add icon representing the feedback in bindings.
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
	/** \brief Copy input device feedback. */
	deInputDeviceFeedback &operator=( const deInputDeviceFeedback &feedback );
	/*@}*/
};

#endif
