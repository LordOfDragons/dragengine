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
#include "deovrRenderModel.h"
#include "deovrTextureMap.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicCamera.h>


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
pVRSystem( nullptr ),
pVRInput( nullptr ),
pVRRenderModels( nullptr ),
pVRCompositor( nullptr ),
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

vr::IVRSystem &deVROpenVR::GetVRSystem() const{
	if( ! pVRSystem ){
		DETHROW( deeInvalidAction );
	}
	return *pVRSystem;
}

vr::IVRInput &deVROpenVR::GetVRInput() const{
	if( ! pVRInput ){
		DETHROW( deeInvalidAction );
	}
	return *pVRInput;
}

vr::IVRRenderModels &deVROpenVR::GetVRRenderModels() const{
	if( ! pVRRenderModels ){
		DETHROW( deeInvalidAction );
	}
	return *pVRRenderModels;
}

vr::IVRCompositor & deVROpenVR::GetVRCompositor() const{
	if( ! pVRCompositor ){
		DETHROW( deeInvalidAction );
	}
	return *pVRCompositor;
}

deovrRenderModel *deVROpenVR::GetRenderModelNamed( const char *name ){
	deObject *findObject;
	if( pRenderModels.GetAt( name, &findObject ) ){
		return ( deovrRenderModel* )findObject;
	}
	
	const deovrRenderModel::Ref renderModel( deovrRenderModel::Ref::New( new deovrRenderModel( *this, name ) ) );
	pRenderModels.SetAt( name, renderModel );
	return renderModel;
}

deovrTextureMap *deVROpenVR::GetTextureMapWithID( vr::TextureID_t id ){
	const int count = pTextureMaps.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deovrTextureMap * const textureMap = ( deovrTextureMap* )pTextureMaps.GetAt( i );
		if( textureMap->GetID() == id ){
			return textureMap;
		}
	}
	
	const deovrTextureMap::Ref textureMap( deovrTextureMap::Ref::New( new deovrTextureMap( *this, id ) ) );
	pTextureMaps.Add( textureMap );
	return textureMap;
}

void deVROpenVR::SendEvent( const deInputEvent &event ){
	GetGameEngine()->GetVRSystem()->GetEventQueue().AddEvent( event );
}

decMatrix deVROpenVR::ConvertMatrix( const vr::HmdMatrix34_t &matrix ) const{
	// OpenVR is right handed: right=x, up=y, forward=-z
	// to transform the matrix apply the conversion (-z) for both the row vectors and
	// the column vectors
	decMatrix m;
	m.a11 =  matrix.m[ 0 ][ 0 ]; m.a12 =  matrix.m[ 0 ][ 1 ]; m.a13 = -matrix.m[ 0 ][ 2 ]; m.a14 =  matrix.m[ 0 ][ 3 ];
	m.a21 =  matrix.m[ 1 ][ 0 ]; m.a22 =  matrix.m[ 1 ][ 1 ]; m.a23 = -matrix.m[ 1 ][ 2 ]; m.a24 =  matrix.m[ 1 ][ 3 ];
	m.a31 = -matrix.m[ 2 ][ 0 ]; m.a32 = -matrix.m[ 2 ][ 1 ]; m.a33 =  matrix.m[ 2 ][ 2 ]; m.a34 = -matrix.m[ 2 ][ 3 ];
	return m;
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
	SetCamera( nullptr );
	pRenderModels.RemoveAll();
	pTextureMaps.RemoveAll();
}



// Runtime
////////////

bool deVROpenVR::RuntimeUsable(){
	return pRuntimeInstalled && vr::VR_IsHmdPresent();
}

void deVROpenVR::StartRuntime(){
	if( pVRSystem ){
		return;
	}
	
	LogInfo( "Start Runtime" );
	
	try{
		// init runtime
		vr::HmdError error = vr::VRInitError_None;
		pVRSystem = vr::VR_Init( &error, vr::VRApplication_Scene );
		
		if( ! pVRSystem || error != vr::VRInitError_None ){
			LogErrorFormat( "Failed starting runtime: %s", vr::VR_GetVRInitErrorAsSymbol( error ) );
			DETHROW_INFO( deeInvalidAction, "Failed starting runtime" );
		}
		
		LogInfoFormat( "Runtime Version: %s", pVRSystem->GetRuntimeVersion() );
		
		// render models
		pVRRenderModels = vr::VRRenderModels();
		
		// compositor
		pVRCompositor = vr::VRCompositor();
		
		// load input action manifest
		pVRInput = vr::VRInput();
		
		decPath pathActionManifest( decPath::CreatePathNative( GetNativePathShare() ) );
		pathActionManifest.AddComponent( "input" );
		pathActionManifest.AddComponent( "actions.json" );
		
		vr::EVRInputError inputError = pVRInput->SetActionManifestPath( pathActionManifest.GetPathNative() );
		if( inputError != vr::VRInputError_None ){
			LogErrorFormat( "Failed loading action manifest: %d", inputError );
			DETHROW_INFO( deeInvalidAction, "Failed loading action manifest" );
		}
		
		// get input handlers
		inputError = pVRInput->GetActionSetHandle( "/actions/dragengine", &pActionSetHandle );
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
			inputError = pVRInput->GetActionHandle( inputHandleMap[ i ].path, &pActionHandle[ inputHandleMap[ i ].action ] );
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
		
		// attach camera if set
		if( pCamera && pCamera->GetPeerGraphic() ){
			pCamera->GetPeerGraphic()->VRAssignedToHMD();
		}
		
	}catch( const deException &e ){
		LogException( e );
		StopRuntime();
		throw;
	}
}

