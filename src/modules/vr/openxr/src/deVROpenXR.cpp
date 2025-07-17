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

#include "deVROpenXR.h"
#include "deoxrThreadSync.h"
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
#include "device/profile/deoxrDPMetaQuestTouchProController.h"
#include "device/profile/deoxrDPMetaTouchControllerPlus.h"
#include "device/profile/deoxrDPHPMixedRealityController.h"
#include "device/profile/deoxrDPSamsungOdysseyController.h"
#include "device/profile/deoxrDPHTCViveCosmosControllerInteraction.h"
#include "device/profile/deoxrDPHTCViveFocus3ControllerInteraction.h"
#include "device/profile/deoxrDPHUAWEIControllerInteraction.h"
#include "device/profile/deoxrDPMSFTHandInteraction.h"
#include "device/profile/deoxrDPNoControllerHands.h"
#include "device/profile/deoxrDPEyeGazeInteraction.h"
#include "device/profile/deoxrDPHandInteraction.h"
#include "device/profile/deoxrDPHTCHandInteraction.h"
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


#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *OpenXRCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif
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
pLastDetectedSystem( deoxrSystem::esUnknown ),
pThreadSync( nullptr ),
pRequestFeatureEyeGazeTracking( efslDisabled ),
pRequestFeatureFacialTracking( efslDisabled )
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
	event.SetTime( { ( time_t )decDateTime().ToSystemTime(), 0 } );
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
		if( result != XR_SUCCESS ){
			continue;
		}
		
		LogInfoFormat( "WaitUntilReadyExit: Event %d", event.type );
		switch( event.type ){
		case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED:
			pSessionState = ( ( XrEventDataSessionStateChanged& )event ).state;
			
			switch( pSessionState ){
			case XR_SESSION_STATE_IDLE:
				LogInfo( "WaitUntilReadyExit: Session State Changed: idle" );
				if( pSession ){
					pInstance->xrRequestExitSession( pSession->GetSession() );
					pPassthrough = nullptr;
					pSession = nullptr;
					pDeviceProfiles.CheckAllAttached();
				}
				break;
				
			case XR_SESSION_STATE_READY:
				LogInfo( "WaitUntilReadyExit: Session State Changed: ready" );
				break;
				
			case XR_SESSION_STATE_SYNCHRONIZED:
				LogInfo( "WaitUntilReadyExit: Session State Changed: synchronized" );
				break;
				
			case XR_SESSION_STATE_VISIBLE:
				LogInfo( "WaitUntilReadyExit: Session State Changed: visible" );
				if( pSession ){
					pSession->ForceEnd();
				}
				break;
				
			case XR_SESSION_STATE_FOCUSED:
				LogInfo( "WaitUntilReadyExit: Session State Changed: focused" );
				if( pSession ){
					pSession->ForceEnd();
				}
				break;
				
			case XR_SESSION_STATE_STOPPING:
				LogInfo( "WaitUntilReadyExit: Session State Changed: stopping" );
				if( pSession ){
					pSession->ForceEnd();
				}
				break;
				
			case XR_SESSION_STATE_LOSS_PENDING:
				LogInfo( "WaitUntilReadyExit: Session State Changed: loss pending" );
				break;
				
			case XR_SESSION_STATE_EXITING:
				LogInfo( "WaitUntilReadyExit: Session State Changed: exiting" );
				break;
				
			default:
				break;
			}
			break;
			
		default:
			break;
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
	pSessionState = XR_SESSION_STATE_UNKNOWN;
	pShutdownRequested = false;
	pPreventDeletion = false;
	pRestartSession = false;
	
	try{
		pLoader = new deoxrLoader( *this );
		
	}catch( const deException &e ){
		LogException( e );
		if( pLoader ){
			delete pLoader;
			pLoader = nullptr;
		}
	}
	
// 	LogInfoFormat( "Runtime Installed: %s", pInstance ? "Yes" : "No" );
	
	/*
	if( pLoader ){
		LogInfoFormat( "Runtime Config File: %s", pLoader->GetRuntimeConfigFile().GetString() );
		LogInfoFormat( "Runtime Library: %s", pLoader->GetRuntimeLibraryPath().GetString() );
	}
	*/
	
	return true;
}

