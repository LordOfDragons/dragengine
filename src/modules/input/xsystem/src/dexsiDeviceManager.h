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
 * X-System input devices.
 */
class dexsiDeviceManager{
private:
	deXSystemInput &pModule;
	
	decObjectOrderedSet pDevices;
	
	dexsiDeviceCoreMouse *pX11CoreMouse;
	dexsiDeviceCoreKeyboard *pX11CoreKeyboard;
	
	dexsiDevice *pPrimaryMouse;
	dexsiDevice *pPrimaryKeyboard;
	
	int pInotifyFd;
	int pInotifyWatchEvdev;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device list. */
	dexsiDeviceManager( deXSystemInput &module );
	
	/** Clean up device list. */
	~dexsiDeviceManager();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** Update list of available devices. */
	void UpdateDeviceList();
	
	
	
	/** Number of devices. */
	int GetCount() const;
	
	/** Device at index. */
	dexsiDevice *GetAt( int index ) const;
	
	/** Device with identifier or \em NULL if absent. */
	dexsiDevice *GetWithID( const char *id );
	
	/** Index of device with identifier or -1 if absent. */
	int IndexOfWithID( const char *id );
	
	
	
	/** Core mouse device. */
	inline dexsiDeviceCoreMouse *GetX11CoreMouse() const{ return pX11CoreMouse; }
	
	/** Core keyboard device. */
	inline dexsiDeviceCoreKeyboard *GetX11CoreKeyboard() const{ return pX11CoreKeyboard; }
	
	/** Primary mouse device. */
	inline dexsiDevice *GetPrimaryMouse() const{ return pPrimaryMouse; }
	
	/** Primary keyboard device. */
	inline dexsiDevice *GetPrimaryKeyboard() const{ return pPrimaryKeyboard; }
	
	
	
	/** Update. */
	void Update();
	
	/** Log list of input devices. */
	void LogDevices();
	
	/** Log device. */
	void LogDevice( const dexsiDevice &device );
	
	
	
	/** Normalize identifier. */
	static decString NormalizeID( const char *id );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateDevices();
	
	void pCreateXInputDevices();
	void pCreateEvdevDevices();
	
	void pFindPrimaryDevices();
	
	void pStartWatchEvdev();
	void pStopWatchEvdev();
	void pUpdateWatchEvdev();
	bool pEvdevAppeared( const decString &path );
	bool pEvdevDisappeared( const decString &path );
};

#endif
 
