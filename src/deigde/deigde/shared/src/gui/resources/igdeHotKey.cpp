/* 
 * Drag[en]gine IGDE
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

#include "igdeHotKey.h"

#include <dragengine/common/exceptions.h>



// Class igdeHotKey
/////////////////////

// Constructor, destructor
////////////////////////////

igdeHotKey::igdeHotKey() :
pKeyCode( deInputEvent::ekcUndefined ),
pKey( 0 ),
pModifiers( deInputEvent::esmNone ){
}

igdeHotKey::igdeHotKey( int modifiers, deInputEvent::eKeyCodes keyCode ) :
pKeyCode( keyCode ),
pKey( 0 ),
pModifiers( modifiers ){
}

igdeHotKey::igdeHotKey( int modifiers, int key ) :
pKeyCode( deInputEvent::ekcUndefined ),
pKey( key ),
pModifiers( modifiers )
{
	if( key < 0 ){
		DETHROW( deeInvalidParam );
	}
}

igdeHotKey::igdeHotKey( const igdeHotKey &hotKey ) :
pKeyCode( hotKey.pKeyCode ),
pKey( hotKey.pKey ),
pModifiers( hotKey.pModifiers ){
}



// Management
///////////////

void igdeHotKey::SetKeyCode( deInputEvent::eKeyCodes keyCode ){
	pKeyCode = keyCode;
}

void igdeHotKey::SetKey( int key ){
	if( key < 0 ){
		DETHROW( deeInvalidParam );
	}
	pKey = key;
}

void igdeHotKey::SetModifiers( int modifiers ){
	pModifiers = modifiers;
}



// Operators
//////////////

const bool igdeHotKey::operator==( const igdeHotKey &hotKey ) const{
	return pKeyCode == hotKey.pKeyCode && pKey == hotKey.pKey && pModifiers == hotKey.pModifiers;
}

igdeHotKey &igdeHotKey::operator=( const igdeHotKey &hotKey ){
	pKeyCode = hotKey.pKeyCode;
	pKey = hotKey.pKey;
	pModifiers = hotKey.pModifiers;
	return *this;
}