void deVROpenXR::CleanUp(){
	{
	const deMutexGuard lock( pMutexOpenXR );
	SetCamera( nullptr );
	}
	
	StopRuntime();
// 	WaitUntilReadyExit();
	
	const deMutexGuard lock( pMutexOpenXR );
	pDeviceProfiles.RemoveAll(); // has to come before clearing devices
	
	pDevices.Clear();
	pDevices.CheckNotifyAttachedDetached();
	
	pDestroyActionSet();
	
	// prevent deletion of graphic api resources that are typically linked to another
	// thread. this will cause memory leaks but better leak than crash if the runtime
	// is buggy or not very resiliant (like SteamVR for example)
	pPreventDeletion = true;
	pPassthrough = nullptr;
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
	return true; //pInstance;
}

void deVROpenXR::RequestFeatureEyeGazeTracking( eFeatureSupportLevel level ){
	pRequestFeatureEyeGazeTracking = level;
}

void deVROpenXR::RequestFeatureFacialTracking( eFeatureSupportLevel level ){
	pRequestFeatureFacialTracking = level;
}

void deVROpenXR::StartRuntime(){
	deMutexGuard lock( pMutexOpenXR );
	
	const bool enableDebug = false; //true;
	
	LogInfo( "Start Runtime" );
	pShutdownRequested = false;
	pDeviceProfiles.RemoveAll();
	
	try{
		pInstance.TakeOver( new deoxrInstance( *this, enableDebug ) );
		pCreateDeviceProfiles();
		
		pSystem.TakeOver( new deoxrSystem( pInstance ) );
		pLastDetectedSystem = pSystem->GetSystem();
		
		if( ! pThreadSync ){
			// pThreadSync = new deoxrThreadSync( *this );
			// pThreadSync->Start();
			// for some strange reason this async version can cause XR_ERROR_CALL_ORDER_INVALID.
			// I guess steamvr is not implemented according to spec here... how annoying
		}
		
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
	
	if( pThreadSync ){
		pThreadSync->ExitThread();
		pThreadSync->WaitForExit();
	}
}

bool deVROpenXR::IsRuntimeRunning(){
	return pInstance != nullptr;
}

void deVROpenXR::SetCamera( deCamera *camera ){
	if( pCamera == camera ){
		return;
	}
	
	if( pCamera && pCamera->GetPeerGraphic() ){
		// pCamera->GetPeerGraphic()->VRResignedFromHMD();
	}
	
	pCamera = camera;
	
	if( camera && camera->GetPeerGraphic() ){
		camera->GetPeerGraphic()->VRAssignedToHMD();
	}
}

bool deVROpenXR::SupportsPassthrough(){
	return pPassthrough;
}

void deVROpenXR::SetEnablePassthrough( bool enable ){
	if( pPassthrough ){
		pPassthrough->SetEnabled( enable );
	}
}

void deVROpenXR::SetPassthroughTransparency( float transparency ){
	if( pPassthrough ){
		pPassthrough->SetTransparency( transparency );
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

bool deVROpenXR::GetButtonNear(int device, int button){
	return pDevices.GetAt(device)->GetButtonAt(button)->GetNear();
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

float deVROpenXR::GetDeviceFaceExpression( int device, int expression ){
	deoxrFaceTracker * const faceTracker = pDevices.GetAt( device )->GetFaceTracker();
	if( faceTracker ){
		return faceTracker->GetFaceExpressionAt( expression );
	}
	return 0.0f;
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
				LogInfo( "Done Session State Changed: ready" );
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
			LogInfoFormat( "VIVE Tracker Connected Event, updating devices: path='%s' rolePath='%s'",
				path.GetName().GetString(), pathRole.GetName().GetString() );
			//LogInfo( "VIVE Tracker connected. Updating devices" );
			pDeviceProfiles.CheckAllAttached();
			}break;
			
		default:
			break;
		}
	}
	
	pDevices.CheckNotifyAttachedDetached();
	
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

deBaseVRModule::eVRRenderFormat deVROpenXR::GetRenderFormat(){
	if(pSession && pSession->GetSwapchainRightEye()){
		return pSession->GetSwapchainRightEye()->GetVRRenderFormat();
	}
	
	// WARNING
	// 
	// SteamVR has a huge problem. under windows their OpenXR implementation expects the
	// rendered images in linear space. under linux though their OpenXR implementation
	// expects rendered images in gamma corrected space. this is a huge problem since we
	// can not use the same code for both systems. furthermore non-SteamVR runtimes can
	// potentially not show such deviating behavior at all.
	// 
	// there is no automatic workaround for this problem since OpenXR does not provide
	// any indication of the behavior used. the only thing we can do is trying to detect
	// the one failing case, which is SteamVR under Linux. this is far from being a nice
	// solution but it should keep us running until this mess is fixed or OpenXR obtains
	// a method to reliably determine what we have to do
	#ifdef OS_UNIX
	if( pSystem && pSystem->GetSystem() == deoxrSystem::esSteamVR ){
		return evrrfSRGBA8;
	}
	#endif
	return evrrfRGBA16;
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
			left = -1.24382f;
			right = 1.39166f;
			top = -1.47029f;
			bottom = 1.45786f;
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
			return decMatrix::CreateTranslation(0.0303f, 0.0f, 0.015f);
		}
		
	case deBaseVRModule::evreRight:
		if( pSession ){
			return pSession->GetRightEyeMatrix();
			
		}else{
			return decMatrix::CreateTranslation(-0.0303f, 0.0f, 0.015f);
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

void deVROpenXR::StartBeginFrame(){
	const deMutexGuard guard( pMutexOpenXR );
	if( ! pBeginFrame() ){
		return;
	}
	if( pThreadSync ){
		pThreadSync->StartWaitFrame();
	}
}

void deVROpenXR::WaitBeginFrameFinished(){
	const deMutexGuard guard( pMutexOpenXR );
	if( ! pSystem ){
		return;
	}
	
	if( ! pSession ){
		return;
	}
	if( pThreadSync ){
		pThreadSync->WaitWaitFrameFinished();
		
	}else{
		pSession->WaitFrame();
	}
	
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
	pDevices.CheckNotifyAttachedDetached();
	
	pPassthrough = nullptr;
	pSession = nullptr;
	pSystem = nullptr;
	pDestroyActionSet();
	
	pInstance = nullptr;
	
	pShutdownRequested = false;
}

void deVROpenXR::pCreateActionSet(){
	pActionSet.TakeOver( new deoxrActionSet( pInstance ) );
	
	pActionSet->AddBoolAction( "trigger_press", "Press Trigger" );
	pActionSet->AddBoolAction("trigger_force", "Force Trigger");
	pActionSet->AddBoolAction( "trigger_touch", "Touch Trigger" );
	pActionSet->AddFloatAction( "trigger_analog", "Pull Trigger" );
	pActionSet->AddVibrationAction( "trigger_haptic", "Trigger Haptic" );
	pActionSet->AddFloatAction("trigger_curl", "Curl Trigger");
	pActionSet->AddFloatAction("trigger_slide", "Slide Trigger");
	pActionSet->AddBoolAction("trigger_near", "Near Trigger");
	
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
	pActionSet->AddFloatAction("thumbrest_press", "Press Thumbrest");
	pActionSet->AddBoolAction("thumbrest_near", "Near Thumbrest");
	pActionSet->AddVibrationAction("thumbrest_haptic", "Thumbrest Haptic");
	
	pActionSet->AddBoolAction( "grip_press", "Squeeze Grip" );
	pActionSet->AddBoolAction( "grip_touch", "Touch Grip" );
	pActionSet->AddFloatAction( "grip_grab", "Grip Grab" );
	pActionSet->AddFloatAction( "grip_squeeze", "Grip Squeeze" );
	pActionSet->AddFloatAction( "grip_pinch", "Grip Pinch" );
	pActionSet->AddVibrationAction( "grip_haptic", "Haptic Grip" );
	
	pActionSet->AddFloatAction( "gesture_pinch", "Gesture Pinch" );
	pActionSet->AddFloatAction( "gesture_aim", "Gesture Aim" );
	pActionSet->AddFloatAction( "gesture_grasp", "Gesture Grasp" );
	
	pActionSet->AddPoseAction( "pose", "Pose" );
	pActionSet->AddPoseAction( "pose_left", "Pose Left" );
	pActionSet->AddPoseAction( "pose_right", "Pose Right" );
	pActionSet->AddPoseAction( "pose_left2", "Pose Left 2" );
	pActionSet->AddPoseAction( "pose_right2", "Pose Right 2" );
	
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
	pDeviceProfiles.Add(deoxrDeviceProfile::Ref::New(new deoxrDPMetaTouchControllerPlus(pInstance)));
	pDeviceProfiles.Add(deoxrDeviceProfile::Ref::New(new deoxrDPMetaQuestTouchProController(pInstance)));
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPOculusGoController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPOculusTouchController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPSamsungOdysseyController( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPValveIndexController( pInstance ) ) );
	
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHandInteraction( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHTCHandInteraction( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPMSFTHandInteraction( pInstance ) ) );
	
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPHtcViveTracker( pInstance ) ) );
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPEyeGazeInteraction( pInstance ) ) );
	
	// has to come last since it adds a device only if no other controller provides hand tracking
	pDeviceProfiles.Add( deoxrDeviceProfile::Ref::New( new deoxrDPNoControllerHands( pInstance ) ) );
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
}

