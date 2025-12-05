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

#include "dewiDeviceButton.h"
#include "dewiDeviceWinRTController.h"
#include "deWindowsInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputDeviceButton.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>

#include <winrt/Windows.System.Power.h>

namespace wrsp = winrt::Windows::System::Power;



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
pKeyLocation( deInputEvent::eklNone ),
pWinRTReadingIndex( -1 ),
pIsBatteryCharging( false )
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

void dewiDeviceButton::SetType( deInputDeviceButton::eButtonTypes type ){
	pType = type;
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
	deImage::Ref icon;
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

void dewiDeviceButton::SetWinRTReadingIndex( int index ){
	pWinRTReadingIndex = index;
}

void dewiDeviceButton::SetIsBatteryCharging( bool isBatteryCharging ){
	pIsBatteryCharging = isBatteryCharging;
}


void dewiDeviceButton::GetInfo( deInputDeviceButton &info ) const{
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

void dewiDeviceButton::WinRTReading( dewiDeviceWinRTController &device ){
	bool pressed = false;

	if( pWinRTReadingIndex != -1 ){
		pressed = device.GetReadingButton( pWinRTReadingIndex );

	}else if(pIsBatteryCharging){
		if(device.GetBatteryReport()){
			pressed = device.GetBatteryReport().Status() == wrsp::BatteryStatus::Charging;
		}
	}

	if( pressed ){
		if( ! pPressed ){
			pPressed = true;
			pModule.AddButtonPressed( device.GetIndex(), pWinRTReadingIndex, device.GetReadingTime() );
		}

	}else{
		if( pPressed ){
			pPressed = false;
			pModule.AddButtonReleased( device.GetIndex(), pWinRTReadingIndex, device.GetReadingTime() );
		}
	}
}
