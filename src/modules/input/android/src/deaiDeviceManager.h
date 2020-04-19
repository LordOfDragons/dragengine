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

#ifndef _DEAIDEVICEMANAGER_H_
#define _DEAIDEVICEMANAGER_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class deAndroidInput;
class deaiDevice;
class deaiDeviceMouse;
class deaiDeviceKeyboard;



/**
 * \brief Android input devices.
 */
class deaiDeviceManager{
private:
	deAndroidInput &pModule;
	
	decObjectOrderedSet pDevices;
	
	deaiDeviceMouse *pMouse;
	deaiDeviceKeyboard *pKeyboard;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device list. */
	deaiDeviceManager( deAndroidInput &module );
	
	/** \brief Clean up device list. */
	~deaiDeviceManager();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Update list of available devices. */
	void UpdateDeviceList();
	
	
	
	/** \brief Number of devices. */
	int GetCount() const;
	
	/** \brief Device at index. */
	deaiDevice *GetAt( int index ) const;
	
	/** \brief Device with identifier or \em NULL if absent. */
	deaiDevice *GetWithID( const char *id ) const;
	
	/** \brief Index of device with identifier or -1 if absent. */
	int IndexOfWithID( const char *id ) const;
	
	
	
	/** \brief  mouse device. */
	inline deaiDeviceMouse *GetMouse() const{ return pMouse; }
	
	/** \brief  keyboard device. */
	inline deaiDeviceKeyboard *GetKeyboard() const{ return pKeyboard; }
	
	
	
	/** \brief Log list of input devices. */
	void LogDevices();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateDevices();
};

#endif
 
