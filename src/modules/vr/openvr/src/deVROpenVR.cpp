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
#include <string.h>

#include "deVROpenVR.h"
#include "deovrDevice.h"
#include "deovrDeviceAxis.h"
#include "deovrDeviceButton.h"
#include "deovrDeviceFeedback.h"
#include "deovrDeviceManager.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/systems/deVRSystem.h>


#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *OpenVRCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// Entry Function
///////////////////

deBaseModule *OpenVRCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deVROpenVR( *loadableModule );
		
	}catch( const deException & ){
		return nullptr;
	}
}



// Class deVROpenVR
/////////////////////

// Constructor, destructor
////////////////////////////

deVROpenVR::deVROpenVR( deLoadableModule &loadableModule ) :
deBaseVRModule( loadableModule ),
pRuntimeInstalled( false ),
pDevices( *this ),
pSystem( nullptr ){
}

deVROpenVR::~deVROpenVR(){
	CleanUp();
}



// Management
///////////////

vr::IVRSystem &deVROpenVR::GetSystem() const{
	if( ! pSystem ){
		DETHROW( deeInvalidAction );
	}
	return *pSystem;
}



// Module Management
//////////////////////

bool deVROpenVR::Init(){
	pRuntimeInstalled = vr::VR_IsRuntimeInstalled();
	
	LogInfoFormat( "Runtime Installed: %s", pRuntimeInstalled ? "Yes" : "No" );
	
	char pathRuntime[ 256 ];
	uint32_t pathRuntimeLen = 0;
	memset( pathRuntime, 0, sizeof( pathRuntime ) );
	if( vr::VR_GetRuntimePath( pathRuntime, 255, &pathRuntimeLen ) ){
		pPathRuntime = pathRuntime;
		LogInfoFormat( "Runtime Path: %s", pathRuntime );
		
	}else{
		LogError( "Failed retrieving runtime path" );
		pRuntimeInstalled = false;
		return false;
	}
	
	LogInfoFormat( "HMD Present: %s", vr::VR_IsHmdPresent() ? "Yes" : "No" );
	return true;
}

void deVROpenVR::CleanUp(){
	StopRuntime();
}



// Runtime
////////////

bool deVROpenVR::RuntimeUsable(){
	return pRuntimeInstalled && vr::VR_IsHmdPresent();
}

void deVROpenVR::StartRuntime(){
	if( pSystem ){
		return;
	}
	
	LogInfo( "Start Runtime" );
	
	try{
		vr::HmdError error = vr::VRInitError_None;
		pSystem = vr::VR_Init( &error, vr::VRApplication_Scene );
		
		if( ! pSystem || error != vr::VRInitError_None ){
			LogErrorFormat( "Failed starting runtime: %s", vr::VR_GetVRInitErrorAsSymbol( error ) );
			DETHROW_INFO( deeInvalidAction, "Failed starting runtime" );
		}
		
		pDevices.InitDevices();
		pDevices.LogDevices();
		
		LogInfo( "Runtime Ready" );
		
	}catch( const deException &e ){
		LogException( e );
		StopRuntime();
		throw;
	}
}

void deVROpenVR::StopRuntime(){
	if( ! pSystem ){
		return;
	}
	
	LogInfo( "Shutdown runtime" );
	
	pDevices.Clear();
	
	vr::VR_Shutdown();
	pSystem = nullptr;
}

void deVROpenVR::SetCamera( deCamera* ){
}



// Devices
////////////

int deVROpenVR::GetDeviceCount(){
	return pDevices.GetCount();
}

deInputDevice *deVROpenVR::GetDeviceAt( int index ){
	deInputDevice *device = nullptr;
	
	try{
		device = new deInputDevice;
		pDevices.GetAt( index )->GetInfo( *device );
		
	}catch( const deException & ){
		if( device ){
			device->FreeReference();
		}
		throw;
	}
	
	return device;
}

int deVROpenVR::IndexOfDeviceWithID( const char *id ){
	return pDevices.IndexOfWithID( id );
}


int deVROpenVR::IndexOfButtonWithID( int device, const char *id ){
	return pDevices.GetAt( device )->IndexOfButtonWithID( id );
}

