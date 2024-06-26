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

#include "deovrDevice.h"
#include "deovrDeviceAxis.h"
#include "deovrDeviceButton.h"
#include "deovrDeviceFeedback.h"
#include "deovrDeviceComponent.h"
#include "deovrDeviceManager.h"
#include "deVROpenVR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDevicePose.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deovrDevice
//////////////////////

// Constructor, destructor
////////////////////////////

deovrDevice::deovrDevice( deVROpenVR &ovr, vr::TrackedDeviceIndex_t deviceIndex ) :
pOvr( ovr ),
pIndex( -1 ),
pDeviceIndex( deviceIndex ),
pDeviceClass( vr::TrackedDeviceClass_Invalid ),
pInputValueHandle( vr::k_ulInvalidInputValueHandle ),
pActionPose( vr::k_ulInvalidActionHandle ),
pActionHandPose( vr::k_ulInvalidActionHandle ),
pBoneConfiguration( deInputDevice::ebcNone ),
pNameNumber( -1 ),
pBoneTransformData( nullptr ),
pBoneCount( 0 ),
pPoseBones( nullptr ),
pPoseBoneCount( 0 )
{
	UpdateParameters();
}

deovrDevice::~deovrDevice(){
	if( pPoseBones ){
		delete [] pPoseBones;
	}
	if( pBoneTransformData ){
		delete [] pBoneTransformData;
	}
}



// Management
///////////////

void deovrDevice::SetIndex( int index ){
	pIndex = index;
}

void deovrDevice::SetType( deInputDevice::eDeviceTypes type ){
	pType = type;
}

void deovrDevice::SetBoneConfiguration( deInputDevice::eBoneConfigurations config ){
	pBoneConfiguration = config;
}

void deovrDevice::SetID( const char *id ){
	pID = id;
}

void deovrDevice::SetName( const char *name ){
	pName = name;
}

void deovrDevice::SetDisplayImages( const char *name ){
	pDisplayImage = nullptr;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pOvr.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pOvr.GetVFS();
	const char * const basePath = "/share/image/device";
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

void deovrDevice::SetDisplayText( const char *text ){
	pDisplayText = text;
}

void deovrDevice::SetRenderModel( deovrRenderModel *renderModel ){
	pRenderModel = renderModel;
}

void deovrDevice::SetTextureMap( deovrTextureMap *textureMap ){
	pTextureMap = textureMap;
}



int deovrDevice::GetButtonCount() const{
	return pButtons.GetCount();
}

void deovrDevice::AddButton( deovrDeviceButton *button ){
	if( ! button ){
		DETHROW( deeNullPointer );
	}
	pButtons.Add( button );
}

deovrDeviceButton *deovrDevice::GetButtonAt( int index ) const{
	return ( deovrDeviceButton* )pButtons.GetAt( index );
}

deovrDeviceButton *deovrDevice::GetButtonWithID( const char *id ) const{
	const int count = pButtons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deovrDeviceButton * const button = ( deovrDeviceButton* )pButtons.GetAt( i );
		if( button->GetID() == id ){
			return button;
		}
	}
	
	return nullptr;
}

