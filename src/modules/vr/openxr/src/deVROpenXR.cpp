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
#include "device/deoxrDevice.h"
#include "device/deoxrDeviceAxis.h"
#include "device/deoxrDeviceButton.h"
#include "device/deoxrDeviceFeedback.h"
#include "device/profile/deoxrDPSimpleController.h"
#include "device/profile/deoxrDPHTCViveController.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/utils/decDateTime.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputDevice.h>
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
pDevices( *this ),
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

void deVROpenXR::SendEvent( const deInputEvent &event ){
	GetGameEngine()->GetVRSystem()->GetEventQueue().AddEvent( event );
}

void deVROpenXR::InputEventSetTimestamp( deInputEvent &event ) const{
	#ifdef OS_W32
	event.SetTime( { ( long )decDateTime().ToSystemTime(), 0 } );
	#else
	event.SetTime( { decDateTime().ToSystemTime(), 0 } );
	#endif
}



// Module Management
//////////////////////

bool deVROpenXR::Init(){
	const bool enableDebug = true;
	
	try{
		pLoader = new deoxrLoader( *this );
		pInstance.TakeOver( new deoxrInstance( *this, enableDebug ) );
		pCreateActionSet();
		pCreateDeviceProfiles();
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
	
	const deMutexGuard lock( pMutexOpenXR );
	SetCamera( nullptr );
	
	pDevices.Clear();
	pDeviceProfiles.RemoveAll();
	
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
		const deMutexGuard lock( pMutexOpenXR );
		pSystem.TakeOver( new deoxrSystem( pInstance ) );
		
	}catch( const deException &e ){
		LogException( e );
		StopRuntime();
		throw;
	}
}

void deVROpenXR::StopRuntime(){
	LogInfo( "Shutdown runtime" );
	const deMutexGuard lock( pMutexOpenXR );
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
	return pDevices.GetCount();
}

deInputDevice *deVROpenXR::GetDeviceAt( int index ){
	deInputDevice::Ref device( deInputDevice::Ref::New( new deInputDevice ) );
	pDevices.GetAt( index )->GetInfo( *device );
	
	device->AddReference(); // caller takes over reference
	return device;
}

int deVROpenXR::IndexOfDeviceWithID( const char *id ){
	return pDevices.IndexOfWithID( id );
}


int deVROpenXR::IndexOfButtonWithID( int device, const char *id ){
	return pDevices.GetAt( device )->IndexOfButtonWithID( id );
}

int deVROpenXR::IndexOfAxisWithID( int device, const char *id ){
	return pDevices.GetAt( device )->IndexOfAxisWithID( id );
}

int deVROpenXR::IndexOfFeedbackWithID( int device, const char *id ){
	return pDevices.GetAt( device )->IndexOfFeedbackWithID( id );
}

int deVROpenXR::IndexOfComponentWithID( int device, const char *id ){
	return pDevices.GetAt( device )->IndexOfComponentWithID( id );
}

bool deVROpenXR::GetButtonPressed( int device, int button ){
	return pDevices.GetAt( device )->GetButtonAt( button )->GetPressed();
}

bool deVROpenXR::GetButtonTouched( int device, int button ){
	return pDevices.GetAt( device )->GetButtonAt( button )->GetTouched();
}

float deVROpenXR::GetAxisValue( int device, int axis ){
	return pDevices.GetAt( device )->GetAxisAt( axis )->GetValue();
}

float deVROpenXR::GetFeedbackValue( int device, int feedback ){
	return pDevices.GetAt( device )->GetFeedbackAt( feedback )->GetValue();
}

void deVROpenXR::SetFeedbackValue( int device, int feedback, float value ){
	pDevices.GetAt( device )->GetFeedbackAt( feedback )->SetValue( value );
}

void deVROpenXR::GetDevicePose( int device, deInputDevicePose &pose ){
	pDevices.GetAt( device )->GetDevicePose( pose );
}

void deVROpenXR::GetDeviceBonePose( int device, int bone, bool withController, deInputDevicePose &pose ){
	pDevices.GetAt( device )->GetBonePose( bone, withController, pose );
}



