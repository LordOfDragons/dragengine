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
#include "deoxrDeviceAxis.h"
#include "../deVROpenXR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deoxrDeviceAxis
//////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDeviceAxis::deoxrDeviceAxis( deoxrDevice &device ) :
pDevice( device ),
pIndex( -1 ),
pComponent( 0 ),
pFinger( -1 ),
pType( deInputDeviceAxis::eatGeneric ),
pMinimum( -1.0f ),
pMaximum( 1.0f ),
pCenter( 0.0f ),
pDeadZone( 0.0f ),
pResolution( 0.01f ),
pValue( 0.0f ){
}

deoxrDeviceAxis::~deoxrDeviceAxis(){
}



// Management
///////////////

void deoxrDeviceAxis::SetIndex( int index ){
	pIndex = index;
}

void deoxrDeviceAxis::SetActionAnalog( deoxrAction *action ){
	pActionAnalog = action;
}

void deoxrDeviceAxis::SetComponent( int component ){
	pComponent = component;
}

void deoxrDeviceAxis::SetFinger( int finger ){
	pFinger = finger;
}

void deoxrDeviceAxis::SetID( const char *id ){
	pID = id;
}

void deoxrDeviceAxis::SetName( const char *name ){
	pName = name;
}

void deoxrDeviceAxis::SetType( deInputDeviceAxis::eAxisTypes type ){
	pType = type;
}

void deoxrDeviceAxis::SetInputDeviceComponent( deoxrDeviceComponent *component ){
	pInputDeviceComponent = component;
}



void deoxrDeviceAxis::SetDisplayImages( const char *name ){
	pDisplayImage = nullptr;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pDevice.GetOxr().GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pDevice.GetOxr().GetVFS();
	const char * const basePath = "/share/image/axis";
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

void deoxrDeviceAxis::SetDisplayText( const char *text ){
	pDisplayText = text;
}



void deoxrDeviceAxis::SetRange( float minimum, float maximum ){
	pMinimum = minimum;
	pMaximum = maximum;
}

void deoxrDeviceAxis::SetCenter( float center ){
	pCenter = center;
}

void deoxrDeviceAxis::SetDeadZone( float deadZone ){
	pDeadZone = deadZone;
}

void deoxrDeviceAxis::SetResolution( float resolution ){
	pResolution = resolution;
}

void deoxrDeviceAxis::SetValue( float value ){
	pValue = value;
}

void deoxrDeviceAxis::UpdateValue( float value ){
	value = decMath::clamp( value, -1.0f, 1.0f );
	
	if( fabsf( value - pCenter ) < pDeadZone ){
		value = pCenter;
	}
	
	if( fabsf( value - pValue ) < pResolution ){
		return;
	}
	
	SetValue( value );
	
	deInputEvent event;
	event.SetType( deInputEvent::eeAxisMove );
	event.SetSource( deInputEvent::esVR );
	event.SetDevice( pDevice.GetIndex() );
	event.SetCode( pIndex );
	event.SetValue( value );
	pDevice.GetOxr().InputEventSetTimestamp( event );
	pDevice.GetOxr().SendEvent( event );
}

void deoxrDeviceAxis::TrackState(){
	switch( pType ){
	case deInputDeviceAxis::eatFingerBend:
// 		UpdateValue( decMath::linearStep( pDevice.GetSkeletalSummaryData().flFingerCurl[ pFinger ],
// 			pMinimum, pMaximum, -1.0f, 1.0f ) );
		break;
		
	case deInputDeviceAxis::eatFingerSpread:
// 		UpdateValue( decMath::linearStep( pDevice.GetSkeletalSummaryData().flFingerSplay[ pFinger ],
// 			pMinimum, pMaximum, -1.0f, 1.0f ) );
		break;
		
	default:{
		/*
		vr::IVRInput &vrinput = pDevice.GetOvr().GetVRInput();
		
		vr::InputAnalogActionData_t dataAnalog;
		vr::EVRInputError error = vrinput.GetAnalogActionData( pActionAnalogHandle,
			&dataAnalog, sizeof( dataAnalog ), pDevice.GetInputValueHandle() );
		
		if( error == vr::VRInputError_None ){
			UpdateValue( decMath::linearStep( ( &dataAnalog.x )[ pComponent ],
				pMinimum, pMaximum, -1.0f, 1.0f ) );
			
		}else{
			//UpdateValue( pCenterValue );
			// keep the last known value
		}
		*/
		}break;
	}
}

void deoxrDeviceAxis::ResetState(){
	UpdateValue( pCenter );
	SetValue( pCenter );
}



void deoxrDeviceAxis::GetInfo( deInputDeviceAxis &info ) const{
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
