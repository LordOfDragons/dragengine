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
#include "device/profile/deoxrDPHMD.h"
#include "device/profile/deoxrDPSimpleController.h"
#include "device/profile/deoxrDPHTCViveController.h"
#include "device/profile/deoxrDPGoogleDaydreamController.h"
#include "device/profile/deoxrDPHTCVivePro.h"
#include "device/profile/deoxrDPMicrosoftMixedRealityMotionController.h"
#include "device/profile/deoxrDPMicrosoftXboxController.h"
#include "device/profile/deoxrDPOculusGoController.h"
#include "device/profile/deoxrDPValveIndexController.h"
#include "device/profile/deoxrDPHtcViveTracker.h"

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
pGraphicApiOpenGL( *this ),
pLoader( nullptr ),
pSessionState( XR_SESSION_STATE_UNKNOWN )
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

void deVROpenXR::WaitUntilReadyExit(){
	if( ! pInstance || pSessionState == XR_SESSION_STATE_EXITING ){
		return;
	}
	
	deoxrInstance &instance = pInstance;
	
	XrEventDataBuffer event;
	memset( &event, 0, sizeof( event ) );
	event.type = XR_TYPE_EVENT_DATA_BUFFER;
	
	while( pSessionState != XR_SESSION_STATE_EXITING ){
		const XrResult result = instance.xrPollEvent( instance.GetInstance(), &event );
		if( result == XR_SUCCESS ){
			LogInfoFormat( "WaitUntilReadyExit: Event %d", event.type );
			if( event.type == XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED ){
				pSessionState = ( ( XrEventDataSessionStateChanged& )event ).state;
				LogInfoFormat( "WaitUntilReadyExit: Session State Changed %d", pSessionState );
			}
		}
	}
}

deoxrSwapchain *deVROpenXR::GetEyeSwapchain( eEye eye ) const{
	if( ! pSession ){
		return nullptr;
	}
	
	switch( eye ){
	case deBaseVRModule::evreLeft:
		return pSession->GetSwapchainLeftEye();
		
	case deBaseVRModule::evreRight:
		return pSession->GetSwapchainRightEye();
		
	default:
		return nullptr;
	}
}



// Module Management
//////////////////////

