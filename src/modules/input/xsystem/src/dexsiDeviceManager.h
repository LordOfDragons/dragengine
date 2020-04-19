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

#ifndef _DEXSIDEVICEMANAGER_H_
#define _DEXSIDEVICEMANAGER_H_

#include "dexsiXInclude.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>

class deXSystemInput;
class dexsiDevice;
class dexsiDeviceCoreMouse;
class dexsiDeviceCoreKeyboard;



/**
 * \brief X-System input devices.
 */
class dexsiDeviceManager{
private:
	deXSystemInput &pModule;
	
	decObjectOrderedSet pDevices;
	
	dexsiDeviceCoreMouse *pX11CoreMouse;
	dexsiDeviceCoreKeyboard *pX11CoreKeyboard;
	
	dexsiDevice *pPrimaryMouse;
	dexsiDevice *pPrimaryKeyboard;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device list. */
	dexsiDeviceManager( deXSystemInput &module );
	
	/** \brief Clean up device list. */
	~dexsiDeviceManager();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Update list of available devices. */
	void UpdateDeviceList();
	
	
	
	/** \brief Number of devices. */
	int GetCount() const;
	
	/** \brief Device at index. */
	dexsiDevice *GetAt( int index ) const;
	
	/** \brief Device with identifier or \em NULL if absent. */
	dexsiDevice *GetWithID( const char *id );
	
	/** \brief Index of device with identifier or -1 if absent. */
	int IndexOfWithID( const char *id );
	
	
	
	/** \brief Core mouse device. */
	inline dexsiDeviceCoreMouse *GetX11CoreMouse() const{ return pX11CoreMouse; }
	
	/** \brief Core keyboard device. */
	inline dexsiDeviceCoreKeyboard *GetX11CoreKeyboard() const{ return pX11CoreKeyboard; }
	
	/** \brief Primary mouse device. */
	inline dexsiDevice *GetPrimaryMouse() const{ return pPrimaryMouse; }
	
	/** \brief Primary keyboard device. */
	inline dexsiDevice *GetPrimaryKeyboard() const{ return pPrimaryKeyboard; }
	
	
	
	/** \brief Log list of input devices. */
	void LogDevices();
	
	
	
	/** \brief Normalize identifier. */
	static decString NormalizeID( const char *id );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateDevices();
	
	void pCreateXInputDevices();
	void pCreateEvdevDevices();
	
	void pFindPrimaryDevices();
};

#endif
 
