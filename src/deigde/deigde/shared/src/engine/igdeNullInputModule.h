/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDENULLINPUTMODULE_H_
#define _IGDENULLINPUTMODULE_H_


#include "dragengine/systems/modules/deInternalModule.h"
#include "dragengine/systems/modules/input/deBaseInputModule.h"



/**
 * \brief Null Input Module.
 * 
 * Provides an input module which does nothing as the input is handled by the map editor already.
 */
class DE_DLL_EXPORT igdeNullInputModule : public deBaseInputModule{
public:
	class cModule : public deInternalModule{
	public:
		cModule( deModuleSystem *system );
		virtual ~cModule();
		virtual void CreateModule();
	};
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	igdeNullInputModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~igdeNullInputModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Initialize module. */
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
	
	/** \brief Index of button best matching key code or -1 if not found. */
	virtual int ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode );
	
	/** \brief Index of button best matching character or -1 if not found. */
	virtual int ButtonMatchingKeyChar( int device, int character );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/** \brief Process events. */
	virtual void ProcessEvents();
	
	/** \brief Get event. */
	virtual bool GetEvent( deInputEvent *event );
	
	/** \brief Clear events. */
	virtual void ClearEvents();
	/*@}*/
};

#endif
