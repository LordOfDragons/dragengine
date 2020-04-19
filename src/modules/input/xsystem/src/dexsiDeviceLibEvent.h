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

#ifndef _DEXSIDEVICELIBEVENT_H_
#define _DEXSIDEVICELIBEVENT_H_

#include "dexsiDevice.h"



/**
 * \brief X-System libEvent input device.
 */
class dexsiDeviceLibEvent : public dexsiDevice{
private:
	decString pEvdevPath;
	int pEvdevFile;
	libevdev *pEvdevDevice;
	short *pEvdevMapRelAxis;
	short *pEvdevMapAbsAxis;
	short *pEvdevMapKeys;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	dexsiDeviceLibEvent( deXSystemInput &module, const char *pathDevice );
	
protected:
	/** \brief Clean up device. */
	virtual ~dexsiDeviceLibEvent();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief Libevdev event file path or empty string if not using libevdev. */
	inline const decString &GetEvdevPath() const{ return pEvdevPath; }
	
	/** \brief Libevdev device or \em NULL if not using libevdev. */
	inline libevdev *GetEvdevDevice() const{ return pEvdevDevice; }
	
	
	
	/** \brief Update device state. */
	virtual void Update();
	/*@}*/
};

#endif
