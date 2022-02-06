/* 
 * Drag[en]gine OpenXR VR Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
	info.SetComponent( pInputDeviceComponent ? pInputDeviceComponent->GetID() : "" );
	
	info.SetDisplayImage( pDisplayImage );
	
	int i;
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
}

void deoxrDeviceButton::TrackState(){
	/*
	vr::IVRInput &vrinput = pDevice.GetOxr().GetVRInput();
	
	vr::InputDigitalActionData_t dataDigital;
	vr::EVRInputError error = vrinput.GetDigitalActionData( pActionPress,
		&dataDigital, sizeof( dataDigital ), pDevice.GetInputValueHandle() );
	UpdatePressed( error == vr::VRInputError_None ? dataDigital.bState : false );
	
	if( pActionTouch != vr::k_ulInvalidActionHandle ){
		vr::EVRInputError error = vrinput.GetDigitalActionData( pActionTouch,
			&dataDigital, sizeof( dataDigital ), pDevice.GetInputValueHandle() );
		UpdateTouched( error == vr::VRInputError_None ? dataDigital.bState : false );
	}
	*/
}