// Events
///////////

void deVROpenXR::ProcessEvents(){
	if( ! pInstance ){
		return;
	}
	
	const deMutexGuard lock( pMutexOpenXR );
	deoxrInstance &instance = pInstance;
	
	XrEventDataBuffer event;
	memset( &event, 0, sizeof( event ) );
	event.type = XR_TYPE_EVENT_DATA_BUFFER;
	
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
	
	if( pFocused ){
		pSession->SyncActions();
// 		pDevices.TrackDeviceStates();
			// copy state from mutex protected to non mutex protected memory
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
	const deMutexGuard lock( pMutexOpenXR );
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
	
	pSession->BeginFrame();
}

void deVROpenXR::SubmitOpenGLTexture2D( eEye eye, void *texture, const decVector2 &tcFrom,
const decVector2 &tcTo, bool distortionApplied ){
	const deMutexGuard lock( pMutexOpenXR );
	if( ! pSession ){
		return;
	}
}

void deVROpenXR::EndFrame(){
	const deMutexGuard lock( pMutexOpenXR );
	if( ! pSession ){
		return;
	}
	
	pSession->EndFrame();
}



// Private Functions
//////////////////////

void deVROpenXR::pCreateActionSet(){
	pActionSet.TakeOver( new deoxrActionSet( pInstance ) );
	
	pActionSet->AddBoolAction( "trigger_press", "Press Trigger" );
	pActionSet->AddBoolAction( "trigger_touch", "Touch Trigger" );
	pActionSet->AddFloatAction( "trigger_analog", "Pull Trigger" );
	pActionSet->AddVibrationAction( "trigger_haptic", "Trigger Haptic" );
	
	pActionSet->AddBoolAction( "button_primary_press", "Press Primary Button" );
	pActionSet->AddBoolAction( "button_primary_touch", "Touch Button Primary" );
	
	pActionSet->AddBoolAction( "button_secondary_press", "Press Secondary Button" );
	pActionSet->AddBoolAction( "button_secondary_touch", "Touch Button Secondary" );
	
	pActionSet->AddBoolAction( "joystick_press", "Press Joystick" );
	pActionSet->AddBoolAction( "joystick_touch", "Touch Joystick" );
	pActionSet->AddVector2Action( "joystick_analog", "Joystick Analog" );
	
	pActionSet->AddBoolAction( "trackpad_press", "Press TrackPad" );
	pActionSet->AddBoolAction( "trackpad_touch", "Touch TrackPad" );
	pActionSet->AddVector2Action( "trackpad_analog", "TrackPad Analog" );
	
	pActionSet->AddBoolAction( "grip_press", "Squeeze Grip" );
	pActionSet->AddBoolAction( "grip_touch", "Touch Grip" );
	pActionSet->AddFloatAction( "grip_grab", "Grip Grab" );
	pActionSet->AddFloatAction( "grip_squeeze", "Grip Squeeze" );
	pActionSet->AddFloatAction( "grip_pinch", "Grip Pinch" );
	pActionSet->AddVibrationAction( "grip_haptic", "Haptic Grip" );
	
	pActionSet->AddPoseAction( "pose", "Pose" );
	pActionSet->AddPoseAction( "skeleton_hand_right", "Skeleton Hand Right" ); // "skeleton": "/skeleton/hand/right"
	pActionSet->AddPoseAction( "skeleton_hand_left", "Skeleton Hand Left" ); // "skeleton": "/skeleton/hand/left"
	
	// store actions for quick retrieval
	int i;
	for( i=0; i<InputActionCount; i++ ){
		pActions[ i ] = pActionSet->GetActionAt( i );
	}
}

void deVROpenXR::pCreateDeviceProfiles(){
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPSimpleController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHTCViveController( pInstance ) ) );
}

void deVROpenXR::pSuggestBindings(){
	const int count = pDeviceProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pDeviceProfiles.GetAt( i )->SuggestBindings();
	}
	
	/*
	pSuggestBindingsDaydreamController();
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
	*/
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
