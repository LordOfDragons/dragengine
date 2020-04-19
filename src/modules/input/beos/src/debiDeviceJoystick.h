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

#ifndef _DEBIDEVICEJOYSTICK_H_
#define _DEBIDEVICEJOYSTICK_H_

#include <dragengine/common/string/decString.h>
#include <device/Joystick.h>

#include "debiDevice.h"


/**
 * \brief Joystick input device using BJoystick API.
 */
class debiDeviceJoystick : public debiDevice{
private:
	decString pDevName;
	BJoystick *pJoystick;
	
	int pStateAxisCount;
	int16 *pStateAxis;
	int pStateHatCount;
	uint8 *pStateHat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	debiDeviceJoystick( deBeOSInput &module, const char *name );
	
	
	
protected:
	/** \brief Clean up device. */
	virtual ~debiDeviceJoystick();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief Device name. */
	inline const decString &GetName() const{ return pDevName; }
	
	/** \brief Joystick instance. */
	inline BJoystick *GetJoystick() const{ return pJoystick; }
	
	
	
	/** \brief Update device state. */
	virtual void Update();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
