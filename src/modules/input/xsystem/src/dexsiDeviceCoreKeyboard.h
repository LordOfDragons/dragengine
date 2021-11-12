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
 * X-System core keyboard input device.
 */
class dexsiDeviceCoreKeyboard : public dexsiDevice{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device. */
	dexsiDeviceCoreKeyboard( deXSystemInput &module );
	
protected:
	/** Clean up device. */
	virtual ~dexsiDeviceCoreKeyboard();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Get key code for X11 key sym. */
	static deInputEvent::eKeyCodes KeyCodeForKeySym( KeySym keysym );
	
	/** Get key location for X11 key sym. */
	static deInputEvent::eKeyLocation KeyLocationForKeySym( KeySym keysym );
	
	/** Get matching priority for X11 key sym. */
	static int MatchingPriorityForKeySym( KeySym keysym );
	
	/** Get button best matching a character. */
	int ButtonMatchingKeyChar( int keyChar ) const;
	/*@}*/
};

#endif
