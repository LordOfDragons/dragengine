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
#include <string.h>

#include "deoxrDevice.h"
#include "deoxrDeviceButton.h"
#include "../deVROpenXR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputDeviceButton.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deoxrDeviceButton
////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDeviceButton::deoxrDeviceButton( deoxrDevice &device ) :
pDevice( device ),
pIndex( -1 ),
pType( deInputDeviceButton::ebtGeneric ),
pPressed( false ),
pTouched( false ){
}

deoxrDeviceButton::~deoxrDeviceButton(){
}



// Management
///////////////

void deoxrDeviceButton::SetIndex( int index ){
	pIndex = index;
}

void deoxrDeviceButton::SetActionPress( deoxrAction *action ){
	pActionPress = action;
}

void deoxrDeviceButton::SetActionTouch( deoxrAction *action ){
	pActionTouch = action;
}

void deoxrDeviceButton::SetFakeFromAxis( const deoxrDeviceAxis::Ref &axis ){
	pFakeFromAxis = axis;
}

void deoxrDeviceButton::SetID( const char *id ){
	pID = id;
}

void deoxrDeviceButton::SetName( const char *name ){
	pName = name;
}

void deoxrDeviceButton::SetType( deInputDeviceButton::eButtonTypes type ){
	pType = type;
}

void deoxrDeviceButton::SetInputDeviceComponent( deoxrDeviceComponent *component ){
	pInputDeviceComponent = component;
}

void deoxrDeviceButton::SetPressed( bool pressed ){
	pPressed = pressed;
}

void deoxrDeviceButton::UpdatePressed( bool pressed ){
	if( pressed == pPressed ){
		return;
	}
	
	SetPressed( pressed );
	
	deInputEvent event;
	event.SetType( pressed ? deInputEvent::eeButtonPress : deInputEvent::eeButtonRelease );
	event.SetSource( deInputEvent::esVR );
	event.SetDevice( pDevice.GetIndex() );
	event.SetCode( pIndex );
	pDevice.GetOxr().InputEventSetTimestamp( event );
	pDevice.GetOxr().SendEvent( event );
}

void deoxrDeviceButton::SetTouched( bool touched ){
	pTouched = touched;
}

void deoxrDeviceButton::UpdateTouched( bool touched ){
	if( touched == pTouched ){
		return;
	}
	
	SetTouched( touched );
	
	deInputEvent event;
	event.SetType( touched ? deInputEvent::eeButtonTouch : deInputEvent::eeButtonUntouch );
	event.SetSource( deInputEvent::esVR );
	event.SetDevice( pDevice.GetIndex() );
	event.SetCode( pIndex );
	pDevice.GetOxr().InputEventSetTimestamp( event );
	pDevice.GetOxr().SendEvent( event );
}



void deoxrDeviceButton::SetDisplayImages( const char *name ){
	pDisplayImage = nullptr;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pDevice.GetOxr().GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pDevice.GetOxr().GetVFS();
	const char * const basePath = "/share/image/button";
	decString filename;
	
	filename.Format( "%s/%s/image.png", basePath, name );
	pDisplayImage.TakeOver( imageManager.LoadImage( vfs, filename, "/" ) );
	
	const int sizes[ 4 ] = {128, 64, 32, 16};
	int i;
	
	for( i=0; i<4; i++ ){
		filename.Format( "%s/%s/icon%d.png", basePath, name, sizes[ i ] );
		pDisplayIcons.Add( deImage::Ref::New( imageManager.LoadImage( vfs, filename, "/" ) ) );
	}
}

void deoxrDeviceButton::SetDisplayImages( const deoxrDeviceButton &button ){
	pDisplayImage = button.pDisplayImage;
	pDisplayIcons = button.pDisplayIcons;
}

void deoxrDeviceButton::SetDisplayText( const char *text ){
	pDisplayText = text;
}



void deoxrDeviceButton::GetInfo( deInputDeviceButton &info ) const{
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
	info.SetComponent( pInputDeviceComponent ? pInputDeviceComponent->GetID().GetString() : "" );
	info.SetTouchable( pActionTouch );
	
	info.SetDisplayImage( pDisplayImage );
	
	int i;
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
}

void deoxrDeviceButton::TrackState(){
	const deoxrInstance &instance = pDevice.GetOxr().GetInstance();
	const deoxrSession &session = pDevice.GetOxr().GetSession();
	
	XrActionStateGetInfo getInfo;
	memset( &getInfo, 0, sizeof( getInfo ) );
	getInfo.type = XR_TYPE_ACTION_STATE_GET_INFO;
	getInfo.subactionPath = pDevice.GetSubactionPath();
	
	if( pFakeFromAxis ){
		UpdatePressed( pFakeFromAxis->GetValue() > 0.9f ); // range -1..1
		return;
	}

	XrActionStateBoolean state;
	memset( &state, 0, sizeof( state ) );
	state.type = XR_TYPE_ACTION_STATE_BOOLEAN;
	
	if( pActionPress ){
		getInfo.action = pActionPress->GetAction();
		UpdatePressed(
			XR_SUCCEEDED( instance.xrGetActionStateBoolean( session.GetSession(), &getInfo, &state ) )
			&& state.isActive && state.currentState );
	}
	
	if( pActionTouch ){
		getInfo.action = pActionTouch->GetAction();
		UpdateTouched(
			XR_SUCCEEDED( instance.xrGetActionStateBoolean( session.GetSession(), &getInfo, &state ) )
			&& state.isActive && state.currentState );
	}
}
