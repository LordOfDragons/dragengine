/* 
 * Drag[en]gine OpenXR VR Module
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

#include "deVROpenXR.h"
#include "deoxrLoader.h"
#include "deoxrPath.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicCamera.h>
#include <dragengine/threading/deMutexGuard.h>


#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *OpenXRCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// Entry Function
///////////////////

deBaseModule *OpenXRCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deVROpenXR( *loadableModule );
		
	}catch( const deException & ){
		return nullptr;
	}
}



// Class deVROpenXR
/////////////////////

// Constructor, destructor
////////////////////////////

deVROpenXR::deVROpenXR( deLoadableModule &loadableModule ) :
deBaseVRModule( loadableModule ),
pLoader( nullptr ),
pFocused( false )
{
	memset( pActions, 0, sizeof( pActions ) );
}

deVROpenXR::~deVROpenXR(){
	CleanUp();
}



// Management
///////////////



// Module Management
//////////////////////

bool deVROpenXR::Init(){
	const bool enableDebug = true;
	
	try{
		pLoader = new deoxrLoader( *this );
		pInstance.TakeOver( new deoxrInstance( *this, enableDebug ) );
		pCreateActionSet();
		pSuggestBindings();
		
	}catch( const deException &e ){
		LogException( e );
		pActionSet = nullptr;
		pInstance = nullptr;
		if( pLoader ){
			delete pLoader;
			pLoader = nullptr;
		}
	}
	
	LogInfoFormat( "Runtime Installed: %s", pInstance ? "Yes" : "No" );
	
	if( pLoader ){
		LogInfoFormat( "Runtime Config File: %s", pLoader->GetRuntimeConfigFile().GetString() );
		LogInfoFormat( "Runtime Library: %s", pLoader->GetRuntimeLibraryPath().GetString() );
	}
	
// 	LogInfoFormat( "HMD Present: %s", vr::VR_IsHmdPresent() ? "Yes" : "No" );
	return true;
}

void deVROpenXR::CleanUp(){
	StopRuntime();
	SetCamera( nullptr );
	
	memset( pActions, 0, sizeof( pActions ) );
	pActionSet = nullptr;
	pInstance = nullptr;
	if( pLoader ){
		delete pLoader;
		pLoader = nullptr;
	}
}



// Runtime
////////////

bool deVROpenXR::RuntimeUsable(){
	return pInstance;
}

void deVROpenXR::StartRuntime(){
	if( ! pInstance ){
		DETHROW_INFO( deeInvalidAction, "runtime not found" );
	}
	
	LogInfo( "Start Runtime" );
	
	try{
		pSystem.TakeOver( new deoxrSystem( pInstance ) );
// 		pSession.TakeOver( new deoxrSession( pSystem ) );
		
	}catch( const deException &e ){
		LogException( e );
		StopRuntime();
		throw;
	}
}

void deVROpenXR::StopRuntime(){
	LogInfo( "Shutdown runtime" );
	pSession = nullptr;
	pFocused = false;
	pSystem = nullptr;
}

void deVROpenXR::SetCamera( deCamera *camera ){
	if( pCamera == camera ){
		return;
	}
	
	if( pCamera && pCamera->GetPeerGraphic() ){
		pCamera->GetPeerGraphic()->VRResignedFromHMD();
	}
	
	pCamera = camera;
	
	if( camera && camera->GetPeerGraphic() ){
		camera->GetPeerGraphic()->VRAssignedToHMD();
	}
}



// Devices
////////////

int deVROpenXR::GetDeviceCount(){
	return 0;
}

deInputDevice *deVROpenXR::GetDeviceAt( int index ){
	DETHROW( deeInvalidParam );
}

int deVROpenXR::IndexOfDeviceWithID( const char *id ){
	return -1;
}


int deVROpenXR::IndexOfButtonWithID( int device, const char *id ){
	return -1;
}

int deVROpenXR::IndexOfAxisWithID( int device, const char *id ){
	return -1;
}

int deVROpenXR::IndexOfFeedbackWithID( int device, const char *id ){
	return -1;
}

int deVROpenXR::IndexOfComponentWithID( int device, const char *id ){
	return -1;
}

bool deVROpenXR::GetButtonPressed( int device, int button ){
	DETHROW( deeInvalidParam );
}

bool deVROpenXR::GetButtonTouched( int device, int button ){
	DETHROW( deeInvalidParam );
}

float deVROpenXR::GetAxisValue( int device, int axis ){
	DETHROW( deeInvalidParam );
}

float deVROpenXR::GetFeedbackValue( int device, int feedback ){
	DETHROW( deeInvalidParam );
}

void deVROpenXR::SetFeedbackValue( int device, int feedback, float value ){
	DETHROW( deeInvalidParam );
}

void deVROpenXR::GetDevicePose( int device, deInputDevicePose &pose ){
	DETHROW( deeInvalidParam );
}

void deVROpenXR::GetDeviceBonePose( int device, int bone, bool withController, deInputDevicePose &pose ){
	DETHROW( deeInvalidParam );
}



// Events
///////////

void deVROpenXR::ProcessEvents(){
	if( ! pInstance ){
		return;
	}
	
	XrEventDataBuffer event;
	memset( &event, 0, sizeof( event ) );
	event.type = XR_TYPE_EVENT_DATA_BUFFER;
	
	deoxrInstance &instance = pInstance;
	
	while( true ){
		const XrResult result = instance.xrPollEvent( instance.GetInstance(), &event );
		if( result == XR_EVENT_UNAVAILABLE ){
			break;
			
		}else if( result != XR_SUCCESS ){
			#ifdef WITH_DEBUG
			OXR_CHECK( *this, result );
			#endif
			break;
		}
		
		LogInfoFormat( "Event: %d", event.type );
		
		switch( event.type ){
		case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
			const XrEventDataSessionStateChanged& state = ( XrEventDataSessionStateChanged& )event;
			switch( state.state ){
			case XR_SESSION_STATE_IDLE:
				LogInfo( "Session State Changed: idle" );
				pFocused = false;
				break;
				
			case XR_SESSION_STATE_READY:
				LogInfo( "Session State Changed: ready" );
				pFocused = false;
				if( pSession ){
					pSession->Begin();
					pSession->AttachActionSet( pActionSet );
				}
				break;
				
			case XR_SESSION_STATE_SYNCHRONIZED:
				LogInfo( "Session State Changed: synchronized" );
				pFocused = false;
				break;
				
			case XR_SESSION_STATE_VISIBLE:
				LogInfo( "Session State Changed: visible" );
				pFocused = false;
				break;
				
			case XR_SESSION_STATE_FOCUSED:
				LogInfo( "Session State Changed: focused" );
				pFocused = true;
				break;
				
			case XR_SESSION_STATE_STOPPING:
				LogInfo( "Session State Changed: stopping" );
				pFocused = false;
				if( pSession ){
					pSession->End();
				}
				break;
				
			case XR_SESSION_STATE_LOSS_PENDING:
				LogInfo( "Session State Changed: loss pending" );
				pFocused = false;
				break;
				
			case XR_SESSION_STATE_EXITING:
				LogInfo( "Session State Changed: exiting" );
				pFocused = false;
				break;
				
			default:
				break;
			}
			}break;
			
		case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
// 			const XrEventDataInstanceLossPending& instance_loss_pending_event =
// 				( XrEventDataInstanceLossPending& )event;
			StopRuntime();
			// according to documentation we could try restarting the runtime (recreating
			// XrInstance) after some time:
			// - lossTime is the absolute time at which the indicated instance will
			//   be considered lost and become unusable
			// After the application has destroyed all of its instances and their children
			// and waited past the specified time, it may then re-try xrCreateInstance in
			// a loop waiting for whatever maintenance the runtime is performing to complete.
			// The runtime will return XR_ERROR_RUNTIME_UNAVAILABLE from xrCreateInstance
			// as long as it is unable to create the instance.
			}break;
			
		case XR_TYPE_EVENT_DATA_INTERACTION_PROFILE_CHANGED:{
			// pfft... what a great design. this event only tells us "something" changed but
			// not "what" changed. we have to check all known top level path to figure out
			// if they changed (and thus connected or disconnected) using
			// 
			// xrGetCurrentInteractionProfile(XrSession session, XrPath topLevelUserPath,
			//    XrInteractionProfileState* interactionProfile);
			// 
			// so "/user/head", "/user/hand/left" and "/user/hand/right" we have to manually
			// track while trackers at least tells us their path but then we still have to
			// manually track them. lots of overhead due to bad design.
			// 
			}break;
			
		case XR_TYPE_EVENT_DATA_VIVE_TRACKER_CONNECTED_HTCX:{
			const XrEventDataViveTrackerConnectedHTCX &connected = ( XrEventDataViveTrackerConnectedHTCX& )event;
			const deoxrPath path( pInstance, connected.paths->persistentPath );
			const deoxrPath pathRole( pInstance, connected.paths->rolePath );
			LogInfoFormat( "VIVE Tracker Connected: path='%s' rolePath='%s'",
				path.GetName().GetString(), pathRole.GetName().GetString() );
			}break;
			
		default:
			break;
		}
	}
}



// Graphic Module use only
////////////////////////////

decPoint deVROpenXR::GetRenderSize(){
	uint32_t width, height;
	width = 1024; height = 1024;
	return decPoint( ( int )width, ( int )height );
}

void deVROpenXR::GetProjectionParameters( eEye eye, float &left, float &right, float &top, float &bottom ){
	left = -1; right = 1; top = -1; bottom = 1;
}

decMatrix deVROpenXR::GetMatrixViewEye( eEye eye ){
	return decMatrix();
}

deModel *deVROpenXR::GetHiddenArea( eEye eye ){
	return nullptr;
}

deImage *deVROpenXR::GetDistortionMap( eEye eye ){
	return nullptr;
}

void deVROpenXR::BeginFrame(){
	if( ! pSystem ){
		return;
	}
	
	if( ! pSession ){
		LogInfo( "BeginFrame: Create Session" );
		try{
			pSession.TakeOver( new deoxrSession( pSystem ) );
			
		}catch( const deException &e ){
			LogException( e );
			return;
		}
	}
	
	const deMutexGuard lock( pMutexState );
	pSession->BeginFrame();
	
	if( pFocused ){
		pSession->SyncActions();
	}
}

void deVROpenXR::SubmitOpenGLTexture2D( eEye eye, void *texture, const decVector2 &tcFrom,
const decVector2 &tcTo, bool distortionApplied ){
	if( ! pSession ){
		return;
	}
}

void deVROpenXR::EndFrame(){
	if( ! pSession ){
		return;
	}
	
	pSession->EndFrame();
}



// Private Functions
//////////////////////

void deVROpenXR::pCreateActionSet(){
	pActionSet.TakeOver( new deoxrActionSet( pInstance ) );
	
	pActionSet->AddAction( deoxrAction::etInputBool, "trigger_press", "Press Trigger" );
	pActionSet->AddAction( deoxrAction::etInputBool, "trigger_touch", "Touch Trigger" );
	pActionSet->AddAction( deoxrAction::etInputFloat, "trigger_analog", "Pull Trigger" );
	pActionSet->AddAction( deoxrAction::etOutputVibration, "trigger_haptic", "Trigger Haptic" );
	
	pActionSet->AddAction( deoxrAction::etInputBool, "button_primary_press", "Press Primary Button" );
	pActionSet->AddAction( deoxrAction::etInputBool, "button_primary_touch", "Touch Button Primary" );
	
	pActionSet->AddAction( deoxrAction::etInputBool, "button_secondary_press", "Press Secondary Button" );
	pActionSet->AddAction( deoxrAction::etInputBool, "button_secondary_touch", "Touch Button Secondary" );
	
	pActionSet->AddAction( deoxrAction::etInputBool, "joystick_press", "Press Joystick" );
	pActionSet->AddAction( deoxrAction::etInputBool, "joystick_touch", "Touch Joystick" );
	pActionSet->AddAction( deoxrAction::etInputVector2, "joystick_analog", "Joystick Analog" );
	
	pActionSet->AddAction( deoxrAction::etInputBool, "trackpad_press", "Press TrackPad" );
	pActionSet->AddAction( deoxrAction::etInputBool, "trackpad_touch", "Touch TrackPad" );
	pActionSet->AddAction( deoxrAction::etInputVector2, "trackpad_analog", "TrackPad Analog" );
	
	pActionSet->AddAction( deoxrAction::etInputBool, "grip_press", "Squeeze Grip" );
	pActionSet->AddAction( deoxrAction::etInputBool, "grip_touch", "Touch Grip" );
	pActionSet->AddAction( deoxrAction::etInputFloat, "grip_grab", "Grip Grab" );
	pActionSet->AddAction( deoxrAction::etInputFloat, "grip_squeeze", "Grip Squeeze" );
	pActionSet->AddAction( deoxrAction::etInputFloat, "grip_pinch", "Grip Pinch" );
	pActionSet->AddAction( deoxrAction::etOutputVibration, "grip_haptic", "Haptic Grip" );
	
	pActionSet->AddAction( deoxrAction::etInputPose, "pose", "Pose" );
	pActionSet->AddAction( deoxrAction::etInputPose, "skeleton_hand_right", "Skeleton Hand Right" ); // "skeleton": "/skeleton/hand/right"
	pActionSet->AddAction( deoxrAction::etInputPose, "skeleton_hand_left", "Skeleton Hand Left" ); // "skeleton": "/skeleton/hand/left"
	
	// store actions for quick retrieval
	int i;
	for( i=0; i<InputActionCount; i++ ){
		pActions[ i ] = pActionSet->GetActionAt( i );
	}
}

void deVROpenXR::pSuggestBindings(){
	pSuggestBindingsSimpleController();
	pSuggestBindingsDaydreamController();
	pSuggestBindingsHTCViveController();
	pSuggestBindingsHTCVivePro();
	pSuggestBindingsMicrosoftMixedRealityMotionController();
	pSuggestBindingsMicrosoftXboxController();
	pSuggestBindingsOculusGoController();
	pSuggestBindingsOculusTouchController();
	pSuggestBindingsValveIndexController();
	
	if( pInstance->SupportsExtension( deoxrInstance::extEXTEyeGazeInteraction ) ){
		pSuggestBindingsEyeGazeInput();
	}
// 	if( pInstance->SupportsExtension( deoxrInstance::extHPMixedRealityController ) ){
// 		pSuggestBindingsHPMixedRealityController();
// 	}
// 	if( pInstance->SupportsExtension( deoxrInstance::extEXTSamsungOdysseyController ) ){
// 		pSuggestBindingsSamsungOdysseyController();
// 	}
// 	if( pInstance->SupportsExtension( deoxrInstance::extHTCVveCosmosControllerInteraction ) ){
// 		pSuggestBindingsHTCVveCosmosControllerInteraction();
// 	}
// 	if( pInstance->SupportsExtension( deoxrInstance::extHTCViveFocus3ControllerInteraction ) ){
// 		pSuggestBindingsHTCViveFocus3ControllerInteraction();
// 	}
// 	if( pInstance->SupportsExtension( deoxrInstance::extHUAWEIControllerInteraction ) ){
// 		pSuggestBindingsHUAWEIControllerInteraction();
// 	}
// 	if( pInstance->SupportsExtension( deoxrInstance::extMSFTHandInteraction ) ){
// 		pSuggestBindingsMSFTHandInteraction();
// 	}
	if( pInstance->SupportsExtension( deoxrInstance::extHTCXViveTrackerInteraction ) ){
		pSuggestBindingsHTCXViveTrackerInteraction();
	}
}

void deVROpenXR::pSuggestBindingsSimpleController(){
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/select/click
	// - /input/menu/click
	// - /input/grip/pose
	// - /input/aim/pose
	// - /output/haptic
	
	deoxrInstance::sSuggestBinding bindings[ 12 ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	( b++ )->Set( pActions[ eiaGripHaptic ], deoxrPath( pInstance, "/user/hand/right/output/haptic" ) );
	( b++ )->Set( pActions[ eiaGripHaptic ], deoxrPath( pInstance, "/user/hand/left/output/haptic" ) );
	
	( b++ )->Set( pActions[ eiaTriggerTouch ], deoxrPath( pInstance, "/user/hand/left/input/select/click" ) );
	( b++ )->Set( pActions[ eiaTriggerPress ], deoxrPath( pInstance, "/user/hand/left/input/select/click" ) );
	( b++ )->Set( pActions[ eiaTriggerAnalog ], deoxrPath( pInstance, "/user/hand/left/input/select/click" ) );
	
	( b++ )->Set( pActions[ eiaTriggerTouch ], deoxrPath( pInstance, "/user/hand/right/input/select/click" ) );
	( b++ )->Set( pActions[ eiaTriggerPress ], deoxrPath( pInstance, "/user/hand/right/input/select/click" ) );
	( b++ )->Set( pActions[ eiaTriggerAnalog ], deoxrPath( pInstance, "/user/hand/right/input/select/click" ) );
	
	( b++ )->Set( pActions[ eiaButtonPrimaryPress ], deoxrPath( pInstance, "/user/hand/left/input/menu/click" ) );
	( b++ )->Set( pActions[ eiaButtonPrimaryTouch ], deoxrPath( pInstance, "/user/hand/left/input/menu/click" ) );
	
	( b++ )->Set( pActions[ eiaButtonPrimaryPress ], deoxrPath( pInstance, "/user/hand/right/input/menu/click" ) );
	( b++ )->Set( pActions[ eiaButtonPrimaryTouch ], deoxrPath( pInstance, "/user/hand/right/input/menu/click" ) );
	
	pInstance->SuggestBindings( deoxrPath( pInstance, "/interaction_profiles/khr/simple_controller" ), bindings, 12 );
}

void deVROpenXR::pSuggestBindingsDaydreamController(){
	// Path: /interaction_profiles/google/daydream_controller
	// 
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/select/click
	// - /input/trackpad/x
	// - /input/trackpad/y
	// - /input/trackpad/click
	// - /input/trackpad/touch
	// - /input/grip/pose
	// - /input/aim/pose
}

void deVROpenXR::pSuggestBindingsHTCViveController(){
	// Path: /interaction_profiles/htc/vive_controller
	// 
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/system/click (may not be available for application use)
	// - /input/squeeze/click
	// - /input/menu/click
	// - /input/trigger/click
	// - /input/trigger/value
	// - /input/trackpad/x
	// - /input/trackpad/y
	// - /input/trackpad/click
	// - /input/trackpad/touch
	// - /input/grip/pose
	// - /input/aim/pose
	// - /output/haptic
}

void deVROpenXR::pSuggestBindingsHTCVivePro(){
	// Path: /interaction_profiles/htc/vive_pro
	// 
	// Valid for user paths:
	// - /user/head
	// 
	// Supported component paths:
	// - /input/system/click (may not be available for application use)
	// - /input/volume_up/click
	// - /input/volume_down/click
	// - /input/mute_mic/click
}

void deVROpenXR::pSuggestBindingsMicrosoftMixedRealityMotionController(){
	// Path: /interaction_profiles/microsoft/motion_controller
	// 
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/menu/click
	// - /input/squeeze/click
	// - /input/trigger/value
	// - /input/thumbstick/x
	// - /input/thumbstick/y
	// - /input/thumbstick/click
	// - /input/trackpad/x
	// - /input/trackpad/y
	// - /input/trackpad/click
	// - /input/trackpad/touch
	// - /input/grip/pose
	// - /input/aim/pose
	// - /output/haptic
}

void deVROpenXR::pSuggestBindingsMicrosoftXboxController(){
	// Path: /interaction_profiles/microsoft/xbox_controller
	// 
	// Valid for user paths:
	// - /user/gamepad
	// 
	// Supported component paths:
	// - /input/menu/click
	// - /input/view/click
	// - /input/a/click
	// - /input/b/click
	// - /input/x/click
	// - /input/y/click
	// - /input/dpad_down/click
	// - /input/dpad_right/click
	// - /input/dpad_up/click
	// - /input/dpad_left/click
	// - /input/shoulder_left/click
	// - /input/shoulder_right/click
	// - /input/thumbstick_left/click
	// - /input/thumbstick_right/click
	// - /input/trigger_left/value
	// - /input/trigger_right/value
	// - /input/thumbstick_left/x
	// - /input/thumbstick_left/y
	// - /input/thumbstick_right/x
	// - /input/thumbstick_right/y
	// - /output/haptic_left
	// - /output/haptic_right
	// - /output/haptic_left_trigger
	// - /output/haptic_right_trigger
}

void deVROpenXR::pSuggestBindingsOculusGoController(){
	// Path: /interaction_profiles/oculus/go_controller
	// 
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/system/click (may not be available for application use)
	// - /input/trigger/click
	// - /input/back/click
	// - /input/trackpad/x
	// - /input/trackpad/y
	// - /input/trackpad/click
	// - /input/trackpad/touch
	// - /input/grip/pose
	// - /input/aim/pose
}

void deVROpenXR::pSuggestBindingsOculusTouchController(){
	// Path: /interaction_profiles/oculus/touch_controller
	// 
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// On /user/hand/left only:
	// - /input/x/click
	// - /input/x/touch
	// - /input/y/click
	// - /input/y/touch
	// - /input/menu/click
	// 
	// On /user/hand/right only:
	// - /input/a/click
	// - /input/a/touch
	// - /input/b/click
	// - /input/b/touch
	// - /input/system/click (may not be available for application use)
	// 
	// Both hands:
	// - /input/squeeze/value
	// - /input/trigger/value
	// - /input/trigger/touch
	// - /input/thumbstick/x
	// - /input/thumbstick/y
	// - /input/thumbstick/click
	// - /input/thumbstick/touch
	// - /input/thumbrest/touch
	// - /input/grip/pose
	// - /input/aim/pose
	// - /output/haptic
}

void deVROpenXR::pSuggestBindingsValveIndexController(){
	// Path: /interaction_profiles/valve/index_controller
	// 
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/system/click (may not be available for application use)
	// - /input/system/touch (may not be available for application use)
	// - /input/a/click
	// - /input/a/touch
	// - /input/b/click
	// - /input/b/touch
	// - /input/squeeze/value
	// - /input/squeeze/force
	// - /input/trigger/click
	// - /input/trigger/value
	// - /input/trigger/touch
	// - /input/thumbstick/x
	// - /input/thumbstick/y
	// - /input/thumbstick/click
	// - /input/thumbstick/touch
	// - /input/trackpad/x
	// - /input/trackpad/y
	// - /input/trackpad/force
	// - /input/trackpad/touch
	// - /input/grip/pose
	// - /input/aim/pose
	// - /output/haptic
}

void deVROpenXR::pSuggestBindingsEyeGazeInput(){
	// Extension: XR_EXT_eye_gaze_interaction
	// Path: /interaction_profiles/ext/eye_gaze_interaction
	// 
	// Valid for user path:
	// - /user/eyes_ext
	// 
	// Supported component paths:
	// - /input/gaze_ext/pose
}

void deVROpenXR::pSuggestBindingsHPMixedRealityController(){
	// Extension: XR_EXT_hp_mixed_reality_controller
	// Path: /interaction_profiles/hp/mixed_reality_controller
	// 
	// Valid for the user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// On /user/hand/left only
	// - /input/x/click
	// - /input/y/click
	// 
	// On /user/hand/right only:
	// - /input/a/click
	// - /input/b/click
	// 
	// On both hands:
	// - /input/menu/click
	// - /input/squeeze/value
	// - /input/trigger/value
	// - /input/thumbstick/x
	// - /input/thumbstick/y
	// - /input/thumbstick/click
	// - /input/grip/pose
	// - /input/aim/pose
	// - /output/haptic
}

void deVROpenXR::pSuggestBindingsSamsungOdysseyController(){
	// Extension: XR_EXT_samsung_odyssey_controller
	// Path: /interaction_profiles/samsung/odyssey_controller
	// Same as /interaction_profiles/microsoft/motion_controller
}

void deVROpenXR::pSuggestBindingsHTCViveCosmosControllerInteraction(){
	// Extension: XR_HTC_vive_cosmos_controller_interaction
	// Path: /interaction_profiles/htc/vive_cosmos_controller
	// 
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// On /user/hand/left only:
	// - /input/x/click
	// - /input/y/click
	// - /input/menu/click
	// 
	// On /user/hand/right only:
	// - /input/a/click
	// - /input/b/click
	// - /input/system/click (may not be available for application use)
	// 
	// On Both Hands:
	// - /input/shoulder/click
	// - /input/squeeze/click
	// - /input/trigger/click
	// - /input/trigger/value
	// - /input/thumbstick/x
	// - /input/thumbstick/y
	// - /input/thumbstick/click
	// - /input/thumbstick/touch
	// - /input/grip/pose
	// - /input/aim/pose
	// - /output/haptic
}

void deVROpenXR::pSuggestBindingsHTCViveFocus3ControllerInteraction(){
	// Extension: XR_HTC_vive_focus3_controller_interaction
	// Path: /interaction_profiles/htc/vive_focus3_controller
	// 
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// On /user/hand/left only:
	// - /input/x/click
	// - /input/y/click
	// - /input/menu/click
	// 
	// On /user/hand/right only:
	// - /input/a/click
	// - /input/b/click
	// - /input/system/click (may not be available for application use)
	// 
	// On Both Hands:
	// - /input/squeeze/click
	// - /input/squeeze/touch
	// - /input/trigger/click
	// - /input/trigger/touch
	// - /input/trigger/value
	// - /input/thumbstick/x
	// - /input/thumbstick/y
	// - /input/thumbstick/click
	// - /input/thumbstick/touch
	// - /input/thumbrest/touch
	// - /input/grip/pose
	// - /input/aim/pose
	// - /output/haptic
}

void deVROpenXR::pSuggestBindingsHUAWEIControllerInteraction(){
	// Extension: XR_HUAWEI_controller_interaction
	// Path: /interaction_profiles/huawei/controller
	// 
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/home/click
	// - /input/back/click
	// - /input/volume_up/click
	// - /input/volume_down/click
	// - /input/trigger/value
	// - /input/trigger/click
	// - /input/trackpad/x
	// - /input/trackpad/y
	// - /input/trackpad/click
	// - /input/trackpad/touch
	// - /input/aim/pose
	// - /input/grip/pose
	// - /output/haptic
}

void deVROpenXR::pSuggestBindingsMSFTHandInteraction(){
	// Extension: XR_MSFT_hand_interaction
	// Path: /interaction_profiles/microsoft/hand_interaction
	// 
	// Valid for top level user path:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/select/value
	// - /input/squeeze/value
	// - /input/aim/pose
	// - /input/grip/pose
}

void deVROpenXR::pSuggestBindingsHTCXViveTrackerInteraction(){
	// Extension: XR_HTCX_vive_tracker_interaction
	// Path: /interaction_profiles/htc/vive_tracker_htcx
	// 
	// Valid for top level user path:
	// - VIVE tracker persistent path (unspecified format, enumerate)
	// - /user/vive_tracker_htcx/role/<role-type> (find by assigned role)
	//   - XR_NULL_PATH
	//   - handheld_object
	//   - left_foot
	//   - right_foot
	//   - left_shoulder
	//   - right_shoulder
	//   - left_elbow
	//   - right_elbow
	//   - left_knee
	//   - right_knee
	//   - waist
	//   - chest
	//   - camera
	//   - keyboard
	// 
	// Supported component paths:
	// - /input/system/click (may not be available for application use)
	// - /input/menu/click
	// - /input/trigger/click
	// - /input/squeeze/click
	// - /input/trigger/value
	// - /input/trackpad/x
	// - /input/trackpad/y
	// - /input/trackpad/click
	// - /input/trackpad/touch
	// - /input/grip/pose
	// - /output/haptic
	// 
	// Enumeration support:
	// typedef struct XrViveTrackerPathsHTCX {
	//    XrStructureType    type;
	//    void*              next;
	//    XrPath             persistentPath;
	//    XrPath             rolePath;
	// } XrViveTrackerPathsHTCX;
	// 
	// xrEnumerateViveTrackerPathsHTCX(
	//    XrInstance                                  instance,
	//    uint32_t                                    pathCapacityInput, /* use 0 to get required count in pathCountOutput */
	//    uint32_t*                                   pathCountOutput,
	//    XrViveTrackerPathsHTCX*                     paths);
	
}
