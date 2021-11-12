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

#ifndef _DEBASEINPUTMODULE_H_
#define _DEBASEINPUTMODULE_H_

#include <sys/time.h>

#if defined OS_UNIX && defined HAS_LIB_X11
#include <X11/Xlib.h>
#endif

#ifdef OS_W32
#include "../../../app/include_windows.h"
#endif

#include "../deBaseModule.h"

class deInputDevice;
class deInputDevicePose;
class deInputEvent;

#ifdef OS_ANDROID
struct AInputEvent;
#endif

#ifdef OS_BEOS
class BMessage;
#endif

#ifdef OS_MACOS
#ifdef __OBJC__
@class NSEvent;
#else
class NSEvent;
#endif
#endif

#include "../../../input/deInputEvent.h"


/**
 * \brief Base Input Module.
 */
class deBaseInputModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseInputModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseInputModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Init the module.
	 * \returns true on success.
	 * \note To access the os object of the engine use the GetOS function.
	 */
	virtual bool Init() = 0;
	
	/** \brief Clean up module. */
	virtual void CleanUp() = 0;
	/*@}*/
	
	
	
	/** \name Devices */
	/*@{*/
	/** \brief Number of input devices. */
	virtual int GetDeviceCount() = 0;
	
	/** \brief Information for input device at index. */
	virtual deInputDevice *GetDeviceAt( int index ) = 0;
	
	/** \brief Index of device with identifier or -1 if absent. */
	virtual int IndexOfDeviceWithID( const char *id ) = 0;
	
	/** \brief Index of button with identifier on device at index or -1 if absent. */
	virtual int IndexOfButtonWithID( int device, const char *id ) = 0;
	
	/** \brief Index of axis with identifier on device at index or -1 if absent. */
	virtual int IndexOfAxisWithID( int device, const char *id ) = 0;
	
	/** \brief Index of feedback with identifier on device at index or -1 if absent. */
	virtual int IndexOfFeedbackWithID( int device, const char *id ) = 0;
	
	/**
	 * \brief Index of component with identifier on device at index or -1 if absent.
	 * \version 1.6
	 */
	virtual int IndexOfComponentWithID( int device, const char *id );
	
	/** \brief Button at index on device is pressed down. */
	virtual bool GetButtonPressed( int device, int button ) = 0;
	
	/**
	 * \brief Button at index on device is touched.
	 * \version 1.6
	 */
	virtual bool GetButtonTouched( int device, int button );
	
	/** \brief Value of axis at index on device. */
	virtual float GetAxisValue( int device, int axis ) = 0;
	
	/** \brief Value of feedback at index on device. */
	virtual float GetFeedbackValue( int device, int feedback ) = 0;
	
	/** \brief Set value of feedback at index on device. */
	virtual void SetFeedbackValue( int device, int feedback, float value ) = 0;
	
	/**
	 * \brief Device pose or identity if not supported.
	 * \version 1.6
	 */
	virtual void GetDevicePose( int device, deInputDevicePose &pose );
	
	/**
	 * \brief Device bone pose or identity if not supported.
	 * \version 1.6
	 */
	virtual void GetDeviceBonePose( int device, int bone,
		bool withController, deInputDevicePose &pose );
	
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
	virtual int ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode ) = 0;
	
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
	virtual int ButtonMatchingKeyChar( int device, int character ) = 0;
	
	/**
	 * \brief Index of button best matching key code or -1 if not found.
	 * \version 1.7
	 * 
	 * Same as ButtonMatchingKeyChar(int,int) but allows to distinguish between multiple
	 * keys of the same type, for example left and right shift key.
	 */
	virtual int ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode,
		deInputEvent::eKeyLocation location );
	
	/**
	 * \brief Index of button best matching character or -1 if not found.
	 * \version 1.7
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
	
	/** \brief Screen size changed. */
	virtual void ScreenSizeChanged();
	
	/** \brief Capture input devices changed. */
	virtual void CaptureInputDevicesChanged();
	
	/** \brief Application activated or deactivated. */
	virtual void AppActivationChanged();
	
	/** \brief An event processed by the application event loop. */
	#if defined OS_UNIX && defined HAS_LIB_X11
	virtual void EventLoop( XEvent &event );
	#endif
	
	#ifdef OS_W32
	virtual void EventLoop( const MSG &message );
	#endif
	
	#ifdef OS_ANDROID
	virtual void EventLoop( const AInputEvent &event );
	#endif
	
	#ifdef OS_BEOS
	virtual void EventLoop( const BMessage &message );
	#endif
	
	#ifdef OS_MACOS
	virtual void EventLoop( const NSEvent &event );
	#endif
	/*@}*/
};

#endif
