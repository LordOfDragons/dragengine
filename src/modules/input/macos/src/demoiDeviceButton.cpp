/* 
 * Drag[en]gine MacOS Input Module
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

#include <stdlib.h>

#include "demoiDeviceButton.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDeviceButton.h>



// Class demoiDeviceButton
////////////////////////////

// Constructor, destructor
////////////////////////////

demoiDeviceButton::demoiDeviceButton() :
pPressed( false ),
pMOCode( 0 ),
pMOChar( 0 ),
pKeyCode( deInputEvent::ekcUndefined ),
pMatchPriority( 10 ){
}

demoiDeviceButton::~demoiDeviceButton(){
}



// Management
///////////////

void demoiDeviceButton::SetID( const char *id ){
	pID = id;
}

void demoiDeviceButton::SetName( const char *name ){
	pName = name;
}
 
void demoiDeviceButton::SetPressed( bool pressed ){
	pPressed = pressed;
}

void demoiDeviceButton::SetMOCode( int code ){
    pMOCode = code;
}

void demoiDeviceButton::SetMOChar( int character ){
    pMOChar = character;
}

void demoiDeviceButton::SetKeyCode( deInputEvent::eKeyCodes keyCode ){
    pKeyCode = keyCode;
}

void demoiDeviceButton::SetMatchPriority( int priority ){
    pMatchPriority = priority;
}



void demoiDeviceButton::GetInfo( deInputDeviceButton &info ) const{
	info.SetID( pID );
	info.SetName( pName );
}
