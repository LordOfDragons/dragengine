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
#include <dragengine/common/file/decPath.h>
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
pSystem( nullptr ),
pInput( nullptr ),
pActionSetHandle( vr::k_ulInvalidActionSetHandle )
{
	int i;
	for( i=0; i<InputActionCount; i++ ){
		pActionHandle[ i ] = vr::k_ulInvalidActionHandle;
	}
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

vr::IVRInput &deVROpenVR::GetInput() const{
	if( ! pInput ){
		DETHROW( deeInvalidAction );
	}
	return *pInput;
}

void deVROpenVR::SendEvent( const deInputEvent &event ){
	GetGameEngine()->GetVRSystem()->GetEventQueue().AddEvent( event );
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
		// init runtime
		vr::HmdError error = vr::VRInitError_None;
		pSystem = vr::VR_Init( &error, vr::VRApplication_Scene );
		
		if( ! pSystem || error != vr::VRInitError_None ){
			LogErrorFormat( "Failed starting runtime: %s", vr::VR_GetVRInitErrorAsSymbol( error ) );
			DETHROW_INFO( deeInvalidAction, "Failed starting runtime" );
		}
		
		LogInfoFormat( "Runtime Version: %s", pSystem->GetRuntimeVersion() );
		
		// load input action manifest
		pInput = vr::VRInput();
		
		decPath pathActionManifest( decPath::CreatePathNative( GetNativePathShare() ) );
		pathActionManifest.AddComponent( "input" );
		pathActionManifest.AddComponent( "actions.json" );
		
		vr::EVRInputError inputError = pInput->SetActionManifestPath( pathActionManifest.GetPathNative() );
		if( inputError != vr::VRInputError_None ){
			LogErrorFormat( "Failed loading action manifest: %d", inputError );
			DETHROW_INFO( deeInvalidAction, "Failed loading action manifest" );
		}
		
		// get input handlers
		inputError = pInput->GetActionSetHandle( "/actions/dragengine", &pActionSetHandle );
		if( inputError != vr::VRInputError_None ){
			LogErrorFormat( "Failed retrieving action set handle: %d", inputError );
			DETHROW_INFO( deeInvalidAction, "Failed retrieving action set handle" );
		}
		
		static const struct InputHandleMap{
			eInputActions action;
			const char *path;
		} inputHandleMap[ InputActionCount ] = {
			{ eiaTriggerPress, "/actions/dragengine/in/trigger_press" },
			{ eiaTriggerTouch, "/actions/dragengine/in/trigger_touch" },
			{ eiaTriggerAnalog, "/actions/dragengine/in/trigger_analog" },
			{ eiaTriggerHaptic, "/actions/dragengine/out/trigger_haptic" },
			{ eiaButtonPrimaryPress, "/actions/dragengine/in/button_primary_press" },
			{ eiaButtonPrimaryTouch, "/actions/dragengine/in/button_primary_touch" },
			{ eiaButtonSecondaryPress, "/actions/dragengine/in/button_secondary_press" },
			{ eiaButtonSecondaryTouch, "/actions/dragengine/in/button_secondary_touch" },
			{ eiaJoystickPress, "/actions/dragengine/in/joystick_press" },
			{ eiaJoystickTouch, "/actions/dragengine/in/joystick_touch" },
			{ eiaJoystickAnalog, "/actions/dragengine/in/joystick_analog" },
			{ eiaTrackpadPress, "/actions/dragengine/in/trackpad_press" },
			{ eiaTrackpadTouch, "/actions/dragengine/in/trackpad_touch" },
			{ eiaTrackpadAnalog, "/actions/dragengine/in/trackpad_analog" },
			{ eiaGripPress, "/actions/dragengine/in/grip_press" },
			{ eiaGripTouch, "/actions/dragengine/in/grip_touch" },
			{ eiaGripGrab, "/actions/dragengine/in/grip_grab" },
			{ eiaGripSqueeze, "/actions/dragengine/in/grip_squeeze" },
			{ eiaGripPinch, "/actions/dragengine/in/grip_pinch" },
			{ eiaGripHaptic, "/actions/dragengine/out/grip_haptic" },
			{ eiaPose, "/actions/dragengine/in/pose" },
			{ eiaSkeletonHandRight, "/actions/dragengine/in/skeleton_hand_right" },
			{ eiaSkeletonHandLeft, "/actions/dragengine/in/skeleton_hand_left" }
		};
		int i;
		for( i=0; i<InputActionCount; i++ ){
			inputError = pInput->GetActionHandle( inputHandleMap[ i ].path, &pActionHandle[ inputHandleMap[ i ].action ] );
			if( inputError != vr::VRInputError_None ){
				LogErrorFormat( "Failed retrieving action handle '%s': %d", inputHandleMap[ i ].path, inputError );
				DETHROW_INFO( deeInvalidAction, "Failed retrieving action handle" );
			}
		}
		
		// update action state
		vr::VRActiveActionSet_t actionSet;
		memset( &actionSet, 0, sizeof( actionSet ) );
		actionSet.nPriority = 0;
		actionSet.ulActionSet = pActionSetHandle;
		actionSet.ulRestrictedToDevice = vr::k_ulInvalidInputValueHandle;
		actionSet.ulSecondaryActionSet = vr::k_ulInvalidActionSetHandle;
		inputError = vr::VRInput()->UpdateActionState( &actionSet, sizeof( actionSet ), 1 );
		if( inputError != vr::VRInputError_None ){
			LogErrorFormat( "Failed update action state: %d", inputError );
			DETHROW_INFO( deeInvalidAction, "Failed update action state" );
		}
		
		// init devies
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
	
	int i;
	for( i=0; i<InputActionCount; i++ ){
		pActionHandle[ i ] = vr::k_ulInvalidActionHandle;
	}
	
	pActionSetHandle = vr::k_ulInvalidActionSetHandle;
	
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

bool deVROpenVR::GetButtonTouched( int device, int button ){
	return pDevices.GetAt( device )->GetButtonAt( button )->GetTouched();
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

void deVROpenVR::GetDevicePose( int device, deInputDevicePose &pose ){
	pDevices.GetAt( device )->GetDevicePose( pose );
}

void deVROpenVR::GetDeviceBonePose( int device, int bone,
bool withController, deInputDevicePose &pose ){
	pDevices.GetAt( device )->GetBonePose( bone, withController, pose );
}



// Events
///////////

void deVROpenVR::ProcessEvents(){
	if( ! pSystem ){
		return;
	}
	
	vr::VREvent_t event;
	while( pSystem->PollNextEvent( &event, sizeof( event ) ) ){
		switch( ( vr::EVREventType )event.eventType ){
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
			
		case vr::VREvent_LensDistortionChanged:
			LogInfo( "ProcessEvents: Lens distortion changed" );
			break;
			
		case vr::VREvent_Quit:
			LogInfo( "ProcessEvents: VR quit request received" );
			
			// buy us some time before steam tries to kill us
			pSystem->AcknowledgeQuit_Exiting();
			
			// shutdown VR runtime. this should prevent us from getting killed
			// TODO add a notification to the scripting system so it can be notified
			StopRuntime();
			
			// do not continue since runtime is stopped
			return;
			
// 		case vr::VREvent_ButtonPress: // old input system
// 		case vr::VREvent_ButtonUnpress: // old input system
// 		case vr::VREvent_ButtonTouch: // old input system
// 		case vr::VREvent_ButtonUntouch: // old input system
			
		case vr::VREvent_TrackedDeviceRoleChanged:
		case vr::VREvent_PropertyChanged:
		case vr::VREvent_ProcessConnected:
		case vr::VREvent_SceneApplicationStateChanged:
		case vr::VREvent_Input_BindingLoadSuccessful:
		case vr::VREvent_Input_BindingLoadFailed:
		case vr::VREvent_Input_ActionManifestReloaded:
		case vr::VREvent_ActionBindingReloaded:
		case vr::VREvent_Input_HapticVibration:
			break;
			
		default:
			LogInfoFormat( "ProcessEvents: Event type %d (%s)", event.eventType,
				pSystem->GetEventTypeNameFromEnum( ( vr::EVREventType )event.eventType ) );
			break;
		}
	}
	
	vr::VRActiveActionSet_t actionSet;
	memset( &actionSet, 0, sizeof( actionSet ) );
	actionSet.nPriority = 0;
	actionSet.ulActionSet = pActionSetHandle;
	actionSet.ulRestrictedToDevice = vr::k_ulInvalidInputValueHandle;
	actionSet.ulSecondaryActionSet = vr::k_ulInvalidActionSetHandle;
	vr::EVRInputError inputError = vr::VRInput()->UpdateActionState( &actionSet, sizeof( actionSet ), 1 );
	if( inputError != vr::VRInputError_None ){
		LogErrorFormat( "Failed update action state: %d", inputError );
		DETHROW_INFO( deeInvalidAction, "Failed update action state" );
	}
	
	pDevices.TrackDeviceStates();
}
