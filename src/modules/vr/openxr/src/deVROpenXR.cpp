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
#include "device/profile/deoxrDPOculusTouchController.h"
#include "device/profile/deoxrDPHPMixedRealityController.h"
#include "device/profile/deoxrDPSamsungOdysseyController.h"
#include "device/profile/deoxrDPHTCViveCosmosControllerInteraction.h"
#include "device/profile/deoxrDPHTCViveFocus3ControllerInteraction.h"
#include "device/profile/deoxrDPHUAWEIControllerInteraction.h"
#include "device/profile/deoxrDPMSFTHandInteraction.h"
#include "loader/deoxrLoader.h"

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
pSessionState( XR_SESSION_STATE_UNKNOWN ),
pShutdownRequested( false ),
pPreventDeletion( false ),
pRestartSession( false ),
pLastDetectedSystem( deoxrSystem::esUnknown )
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

void deVROpenXR::RequestRestartSession(){
	pRestartSession = true;
}



// Module Management
//////////////////////

bool deVROpenXR::Init(){
	const bool enableDebug = true;
	
	pSessionState = XR_SESSION_STATE_UNKNOWN;
	pShutdownRequested = false;
	pPreventDeletion = false;
	pRestartSession = false;
	
	try{
		pLoader = new deoxrLoader( *this );
		pInstance.TakeOver( new deoxrInstance( *this, enableDebug ) );
		pCreateDeviceProfiles();
		
	}catch( const deException &e ){
		LogException( e );
		pDeviceProfiles.ClearActions();
		pDestroyActionSet();
		pInstance = nullptr;
		if( pLoader ){
			delete pLoader;
			pLoader = nullptr;
		}
	}
	
	LogInfoFormat( "Runtime Installed: %s", pInstance ? "Yes" : "No" );
	
	/*
	if( pLoader ){
		LogInfoFormat( "Runtime Config File: %s", pLoader->GetRuntimeConfigFile().GetString() );
		LogInfoFormat( "Runtime Library: %s", pLoader->GetRuntimeLibraryPath().GetString() );
	}
	*/
	
	return true;
}

void deVROpenXR::CleanUp(){
	StopRuntime();
	
	const deMutexGuard lock( pMutexOpenXR );
	SetCamera( nullptr );
	
	pDeviceProfiles.RemoveAll(); // has to come before clearing devices
	
	pDevices.Clear();
	
	pDestroyActionSet();
	
	// prevent deletion of graphic api resources that are typically linked to another
	// thread. this will cause memory leaks but better leak than crash if the runtime
	// is buggy or not very resiliant (like SteamVR for example)
	pPreventDeletion = true;
	pSession = nullptr;
	
	// everything below here should be safe
	pSystem = nullptr;
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
	deMutexGuard lock( pMutexOpenXR );
	if( ! pInstance ){
		DETHROW_INFO( deeInvalidAction, "runtime not found" );
	}
	
	LogInfo( "Start Runtime" );
	pShutdownRequested = false;
	
	try{
		pSystem.TakeOver( new deoxrSystem( pInstance ) );
		pLastDetectedSystem = pSystem->GetSystem();
		
	}catch( const deException &e ){
		LogException( e );
		lock.Unlock(); // StopRuntime does lock
		StopRuntime();
		throw;
	}
}

