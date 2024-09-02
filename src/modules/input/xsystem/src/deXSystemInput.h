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

#ifndef _DEXSYSTEMINPUT_H_
#define _DEXSYSTEMINPUT_H_

#include <dragengine/input/deInputEvent.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>

#include "dexsiXInclude.h"
#include "dexsiDeviceManager.h"

class deOSUnix;



/**
 * input module device identifier prefix.
 */
#define XINP_DEVID_PREFIX "XSys_"



/**
 * X-System input module.
 */
class deXSystemInput : public deBaseInputModule{
private:
	struct sKey{
		int button;
		int virtualKeyCode;
		KeySym keySym;
		int character;
	};
	
	deOSUnix *pOSUnix;
	
	int pWindowWidth;
	int pWindowHeight;
	
	int pMouseButtons;
	int pLastMouseX;
	int pLastMouseY;
	
	bool pIsListening;
	
	bool pSystemAutoRepeatEnabled;
	bool pAutoRepeatEnabled;
	
	dexsiDeviceManager::Ref pDevices;
	
	bool *pKeyStates;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deXSystemInput( deLoadableModule &loadableModule );
	
	/** Clean up module. */
	virtual ~deXSystemInput();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** OS. */
	inline deOSUnix *GetOSUnix() const{ return pOSUnix; }
	
	/** Device manager. */
	inline const dexsiDeviceManager::Ref &GetDevices() const{ return pDevices; }
	
	
	
	/**
	 * Init the module.
	 * \returns \em true on success.
	 * \note To access the os object of the engine use the GetOS function.
	 */
	virtual bool Init();
	
	/** Clean up module. */
	virtual void CleanUp();
	/*@}*/
	
	
	
	/** \name Devices */
	/*@{*/
	/** Number of input devices. */
	virtual int GetDeviceCount();
	
	/** Information for input device at index. */
	virtual deInputDevice *GetDeviceAt( int index );
	
	/** Index of device with identifier or -1 if absent. */
	virtual int IndexOfDeviceWithID( const char *id );
	
	/** Index of button with identifier on device at index or -1 if absent. */
	virtual int IndexOfButtonWithID( int device, const char *id );
	
	/** Index of axis with identifier on device at index or -1 if absent. */
	virtual int IndexOfAxisWithID( int device, const char *id );
	
	/** Index of feedback with identifier on device at index or -1 if absent. */
	virtual int IndexOfFeedbackWithID( int device, const char *id );
	
	/** Button at index on device at index is pressed down. */
	virtual bool GetButtonPressed( int device, int button );
	
	/** Value of axis at index on device at index. */
	virtual float GetAxisValue( int device, int axis );
	
	/** Value of feedback at index on device at index. */
	virtual float GetFeedbackValue( int device, int feedback );
	
	/** Set value of feedback at index on device at index. */
	virtual void SetFeedbackValue( int device, int feedback, float value );
	
	/**
	 * Index of button best matching key code or -1 if not found.
	 * 
	 * If more than one button matches the key code the input module decides which
	 * button is the more likely choice. Once decided the input module is required
	 * to return the same button for the same key code in subsequent queries unless
	 * a deInputEvent::eeDeviceParamsChanged event is issued.
	 * 
	 * Can be used for example to locate keyboard keys to create default binding
	 * layouts without the user pressing input keys.
	 */
	virtual int ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode );
	
	/**
	 * Index of button best matching character or -1 if not found.
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
	virtual int ButtonMatchingKeyChar( int device, int character );
	
	/**
	 * Index of button best matching key code or -1 if not found.
	 * 
	 * Same as ButtonMatchingKeyChar(int,int) but allows to distinguish between multiple
	 * keys of the same type, for example left and right shift key.
	 */
	virtual int ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode,
		deInputEvent::eKeyLocation location );
	
	/**
	 * Index of button best matching character or -1 if not found.
	 * 
	 * Same as ButtonMatchingKeyChar(int,int) but allows to distinguish between multiple
	 * keys of the same type, for example left and right shift key.
	 */
	virtual int ButtonMatchingKeyChar( int device, int character,
		deInputEvent::eKeyLocation location );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/**
	 * Check state of input devices.
	 * 
	 * This function is called before any other frame related tasks are carried out.
	 * Record changes in devices states have to be recored into a game event
	 * reported as engine input events You are expected to check message and event
	 * queues to deliver system notification (like quitting the game) to the game
	 * engine.
	 */
	virtual void ProcessEvents();
	
	/** Clear event queues in case any are used. */
	virtual void ClearEvents();
	
	/** Capture input devices changed. */
	virtual void CaptureInputDevicesChanged();
	
	/** Application activated or deactivated. */
	virtual void AppActivationChanged();
	
	/** An event processed by the application event loop. */
	virtual void EventLoop( XEvent &event );
	
	
	
	/** Add axis changed event. */
	void AddAxisChanged( int device, int axis, float value, const timeval &eventTime );
	
	/** Add button pressed. */
	void AddButtonPressed( int device, int button, const timeval &eventTime );
	
	/** Add button released. */
	void AddButtonReleased( int device, int button, const timeval &eventTime );
	
	/** Add mouse wheel changed event. */
	void AddMouseWheelChanged( int device, int axis, int x, int y, int state,
		const timeval &eventTime );
	
	/** Add device attached/detached event. */
	void AddDeviceAttachedDetached( const timeval &eventTime );
	/*@}*/
	
	
	
private:
	void pCenterPointer();
	
	void pAddKeyPress( int device, int button, int keyChar,
		deInputEvent::eKeyCodes keyCode, int state, const timeval &eventTime );
	
	void pAddKeyRelease( int device, int button, int keyChar,
		deInputEvent::eKeyCodes keyCode, int state, const timeval &eventTime );
	
	void pAddMousePress( int device, int button, int state, const timeval &eventTime );
	void pAddMouseRelease( int device, int button, int state, const timeval &eventTime );
	void pAddMouseMove( int device, int state, int x, int y, const timeval &eventTime );
	void pQueryMousePosition( bool sendEvents );
	int pModifiersFromXState( int xstate ) const;
	bool pLookUpKey( XKeyEvent &event, sKey &key );
	//int pModifiersFromKeyState() const;
	void pUpdateAutoRepeat();
	void pSetAutoRepeatEnabled( bool enabled );
};

#endif
