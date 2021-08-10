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

#include "deovrDevice.h"
#include "deovrDeviceAxis.h"
#include "deovrDeviceButton.h"
#include "deovrDeviceFeedback.h"
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
pNameNumber( -1 )
{
	UpdateParameters();
}

deovrDevice::~deovrDevice(){
}



// Management
///////////////

void deovrDevice::SetIndex( int index ){
	pIndex = index;
}

void deovrDevice::SetType( deInputDevice::eDeviceTypes type ){
	pType = type;
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

deovrDeviceButton *deovrDevice::GetButtonWithType( vr::EVRButtonId buttonType ) const{
	const int count = pButtons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deovrDeviceButton * const button = ( deovrDeviceButton* )pButtons.GetAt( i );
		if( button->GetButtonType() == buttonType ){
			return button;
		}
	}
	
	return nullptr;
}

deovrDeviceButton *deovrDevice::GetButtonWithMask( uint32_t buttonMask ) const{
	const int count = pButtons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deovrDeviceButton * const button = ( deovrDeviceButton* )pButtons.GetAt( i );
		if( button->GetButtonMask() == buttonMask ){
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

int deovrDevice::IndexOfButtonWithType( vr::EVRButtonId buttonType ) const{
	const int count = pButtons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deovrDeviceButton &button = *( ( deovrDeviceButton* )pButtons.GetAt( i ) );
		if( button.GetButtonType() == buttonType ){
			return i;
		}
	}
	
	return -1;
}

int deovrDevice::IndexOfButtonWithMask( uint32_t buttonMask ) const{
	const int count = pButtons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deovrDeviceButton &button = *( ( deovrDeviceButton* )pButtons.GetAt( i ) );
		if( button.GetButtonMask() == buttonMask ){
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
	
	info.SetBoneConfiguration( deInputDevice::ebcNone );
}

void deovrDevice::UpdateParameters(){
	const vr::TrackedDeviceClass oldDeviceClass = pDeviceClass;
	pDeviceClass = pOvr.GetSystem().GetTrackedDeviceClass( pDeviceIndex );
	
	if( pDeviceClass != oldDeviceClass || pNameNumber == -1 ){
		pNameNumber = -1; // required so we do not block ourselves
		pNameNumber = pOvr.GetDevices().NextNameNumber( pDeviceClass );
	}
	
	pControllerRole = vr::TrackedControllerRole_Invalid;
	
	switch( pDeviceClass ){
	case vr::TrackedDeviceClass_HMD:
		pUpdateParametersHMD();
		break;
		
	case vr::TrackedDeviceClass_Controller:
		pUpdateParametersController();
		break;
		
	case vr::TrackedDeviceClass_GenericTracker:
		pType = deInputDevice::edtVRTracker;
		pName.Format( "Tracker %d", pNameNumber );
		pID.Format( "%str", OVR_DEVID_PREFIX );
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
	vr::ETrackedPropertyError error;
	char buffer[ 256 ];
	
	/*
	pOvr.GetSystem().GetStringTrackedDeviceProperty( pDeviceIndex,
		vr::Prop_TrackingSystemName_String, buffer, 256, &error );
	if( error != vr::TrackedProp_Success ){
		DETHROW_INFO( deeInvalidParam, "GetStringTrackedDeviceProperty failed" );
	}
	pName = buffer;
	*/
	
	// serial number. append to ID to make it unique (hopefully)
	pOvr.GetSystem().GetStringTrackedDeviceProperty( pDeviceIndex,
		vr::Prop_SerialNumber_String, buffer, 256, &error );
	if( error != vr::TrackedProp_Success ){
		DETHROW_INFO( deeInvalidParam, "Prop_SerialNumber_String failed" );
	}
	pID += "_";
	pID += pOvr.GetDevices().NormalizeID( buffer );
	
	// controller only: Prop_AttachedDeviceId_String
}

void deovrDevice::TrackStates(){
	if( ! pOvr.GetSystem().GetControllerState( pDeviceIndex, &pState, sizeof( pState ) ) ){
		ResetStates();
		return;
	}
	
	const int axisCount = pAxes.GetCount();
	int i;
	for( i=0; i<axisCount; i++ ){
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
	pUpdatePose( pOvr.GetDevices().GetDevicePoseAt( pDeviceIndex ), pose );
}

void deovrDevice::GetBonePose( int bone, deInputDevicePose &pose ){
	pose = deInputDevicePose();
}



// Private Functions
//////////////////////

void deovrDevice::pUpdateParametersHMD(){
	pType = deInputDevice::edtVRHMD;
	pName = "HMD";
	pID.Format( "%shmd", OVR_DEVID_PREFIX );
}

void deovrDevice::pUpdateParametersController(){
	vr::ETrackedPropertyError error;
	int i;
	
	pType = deInputDevice::edtVRController;
	pControllerRole = pOvr.GetSystem().GetControllerRoleForTrackedDeviceIndex( pDeviceIndex );
	
	switch( pControllerRole ){
	case vr::TrackedControllerRole_LeftHand:
		pName.Format( "Controller Left %d", pNameNumber );
		pID.Format( "%scl", OVR_DEVID_PREFIX );
		break;
		
	case vr::TrackedControllerRole_RightHand:
		pName.Format( "Controller Right %d", pNameNumber );
		pID.Format( "%scr", OVR_DEVID_PREFIX );
		break;
		
	case vr::TrackedControllerRole_Treadmill:
		pName.Format( "Treadmill %d", pNameNumber );
		pID.Format( "%sct", OVR_DEVID_PREFIX );
		break;
		
	case vr::TrackedControllerRole_Stylus:
		pName.Format( "Stylus %d", pNameNumber );
		pID.Format( "%scs", OVR_DEVID_PREFIX );
		break;
		
	default:
		pName.Format( "Controller %d", pNameNumber );
		pID.Format( "%scg", OVR_DEVID_PREFIX );
	}
	
	// axes
	static const vr::ETrackedDeviceProperty axisProp[ 5 ] = { vr::Prop_Axis0Type_Int32,
		vr::Prop_Axis1Type_Int32, vr::Prop_Axis2Type_Int32, vr::Prop_Axis3Type_Int32,
		vr::Prop_Axis4Type_Int32 };
	
	int axisNumber[ 5 ] = { 1, 1, 1, 1, 1 };
	int nextNumTrigger = 1;
	int nextNumJoystick = 1;
	int nextNumTrackpad = 1;
	
	for( i=0; i<5; i++ ){
		const vr::EVRControllerAxisType type = ( vr::EVRControllerAxisType )
			pOvr.GetSystem().GetInt32TrackedDeviceProperty( pDeviceIndex, axisProp[ i ], &error );
		if( error != vr::TrackedProp_Success ){
			continue; // not present or otherwise not usable. ignore it
		}
		
		switch( type ){
		case vr::k_eControllerAxis_Trigger:
			axisNumber[ i ] = nextNumTrigger;
			pAddAxisTrigger( i, nextNumTrigger );
			break;
			
		case vr::k_eControllerAxis_Joystick:
			axisNumber[ i ] = nextNumJoystick;
			pAddAxesJoystick( i, nextNumJoystick );
			break;
			
		case vr::k_eControllerAxis_TrackPad:
			axisNumber[ i ] = nextNumTrackpad;
			pAddAxesTrackpad( i, nextNumTrackpad );
			break;
			
		default:
			break;
		}
	}
	
	// buttons
	const uint64_t buttons = pOvr.GetSystem().GetUint64TrackedDeviceProperty(
		pDeviceIndex, vr::Prop_SupportedButtons_Uint64, &error );
	if( error != vr::TrackedProp_Success ){
		DETHROW_INFO( deeInvalidParam, "Prop_SerialNumber_String failed" );
	}
	
	static const struct sButtonConfig{
		vr::EVRButtonId buttonType;
		const char *name;
		const char *id;
		const char *displayText;
		vr::ETrackedDeviceProperty axis;
		int axisNum;
	} btnConf[ 14 ] = {
		{ vr::k_EButton_System, "System", "system", "Sys", vr::Prop_Invalid, 0 },
		{ vr::k_EButton_ApplicationMenu, "Menu", "menu", "Menu", vr::Prop_Invalid, 0 },
		{ vr::k_EButton_Grip, "Grip", "grip", "Grip", vr::Prop_Invalid, 0 },
		{ vr::k_EButton_DPad_Left, "Left", "left", "Left", vr::Prop_Invalid, 0 },
		{ vr::k_EButton_DPad_Up, "Up", "up", "Up", vr::Prop_Invalid, 0 },
		{ vr::k_EButton_DPad_Right, "Right", "right", "Right", vr::Prop_Invalid, 0 },
		{ vr::k_EButton_DPad_Down, "Down", "down", "Down", vr::Prop_Invalid, 0 },
		{ vr::k_EButton_A, "A", "a", "A", vr::Prop_Invalid, 0 },
		{ vr::k_EButton_ProximitySensor, "Proximity", "proximity", "Prox", vr::Prop_Invalid, 0 },
		{ vr::k_EButton_Axis0, "Axis 1", "axis1", "A1", vr::Prop_Axis0Type_Int32, 1 },
		{ vr::k_EButton_Axis1, "Axis 2", "axis2", "A2", vr::Prop_Axis1Type_Int32, 2 },
		{ vr::k_EButton_Axis2, "Axis 3", "axis3", "A3", vr::Prop_Axis2Type_Int32, 3 },
		{ vr::k_EButton_Axis3, "Axis 4", "axis4", "A4", vr::Prop_Axis3Type_Int32, 4 },
		{ vr::k_EButton_Axis4, "Axis 5", "axis5", "A5", vr::Prop_Axis4Type_Int32, 5 } };
	
	deovrDeviceButton::Ref button;
	decString text;
	
	for( i=0; i<14; i++ ){
		if( ( buttons & vr::ButtonMaskFromId( btnConf[ i ].buttonType ) ) == 0 ){
			continue;
		}
		
		button.TakeOver( new deovrDeviceButton( *this, btnConf[ i ].buttonType ) );
		button->SetID( btnConf[ i ].id );
		
		if( btnConf[ i ].axis == vr::Prop_Invalid ){
			button->SetName( btnConf[ i ].name );
			button->SetDisplayText( btnConf[ i ].displayText );
			
		}else{
			const vr::EVRControllerAxisType type = ( vr::EVRControllerAxisType )pOvr.GetSystem().
				GetInt32TrackedDeviceProperty( pDeviceIndex, btnConf[ i ].axis, &error );
			if( error == vr::TrackedProp_Success ){
				switch( type ){
				case vr::k_eControllerAxis_Trigger:
					text.Format( "Trigger %d", axisNumber[ btnConf[ i ].axisNum ] );
					button->SetName( text );
					text.Format( "Trig%d", axisNumber[ btnConf[ i ].axisNum ] );
					button->SetDisplayText( text );
					break;
					
				case vr::k_eControllerAxis_Joystick:
					text.Format( "Joystick %d", axisNumber[ btnConf[ i ].axisNum ] );
					button->SetName( text );
					text.Format( "Joy%d", axisNumber[ btnConf[ i ].axisNum ] );
					button->SetDisplayText( text );
					break;
					
				case vr::k_eControllerAxis_TrackPad:
					text.Format( "TrackPad %d", axisNumber[ btnConf[ i ].axisNum ] );
					button->SetName( text );
					text.Format( "Pad%d", axisNumber[ btnConf[ i ].axisNum ] );
					button->SetDisplayText( text );
					break;
					
				default:
					button->SetName( btnConf[ i ].name );
					button->SetDisplayText( btnConf[ i ].displayText );
				}
				
			}else{
				button->SetName( btnConf[ i ].name );
				button->SetDisplayText( btnConf[ i ].displayText );
			}
		}
		
		button->SetIndex( pButtons.GetCount() );
		pButtons.Add( button );
	}
}

void deovrDevice::pAddAxisTrigger( int index, int &nextNum ){
	deovrDeviceAxis::Ref axis;
	decString text;
	
	axis.TakeOver( new deovrDeviceAxis( *this, index ) );
	axis->SetAxisType( vr::k_eControllerAxis_Trigger );
	axis->SetType( deInputDeviceAxis::eatTrigger );
	axis->SetValue( -1.0f );
	text.Format( "Trigger %d", nextNum );
	axis->SetName( text );
	text.Format( "tr%d", nextNum );
	axis->SetID( text );
	axis->SetIndex( pAxes.GetCount() );
	pAxes.Add( axis );
	
	nextNum++;
}

void deovrDevice::pAddAxesJoystick( int index, int &nextNum ){
	static const char *name[ 2 ] = { "X", "Y" };
	static const char *id[ 2 ] = { "x", "y" };
	deovrDeviceAxis::Ref axis;
	decString text;
	int i;
	
	for( i=0; i<2; i++ ){
		axis.TakeOver( new deovrDeviceAxis( *this, index ) );
		axis->SetAxisType( vr::k_eControllerAxis_Joystick );
		axis->SetType( deInputDeviceAxis::eatStick );
		axis->SetUseX( i == 0 );
		text.Format( "Joystick %d %s", nextNum, name[ i ] );
		axis->SetName( text );
		text.Format( "js%d%s", nextNum, id[ i ] );
		axis->SetID( text );
		axis->SetIndex( pAxes.GetCount() );
		pAxes.Add( axis );
	}
	
	nextNum++;
}

void deovrDevice::pAddAxesTrackpad( int index, int &nextNum ){
	static const char *name[ 2 ] = { "X", "Y" };
	static const char *id[ 2 ] = { "x", "y" };
	deovrDeviceAxis::Ref axis;
	decString text;
	int i;
	
	for( i=0; i<2; i++ ){
		axis.TakeOver( new deovrDeviceAxis( *this, index ) );
		axis->SetAxisType( vr::k_eControllerAxis_TrackPad );
		axis->SetType( deInputDeviceAxis::eatTouchPad );
		axis->SetUseX( i == 0 );
		text.Format( "TrackPad %d %s", nextNum, name[ i ] );
		axis->SetName( text );
		text.Format( "tp%d%s", nextNum, id[ i ] );
		axis->SetID( text );
		axis->SetIndex( pAxes.GetCount() );
		pAxes.Add( axis );
	}
	
	nextNum++;
}

void deovrDevice::pUpdatePose( const vr::TrackedDevicePose_t &in, deInputDevicePose &out ) const{
	// OpenVR is right handed: right=x, up=y, forward=-z
	// to transform the matrix apply the conversion (-z) for both the row vectors and
	// the column vectors
	
	if( ! in.bPoseIsValid ){
		out = deInputDevicePose();
	}
	
	const vr::HmdMatrix34_t &m = in.mDeviceToAbsoluteTracking;
	out.SetPosition( decVector( m.m[ 0 ][ 3 ], m.m[ 1 ][ 3 ], -m.m[ 2 ][ 3 ] ) );
	
	decMatrix rm;
	rm.a11 =  m.m[ 0 ][ 0 ]; rm.a12 =  m.m[ 0 ][ 1 ]; rm.a13 = -m.m[ 0 ][ 2 ];
	rm.a21 =  m.m[ 1 ][ 0 ]; rm.a22 =  m.m[ 1 ][ 1 ]; rm.a23 = -m.m[ 1 ][ 2 ];
	rm.a31 = -m.m[ 2 ][ 0 ]; rm.a32 = -m.m[ 2 ][ 1 ]; rm.a33 =  m.m[ 2 ][ 2 ];
	out.SetOrientation( rm.ToQuaternion() );
	
	const vr::HmdVector3_t &lv = in.vVelocity;
	out.SetLinearVelocity( decVector( lv.v[ 0 ], lv.v[ 1 ], -lv.v[ 2 ] ) );
	
	// angular velocity is rotationAxis * rotationAngle. rotationAxis has to be converted
	// by flipping z coordinate. going from right handed to left handed coordinate frame
	// requires flipping the rotationAngle, hence all coordinates
	const vr::HmdVector3_t &av = in.vAngularVelocity;
	out.SetAngularVelocity( decVector( -av.v[ 0 ], -av.v[ 1 ], av.v[ 2 ] ) );
}
