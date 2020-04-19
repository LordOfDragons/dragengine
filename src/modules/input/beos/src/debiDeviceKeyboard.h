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

#ifndef _DEBIDEVICEKEYBOARD_H_
#define _DEBIDEVICEKEYBOARD_H_

#include "debiDevice.h"



/**
 * \brief Keyboard input device.
 */
class debiDeviceKeyboard : public debiDevice{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	debiDeviceKeyboard( deBeOSInput &module );
	
	
	
protected:
	/** \brief Clean up device. */
	virtual ~debiDeviceKeyboard();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief Get button best matching a character. */
	int ButtonMatchingKeyChar( int keyChar ) const;
	/*@}*/
};

#endif
