/* 
 * Drag[en]gine X System Input Module
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

#ifndef _DEXSYSTEMINPUT_H_
#define _DEXSYSTEMINPUT_H_

#include <dragengine/input/deInputEvent.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>

#include "dexsiXInclude.h"

class deOSUnix;
class dexsiDeviceManager;



/**
 * \brief input module device identifier prefix.
 */
#define XINP_DEVID_PREFIX "XSys_"



/**
 * \brief X-System input module.
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
	
	int pOldAccelNom;
	int pOldAccelDenom;
	int pOldThreshold;
	
	bool pSystemAutoRepeatEnabled;
	bool pAutoRepeatEnabled;
	
	dexsiDeviceManager *pDevices;
	
	bool *pKeyStates;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deXSystemInput( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deXSystemInput();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief OS. */
	inline deOSUnix *GetOSUnix() const{ return pOSUnix; }
	
	
	
	/**
	 * \brief Init the module.
	 * \returns \em true on success.
	 * \note To access the os object of the engine use the GetOS function.
	 */
	virtual bool Init();
	
	/** \brief Clean up module. */
	virtual void CleanUp();
	/*@}*/
	
	
	
	/** \name Devices */
	/*@{*/
	/** \brief Number of input devices. */
	virtual int GetDeviceCount();
	
	/** \brief Information for input device at index. */
	virtual deInputDevice *GetDeviceAt( int index );
	
	/** \brief Index of device with identifier or -1 if absent. */
	virtual int IndexOfDeviceWithID( const char *id );
	
	/** \brief Index of button with identifier on device at index or -1 if absent. */
	virtual int IndexOfButtonWithID( int device, const char *id );
	
	/** \brief Index of axis with identifier on device at index or -1 if absent. */
	virtual int IndexOfAxisWithID( int device, const char *id );
	
	/** \brief Index of feedback with identifier on device at index or -1 if absent. */
	virtual int IndexOfFeedbackWithID( int device, const char *id );
	
	/** \brief Button at index on device at index is pressed down. */
	virtual bool GetButtonPressed( int device, int button );
	
	/** \brief Value of axis at index on device at index. */
	virtual float GetAxisValue( int device, int axis );
	
	/** \brief Value of feedback at index on device at index. */
	virtual float GetFeedbackValue( int device, int feedback );
	
	/** \brief Set value of feedback at index on device at index. */
	virtual void SetFeedbackValue( int device, int feedback, float value );
	
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
	virtual int ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode );
	
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
	virtual int ButtonMatchingKeyChar( int device, int character );
	
	/**
	 * \brief Index of button best matching key code or -1 if not found.
	 * 
	 * Same as ButtonMatchingKeyChar(int,int) but allows to distinguish between multiple
	 * keys of the same type, for example left and right shift key.
	 */
	virtual int ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode,
		deInputEvent::eKeyLocation location );
	
	/**
	 * \brief Index of button best matching character or -1 if not found.
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
	 * \brief Check state of input devices.
	 * 
	 * This function is called before any other frame related tasks are carried out.
	 * Record changes in devices states have to be recored into a game event
	 * reported as engine input events You are expected to check message and event
	 * queues to deliver system notification (like quitting the game) to the game
	 * engine.
	 */
	virtual void ProcessEvents();
	
	/** \brief Clear event queues in case any are used. */
	virtual void ClearEvents();
	
	/** \brief Capture input devices changed. */
	virtual void CaptureInputDevicesChanged();
	
	/** \brief Application activated or deactivated. */
	virtual void AppActivationChanged();
	
	/** \brief An event processed by the application event loop. */
	virtual void EventLoop( XEvent &event );
	
	
	
	/** \brief Add axis changed event. */
	void AddAxisChanged( int device, int axis, float value, const timeval &eventTime );
	
	/** \brief Add button pressed. */
	void AddButtonPressed( int device, int button, const timeval &eventTime );
	
	/** \brief Add button released. */
	void AddButtonReleased( int device, int button, const timeval &eventTime );
	
	/** \brief Add mouse wheel changed event. */
	void AddMouseWheelChanged( int device, int axis, int x, int y, int state,
		const timeval &eventTime );
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