void deVROpenXR::StopRuntime(){
	// we have to delay this since this is the main thread and destroying session
	// does destroy graphic api resources which typically are linked to another thread
	LogInfo( "Shutdown runtime requested" );
	const deMutexGuard lock( pMutexOpenXR );
	
	if( pSession ){
		pShutdownRequested = true;
	}
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
	// with controller not yet supported
	(void)withController;
	deoxrHandTracker * const handTracker = pDevices.GetAt( device )->GetHandTracker();
	if( handTracker ){
		pose = handTracker->GetPoseBoneAt( bone );
	}
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
	
	while( true ){
		// WARNING it is important to set type before every call to xrPollEvent or things turn sour
		event.type = XR_TYPE_EVENT_DATA_BUFFER;
		
		const XrResult result = instance.xrPollEvent( instance.GetInstance(), &event );
		if( result == XR_EVENT_UNAVAILABLE ){
			break;
			
		}else if( result != XR_SUCCESS ){
			OXR_CHECK( result );
			break;
		}
		
// 		LogInfoFormat( "Event: %d", event.type );
		
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
			
			pDeviceProfiles.AllOnSessionStateChanged();
			}break;
			
		case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
			LogInfo( "Instance loss pending. Shutting down runtime" );
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
			// "something" changed but we do not know what. the spec requires us to check
			// all known top level path to figure out if something changed
			LogInfo( "Interaction profile changed. Updating devices." );
			pDeviceProfiles.CheckAllAttached();
			}break;
			
		case XR_TYPE_EVENT_DATA_VISIBILITY_MASK_CHANGED_KHR:
			if( pSession && pInstance->SupportsExtension( deoxrInstance::extKHRVisibilityMask ) ){
				LogInfo( "Visibility mask changed. Updating hidden meshes" );
				const XrEventDataVisibilityMaskChangedKHR &changed = ( XrEventDataVisibilityMaskChangedKHR& )event;
				if( changed.viewConfigurationType == XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO ){
					switch( changed.viewIndex ){
					case 0:
						pSession->UpdateLeftEyeHiddenMesh();
						break;
						
					case 1:
						pSession->UpdateRightEyeHiddenMesh();
						break;
					}
				}
			}
			break;
			
		case XR_TYPE_EVENT_DATA_VIVE_TRACKER_CONNECTED_HTCX:{
			const XrEventDataViveTrackerConnectedHTCX &connected =
				( XrEventDataViveTrackerConnectedHTCX& )event;
			const deoxrPath path( pInstance, connected.paths->persistentPath );
			const deoxrPath pathRole( pInstance, connected.paths->rolePath );
			LogInfoFormat( "VIVE Tracker Connected, updating devices: path='%s' rolePath='%s'",
				path.GetName().GetString(), pathRole.GetName().GetString() );
			//LogInfo( "VIVE Tracker connected. Updating devices" );
			pDeviceProfiles.CheckAllAttached();
			}break;
			
		default:
			break;
		}
	}
	
	// state tracking needs a session
	if( pSession ){
		// according to OpenXR documentation sync action is only allowed in focused state.
		// SteamVR crashes if you try to do it earlier instead of returning an error
		if( pSessionState == XR_SESSION_STATE_FOCUSED ){
			pSession->SyncActions();
			pDeviceProfiles.AllOnActionsSynced();
		}
		
		// but we still want to be able to receive head movement. requires tracking the
		// device states even though actions are not allowed to be synced
		pDevices.TrackDeviceStates();
			// ^=- copies state from mutex protected to non mutex protected memory
	}
}



// Graphic Module use only
////////////////////////////

decPoint deVROpenXR::GetRenderSize(){
	const deMutexGuard lock( pMutexOpenXR );
	if( ! pSystem ){
		return decPoint( 1024, 1024 );
	}
	
	return pSystem->GetRenderSize();
}

