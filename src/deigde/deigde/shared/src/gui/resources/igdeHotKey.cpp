/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
