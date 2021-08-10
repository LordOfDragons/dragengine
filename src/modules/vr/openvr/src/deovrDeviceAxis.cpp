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

deovrDeviceAxis::deovrDeviceAxis( deovrDevice &device, int axisIndex ) :
pDevice( device ),
pIndex( -1 ),
pAxisIndex( axisIndex ),
pAxisType( vr::k_eControllerAxis_None ),
pUseX( true ),
pType( deInputDeviceAxis::eatGeneric ),
pValue( 0.0f ),
pJitterHistorySize( 3 ),
pJitterHistoryOffset( 0 ),
pJitterHistoryCount( 0 ){
}

deovrDeviceAxis::~deovrDeviceAxis(){
}



// Management
///////////////

void deovrDeviceAxis::SetIndex( int index ){
	pIndex = index;
}

void deovrDeviceAxis::SetAxisType( vr::EVRControllerAxisType axisType ){
	pAxisType = axisType;
}

void deovrDeviceAxis::SetUseX( bool useX ){
	pUseX = useX;
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



void deovrDeviceAxis::SetValue( float value ){
	pValue = value;
}

void deovrDeviceAxis::UpdateValue( float value ){
	value = decMath::clamp( value, -1.0f, 1.0f );
	
	/*
	pJitterHistory[ pJitterHistoryOffset ] = value;
	pJitterHistoryOffset = ( pJitterHistoryOffset + 1 ) % pJitterHistorySize;
	pJitterHistoryCount = decMath::min( pJitterHistoryCount + 1, pJitterHistorySize );
	
	int i;
	value = pJitterHistory[ 0 ];
	for( i=1; i<pJitterHistoryCount; i++ ){
		value += pJitterHistory[ i ];
	}
	value /= pJitterHistoryCount;
	*/
	
	if( fabsf( value - pValue ) < 0.01f ){
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
	const vr::VRControllerState_t &state = pDevice.GetState();
	
	switch( pAxisType ){
	case vr::k_eControllerAxis_Trigger:
		UpdateValue( state.rAxis[ pAxisIndex ].x * 2.0f - 1.0f );
		break;
		
	default:
		if( pUseX ){
			UpdateValue( state.rAxis[ pAxisIndex ].x );
			
		}else{
			UpdateValue( state.rAxis[ pAxisIndex ].y );
		}
	}
}

void deovrDeviceAxis::ResetState(){
	pJitterHistoryCount = 0;
	pJitterHistoryOffset = 0;
	
	switch( pAxisType ){
	case vr::k_eControllerAxis_Trigger:
		UpdateValue( -1.0f );
		SetValue( -1.0f );
		break;
		
	default:
		UpdateValue( 0.0f );
		SetValue( 0.0f );
	}
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
