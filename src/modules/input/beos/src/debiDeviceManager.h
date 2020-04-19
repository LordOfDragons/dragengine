/* 
 * Drag[en]gine BeOS Input Module
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

#ifndef _DEBIDEVICEMANAGER_H_
#define _DEBIDEVICEMANAGER_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class deBeOSInput;
class debiDevice;
class debiDeviceMouse;
class debiDeviceKeyboard;



/**
 * \brief Input device manager.
 */
class debiDeviceManager{
private:
	deBeOSInput &pModule;
	
	decObjectOrderedSet pDevices;
	
	debiDeviceMouse *pMouse;
	debiDeviceKeyboard *pKeyboard;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device list. */
	debiDeviceManager( deBeOSInput &module );
	
	/** \brief Clean up device list. */
	~debiDeviceManager();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Update list of available devices. */
	void UpdateDeviceList();
	
	
	
	/** \brief Number of devices. */
	int GetCount() const;
	
	/** \brief Device at index. */
	debiDevice *GetAt( int index ) const;
	
	/** \brief Device with identifier or \em NULL if absent. */
	debiDevice *GetWithID( const char *id );
	
	/** \brief Index of device with identifier or -1 if absent. */
	int IndexOfWithID( const char *id );
	
	
	
	/** \brief Mouse device. */
	inline debiDeviceMouse *GetMouse() const{ return pMouse; }
	
	/** \brief Keyboard device. */
	inline debiDeviceKeyboard *GetKeyboard() const{ return pKeyboard; }
	
	
	
	/** \brief Log list of input devices. */
	void LogDevices();
	
	
	
	/** \brief Normalize identifier. */
	static decString NormalizeID( const char *id );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateDevices();
	void pCreateJoystickDevices();
};

#endif
 
