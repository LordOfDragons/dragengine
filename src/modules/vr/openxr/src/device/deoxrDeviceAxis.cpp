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
	const deoxrInstance &instance = pDevice.GetOxr().GetInstance();
	const deoxrSession &session = pDevice.GetOxr().GetSession();
	
	switch( pType ){
	case deInputDeviceAxis::eatTrigger:{
		float value = pGetActionFloatValue(instance, session);
		if(pDevice.GetHandTracker() && pFinger != -1 && pDevice.GetEnableTwoFingerTriggerSimulation()){
			value = decMath::max(value, decMath::linearStep(
				pDevice.GetHandTracker()->GetFingerInputAt(0), 0.0f, 1.0f, -1.0f, 1.0f));
		}
		UpdateValue(value);
		}break;
		
	case deInputDeviceAxis::eatTwoFingerTrigger:{
		if(pDevice.GetHandTracker() && pFinger != -1){
			UpdateValue(decMath::linearStep(
				pDevice.GetHandTracker()->GetFingerInputAt(pFinger), 0.0f, 1.0f, -1.0f, 1.0f));
		}
		}break;
		
	case deInputDeviceAxis::eatFingerBend:
		if( pDevice.GetHandTracker() ){
			UpdateValue( decMath::linearStep( pDevice.GetHandTracker()->GetBendFingerAt( pFinger ),
				pMinimum, pMaximum, -1.0f, 1.0f ) );
		}
		break;
		
	case deInputDeviceAxis::eatFingerSpread:
		if( pDevice.GetHandTracker() ){
			UpdateValue( decMath::linearStep( pDevice.GetHandTracker()->GetSpreadFingerAt( pFinger ),
				pMinimum, pMaximum, -1.0f, 1.0f ) );
		}
		break;
		
	case deInputDeviceAxis::eatStick:
	case deInputDeviceAxis::eatTouchPad:
		if( pActionAnalog ){
			XrActionStateGetInfo getInfo;
			memset( &getInfo, 0, sizeof( getInfo ) );
			getInfo.type = XR_TYPE_ACTION_STATE_GET_INFO;
			getInfo.action = pActionAnalog->GetAction();
			getInfo.subactionPath = pDevice.GetSubactionPath();
			
			XrActionStateVector2f state;
			memset( &state, 0, sizeof( state ) );
			state.type = XR_TYPE_ACTION_STATE_VECTOR2F;
			
			if( XR_SUCCEEDED( instance.xrGetActionStateVector2f( session.GetSession(), &getInfo, &state ) )
			&& state.isActive ){
				switch( pComponent ){
				case 0:
					UpdateValue( decMath::linearStep( state.currentState.x, pMinimum, pMaximum, -1.0f, 1.0f ) );
					break;
					
				case 1:
					UpdateValue( decMath::linearStep( state.currentState.y, pMinimum, pMaximum, -1.0f, 1.0f ) );
					break;
				}
			}
		}
		break;
		
	default:
		UpdateValue(pGetActionFloatValue(instance, session));
		break;
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
	info.SetComponent( pInputDeviceComponent ? pInputDeviceComponent->GetID().GetString() : "" );
	
	info.SetDisplayImage( pDisplayImage );
	
	int i;
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
}

float deoxrDeviceAxis::pGetActionFloatValue(const deoxrInstance &instance,
const deoxrSession &session) const{
	if(!pActionAnalog){
		return pCenter;
	}
	
	XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
	getInfo.action = pActionAnalog->GetAction();
	getInfo.subactionPath = pDevice.GetSubactionPath();
	
	XrActionStateFloat state{XR_TYPE_ACTION_STATE_FLOAT};
	
	if(XR_SUCCEEDED(instance.xrGetActionStateFloat(session.GetSession(), &getInfo, &state))
	&& state.isActive){
		return decMath::linearStep(state.currentState, pMinimum, pMaximum, -1.0f, 1.0f);
	}
	
	return pCenter;
}