void deVROpenVR::StopRuntime(){
	if( ! pVRSystem ){
		return;
	}
	
	LogInfo( "Shutdown runtime" );
	
	if( pCamera && pCamera->GetPeerGraphic() ){
		pCamera->GetPeerGraphic()->VRResignedFromHMD();
	}
	
	pDevices.Clear();
	
	int i;
	for( i=0; i<InputActionCount; i++ ){
		pActionHandle[ i ] = vr::k_ulInvalidActionHandle;
	}
	
	pActionSetHandle = vr::k_ulInvalidActionSetHandle;
	
	vr::VR_Shutdown();
	pVRSystem = nullptr;
}

void deVROpenVR::SetCamera( deCamera *camera ){
	if( pCamera == camera ){
		return;
	}
	
	if( pVRSystem && pCamera && pCamera->GetPeerGraphic() ){
		pCamera->GetPeerGraphic()->VRResignedFromHMD();
	}
	
	pCamera = camera;
	
	if( pVRSystem && camera && camera->GetPeerGraphic() ){
		camera->GetPeerGraphic()->VRAssignedToHMD();
	}
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
	if( ! pVRSystem ){
		return;
	}
	
	vr::VREvent_t event;
	while( pVRSystem->PollNextEvent( &event, sizeof( event ) ) ){
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
			pVRSystem->AcknowledgeQuit_Exiting();
			
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
				                       pVRSystem->GetEventTypeNameFromEnum( ( vr::EVREventType )event.eventType ) );
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



// Graphic Module use only
////////////////////////////

decPoint deVROpenVR::GetRenderSize(){
	uint32_t width, height;
	GetVRSystem().GetRecommendedRenderTargetSize( &width, &height );
	return decPoint( ( int )width, ( int )height );
}

void deVROpenVR::GetProjectionParameters( eEye eye, float &left, float &right, float &top, float &bottom ){
	GetVRSystem().GetProjectionRaw( ConvertEye( eye ), &left, &right, &top, &bottom );
}

decMatrix deVROpenVR::GetMatrixViewEye( eEye eye ){
	// NOTE openvr documentation states transformation is "Model * View * Eye^-1 * Projection".
	//      what we need is thus the inverse of the eye matrix
	return ConvertMatrix( GetVRSystem().GetEyeToHeadTransform( ConvertEye( eye ) ) ).QuickInvert();
}

deModel *deVROpenVR::GetHiddenArea( eEye eye ){
	// const HiddenAreaMesh_t mesh = GetVRSystem().GetHiddenAreaMesh( ConvertEye( eye ) );
	// ...
	return nullptr;
}

deImage *deVROpenVR::GetDistortionMap( eEye eye ){
	// bool GetVRSystem().ComputeDistortion( Hmd_Eye eEye, float fU, float fV, DistortionCoordinates_t *pDistortionCoordinates )
	return nullptr;
}

void deVROpenVR::BeginFrame(){
	const vr::EVRCompositorError error = GetVRCompositor().WaitGetPoses( nullptr, 0, nullptr, 0 );
	if( error != vr::VRCompositorError_None ){
		LogErrorFormat( "Compositor.WaitGetPoses failed: error=%d", error );
		// ignore errors
	}
}

void deVROpenVR::SubmitOpenGLTexture2D( eEye eye, void *texture, const decVector2 &tcFrom,
const decVector2 &tcTo, bool distortionApplied ){
	const vr::Hmd_Eye vreye = ConvertEye( eye );
	
	vr::Texture_t vrtexture;
	vrtexture.eColorSpace = vr::ColorSpace_Gamma;
	vrtexture.eType = vr::TextureType_OpenGL;
	vrtexture.handle = texture;
	
	vr::VRTextureBounds_t bounds;
	bounds.uMin = tcFrom.x;
	bounds.uMax = tcTo.x;
	bounds.vMin = tcFrom.y;
	bounds.vMax = tcTo.y;
	
	const vr::EVRCompositorError error = GetVRCompositor().Submit( vreye, &vrtexture, &bounds,
		distortionApplied ? vr::Submit_LensDistortionAlreadyApplied : vr::Submit_Default );
	if( error != vr::VRCompositorError_None ){
		LogErrorFormat( "Compositor.Submit failed: eye=%d error=%d", eye, error );
		// ignore errors
	}
}

void deVROpenVR::EndFrame(){
	GetVRCompositor().PostPresentHandoff();
}

vr::Hmd_Eye deVROpenVR::ConvertEye( eEye eye ) const{
	switch( eye ){
	case evreLeft:
		return vr::Eye_Left;
		
	case evreRight:
		return vr::Eye_Right;
		
	default:
		DETHROW( deeInvalidParam );
	}
}
