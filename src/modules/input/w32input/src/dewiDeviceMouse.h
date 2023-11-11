/* 
 * Drag[en]gine Windows Input Module
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

#ifndef _DEWIDEVICEMOUSE_H_
#define _DEWIDEVICEMOUSE_H_

#include "dewiDevice.h"



/**
 * \brief Windows mouse input device.
 */
class dewiDeviceMouse : public dewiDevice{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<dewiDeviceMouse> Ref;

public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	dewiDeviceMouse( deWindowsInput &module );
	
protected:
	/** \brief Clean up device. */
	virtual ~dewiDeviceMouse();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief Update device state. */
	virtual void Update();
	/*@}*/
};

#endif
