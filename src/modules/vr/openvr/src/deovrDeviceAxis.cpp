/* 
 * Drag[en]gine OpenVR VR Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deovrDevice.h"
#include "deovrDeviceAxis.h"
#include "deVROpenVR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deovrDeviceAxis
//////////////////////////

// Constructor, destructor
////////////////////////////

deovrDeviceAxis::deovrDeviceAxis( deovrDevice &device ) :
pDevice( device ),
pIndex( -1 ),
pActionAnalogHandle( vr::k_ulInvalidActionHandle ),
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

deovrDeviceAxis::~deovrDeviceAxis(){
}



// Management
///////////////

void deovrDeviceAxis::SetIndex( int index ){
	pIndex = index;
}

void deovrDeviceAxis::SetActionAnalogHandle( vr::VRActionHandle_t handle ){
	pActionAnalogHandle = handle;
}

void deovrDeviceAxis::SetComponent( int component ){
	pComponent = component;
}

void deovrDeviceAxis::SetFinger( int finger ){
	pFinger = finger;
}

void deovrDeviceAxis::SetID( const char *id ){
	pID = id;
}

void deovrDeviceAxis::SetName( const char *name ){
	pName = name;
}

void deovrDeviceAxis::SetType( deInputDeviceAxis::eAxisTypes type ){
	pType = type;
}



void deovrDeviceAxis::SetDisplayImages( const char *name ){
	pDisplayImage = nullptr;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pDevice.GetOvr().GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pDevice.GetOvr().GetVFS();
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

void deovrDeviceAxis::SetDisplayText( const char *text ){
	pDisplayText = text;
}



void deovrDeviceAxis::SetRange( float minimum, float maximum ){
	pMinimum = minimum;
	pMaximum = maximum;
}

void deovrDeviceAxis::SetCenter( float center ){
	pCenter = center;
}

void deovrDeviceAxis::SetDeadZone( float deadZone ){
	pDeadZone = deadZone;
}

void deovrDeviceAxis::SetResolution( float resolution ){
	pResolution = resolution;
}

void deovrDeviceAxis::SetValue( float value ){
	pValue = value;
}

void deovrDeviceAxis::UpdateValue( float value ){
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
	event.SetTime( { decDateTime().ToSystemTime(), 0 } );
	pDevice.GetOvr().SendEvent( event );
}

void deovrDeviceAxis::TrackState(){
	switch( pType ){
	case deInputDeviceAxis::eatFingerBend:
		UpdateValue( decMath::linearStep( pDevice.GetSkeletalSummaryData().flFingerCurl[ pFinger ],
			pMinimum, pMaximum, -1.0f, 1.0f ) );
		break;
		
	case deInputDeviceAxis::eatFingerSpread:
		UpdateValue( decMath::linearStep( pDevice.GetSkeletalSummaryData().flFingerSplay[ pFinger ],
			pMinimum, pMaximum, -1.0f, 1.0f ) );
		break;
		
	default:{
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
		}break;
	}
}

void deovrDeviceAxis::ResetState(){
	UpdateValue( pCenter );
	SetValue( pCenter );
}



void deovrDeviceAxis::GetInfo( deInputDeviceAxis &info ) const{
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
	
	info.SetDisplayImage( pDisplayImage );
	
	int i;
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
}
