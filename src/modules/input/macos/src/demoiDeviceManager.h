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

#ifndef _DEXSIDEVICEMANAGER_H_
#define _DEXSIDEVICEMANAGER_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class deMacOSInput;
class demoiDevice;
class demoiDeviceMouse;
class demoiDeviceKeyboard;



/**
 * \brief MacOS input devices.
 */
class demoiDeviceManager{
private:
	deMacOSInput &pModule;
	
	decObjectOrderedSet pDevices;
	
	demoiDeviceMouse *pMouse;
	demoiDeviceKeyboard *pKeyboard;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device list. */
	demoiDeviceManager( deMacOSInput &module );
	
	/** \brief Clean up device list. */
	~demoiDeviceManager();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Update list of available devices. */
	void UpdateDeviceList();
	
	
	
	/** \brief Number of devices. */
	int GetCount() const;
	
	/** \brief Device at index. */
	demoiDevice *GetAt( int index ) const;
	
	/** \brief Device with identifier or \em NULL if absent. */
	demoiDevice *GetWithID( const char *id );
	
	/** \brief Index of device with identifier or -1 if absent. */
	int IndexOfWithID( const char *id );
	
	
	
	/** \brief Core mouse device. */
	inline demoiDeviceMouse *GetMouse() const{ return pMouse; }
	
	/** \brief Core keyboard device. */
	inline demoiDeviceKeyboard *GetKeyboard() const{ return pKeyboard; }
	
	
	
	/** \brief Log list of input devices. */
	void LogDevices();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateDevices();
};

#endif
 
