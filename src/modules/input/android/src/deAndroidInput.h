/* 
 * Drag[en]gine Android Input Module
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

#ifndef _DEANDROIDINPUT_H_
#define _DEANDROIDINPUT_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>

#include <android/configuration.h>
#include <android/native_activity.h>
#include <android/sensor.h>

class deOSAndroid;
class deaiOverlaySystem;
class deaiDeviceManager;
class deFont;



/**
 * \brief input module device identifier prefix.
 */
#define AINP_DEVID_PREFIX "AInp_"


/**
 * \brief Android input module.
 */
class deAndroidInput : public deBaseInputModule{
private:
	deOSAndroid *pOSAndroid;
	
	int pMouseButtons;
	bool pIsListening;
	int pOldThreshold;
	int pLastMouseX;
	int pLastMouseY;
	bool *pKeyStates;
	int pPointerMouse;
	
	deaiOverlaySystem *pOverlaySystem;
	
	deFont *pFontDefault;
	
	decTimer pInputTimer;
	float pElapsedTime;
	
	deaiDeviceManager *pDevices;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create input module. */
	deAndroidInput( deLoadableModule &loadableModule );
	
	/** \brief Clean up input module. */
	virtual ~deAndroidInput();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Initialize module. */
	bool Init();
	
	/** \brief Clean up module. */
	void CleanUp();
	
	
	
	/** \brief Screen size. */
	decPoint GetScreenSize() const;
	
	/** \brief Overlay system. */
	inline deaiOverlaySystem *GetOverlaySystem() const{ return pOverlaySystem; }
	
	/** \brief Default font. */
	inline deFont *GetDefaultFont() const{ return pFontDefault; }
	
	/** \brief Elapsed time since the last event processing. */
	inline float GetElapsedTime() const{ return pElapsedTime; }
	
	/** \brief Devices manager. */
	inline deaiDeviceManager &GetDevices() const{ return *pDevices; }
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
	 * \details This function is called before any other frame related tasks are carried out.
	 *          Record changes in devices states have to be recored into a game event reported
	 *          as engine input events You are expected to check message and event queues to
	 *          deliver system notification (like quitting the game) to the game engine.
	 */
	virtual void ProcessEvents();
	
	/** \brief Clear event queues in case any are used. */
	virtual void ClearEvents();
	
	/** \brief Notify module screen size changed in case it uses an overlay canvas. */
	virtual void ScreenSizeChanged();
	
	/** \brief An event processed by the application event loop. */
	virtual void EventLoop( const AInputEvent &event );
	
	
	
	/** \brief Timeval of now. */
	timeval TimeValNow() const;
	
	/** \brief Add key press event to game input queue. */
	void AddKeyPress( int device, int button, int keyChar,
		deInputEvent::eKeyCodes keyCode, const timeval &eventTime );
	
	/** \brief Add key release event to game input queue. */
	void AddKeyRelease( int device, int button, int keyChar,
		deInputEvent::eKeyCodes keyCode, const timeval &eventTime );
	
	/** \brief Add mouse press event to game input queue. */
	void AddMousePress( int device, int button, int state, const timeval &eventTime );
	
	/** \brief Add mouse release event to game input queue. */
	void AddMouseRelease( int device, int button, int state, const timeval &eventTime );
	
	/** \brief Add mouse move event to game input queue. */
	void AddMouseMove( int device, int state, int x, int y, const timeval &eventTime );
	
	/** \brief Add axis changed event. */
	void AddAxisChanged( int device, int axis, float value, const timeval &eventTime );
	
	/** \brief Add button pressed. */
	void AddButtonPressed( int device, int button, const timeval &eventTime );
	
	/** \brief Add button released. */
	void AddButtonReleased( int device, int button, const timeval &eventTime );
	/*@}*/
	
	
	
private:
	void pCenterPointer();
	int pModifiersFromEvent( const AInputEvent &event ) const;
};

#endif
