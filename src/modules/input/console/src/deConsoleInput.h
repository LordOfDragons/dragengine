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

// include only once
#ifndef _DECONSOLEINPUT_H_
#define _DECONSOLEINPUT_H_

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>

#include <dragengine/input/deInputEvent.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>



/**
 * @brief Console Input Module.
 * Processes input from a console.
 */
class deConsoleInput : public deBaseInputModule{
private:
	termios pOldTermSettings;
	
	deInputEvent::eKeyCodes *pKeyCodeMap;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new console input object. */
	deConsoleInput(deLoadableModule &loadableModule);
	/** Cleans up the console input object. */
	virtual ~deConsoleInput();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/*@{*/
	/**
	 * Called to init the module. Returns true on success. To access the os
	 * object of the engine use the GetOS function.
	 */
	virtual bool Init();
	/** Called to cleanup the module. */
	virtual void CleanUp();
	/*@}*/
	
	
	
	/** \name Devices */
	/*@{*/
	/** \brief Number of input devices. */
	virtual int GetDeviceCount();
	
	/** \brief Information for input device at index. */
	virtual deInputDevice *GetDeviceAt(int index);
	
	/** \brief Index of device with identifier or -1 if absent. */
	virtual int IndexOfDeviceWithID(const char *id);
	
	/** \brief Index of button with identifier on device at index or -1 if absent. */
	virtual int IndexOfButtonWithID(int device, const char *id);
	
	/** \brief Index of axis with identifier on device at index or -1 if absent. */
	virtual int IndexOfAxisWithID(int device, const char *id);
	
	/** \brief Index of feedback with identifier on device at index or -1 if absent. */
	virtual int IndexOfFeedbackWithID(int device, const char *id);
	
	/** \brief Button at index on device at index is pressed down. */
	virtual bool GetButtonPressed(int device, int button);
	
	/** \brief Value of axis at index on device at index. */
	virtual float GetAxisValue(int device, int axis);
	
	/** \brief Value of feedback at index on device at index. */
	virtual float GetFeedbackValue(int device, int feedback);
	
	/** \brief Set value of feedback at index on device at index. */
	virtual void SetFeedbackValue(int device, int feedback, float value);
	
	/**
	 * \brief Index of button best matching key code or -1 if not found.
	 * 
	 * If more than one button matches the key code the input module decides which
	 * button is the more likely choice. Once decided the input module is required
	 * to return the same button for the same key code in subsequent queries unless
	 * a deInputEvent::eeDeviceParamsChanged event is issued.
	 * 
	 * Can be used for example to locate keyboard keys to create default binding
	 * layouts without the user pressing input keys.
	 */
	virtual int ButtonMatchingKeyCode(int device, deInputEvent::eKeyCodes keyCode);
	
	/**
	 * \brief Index of button best matching character or -1 if not found.
	 * 
	 * If more than one button matches the character the input module decides which
	 * button is the more likely choice. Once decided the input module is required
	 * to return the same button for the same character in subsequent queries unless
	 * a deInputEvent::eeDeviceParamsChanged event is issued.
	 * 
	 * Keys usually have different characters depending on the state of modifier keys.
	 * The input module matches character case insensitive to increase the chance to
	 * find a match. If mutliple keys match the character but under different modifier
	 * states the input module has to first select the key matching case sensitive
	 * then the key requiring less modifiers.
	 * 
	 * Can be used for example to locate keyboard keys to create default binding
	 * layouts without the user pressing input keys.
	 */
	virtual int ButtonMatchingKeyChar(int device, int character);
	/*@}*/
	
	
	
	/** @name Events */
	/*@{*/
	/**
	 * Check message or event queues. This function is called
	 * before any other frame related tasks are carried out.
	 * You are expected to check message and event queues to
	 * deliver system notification ( like quitting the game )
	 * to the game engine.
	 */
	virtual void ProcessEvents();
	/*@}*/
	
private:
	void pInitKeyCodeMap();
	
	void pAddKeyPress(int device, int button, int keyChar, deInputEvent::eKeyCodes keyCode,
		const timeval &eventTime);
	void pAddKeyRelease(int device, int button, int keyChar, deInputEvent::eKeyCodes keyCode,
		const timeval &eventTime);
	void pProcessEventQueue();
};

#endif