int deVROpenVR::IndexOfAxisWithID( int device, const char *id ){
	return pDevices.GetAt( device )->IndexOfAxisWithID( id );
}

int deVROpenVR::IndexOfFeedbackWithID( int device, const char *id ){
	return pDevices.GetAt( device )->IndexOfFeedbackWithID( id );
}

bool deVROpenVR::GetButtonPressed( int device, int button ){
	return pDevices.GetAt( device )->GetButtonAt( button )->GetPressed();
}

float deVROpenVR::GetAxisValue( int device, int axis ){
	return pDevices.GetAt( device )->GetAxisAt( axis )->GetValue();
}

float deVROpenVR::GetFeedbackValue( int device, int feedback ){
	return pDevices.GetAt( device )->GetFeedbackAt( feedback )->GetValue();
}

void deVROpenVR::SetFeedbackValue( int device, int feedback, float value ){
	pDevices.GetAt( device )->GetFeedbackAt( feedback )->SetValue( value );
}



// Events
///////////

void deVROpenVR::ProcessEvents(){
	if( ! pSystem ){
		return;
	}
	
	vr::VREvent_t event;
	while( pSystem->PollNextEvent( &event, sizeof( event ) ) ){
		switch( event.eventType ){
		case vr::VREvent_TrackedDeviceActivated:
			LogInfoFormat( "ProcessEvents: Tracked device activated %d", event.trackedDeviceIndex );
			pDevices.Add( event.trackedDeviceIndex );
			break;
			
		case vr::VREvent_TrackedDeviceDeactivated:
			LogInfoFormat( "ProcessEvents: Tracked device deactivated %d", event.trackedDeviceIndex );
			pDevices.Remove( event.trackedDeviceIndex );
			break;
			
		case vr::VREvent_TrackedDeviceUpdated:
			LogInfoFormat( "ProcessEvents: Tracked device updated %d", event.trackedDeviceIndex );
// 			pDevices.UpdateParameters( event.trackedDeviceIndex );
			break;
			
		case vr::VREvent_TrackedDeviceUserInteractionStarted:
			LogInfo( "ProcessEvents: HMD put on" );
			break;
			
		case vr::VREvent_TrackedDeviceUserInteractionEnded:
			LogInfo( "ProcessEvents: HMD taken off" );
			break;
			
		case vr::VREvent_IpdChanged:
			LogInfo( "ProcessEvents: IPD changed" );
			break;
			
		case vr::VREvent_EnterStandbyMode:
			LogInfo( "ProcessEvents: Enter StandBy mode" );
			break;
			
		case vr::VREvent_LeaveStandbyMode:
			LogInfo( "ProcessEvents: Leave StandBy mode" );
			break;
			
		case vr::VREvent_TrackedDeviceRoleChanged:
			LogInfoFormat( "ProcessEvents: Tracked device role changed %d", event.trackedDeviceIndex );
			break;
			
		case vr::VREvent_LensDistortionChanged:
			LogInfo( "ProcessEvents: Lens distortion changed" );
			break;
			
		case vr::VREvent_PropertyChanged:
// 			LogInfo( "ProcessEvents: Property changed" );
			break;
			
		case vr::VREvent_ButtonPress:
			LogInfoFormat( "ProcessEvents: Button pressed %d:%d",
				event.trackedDeviceIndex, event.data.controller.button );
			break;
			
		case vr::VREvent_ButtonUnpress:
			LogInfoFormat( "ProcessEvents: Button unpress %d:%d",
				event.trackedDeviceIndex, event.data.controller.button );
			break;
			
		case vr::VREvent_ButtonTouch:
			LogInfoFormat( "ProcessEvents: Button touch %d:%d",
				event.trackedDeviceIndex, event.data.controller.button );
			break;
			
		case vr::VREvent_ButtonUntouch:
			LogInfoFormat( "ProcessEvents: Button untouch %d:%d",
				event.trackedDeviceIndex, event.data.controller.button );
			break;
			
		default:
			LogInfoFormat( "ProcessEvents: Event type %d", event.eventType );
		}
	}
}