int deovrDevice::IndexOfButtonWithID( const char *id ) const{
	const int count = pButtons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deovrDeviceButton &button = *( ( deovrDeviceButton* )pButtons.GetAt( i ) );
		if( button.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



int deovrDevice::GetAxisCount() const{
	return pAxes.GetCount();
}

void deovrDevice::AddAxis( deovrDeviceAxis *axis ){
	if( ! axis ){
		DETHROW( deeNullPointer );
	}
	pAxes.Add( axis );
}

deovrDeviceAxis *deovrDevice::GetAxisAt( int index ) const{
	return ( deovrDeviceAxis* )pAxes.GetAt( index );
}

deovrDeviceAxis *deovrDevice::GetAxisWithID( const char *id ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deovrDeviceAxis * const axis = ( deovrDeviceAxis* )pAxes.GetAt( i );
		if( axis->GetID() == id ){
			return axis;
		}
	}
	
	return nullptr;
}

int deovrDevice::IndexOfAxisWithID( const char *id ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const deovrDeviceAxis &axis = *( ( deovrDeviceAxis* )pAxes.GetAt( i ) );
		if( axis.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



int deovrDevice::GetFeedbackCount() const{
	return pFeedbacks.GetCount();
}

void deovrDevice::AddFeedback( deovrDeviceFeedback *feedback ){
	if( ! feedback ){
		DETHROW( deeNullPointer );
	}
	pFeedbacks.Add( feedback );
}

deovrDeviceFeedback *deovrDevice::GetFeedbackAt( int index ) const{
	return ( deovrDeviceFeedback* )pFeedbacks.GetAt( index );
}

int deovrDevice::IndexOfFeedbackWithID( const char *id ) const{
	const int count = pFeedbacks.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const deovrDeviceFeedback &feedback = *( ( deovrDeviceFeedback* )pFeedbacks.GetAt( i ) );
		if( feedback.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



int deovrDevice::GetComponentCount() const{
	return pComponents.GetCount();
}

void deovrDevice::AddComponent( deovrDeviceComponent *component ){
	if( ! component ){
		DETHROW( deeNullPointer );
	}
	pComponents.Add( component );
}

deovrDeviceComponent *deovrDevice::GetComponentAt( int index ) const{
	return ( deovrDeviceComponent* )pComponents.GetAt( index );
}

int deovrDevice::IndexOfComponentWithID( const char *id ) const{
	const int count = pComponents.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const deovrDeviceComponent &component = *( ( deovrDeviceComponent* )pComponents.GetAt( i ) );
		if( component.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



void deovrDevice::GetInfo( deInputDevice &info ) const{
	int i;
	
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
	info.SetDisplayImage( pDisplayImage );
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
	
	info.SetDisplayModel( nullptr );
	info.SetDisplaySkin( nullptr );
	
	info.SetBoneConfiguration( pBoneConfiguration );
	if( pRenderModel ){
		info.SetVRModel( pRenderModel->GetModel() );
	}
	if( pTextureMap ){
		info.SetVRSkin( pTextureMap->GetSkin() );
	}
	
	const int buttonCount = pButtons.GetCount();
	info.SetButtonCount( buttonCount );
	for( i=0; i<buttonCount; i++ ){
		( ( deovrDeviceButton* )pButtons.GetAt( i ) )->GetInfo( info.GetButtonAt( i ) );
	}
	
	const int axisCount = pAxes.GetCount();
	info.SetAxisCount( axisCount );
	for( i=0; i<axisCount; i++ ){
		( ( deovrDeviceAxis* )pAxes.GetAt( i ) )->GetInfo( info.GetAxisAt( i ) );
	}
	
	const int feedbackCount = pFeedbacks.GetCount();
	info.SetFeedbackCount( feedbackCount );
	for( i=0; i<feedbackCount; i++ ){
		( ( deovrDeviceFeedback* )pFeedbacks.GetAt( i ) )->GetInfo( info.GetFeedbackAt( i ) );
	}
}

void deovrDevice::UpdateParameters(){
	const vr::TrackedDeviceClass oldDeviceClass = pDeviceClass;
	pDeviceClass = pOvr.GetVRSystem().GetTrackedDeviceClass( pDeviceIndex );
	
	if( pDeviceClass != oldDeviceClass || pNameNumber == -1 ){
		pNameNumber = -1; // required so we do not block ourselves
		pNameNumber = pOvr.GetDevices().NextNameNumber( pDeviceClass );
	}
	
	pControllerRole = vr::TrackedControllerRole_Invalid;
	pInputValuePath.Empty();
	pInputValueHandle = vr::k_ulInvalidInputValueHandle;
	pPoseDevice = deInputDevicePose();
	pRenderModel = nullptr;
	pTextureMap = nullptr;
	
	if( pPoseBones ){
		delete [] pPoseBones;
		pPoseBones = nullptr;
		pPoseBoneCount = 0;
	}
	if( pBoneTransformData ){
		delete [] pBoneTransformData;
		pBoneTransformData = nullptr;
		pBoneCount = 0;
	}
	
	// serial number
	vr::ETrackedPropertyError error;
	char buffer[ 256 ];
	pOvr.GetVRSystem().GetStringTrackedDeviceProperty( pDeviceIndex,
		vr::Prop_SerialNumber_String, buffer, 256, &error );
	if( error != vr::TrackedProp_Success ){
		DETHROW_INFO( deeInvalidParam, "Prop_SerialNumber_String failed" );
	}
	pSerialNumber = buffer;
	
	// render model
	pOvr.GetVRSystem().GetStringTrackedDeviceProperty( pDeviceIndex,
		vr::Prop_RenderModelName_String, buffer, 256, &error );
	if( error == vr::TrackedProp_Success ){
		pRenderModel = pOvr.GetRenderModelNamed( buffer );
		pTextureMap = pOvr.GetTextureMapWithID( pRenderModel->GetTextureID() );
	}
	
	// init device dependingon class
	switch( pDeviceClass ){
	case vr::TrackedDeviceClass_HMD:
		pUpdateParametersHMD();
		break;
		
	case vr::TrackedDeviceClass_Controller:
		pUpdateParametersController();
		break;
		
	case vr::TrackedDeviceClass_GenericTracker:
		pUpdateParametersTracker();
		break;
		
	case vr::TrackedDeviceClass_TrackingReference:
		pType = deInputDevice::edtVRBaseStation;
		pName.Format( "Base Station %d", pNameNumber );
// 		pDisplayText.Format( "%d", );
		pID.Format( "%sbs", OVR_DEVID_PREFIX );
		break;
		
	default:
		pType = deInputDevice::edtGeneric;
		pName.Empty();
		pID.Empty();
		return;
	}
	
	// display name. seems to always return "lighthouse". how stupid is that :(
	/*
	pOvr.GetSystem().GetStringTrackedDeviceProperty( pDeviceIndex,
		vr::Prop_TrackingSystemName_String, buffer, 256, &error );
	if( error != vr::TrackedProp_Success ){
		DETHROW_INFO( deeInvalidParam, "GetStringTrackedDeviceProperty failed" );
	}
	pName = buffer;
	*/
	
	// append serial number to id to make it unique (hopefully)
	pID += "_";
	pID += pOvr.GetDevices().NormalizeID( pSerialNumber );
	
	// controller only: Prop_AttachedDeviceId_String
}

void deovrDevice::TrackStates(){
	if( pInputValueHandle == vr::k_ulInvalidInputValueHandle ){
		ResetStates();
		pUpdatePose( pOvr.GetDevices().GetDevicePoseAt( pDeviceIndex ), pPoseDevice );
		return;
	}
	
	vr::EVRInputError error;
	
	if( pActionPose != vr::k_ulInvalidActionHandle ){
		error = pOvr.GetVRInput().GetPoseActionDataForNextFrame( pActionPose,
			vr::TrackingUniverseStanding, &pDevicePoseData, sizeof( pDevicePoseData), pInputValueHandle );
		if( error != vr::VRInputError_None ){
			//pOvr.LogErrorFormat( "GetPoseActionDataForNextFrame failed: %d", error );
			// spams logs if data is lost
			//DETHROW_INFO( deeInvalidParam, "GetPoseActionDataForNextFrame failed" );
			// can be VRInputError_NoData which is fine. better wrong data than fail
		}
		
		if( pDevicePoseData.bActive ){
			pUpdatePose( pDevicePoseData.pose, pPoseDevice );
			
			switch( pControllerRole ){
			case vr::TrackedControllerRole_LeftHand:
			case vr::TrackedControllerRole_RightHand:{
				const decMatrix correction( decMatrix::CreateRotationX( -45.0f * DEG2RAD )
					* decMatrix::CreateFromQuaternion( pPoseDevice.GetOrientation() ) );
				pPoseDevice.SetOrientation( correction.ToQuaternion() );
				pPoseDevice.SetLinearVelocity( correction * pPoseDevice.GetLinearVelocity() );
				}break;
				
			default:
				break;
			}
			
		}else{
			//pPoseDevice = deInputDevicePose();
			// reset pose is not good since this can snap the HMD around. a possible solution
			// is to use the last linear and angular velocity to estimate the next position
			// while reducing velocites to 0 over a short time. when getting back correct
			// results this would require smoothly blending over to the correct state to
			// make the lost tracking not so nauseaing
		}
		
	}else{
		// track devices not having an input assignment like the HMD
		pUpdatePose( pOvr.GetDevices().GetDevicePoseAt( pDeviceIndex ), pPoseDevice );
	}
	
	if( pActionHandPose != vr::k_ulInvalidActionHandle ){
		// skeletal pose can not be limited to a device
		error = pOvr.GetVRInput().GetSkeletalSummaryData( pActionHandPose,
			vr::VRSummaryType_FromAnimation, &pSkeletalSummeryData );
		if( error != vr::VRInputError_None ){
			//pOvr.LogErrorFormat( "GetSkeletalSummaryData failed: %d", error );
			// spams logs if data is lost
			//DETHROW_INFO( deeInvalidParam, "GetSkeletalSummaryData failed" );
			// can be VRInputError_NoData which is fine. better wrong data than fail
		}
		
		// if bones are supported
		if( pBoneCount > 0 ){
			bool validData = true;
			
			error = pOvr.GetVRInput().GetSkeletalBoneData( pActionHandPose, vr::VRSkeletalTransformSpace_Parent,
				vr::VRSkeletalMotionRange_WithoutController, pBoneTransformData, pBoneCount );
			if( error != vr::VRInputError_None ){
				//pOvr.LogErrorFormat( "GetSkeletalBoneData failed: %d", error );
				// spams logs if data is lost
				//DETHROW_INFO( deeInvalidParam, "GetSkeletalBoneData failed" );
				// can be VRInputError_NoData which is fine. better wrong data than fail
				validData = false;
			}
			
			error = pOvr.GetVRInput().GetSkeletalBoneData( pActionHandPose, vr::VRSkeletalTransformSpace_Parent,
				vr::VRSkeletalMotionRange_WithController, pBoneTransformData + pBoneCount, pBoneCount );
			if( error != vr::VRInputError_None ){
				//pOvr.LogErrorFormat( "GetSkeletalBoneData failed: %d", error );
				// spams logs if data is lost
				//DETHROW_INFO( deeInvalidParam, "GetSkeletalBoneData failed" );
				// can be VRInputError_NoData which is fine. better wrong data than fail
				validData = false;
			}
			
			// TODO convert bone transforms. requires remapping bones and thus combining matrices
			if( validData ){
				/*
				pUpdatePose( pBoneTransformData[0], pPoseBones[0] );
				const decVector p(pPoseBones[0].GetPosition());
				const decVector r(pPoseBones[0].GetOrientation().GetEulerAngles() * RAD2DEG);
				pOvr.LogInfoFormat( "Hand(%s): valid=%d p=(%g,%g,%g) r=(%g,%g,%g)",
					pName.GetString(), validData, p.x, p.y, p.z, r.x, r.y, r.z);
				*/
			}
		}
	}
	
	int i, count = pButtons.GetCount();
	for( i=0; i<count; i++ ){
		GetButtonAt( i )->TrackState();
	}
	
	count = pAxes.GetCount();
	for( i=0; i<count; i++ ){
		GetAxisAt( i )->TrackState();
	}
}

void deovrDevice::ResetStates(){
	const int axisCount = pAxes.GetCount();
	int i;
	
	for( i=0; i<axisCount; i++ ){
		GetAxisAt( i )->ResetState();
	}
}

void deovrDevice::GetDevicePose( deInputDevicePose &pose ){
	pose = pPoseDevice;
}

void deovrDevice::GetBonePose( int bone, bool withController, deInputDevicePose &pose ){
	if( bone < 0 || bone >= pBoneCount ){
		pose = deInputDevicePose();
		return;
	}
	
	if( withController ){
		pUpdatePose( pBoneTransformData[ pBoneCount + bone ], pose );
		
	}else{
		pUpdatePose( pBoneTransformData[ bone ], pose );
	}
}



// Private Functions
//////////////////////

void deovrDevice::pUpdateParametersHMD(){
	pType = deInputDevice::edtVRHMD;
	pName = "HMD";
	pID.Format( "%shmd", OVR_DEVID_PREFIX );
	pInputValuePath = "/user/head";
// 	pActionPose = pOvr.GetActionHandle( deVROpenVR::eiaPose ); // not working
	
	// input source handle
	vr::EVRInputError inputError = pOvr.GetVRInput().GetInputSourceHandle( pInputValuePath, &pInputValueHandle );
	if( inputError != vr::VRInputError_None ){
		pOvr.LogErrorFormat( "Failed retrieving input source handle: %d", inputError );
		DETHROW_INFO( deeInvalidAction, "Failed retrieving input source handle" );
	}
}

void deovrDevice::pUpdateParametersController(){
	//pControllerRole = pOvr.GetSystem().GetControllerRoleForTrackedDeviceIndex( pDeviceIndex );
	// ^= old input system. unreliable using new system. resorting to device property
	
	vr::ETrackedPropertyError propError;
	pControllerRole = ( vr::ETrackedControllerRole )pOvr.GetVRSystem().GetInt32TrackedDeviceProperty(
		pDeviceIndex, vr::Prop_ControllerRoleHint_Int32, &propError );
	if( propError != vr::TrackedProp_Success ){
		DETHROW_INFO( deeInvalidParam, "Prop_ControllerRoleHint_Int32 failed" );
	}
	
	vr::VRActionHandle_t actionHandPose = vr::k_ulInvalidActionHandle;
	
	switch( pControllerRole ){
	case vr::TrackedControllerRole_LeftHand:
		pType = deInputDevice::edtVRLeftHand;
		pName = "Left Hand";
		pNameNumber = -1;
		pID.Format( "%scl", OVR_DEVID_PREFIX );
		pInputValuePath = "/user/hand/left";
		pActionPose = pOvr.GetActionHandle( deVROpenVR::eiaPose );
		actionHandPose = pOvr.GetActionHandle( deVROpenVR::eiaSkeletonHandLeft );
		break;
		
	case vr::TrackedControllerRole_RightHand:
		pType = deInputDevice::edtVRRightHand;
		pName = "Right Hand";
		pNameNumber = -1;
		pID.Format( "%scr", OVR_DEVID_PREFIX );
		pInputValuePath = "/user/hand/right";
		pActionPose = pOvr.GetActionHandle( deVROpenVR::eiaPose );
		actionHandPose = pOvr.GetActionHandle( deVROpenVR::eiaSkeletonHandRight );
		break;
		
	case vr::TrackedControllerRole_Treadmill:
		pType = deInputDevice::edtVRTreadmill;
		pName.Format( "Treadmill %d", pNameNumber );
		pID.Format( "%sct", OVR_DEVID_PREFIX );
		break;
		
	case vr::TrackedControllerRole_Stylus:
		pType = deInputDevice::edtVRStylus;
		pName.Format( "Stylus %d", pNameNumber );
		pID.Format( "%scs", OVR_DEVID_PREFIX );
		pActionPose = pOvr.GetActionHandle( deVROpenVR::eiaPose );
		break;
		
	default:
		pType = deInputDevice::edtVRController;
		pName.Format( "Controller %d", pNameNumber );
		pActionPose = pOvr.GetActionHandle( deVROpenVR::eiaPose );
		pID.Format( "%scg", OVR_DEVID_PREFIX );
	}
	
	// input source handle
	if( ! pInputValuePath.IsEmpty() ){
		vr::EVRInputError inputError = pOvr.GetVRInput().GetInputSourceHandle( pInputValuePath, &pInputValueHandle );
		if( inputError != vr::VRInputError_None ){
			pOvr.LogErrorFormat( "Failed retrieving input source handle: %d", inputError );
			DETHROW_INFO( deeInvalidAction, "Failed retrieving input source handle" );
		}
	}
	
	// input data can be only retrieved with a handle
	if( pInputValueHandle == vr::k_ulInvalidInputValueHandle ){
		return;
	}
	
	// axes and components
	const int axisTrigger = pAddAxisTrigger( deInputDeviceAxis::eatTrigger, deVROpenVR::eiaTriggerAnalog, "Trigger", "trig", "Tri" );
	deovrDeviceComponent *compTrigger = nullptr;
	if( axisTrigger != -1 ){
		compTrigger = pAddComponent( deInputDeviceComponent::ectTrigger, "Trigger", "trigger", "Trigger" );
		GetAxisAt( axisTrigger )->SetInputDeviceComponent( compTrigger );
	}
	
	const int axisJoystick = pAddAxesJoystick( deVROpenVR::eiaJoystickAnalog, "Joystick", "js", "Joy" );
	deovrDeviceComponent *compJoystick = nullptr;
	if( axisJoystick != -1 ){
		compJoystick = pAddComponent( deInputDeviceComponent::ectJoystick, "Joystick", "joystick", "Joystick" );
		GetAxisAt( axisJoystick )->SetInputDeviceComponent( compJoystick );
		GetAxisAt( axisJoystick + 1 )->SetInputDeviceComponent( compJoystick );
	}
	
	const int axisTrackpad = pAddAxesTrackpad( deVROpenVR::eiaTrackpadAnalog, "TrackPad", "tp", "Pad" );
	deovrDeviceComponent *compTrackpad = nullptr;
	if( axisTrackpad != -1 ){
		compTrackpad = pAddComponent( deInputDeviceComponent::ectTouchPad, "TrackPad", "trackpad", "TrackPad" );
		GetAxisAt( axisTrackpad )->SetInputDeviceComponent( compTrackpad );
		GetAxisAt( axisTrackpad + 1 )->SetInputDeviceComponent( compTrackpad );
	}
	
	const int axisGripGrab = pAddAxisTrigger( deInputDeviceAxis::eatGripGrab,
		deVROpenVR::eiaGripGrab, "Grab", "gg", "Grab" );
	const int axisGripSqueeze = pAddAxisTrigger( deInputDeviceAxis::eatGripSqueeze,
		deVROpenVR::eiaGripSqueeze, "Squeeze", "gs", "Squ" );
	const int axisGripPinch = pAddAxisTrigger( deInputDeviceAxis::eatGripPinch,
		deVROpenVR::eiaGripPinch, "Pinch", "gp", "Pin" );
	deovrDeviceComponent *compGrip = nullptr;
	if( axisGripGrab != -1 || axisGripSqueeze != -1 || axisGripPinch != -1 ){
		compGrip = pAddComponent( deInputDeviceComponent::ectGeneric, "Grip", "grip", "Grip" );
		if( axisGripGrab != -1 ){
			GetAxisAt( axisGripGrab )->SetInputDeviceComponent( compGrip );
		}
		if( axisGripSqueeze != -1 ){
			GetAxisAt( axisGripSqueeze )->SetInputDeviceComponent( compGrip );
		}
		if( axisGripPinch != -1 ){
			GetAxisAt( axisGripPinch )->SetInputDeviceComponent( compGrip );
		}
	}
	
	// buttons
	if( compTrigger ){
		pAddButton( deInputDeviceButton::ebtTrigger, compTrigger, deVROpenVR::eiaTriggerPress,
			deVROpenVR::eiaTriggerTouch, "Trigger", "trig", "Tri" );
	}
	
	const int btnA = pAddButton( deInputDeviceButton::ebtAction, nullptr,
		deVROpenVR::eiaButtonPrimaryPress, deVROpenVR::eiaButtonPrimaryTouch, "A", "a", "A" );
	if( btnA != -1 ){
		GetButtonAt( btnA )->SetInputDeviceComponent( pAddComponent( deInputDeviceComponent::ectButton, "A", "a", "A" ) );
	}
	
	const int btnB = pAddButton( deInputDeviceButton::ebtAction, nullptr,
		deVROpenVR::eiaButtonSecondaryPress, deVROpenVR::eiaButtonSecondaryTouch, "B", "b", "B" );
	if( btnB != -1 ){
		GetButtonAt( btnB )->SetInputDeviceComponent( pAddComponent( deInputDeviceComponent::ectButton, "B", "b", "B" ) );
	}
	
	if( compJoystick ){
		pAddButton( deInputDeviceButton::ebtStick, compJoystick, deVROpenVR::eiaJoystickPress,
			deVROpenVR::eiaJoystickTouch, "Joystick", "js", "Joy" );
	}
	
	if( compTrackpad ){
		pAddButton( deInputDeviceButton::ebtTouchPad, compTrackpad, deVROpenVR::eiaTrackpadPress,
			deVROpenVR::eiaTrackpadTouch, "TrackPad", "tp", "Pad" );
	}
	
	if( compGrip ){
		pAddButton( deInputDeviceButton::ebtTrigger, compGrip, deVROpenVR::eiaGripPress,
			deVROpenVR::eiaGripTouch, "Grip", "grip", "Grip" );
	}
	
	// hand pose
	if( actionHandPose != vr::k_ulInvalidActionHandle ){
		pUpdateParametersHandPose( actionHandPose );
	}
}

void deovrDevice::pUpdateParametersHandPose( vr::VRActionHandle_t actionHandle ){
	vr::IVRInput &vrinput = pOvr.GetVRInput();
	vr::InputSkeletalActionData_t dataSkeletal;
	
	// finger bending and spreading
	deovrDeviceComponent * const component = pAddComponent(
		deInputDeviceComponent::ectGeneric, "Hand Pose", "handPose", "Hand Pose" );
	
	pAddAxisFinger( deInputDeviceAxis::eatFingerBend, component, 0, "Bend Thumb", "fb1", "FB1" );
	pAddAxisFinger( deInputDeviceAxis::eatFingerBend, component, 1, "Bend Index Finger", "fb2", "FB2" );
	pAddAxisFinger( deInputDeviceAxis::eatFingerBend, component, 2, "Bend Middle Finger", "fb3", "FB3" );
	pAddAxisFinger( deInputDeviceAxis::eatFingerBend, component, 3, "Bend Ring Finger", "fb4", "FB4" );
	pAddAxisFinger( deInputDeviceAxis::eatFingerBend, component, 4, "Bend Pinky Finger", "fb5", "FB5" );
	
	pAddAxisFinger( deInputDeviceAxis::eatFingerSpread, component, 0, "Spread Thumb Index Finger", "fs1", "FS1" );
	pAddAxisFinger( deInputDeviceAxis::eatFingerSpread, component, 1, "Spread Index Middle Finger", "fs2", "FS2" );
	pAddAxisFinger( deInputDeviceAxis::eatFingerSpread, component, 2, "Spread Middle Ring Finger", "fs3", "FS3" );
	pAddAxisFinger( deInputDeviceAxis::eatFingerSpread, component, 3, "Spread Ring Pinky Finger", "fs4", "FS4" );
	
	// skeletal input can not be limited to a device, hence why different actions are used
	vr::EVRInputError inputError = vrinput.GetSkeletalActionData(
		actionHandle, &dataSkeletal, sizeof( dataSkeletal ) );
	if( inputError != vr::VRInputError_None ){
		return;
	}
	
	pActionHandPose = actionHandle;
	
	uint32_t boneCount;
	inputError = vrinput.GetBoneCount( actionHandle, &boneCount );
	if( inputError == vr::VRInputError_None && boneCount > 0 ){
// 		pBoneConfiguration = deInputDevice::ebcHand;
		
		pBoneTransformData = new vr::VRBoneTransform_t[ boneCount * 2 ];
		pBoneCount = ( int )boneCount;
		
		pPoseBones = new deInputDevicePose[ deInputDevice::HandBoneCount ];
		pPoseBoneCount = deInputDevice::HandBoneCount;
	}
}

void deovrDevice::pUpdateParametersTracker(){
	pType = deInputDevice::edtVRTracker;
	pName.Format( "Tracker %d", pNameNumber );
	pID.Format( "%str", OVR_DEVID_PREFIX );
	//pActionPose = pOvr.GetActionHandle( deVROpenVR::eiaPose );
	// not working
	
	// this is ugly but I found no better way yet. try to construct the device path
	pInputValuePath = "/devices/htc/vive_tracker";
	pInputValuePath += pSerialNumber;
	
	/*vr::EVRInputError inputError = */
	pOvr.GetVRInput().GetInputSourceHandle( pInputValuePath, &pInputValueHandle );
	//pOvr.LogInfoFormat("Tracker: path(%s) handle(%lu) error(%d)", pInputValuePath.GetString(), pInputValueHandle, inputError );
}

deovrDeviceComponent *deovrDevice::pAddComponent( deInputDeviceComponent::eComponentTypes type,
const char *name, const char *id, const char *displayText ){
	const deovrDeviceComponent::Ref component( deovrDeviceComponent::Ref::New( new deovrDeviceComponent( *this ) ) );
	component->SetID( id );
	component->SetName( name );
	component->SetType( type );
	component->SetDisplayText( displayText );
	component->SetIndex( pComponents.GetCount() );
	pComponents.Add( component );
	return component;
}

int deovrDevice::pAddButton( deInputDeviceButton::eButtonTypes type, deovrDeviceComponent *component,
deVROpenVR::eInputActions actionPress, deVROpenVR::eInputActions actionTouch,
const char *name, const char *id, const char *displayText ){
	vr::IVRInput &vrinput = pOvr.GetVRInput();
	vr::VRActionHandle_t actionHandle = pOvr.GetActionHandle( actionPress );
	vr::InputDigitalActionData_t dataDigital;
	
	vr::EVRInputError inputError = vrinput.GetDigitalActionData( actionHandle,
		&dataDigital, sizeof( dataDigital ), pInputValueHandle );
	if( inputError != vr::VRInputError_None ){
		return -1;
	}
	
	const deovrDeviceButton::Ref button( deovrDeviceButton::Ref::New( new deovrDeviceButton( *this ) ) );
	button->SetID( id );
	button->SetActionPressHandle( actionHandle );
	
	actionHandle = pOvr.GetActionHandle( actionTouch );
	inputError = vrinput.GetDigitalActionData( actionHandle,
		&dataDigital, sizeof( dataDigital ), pInputValueHandle );
	if( inputError == vr::VRInputError_None ){
		button->SetActionTouchHandle( actionHandle );
	}
	
	button->SetName( name );
	button->SetType( type );
	button->SetDisplayText( displayText );
	button->SetInputDeviceComponent( component );
	
	button->SetIndex( pButtons.GetCount() );
	pButtons.Add( button );
	return pButtons.GetCount() - 1;
}

int deovrDevice::pAddAxisTrigger( deInputDeviceAxis::eAxisTypes type,
deVROpenVR::eInputActions actionAnalog, const char *name, const char *id, const char *displayText ){
	vr::IVRInput &vrinput = pOvr.GetVRInput();
	vr::VRActionHandle_t actionHandle = pOvr.GetActionHandle( actionAnalog );
	
	vr::InputAnalogActionData_t dataAnalog;
	vr::EVRInputError inputError = vrinput.GetAnalogActionData( actionHandle,
		&dataAnalog, sizeof( dataAnalog ), pInputValueHandle );
	if( inputError != vr::VRInputError_None ){
		return -1;
	}
	
	const deovrDeviceAxis::Ref axis( deovrDeviceAxis::Ref::New( new deovrDeviceAxis( *this ) ) );
	axis->SetActionAnalogHandle( actionHandle );
	axis->SetType( type );
	axis->SetRange( 0.0f, 1.0f );
	axis->SetCenter( -1.0f );
	axis->SetValue( -1.0f );
	axis->SetName( name );
	axis->SetID( id );
	axis->SetDisplayText( displayText );
	axis->SetIndex( pAxes.GetCount() );
	pAxes.Add( axis );
	return pAxes.GetCount() - 1;
}

void deovrDevice::pAddAxisFinger( deInputDeviceAxis::eAxisTypes type, deovrDeviceComponent *component,
int finger, const char *name, const char *id, const char *displayText ){
	const deovrDeviceAxis::Ref axis( deovrDeviceAxis::Ref::New( new deovrDeviceAxis( *this ) ) );
	axis->SetType( type );
	axis->SetRange( 0.0f, 1.0f );
	axis->SetCenter( -1.0f );
	axis->SetValue( -1.0f );
	axis->SetName( name );
	axis->SetID( id );
	axis->SetDisplayText( displayText );
	axis->SetIndex( pAxes.GetCount() );
	axis->SetFinger( finger );
	axis->SetInputDeviceComponent( component );
	pAxes.Add( axis );
}

int deovrDevice::pAddAxesJoystick( deVROpenVR::eInputActions actionAnalog,
const char *name, const char *id, const char *displayText ){
	vr::IVRInput &vrinput = pOvr.GetVRInput();
	vr::VRActionHandle_t actionHandle = pOvr.GetActionHandle( actionAnalog );
	vr::InputAnalogActionData_t dataAnalog;
	
	vr::EVRInputError inputError = vrinput.GetAnalogActionData( actionHandle,
		&dataAnalog, sizeof( dataAnalog ), pInputValueHandle );
	if( inputError != vr::VRInputError_None ){
		return -1;
	}
	
	static const char *nameAxis[ 2 ] = { "X", "Y" };
	static const char *idAxis[ 2 ] = { "x", "y" };
	deovrDeviceAxis::Ref axis;
	decString text;
	int i;
	
	for( i=0; i<2; i++ ){
		axis.TakeOver( deovrDeviceAxis::Ref::New( new deovrDeviceAxis( *this ) ) );
		axis->SetActionAnalogHandle( actionHandle );
		axis->SetType( deInputDeviceAxis::eatStick );
		axis->SetComponent( i );
		text.Format( "%s %s", name, nameAxis[ i ] );
		axis->SetName( text );
		text.Format( "%s%s", id, idAxis[ i ] );
		axis->SetID( text );
		axis->SetDisplayText( displayText );
		axis->SetIndex( pAxes.GetCount() );
		axis->SetDeadZone( axis->GetResolution() );
		pAxes.Add( axis );
	}
	
	return pAxes.GetCount() - 2;
}

int deovrDevice::pAddAxesTrackpad( deVROpenVR::eInputActions actionAnalog,
const char *name, const char *id, const char *displayText ){
	vr::IVRInput &vrinput = pOvr.GetVRInput();
	vr::VRActionHandle_t actionHandle = pOvr.GetActionHandle( actionAnalog );
	vr::InputAnalogActionData_t dataAnalog;
	
	vr::EVRInputError inputError = vrinput.GetAnalogActionData( actionHandle,
		&dataAnalog, sizeof( dataAnalog ), pInputValueHandle );
	if( inputError != vr::VRInputError_None ){
		return -1;
	}
	
	static const char *nameAxis[ 2 ] = { "X", "Y" };
	static const char *idAxis[ 2 ] = { "x", "y" };
	deovrDeviceAxis::Ref axis;
	decString text;
	int i;
	
	for( i=0; i<2; i++ ){
		axis.TakeOver( deovrDeviceAxis::Ref::New( new deovrDeviceAxis( *this ) ) );
		axis->SetActionAnalogHandle( actionHandle );
		axis->SetType( deInputDeviceAxis::eatTouchPad );
		axis->SetComponent( i );
		text.Format( "%s %s", name, nameAxis[ i ] );
		axis->SetName( text );
		text.Format( "%s%s", id, idAxis[ i ] );
		axis->SetID( text );
		axis->SetDisplayText( displayText );
		axis->SetIndex( pAxes.GetCount() );
		axis->SetDeadZone( axis->GetResolution() );
		pAxes.Add( axis );
	}
	
	return pAxes.GetCount() - 2;
}

void deovrDevice::pUpdatePose( const vr::TrackedDevicePose_t &in, deInputDevicePose &out ) const{
	// OpenVR is right handed: right=x, up=y, forward=-z
	// to transform the matrix apply the conversion (-z) for both the row vectors and
	// the column vectors
	if( ! in.bPoseIsValid ){
		out = deInputDevicePose();
	}
	
	const decMatrix m( pOvr.ConvertMatrix( in.mDeviceToAbsoluteTracking ) );
	out.SetPosition( m.GetPosition() );
	out.SetOrientation( m.ToQuaternion() );
	
	const vr::HmdVector3_t &lv = in.vVelocity;
	out.SetLinearVelocity( decVector( lv.v[ 0 ], lv.v[ 1 ], -lv.v[ 2 ] ) );
	
	// angular velocity is rotationAxis * rotationAngle. rotationAxis has to be converted
	// by flipping z coordinate. going from right handed to left handed coordinate frame
	// requires flipping the rotationAngle, hence all coordinates
	const vr::HmdVector3_t &av = in.vAngularVelocity;
	out.SetAngularVelocity( decVector( -av.v[ 0 ], -av.v[ 1 ], av.v[ 2 ] ) );
}

void deovrDevice::pUpdatePose( const vr::VRBoneTransform_t &in, deInputDevicePose &out ) const{
	const vr::HmdVector4_t &p = in.position;
	out.SetPosition( decVector( p.v[ 0 ], p.v[ 1 ], -p.v[ 2 ] ) );
	
	const vr::HmdQuaternionf_t &o = in.orientation;
	out.SetOrientation( decQuaternion( -o.x, -o.y, o.z, -o.w ) );
	
	out.SetLinearVelocity( decVector() );
	out.SetAngularVelocity( decVector() );
}
