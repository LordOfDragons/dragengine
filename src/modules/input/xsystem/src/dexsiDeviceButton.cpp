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

#include <stdlib.h>

#include "dexsiDeviceButton.h"
#include "deXSystemInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputDeviceButton.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/image/deImageManager.h>



// Class dexsiDeviceButton
////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiDeviceButton::dexsiDeviceButton( deXSystemInput &module ) :
pModule( module ),

pPressed( false ),
pX11Code( 0 ),
pEvdevCode( 0 ),
pKeyCode( deInputEvent::ekcUndefined ),
pMatchPriority( 10 ){
}

dexsiDeviceButton::~dexsiDeviceButton(){
}



// Management
///////////////

void dexsiDeviceButton::SetID( const char *id ){
	pID = id;
}

void dexsiDeviceButton::SetName( const char *name ){
	pName = name;
}

void dexsiDeviceButton::SetPressed( bool pressed ){
	pPressed = pressed;
}



void dexsiDeviceButton::SetDisplayImages( const char *name ){
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

void dexsiDeviceButton::SetDisplayImages( const dexsiDeviceButton &button ){
	pDisplayImage = button.pDisplayImage;
	pDisplayIcons = button.pDisplayIcons;
}

void dexsiDeviceButton::SetDisplayText( const char *text ){
	pDisplayText = text;
}



void dexsiDeviceButton::SetX11Code( int code ){
	pX11Code = code;
}

void dexsiDeviceButton::SetEvdevCode( int code ){
	pEvdevCode = code;
}

void dexsiDeviceButton::SetKeyCode( deInputEvent::eKeyCodes keyCode ){
	pKeyCode = keyCode;
}

void dexsiDeviceButton::SetMatchPriority( int priority ){
	pMatchPriority = priority;
}



void dexsiDeviceButton::GetInfo( deInputDeviceButton &info ) const{
	int i;
	
	info.SetID( pID );
	info.SetName( pName );
	
	info.SetDisplayImage( pDisplayImage );
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
}
