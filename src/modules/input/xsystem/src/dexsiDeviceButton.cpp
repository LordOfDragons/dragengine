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
pType( deInputDeviceButton::ebtGeneric ),
pX11Code( 0 ),
pEvdevCode( 0 ),
pKeyCode( deInputEvent::ekcUndefined ),
pMatchPriority( 10 ),
pKeyLocation( deInputEvent::eklNone ){
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

void dexsiDeviceButton::SetType( deInputDeviceButton::eButtonTypes type ){
	pType = type;
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

void dexsiDeviceButton::SetKeyLocation( deInputEvent::eKeyLocation location ){
	pKeyLocation = location;
}



void dexsiDeviceButton::GetInfo( deInputDeviceButton &info ) const{
	int i;
	
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
	
	info.SetDisplayImage( pDisplayImage );
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
}
