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

void deovrDeviceAxis::SetInputDeviceComponent( deovrDeviceComponent *component ){
	pInputDeviceComponent = component;
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
	pDevice.GetOvr().InputEventSetTimestamp( event );
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
	info.SetComponent( pInputDeviceComponent ? pInputDeviceComponent->GetID() : "" );
	
	info.SetDisplayImage( pDisplayImage );
	
	int i;
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
}