bool deVROpenXR::Init(){
	const bool enableDebug = true;
	
	pSessionState = XR_SESSION_STATE_UNKNOWN;
	
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
	
	pGraphicApiOpenGL.Unload();
	
	pSessionState = XR_SESSION_STATE_UNKNOWN;
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
	pSystem = nullptr;
	
	pDeviceProfiles.CheckAllAttached();
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
			pSessionState = state.state;
			
			switch( pSessionState ){
			case XR_SESSION_STATE_IDLE:
				LogInfo( "Session State Changed: idle" );
				break;
				
			case XR_SESSION_STATE_READY:
				LogInfo( "Session State Changed: ready" );
				if( pSession ){
					pSession->Begin();
					pDeviceProfiles.CheckAllAttached();
				}
				break;
				
			case XR_SESSION_STATE_SYNCHRONIZED:
				LogInfo( "Session State Changed: synchronized" );
				break;
				
			case XR_SESSION_STATE_VISIBLE:
				LogInfo( "Session State Changed: visible" );
				break;
				
			case XR_SESSION_STATE_FOCUSED:
				LogInfo( "Session State Changed: focused" );
				break;
				
			case XR_SESSION_STATE_STOPPING:
				LogInfo( "Session State Changed: stopping" );
				if( pSession ){
					pSession->End();
					pDeviceProfiles.CheckAllAttached();
				}
				break;
				
			case XR_SESSION_STATE_LOSS_PENDING:
				LogInfo( "Session State Changed: loss pending" );
				break;
				
			case XR_SESSION_STATE_EXITING:
				LogInfo( "Session State Changed: exiting" );
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
			pDeviceProfiles.CheckAllAttached();
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
	
	// according to OpenXR documentation sync action is only allowed in focused state.
	// SteamVR crashes if you try to do it earlier instead of returning an error
	if( pSessionState == XR_SESSION_STATE_FOCUSED ){
		pSession->SyncActions();
		pDevices.TrackDeviceStates();
			// copy state from mutex protected to non mutex protected memory
	}
}



// Graphic Module use only
////////////////////////////

decPoint deVROpenXR::GetRenderSize(){
	if( ! pSystem ){
		return decPoint( 1024, 1024 );
	}
	
	return pSystem->GetRenderSize();
}

void deVROpenXR::GetProjectionParameters( eEye eye, float &left, float &right, float &top, float &bottom ){
	switch( eye ){
	case deBaseVRModule::evreLeft:
		if( pSession ){
			const XrFovf &fov = pSession->GetLeftEyeFov();
			left = fov.angleLeft;
			right = fov.angleRight;
			top = fov.angleUp;
			bottom = fov.angleDown;
			
		}else{
			left = -1.39863f;
			right = 1.24906f;
			top = -1.47526f;
			bottom = 1.46793f;
		}
		break;
		
	case deBaseVRModule::evreRight:
		if( pSession ){
			const XrFovf &fov = pSession->GetRightEyeFov();
			left = fov.angleLeft;
			right = fov.angleRight;
			top = fov.angleUp;
			bottom = fov.angleDown;
			
		}else{
			left = -1.24382;
			right = 1.39166;
			top = -1.47029;
			bottom = 1.45786;
		}
		break;
	}
}

decMatrix deVROpenXR::GetMatrixViewEye( eEye eye ){
	switch( eye ){
	case deBaseVRModule::evreLeft:
		if( pSession ){
			return pSession->GetLeftEyeMatrix();
			
		}else{
			return decMatrix::CreateTranslation(0.0303, 0, 0.015);
		}
		
	case deBaseVRModule::evreRight:
		if( pSession ){
			return pSession->GetRightEyeMatrix();
			
		}else{
			return decMatrix::CreateTranslation(-0.0303, 0, 0.015);
		}
	}
	
	return decMatrix();
}

deModel *deVROpenXR::GetHiddenArea( eEye eye ){
	return nullptr;
}

deImage *deVROpenXR::GetDistortionMap( eEye eye ){
	return nullptr;
}

int deVROpenXR::GetEyeViewImages( eEye eye, int count, void *views ){
	deoxrSwapchain * const swapchain = GetEyeSwapchain( eye );
	if( ! swapchain ){
		return 0;
	}
	
	const int imageCount = swapchain->GetImageCount();
	if( count == 0 ){
		return imageCount;
	}
	
	if( ! views ){
		DETHROW_INFO( deeNullPointer, "images" );
	}
	if( count < imageCount ){
		DETHROW_INFO( deeInvalidParam, "count < imageCount" );
	}
	
	if( pSession->GetIsGACOpenGL() ){
		uint32_t * const viewOpenGL = ( uint32_t* )views;
		int i;
		
		for( i=0; i<imageCount; i++ ){
			viewOpenGL[ i ] = swapchain->GetImageAt( i ).openglImage;
		}
		
	}else{
		DETHROW_INFO( deeInvalidAction, "not implemented yet" );
	}
	
	return imageCount;
}

void deVROpenXR::GetEyeViewRenderTexCoords( eEye eye, decVector2 &tcFrom, decVector2 &tcTo ){
	tcFrom.Set( 0.0f, 0.0f );
	tcTo.Set( 1.0f, 1.0f );
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
			pSession->AttachActionSet( pActionSet );
			
		}catch( const deException &e ){
			LogException( e );
			return;
		}
	}
	
	pSession->BeginFrame();
}

int deVROpenXR::AcquireEyeViewImage( eEye eye ){
	const deMutexGuard lock( pMutexOpenXR );
	deoxrSwapchain * const swapchain = GetEyeSwapchain( eye );
	if( ! swapchain ){
		return -1;
	}
	
	swapchain->AcquireImage();
	return ( int )swapchain->GetAcquiredImage();
}

void deVROpenXR::ReleaseEyeViewImage( eEye eye ){
	const deMutexGuard lock( pMutexOpenXR );
	deoxrSwapchain * const swapchain = GetEyeSwapchain( eye );
	if( swapchain ){
		swapchain->ReleaseImage();
	}
}

void deVROpenXR::SubmitOpenGLTexture2D( eEye eye, void *texture, const decVector2 &tcFrom,
const decVector2 &tcTo, bool distortionApplied ){
	const deMutexGuard lock( pMutexOpenXR );
	deoxrSwapchain * const swapchain = GetEyeSwapchain( eye );
	if( ! swapchain || ! pSession->GetShouldRender() ){
		return;
	}
	
	swapchain->AcquireImage();
	
	// blit
	
	swapchain->ReleaseImage();
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
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHMD( pInstance ) ) );
	
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPSimpleController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHTCViveController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPGoogleDaydreamController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHTCVivePro( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPMicrosoftMixedRealityMotionController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPMicrosoftXboxController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPOculusGoController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPValveIndexController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHtcViveTracker( pInstance ) ) );
}

void deVROpenXR::pSuggestBindings(){
	const int count = pDeviceProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pDeviceProfiles.GetAt( i )->SuggestBindings();
	}
	
	/*
	pSuggestBindingsOculusGoController();
	pSuggestBindingsOculusTouchController();
	
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
	*/
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
