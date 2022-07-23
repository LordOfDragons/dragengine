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

#include <stdlib.h>

#include "dewiDeviceButton.h"
#include "deWindowsInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputDeviceButton.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/image/deImageManager.h>



// Class dewiDeviceButton
////////////////////////////

// Constructor, destructor
////////////////////////////

dewiDeviceButton::dewiDeviceButton( deWindowsInput &module ) :
pModule( module ),
pPressed( false ),
pWICode( 0 ),
pWIChar( -1 ),
pKeyCode( deInputEvent::ekcUndefined ),
pMatchPriority( 10 ),
pKeyLocation( deInputEvent::eklNone )
{
}

dewiDeviceButton::~dewiDeviceButton(){
}



// Management
///////////////

void dewiDeviceButton::SetID( const char *id ){
	pID = id;
}

void dewiDeviceButton::SetName( const char *name ){
	pName = name;
}
 
void dewiDeviceButton::SetPressed( bool pressed ){
	pPressed = pressed;
}



void dewiDeviceButton::SetDisplayImages( const char *name ){
	pDisplayImage = NULL;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pModule.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pModule.GetVFS();
	const char * const basePath = "/share/image/button";
	decString filename;
	
	filename.Format( "%s/%s/image.png", basePath, name );
	pDisplayImage.TakeOver( imageManager.LoadImage( vfs, filename, "/" ) );
	
	const int sizes[ 4 ] = {128, 64, 32, 16};
	deImageReference icon;
	int i;
	
	for( i=0; i<4; i++ ){
		filename.Format( "%s/%s/icon%d.png", basePath, name, sizes[ i ] );
		icon.TakeOver( imageManager.LoadImage( vfs, filename, "/" ) );
		pDisplayIcons.Add( ( deImage* )icon );
	}
}

void dewiDeviceButton::SetDisplayImages( const dewiDeviceButton &button ){
	pDisplayImage = button.pDisplayImage;
	pDisplayIcons = button.pDisplayIcons;
}

void dewiDeviceButton::SetDisplayText( const char *text ){
	pDisplayText = text;
}



void dewiDeviceButton::SetWICode( int code ){
	pWICode = code;
}

void dewiDeviceButton::SetWIChar( int character ){
	pWIChar = character;
}

void dewiDeviceButton::SetKeyCode( deInputEvent::eKeyCodes keyCode ){
	pKeyCode = keyCode;
}

void dewiDeviceButton::SetMatchPriority( int priority ){
	pMatchPriority = priority;
}

void dewiDeviceButton::SetKeyLocation( deInputEvent::eKeyLocation location ){
	pKeyLocation = location;
}



void dewiDeviceButton::GetInfo( deInputDeviceButton &info ) const{
	int i;
	
	info.SetID( pID );
	info.SetName( pName );
	
	info.SetDisplayImage( pDisplayImage );
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
}
