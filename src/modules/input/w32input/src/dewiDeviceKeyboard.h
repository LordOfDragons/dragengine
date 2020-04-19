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

#ifndef _DEWIDEVICEKEYBOARD_H_
#define _DEWIDEVICEKEYBOARD_H_

#include "dewiDevice.h"

#include <dragengine/input/deInputEvent.h>



/**
 * \brief Windows keyboard input device.
 */
class dewiDeviceKeyboard : public dewiDevice{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	dewiDeviceKeyboard( deWindowsInput &module );
	
protected:
	/** \brief Clean up device. */
	virtual ~dewiDeviceKeyboard();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief Get key code for windows virtual key code. */
	static deInputEvent::eKeyCodes KeyCodeForWICode( int code );
	
	/** \brief Get matching priority for virtual key code. */
	static int MatchingPriorityForWICode( int code );
	
	/** \brief Get button best matching a character. */
	int ButtonMatchingKeyChar( int keyChar ) const;
	/*@}*/
	
	
	
private:
	void pSetButtonAt( int index, const char *id, const char *name, int wiCode,
		deInputEvent::eKeyCodes keyCode, int wiChar, int matchPriority );
};

#endif
