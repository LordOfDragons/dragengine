/* 
 * Drag[en]gine MacOS Input Module
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

#ifndef _DEMACOSINPUT_H_
#define _DEMACOSINPUT_H_

#include <dragengine/input/deInputEvent.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>

class deOSMacOS;
class demoiDeviceManager;



/** \brief Input module device identifier prefix. */
#define MOINP_DEVID_PREFIX "MOInp"



/**
 * \brief MacOS input module.
 */
class deMacOSInput : public deBaseInputModule{
private:
	deOSMacOS *pOSMacOS;
	
	int pWindowWidth;
	int pWindowHeight;
	
	int pMouseButtons;
	int pLastMouseX;
	int pLastMouseY;
	
	bool pIsListening;
	
	bool *pKeyStates;
	int pModifierStates;
	bool pSystemAutoRepeatEnabled;
	bool pAutoRepeatEnabled;
	
	demoiDeviceManager *pDevices;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deMacOSInput( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deMacOSInput();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
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
	virtual void EventLoop( const NSEvent &event );
    
    
    
    /** \brief Axis changed value. */
    void AddAxisChanged( int device, int axis, float value, const timeval &eventTime );
	/*@}*/
	
	
	
private:
	void pCenterPointer();
	void pAddKeyPress( int device, int button, int keyChar, deInputEvent::eKeyCodes keyCode,
		int state, const timeval &eventTime );
	void pAddKeyRelease( int device, int button, int keyChar, deInputEvent::eKeyCodes keyCode,
		int state, const timeval &eventTime );
	void pAddMousePress( int device, int button, int state, const timeval &eventTime );
	void pAddMouseRelease( int device, int button, int state, const timeval &eventTime );
	void pAddMouseMove( int device, int state, int x, int y, const timeval &eventTime );
    void pAddMouseWheel( int device, int axis, int x, int y, int state,
        const timeval &eventTime );
	void pQueryMousePosition( bool sendEvents );
	int pModifiersFromEvent( int modifierFlags ) const;
	int pModifiersFromKeyState() const;
	void pUpdateAutoRepeat();
	void pSetAutoRepeatEnabled( bool enabled );
};

#endif
