/* 
 * Drag[en]gine OpenXR VR Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "deoxrDPValveIndexController.h"
#include "../deoxrDeviceAxis.h"
#include "../deoxrDeviceButton.h"
#include "../deoxrDeviceComponent.h"
#include "../deoxrDeviceFeedback.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPValveIndexController
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPValveIndexController::deoxrDPValveIndexController( deoxrInstance &instance ) :
deoxrDeviceProfile( instance,
	deoxrPath( instance, "/interaction_profiles/valve/index_controller" ),
	"Valve Index Controller" )
{
}

deoxrDPValveIndexController::~deoxrDPValveIndexController(){
	pRemoveDevice( true );
	pRemoveDevice( false );
}



// Management
///////////////

void deoxrDPValveIndexController::CheckAttached(){
	if( pMatchesProfile( GetInstance().GetPathHandLeft() ) ){
		pAddDevice( true );
		
	}else{
		pRemoveDevice( true );
	}
	
	if( pMatchesProfile( GetInstance().GetPathHandRight() ) ){
		pAddDevice( false );
		
	}else{
		pRemoveDevice( false );
	}
}

void deoxrDPValveIndexController::SuggestBindings(){
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
	
	const int bindingCount = 17 * 2;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[ 2 ] = { "/user/hand/left", "/user/hand/right" };
	int i;
	
	for( i=0; i<2; i++ ){
		const decString &basePath = basePathList[ i ];
		
		pAdd( b, deVROpenXR::eiaPose, basePath + "/input/aim/pose" );
		
		pAdd( b, deVROpenXR::eiaGripGrab, basePath + "/input/squeeze/value" );
		pAdd( b, deVROpenXR::eiaGripSqueeze, basePath + "/input/squeeze/force" );
		
		pAdd( b, deVROpenXR::eiaTriggerPress, basePath + "/input/trigger/click" );
		pAdd( b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/trigger/value" );
		pAdd( b, deVROpenXR::eiaTriggerTouch, basePath + "/input/trigger/touch" );
		
		pAdd( b, deVROpenXR::eiaButtonPrimaryPress, basePath + "/input/a/click" );
		pAdd( b, deVROpenXR::eiaButtonPrimaryTouch, basePath + "/input/a/touch" );
		
		pAdd( b, deVROpenXR::eiaButtonSecondaryPress, basePath + "/input/b/click" );
		pAdd( b, deVROpenXR::eiaButtonSecondaryTouch, basePath + "/input/b/touch" );
		
		pAdd( b, deVROpenXR::eiaJoystickAnalog, basePath + "/input/thumbstick" );
		pAdd( b, deVROpenXR::eiaJoystickPress, basePath + "/input/thumbstick/click" );
		pAdd( b, deVROpenXR::eiaJoystickTouch, basePath + "/input/thumbstick/touch" );
		
		pAdd( b, deVROpenXR::eiaTrackpadAnalog, basePath + "/input/trackpad" );
		pAdd( b, deVROpenXR::eiaTrackpadPress, basePath + "/input/trackpad/force" );
		pAdd( b, deVROpenXR::eiaTrackpadTouch, basePath + "/input/trackpad/touch" );
		
		pAdd( b, deVROpenXR::eiaGripHaptic, basePath + "/output/haptic" );
	}
	
	
	GetInstance().SuggestBindings( GetPath(), bindings, bindingCount );
}



// Private Functions
//////////////////////

void deoxrDPValveIndexController::pAddDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if( device ){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	device.TakeOver( new deoxrDevice( oxr, *this ) );
	
	decString id;
	
	if( left ){
		device->SetType( deInputDevice::edtVRLeftHand );
		device->SetName( "Left Hand" );
		id.Format( "%scl", OXR_DEVID_PREFIX );
		device->SetSubactionPath( GetInstance().GetPathHandLeft() );
// 		device->SetActionHandPose( oxr.GetAction( deVROpenXR::eiaSkeletonHandLeft ) );
		
	}else{
		device->SetType( deInputDevice::edtVRRightHand );
		device->SetName( "Right Hand" );
		id.Format( "%scr", OXR_DEVID_PREFIX );
		device->SetSubactionPath( GetInstance().GetPathHandRight() );
// 		device->SetActionHandPose( oxr.GetAction( deVROpenXR::eiaSkeletonHandRight ) );
	}
	
	device->SetID( id );
	device->SetActionPose( oxr.GetAction( deVROpenXR::eiaPose ) );
	
	// axes and components
	deoxrDeviceComponent * const compTrigger = device->AddComponent(
		deInputDeviceComponent::ectTrigger, "Trigger", "trigger", "Trigger" );
	device->AddAxisTrigger( deInputDeviceAxis::eatTrigger, compTrigger,
		deVROpenXR::eiaTriggerAnalog, "Trigger", "trig", "Tri" );
	
	deoxrDeviceComponent * const compJoystick = device->AddComponent(
		deInputDeviceComponent::ectJoystick, "Joystick", "joystick", "Joystick" );
	device->AddAxesJoystick( compJoystick, deVROpenXR::eiaJoystickAnalog, "Joystick", "js", "Joy" );
	
	deoxrDeviceComponent * const compTrackpad = device->AddComponent(
		deInputDeviceComponent::ectTouchPad, "TrackPad", "trackpad", "TrackPad" );
	device->AddAxesTrackpad( compTrackpad, deVROpenXR::eiaTrackpadAnalog, "TrackPad", "tp", "Pad" );
	
	deoxrDeviceComponent * const compGrip = device->AddComponent(
		deInputDeviceComponent::ectGeneric, "Grip", "grip", "Grip" );
	device->AddAxisTrigger( deInputDeviceAxis::eatGripGrab, compGrip,
		deVROpenXR::eiaGripGrab, "Grab", "gg", "Grab" );
	device->AddAxisTrigger( deInputDeviceAxis::eatGripSqueeze, compGrip,
		deVROpenXR::eiaGripSqueeze, "Squeeze", "gs", "Squ" );
// 	device->AddAxisTrigger( deInputDeviceAxis::eatGripPinch, compGrip,
// 		deVROpenXR::eiaGripPinch, "Pinch", "gp", "Pin" );
	
	// buttons
	device->AddButton( deInputDeviceButton::ebtTrigger, compTrackpad, deVROpenXR::eiaTriggerPress,
		deVROpenXR::eiaTriggerTouch, "Trigger", "trig", "Tri" );
	
	deoxrDeviceComponent * const compA = device->AddComponent(
		deInputDeviceComponent::ectButton, "A", "a", "A" );
	device->AddButton( deInputDeviceButton::ebtAction, compA, deVROpenXR::eiaButtonPrimaryPress,
		deVROpenXR::eiaButtonPrimaryTouch, "A", "a", "A" );
	
	deoxrDeviceComponent * const compB = device->AddComponent(
		deInputDeviceComponent::ectButton, "B", "b", "B" );
	device->AddButton( deInputDeviceButton::ebtAction, compB, deVROpenXR::eiaButtonSecondaryPress,
		deVROpenXR::eiaButtonSecondaryTouch, "B", "b", "B" );
	
	device->AddButton( deInputDeviceButton::ebtStick, compJoystick, deVROpenXR::eiaJoystickPress,
		deVROpenXR::eiaJoystickTouch, "Joystick", "js", "Joy" );
	
	device->AddButton( deInputDeviceButton::ebtTouchPad, compTrackpad, deVROpenXR::eiaTrackpadPress,
		deVROpenXR::eiaTrackpadTouch, "TrackPad", "tp", "Pad" );
	
	device->AddButton( deInputDeviceButton::ebtTrigger, compGrip, deVROpenXR::eiaGripPress,
		deVROpenXR::eiaGripTouch, "Grip", "grip", "Grip" );
	
	// finger bending and spreading
	/*
	deoxrDeviceComponent * const compHand = device->AddComponent(
		deInputDeviceComponent::ectGeneric, "Hand Pose", "handPose", "Hand Pose" );
	
	device->AddAxisFinger( deInputDeviceAxis::eatFingerBend, compHand, 0, "Bend Thumb", "fb1", "FB1" );
	device->AddAxisFinger( deInputDeviceAxis::eatFingerBend, compHand, 1, "Bend Index Finger", "fb2", "FB2" );
	device->AddAxisFinger( deInputDeviceAxis::eatFingerBend, compHand, 2, "Bend Middle Finger", "fb3", "FB3" );
	device->AddAxisFinger( deInputDeviceAxis::eatFingerBend, compHand, 3, "Bend Ring Finger", "fb4", "FB4" );
	device->AddAxisFinger( deInputDeviceAxis::eatFingerBend, compHand, 4, "Bend Pinky Finger", "fb5", "FB5" );
	
	device->AddAxisFinger( deInputDeviceAxis::eatFingerSpread, compHand, 0, "Spread Thumb Index Finger", "fs1", "FS1" );
	device->AddAxisFinger( deInputDeviceAxis::eatFingerSpread, compHand, 1, "Spread Index Middle Finger", "fs2", "FS2" );
	device->AddAxisFinger( deInputDeviceAxis::eatFingerSpread, compHand, 2, "Spread Middle Ring Finger", "fs3", "FS3" );
	device->AddAxisFinger( deInputDeviceAxis::eatFingerSpread, compHand, 3, "Spread Ring Pinky Finger", "fs4", "FS4" );
	*/
	
	// add device
	oxr.GetDevices().Add( device );
}

void deoxrDPValveIndexController::pRemoveDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if( ! device ){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	oxr.GetDevices().Remove( device );
	device = nullptr;
}
