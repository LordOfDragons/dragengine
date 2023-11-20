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

#ifndef _DEXSIDEVICECOREMOUSE_H_
#define _DEXSIDEVICECOREMOUSE_H_

#include "dexsiDevice.h"



/**
 * \brief X-System core mouse input device.
 */
class dexsiDeviceCoreMouse : public dexsiDevice{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<dexsiDeviceCoreMouse> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	dexsiDeviceCoreMouse( deXSystemInput &module );
	
protected:
	/** \brief Clean up device. */
	virtual ~dexsiDeviceCoreMouse();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief Update device state. */
	virtual void Update();
	/*@}*/
};

#endif
