/* 
 * Drag[en]gine Game Engine
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "deInputEvent.h"
#include "../common/exceptions.h"
#include "../common/math/decMath.h"



// Class deInputEvent
///////////////////////

// Constructors and Destructors
/////////////////////////////////

deInputEvent::deInputEvent() :
pType( eeKeyPress ),
pDevice( 0 ),
pCode( 0 ),
pState( 0 ),
pKeyCode( ekcUndefined ),
pKeyChar( 0 ),
pX( 0 ),
pY( 0 ),
pValue( 0.0f ),
pSource( esInput )
{
	memset( &pTime, '\0', sizeof( pTime ) );
}

deInputEvent::deInputEvent( const deInputEvent &event ) :
pType( event.pType ),
pDevice( event.pDevice ),
pCode( event.pCode ),
pState( event.pState ),
pKeyCode( event.pKeyCode ),
pKeyChar( event.pKeyChar ),
pX( event.pX ),
pY( event.pY ),
pValue( event.pValue ),
pTime( event.pTime ),
pSource( event.pSource ){
}

deInputEvent::~deInputEvent(){
}



// Management
///////////////

void deInputEvent::SetType( eEvents type ){
	pType = type;
}

void deInputEvent::SetDevice( int device ){
	if( device < 0 ){
		DETHROW( deeInvalidParam );
	}
	pDevice = device;
}

void deInputEvent::SetCode( int code ){
	pCode = code;
}

void deInputEvent::SetState( int state ){
	pState = state;
}

void deInputEvent::SetKeyCode( eKeyCodes keyCode ){
	pKeyCode = keyCode;
}

void deInputEvent::SetKeyChar( int keyChar ){
	pKeyChar = keyChar;
}

void deInputEvent::SetX( int x ){
	pX = x;
}

void deInputEvent::SetY( int y ){
	pY = y;
}

void deInputEvent::SetValue( float value ){
	pValue = value;
}

void deInputEvent::SetTime( const timeval &eventTime ){
	memcpy( &pTime, &eventTime, sizeof( timeval ) );
}

void deInputEvent::SetSource( eSources source ){
	pSource = source;
}



void deInputEvent::SetFrom( const deInputEvent &event ){
	pType = event.pType;
	pDevice = event.pDevice;
	pCode = event.pCode;
	pState = event.pState;
	pKeyCode = event.pKeyCode;
	pKeyChar = event.pKeyChar;
	pX = event.pX;
	pY = event.pY;
	pValue = event.pValue;
	memcpy( &pTime, &event.pTime, sizeof( timeval ) );
	pSource = event.pSource;
}



// Operators
//////////////

bool deInputEvent::operator==( const deInputEvent &event ) const{
	return pType == event.pType
		&& pDevice == event.pDevice
		&& pCode == event.pCode
		&& pState == event.pState
		&& pKeyCode == event.pKeyCode
		&& pKeyChar == event.pKeyChar
		&& pX == event.pX
		&& pY == event.pY
		&& fabsf( pValue - event.pValue ) < FLOAT_SAFE_EPSILON
		&& memcmp( &pTime, &event.pTime, sizeof( timeval ) ) == 0
		&& pSource == event.pSource;
}

deInputEvent &deInputEvent::operator=( const deInputEvent &event ){
	pType = event.pType;
	pDevice = event.pDevice;
	pCode = event.pCode;
	pState = event.pState;
	pKeyCode = event.pKeyCode;
	pKeyChar = event.pKeyChar;
	pX = event.pX;
	pY = event.pY;
	pValue = event.pValue;
	memcpy( &pTime, &event.pTime, sizeof( timeval ) );
	pSource = event.pSource;
	return *this;
}