bool deVROpenXR::pBeginFrame(){
	if( ! pSystem ){
		return false;
	}
	
	if( pSession && pRestartSession ){
		LogInfo( "Restarting session (somebody requested this)" );
		pDeviceProfiles.ClearActions();
		pPassthrough = nullptr;
		pSession = nullptr;
		pDestroyActionSet();
		
		pRestartSession = false;
	}
	
	if( pSession ){
		return true;
	}
	
	// create session
	if( pShutdownRequested ){
		pRealShutdown();
		return false;
	}
	
	LogInfo( "BeginFrame: Create Session" );
	pRestartSession = false;
	try{
		pSession.TakeOver( new deoxrSession( pSystem ) );
		
		// required before CheckAllAttached since this could add devices which in turn
		// accesses actions. creating actions does access device profiles but this is
		// a special call that has to always succeed
		pCreateActionSet();

		pDeviceProfiles.CheckAllAttached();
		// no CheckNotifyAttachedDetached call here since we potentially outside main thread
		
		pSuggestBindings();
		
		pSession->AttachActionSet( pActionSet );
		
		if( pSystem->GetSupportsPassthrough() ){
			pPassthrough.TakeOver( new deoxrPassthrough( pSession ) );
		}
		return true;
		
	}catch( const deException &e ){
		LogException( e );
		
		LogError( "Runtime failed during BeginFrame. Shutting down runtime. Restart runtime to continue." );
		pDeviceProfiles.ClearActions();
		pDestroyActionSet();
		pPassthrough = nullptr;
		pSession = nullptr;
		
		pRealShutdown();
		return false;
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class deoxrModuleInternal : public deInternalModule{
public:
	deoxrModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("OpenXR");
		SetDescription("OpenXR Support.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtVR);
		SetDirectoryName("openxr");
		SetPriority(2);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(OpenXRCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *deoxrRegisterInternalModule(deModuleSystem *system){
	return new deoxrModuleInternal(system);
}
#endif
