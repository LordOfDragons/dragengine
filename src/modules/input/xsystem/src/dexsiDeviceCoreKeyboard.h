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

#ifndef _DEXSIDEVICECOREKEYBOARD_H_
#define _DEXSIDEVICECOREKEYBOARD_H_

#include "dexsiDevice.h"

#include <dragengine/input/deInputEvent.h>



/**
 * \brief X-System core keyboard input device.
 */
class dexsiDeviceCoreKeyboard : public dexsiDevice{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	dexsiDeviceCoreKeyboard( deXSystemInput &module );
	
protected:
	/** \brief Clean up device. */
	virtual ~dexsiDeviceCoreKeyboard();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief Get key code for X11 key sym. */
	static deInputEvent::eKeyCodes KeyCodeForKeySym( KeySym keysym );
	
	/** \brief Get matching priority for X11 key sym. */
	static int MatchingPriorityForKeySym( KeySym keysym );
	
	/** \brief Get button best matching a character. */
	int ButtonMatchingKeyChar( int keyChar ) const;
	/*@}*/
};

#endif
