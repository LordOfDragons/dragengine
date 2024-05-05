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
class DE_DLL_EXPORT deInputDeviceFeedback{
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
	
	/** \brief Identifier of component or empty string. */
	decString pComponent;
	
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