void deVROpenXR::GetProjectionParameters( eEye eye, float &left, float &right, float &top, float &bottom ){
	const deMutexGuard lock( pMutexOpenXR );
	// returned values are used directly in projection matrix. these values are also
	// half tan angles from center. hence calculating the tan of the angles works
	switch( eye ){
	case deBaseVRModule::evreLeft:
		if( pSession ){
			const XrFovf &fov = pSession->GetLeftEyeFov();
			left = tanf( fov.angleLeft );
			right = tanf( fov.angleRight );
			top = tanf( -fov.angleUp );
			bottom = tanf( -fov.angleDown );
			
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
			left = tanf( fov.angleLeft );
			right = tanf( fov.angleRight );
			top = tanf( -fov.angleUp );
			bottom = tanf( -fov.angleDown );
			
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
	const deMutexGuard lock( pMutexOpenXR );
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
	const deMutexGuard lock( pMutexOpenXR );
	if( ! pSession ){
		return nullptr;
	}
	
	const deoxrHiddenMesh *hiddenMesh = nullptr;
	
	switch( eye ){
	case deBaseVRModule::evreLeft:
		hiddenMesh = pSession->GetLeftEyeHiddenMesh();
		break;
		
	case deBaseVRModule::evreRight:
		hiddenMesh = pSession->GetRightEyeHiddenMesh();
		break;
	}
	
	return hiddenMesh ? hiddenMesh->GetModel() : nullptr;
}

deImage *deVROpenXR::GetDistortionMap( eEye ){
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
	
	if( pSession && pRestartSession ){
		LogInfo( "Restarting session (somebody requested this)" );
		pDeviceProfiles.ClearActions();
		pSession = nullptr;
		pDestroyActionSet();
		
		pRestartSession = false;
	}
	
	if( ! pSession ){
		if( pShutdownRequested ){
			pRealShutdown();
			return;
		}
		
		LogInfo( "BeginFrame: Create Session" );
		pRestartSession = false;
		try{
			pSession.TakeOver( new deoxrSession( pSystem ) );
			
			pDeviceProfiles.CheckAllAttached();
			pCreateActionSet();
			pSuggestBindings();
			
			pSession->AttachActionSet( pActionSet );
			
		}catch( const deException &e ){
			LogException( e );
			
			LogError( "Runtime failed during BeginFrame. Shutting down runtime. Restart runtime to continue." );
			pDeviceProfiles.ClearActions();
			pDestroyActionSet();
			pSession = nullptr;
			
			pRealShutdown();
			return;
		}
	}
	
	pSession->WaitFrame();
	
	if( pShutdownRequested ){
		pRealShutdown();
		
	}else{
		pSession->BeginFrame();
	}
}

int deVROpenXR::AcquireEyeViewImage( eEye eye ){
	const deMutexGuard lock( pMutexOpenXR );
	deoxrSwapchain * const swapchain = GetEyeSwapchain( eye );
	if( ! swapchain || ! pSession->GetFrameRunning() ){
		return -1;
	}
	
	swapchain->AcquireImage();
	return ( int )swapchain->GetAcquiredImage();
}

void deVROpenXR::ReleaseEyeViewImage( eEye eye ){
	const deMutexGuard lock( pMutexOpenXR );
	deoxrSwapchain * const swapchain = GetEyeSwapchain( eye );
	if( swapchain || ! pSession->GetFrameRunning() ){
		swapchain->ReleaseImage();
	}
}

void deVROpenXR::SubmitOpenGLTexture2D( eEye, void*, const decVector2&, const decVector2&, bool ){
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

void deVROpenXR::pRealShutdown(){
	// WARNING caller has to hold mutex lock while calling function
	LogInfo( "Shutdown runtime" );
	
	pDeviceProfiles.ClearActions();
	
	pSession = nullptr;
	pSystem = nullptr;
	pDestroyActionSet();
	
	pShutdownRequested = false;
}

void deVROpenXR::pCreateActionSet(){
	pActionSet.TakeOver( new deoxrActionSet( pInstance ) );
	
	pActionSet->AddBoolAction( "trigger_press", "Press Trigger" );
	pActionSet->AddBoolAction( "trigger_touch", "Touch Trigger" );
	pActionSet->AddFloatAction( "trigger_analog", "Pull Trigger" );
	pActionSet->AddVibrationAction( "trigger_haptic", "Trigger Haptic" );
	
	pActionSet->AddBoolAction( "button_primary_press", "Press Primary Button" );
	pActionSet->AddBoolAction( "button_primary_touch", "Touch Primary Button" );
	
	pActionSet->AddBoolAction( "button_secondary_press", "Press Secondary Button" );
	pActionSet->AddBoolAction( "button_secondary_touch", "Touch Secondary Button" );
	
	pActionSet->AddBoolAction( "button_auxiliary1_press", "Press Auxiliary Button 1" );
	pActionSet->AddBoolAction( "button_auxiliary1_touch", "Touch Auxiliary Button 1" );
	
	pActionSet->AddBoolAction( "button_auxiliary2_press", "Press Auxiliary Button 2" );
	pActionSet->AddBoolAction( "button_auxiliary2_touch", "Touch Auxiliary Button 2" );
	
	pActionSet->AddBoolAction( "joystick_press", "Press Joystick" );
	pActionSet->AddBoolAction( "joystick_touch", "Touch Joystick" );
	pActionSet->AddVector2Action( "joystick_analog", "Joystick Analog" );
	
	pActionSet->AddBoolAction( "trackpad_press", "Press TrackPad" );
	pActionSet->AddBoolAction( "trackpad_touch", "Touch TrackPad" );
	pActionSet->AddVector2Action( "trackpad_analog", "TrackPad Analog" );
	
	pActionSet->AddBoolAction( "thumbrest_touch", "Touch Thumbrest" );
	
	pActionSet->AddBoolAction( "grip_press", "Squeeze Grip" );
	pActionSet->AddBoolAction( "grip_touch", "Touch Grip" );
	pActionSet->AddFloatAction( "grip_grab", "Grip Grab" );
	pActionSet->AddFloatAction( "grip_squeeze", "Grip Squeeze" );
	pActionSet->AddFloatAction( "grip_pinch", "Grip Pinch" );
	pActionSet->AddVibrationAction( "grip_haptic", "Haptic Grip" );
	
	pActionSet->AddPoseAction( "pose", "Pose" );
	pActionSet->AddPoseAction( "pose_left", "Pose Left" );
	pActionSet->AddPoseAction( "pose_right", "Pose Right" );
	
	// allow device profiles to add actions
	const int count = pDeviceProfiles.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pDeviceProfiles.GetAt( i )->CreateActions( pActionSet );
	}
	
	// store actions for quick retrieval
	for( i=0; i<InputActionCount; i++ ){
		pActions[ i ] = pActionSet->GetActionAt( i );
	}
}

void deVROpenXR::pDestroyActionSet(){
	memset( pActions, 0, sizeof( pActions ) );
	pActionSet = nullptr;
}

void deVROpenXR::pCreateDeviceProfiles(){
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHMD( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHTCVivePro( pInstance ) ) );
	
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPSimpleController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPGoogleDaydreamController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHPMixedRealityController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHTCViveController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHTCViveCosmosControllerInteraction( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHTCViveFocus3ControllerInteraction( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHUAWEIControllerInteraction( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPMicrosoftMixedRealityMotionController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPMicrosoftXboxController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPMSFTHandInteraction( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPOculusGoController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPOculusTouchController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPSamsungOdysseyController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPValveIndexController( pInstance ) ) );
	
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHtcViveTracker( pInstance ) ) );
}

void deVROpenXR::pSuggestBindings(){
	const int count = pDeviceProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoxrDeviceProfile &profile = *pDeviceProfiles.GetAt( i );
		
		try{
			profile.SuggestBindings();
			
		}catch( const deException &e ){
			LogException( e );
			LogWarnFormat( "Device profile '%s' failed suggesting bindings. "
				"Ignoring device profile", profile.GetPath().GetName().GetString() );
		}
	}
	
	/*
	if( pInstance->SupportsExtension( deoxrInstance::extEXTEyeGazeInteraction ) ){
		pSuggestBindingsEyeGazeInput();
	}
	*/
}

#if 0

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

#endif
