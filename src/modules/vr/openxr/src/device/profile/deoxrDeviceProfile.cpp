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

#include "deoxrDeviceProfile.h"
#include "../deoxrDeviceAxis.h"
#include "../deoxrDeviceButton.h"
#include "../deoxrDeviceComponent.h"
#include "../../deoxrHandTracker.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDeviceProfile
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDeviceProfile::deoxrDeviceProfile( deoxrInstance &instance, const deoxrPath &path, const char *name ) :
pInstance( instance ),
pPath( path ),
pName( name )
{
	// if using raw device pose
// 	pDeviceRotation.Set( 45.0f, 0.0f, 0.0f );
}

deoxrDeviceProfile::~deoxrDeviceProfile(){
}



// Management
///////////////

void deoxrDeviceProfile::SetDeviceRotation( const decVector &rotation ){
	pDeviceRotation = rotation;
}

void deoxrDeviceProfile::CreateActions( deoxrActionSet& ){
}

void deoxrDeviceProfile::OnSessionStateChanged(){
}

void deoxrDeviceProfile::OnActionsSynced(){
}

void deoxrDeviceProfile::CheckAttached(){
}

void deoxrDeviceProfile::ClearActions(){
}

void deoxrDeviceProfile::RemoveDevice( deInputDevice::eDeviceTypes ){
}

void deoxrDeviceProfile::OnSessionEnd(){
}



// Protected Functions
////////////////////////

deoxrSession *deoxrDeviceProfile::pGetSession() const{
	return GetInstance().GetOxr().GetSession();
}

void deoxrDeviceProfile::pHasHandDevices( deoxrDevice *deviceLeft, deoxrDevice *deviceRight,
bool &hasLeft, bool &hasRight ) const{
	const deoxrDeviceManager &devices = GetInstance().GetOxr().GetDevices();
	const int count = devices.GetCount();
	int i;
	
	hasLeft = false;
	hasRight = false;
	
	for( i=0; i<count; i++ ){
		deoxrDevice * const device = devices.GetAt( i );
		if( deviceLeft == device || deviceRight == device ){
			continue;
		}
		
		switch( device->GetType() ){
		case deInputDevice::edtVRLeftHand:
			hasLeft = true;
			break;
			
		case deInputDevice::edtVRRightHand:
			hasRight = true;
			break;
			
		default:
			break;
		}
	}
}

bool deoxrDeviceProfile::pHasAnyHandDevice( deoxrDevice *deviceLeft, deoxrDevice *deviceRight ) const{
	bool hasLeft = false, hasRight = false;
	pHasHandDevices( deviceLeft, deviceRight, hasLeft, hasRight );
	return hasLeft || hasRight;
}

deVROpenXR::eInputActions deoxrDeviceProfile::pPoseAction( bool left ) const{
	return left ? deVROpenXR::eiaPoseLeft : deVROpenXR::eiaPoseRight;
}

deVROpenXR::eInputActions deoxrDeviceProfile::pPoseAction2( bool left ) const{
	return left ? deVROpenXR::eiaPoseLeft2 : deVROpenXR::eiaPoseRight2;
}

void deoxrDeviceProfile::pAdd( deoxrInstance::sSuggestBinding *&bindings,
deVROpenXR::eInputActions inputAction, const char *path ) const{
	( bindings++ )->Set( pInstance.GetOxr().GetAction( inputAction ), deoxrPath( pInstance, path ) );
}

bool deoxrDeviceProfile::pMatchesProfile( const deoxrPath &path ) const{
	const deoxrSession * const session = pInstance.GetOxr().GetSession();
	if(!session || !session->GetAttachedActionSet()){
		return false;
	}
	
	XrInteractionProfileState state{XR_TYPE_INTERACTION_PROFILE_STATE};
	return XR_SUCCEEDED(pInstance.xrGetCurrentInteractionProfile(session->GetSession(), path, &state))
		&& pPath == state.interactionProfile;
}

void deoxrDeviceProfile::pCreateDevice( deoxrDevice::Ref &device, bool leftHand,
const char *idPrefix, bool withOrientationAction ){
	deVROpenXR &oxr = GetInstance().GetOxr();
	device.TakeOver( new deoxrDevice( oxr, *this ) );
	
	decString id;
	
	if( leftHand ){
		device->SetType( deInputDevice::edtVRLeftHand );
		device->SetName( "Left Hand" );
		id.Format( "%s%scl", OXR_DEVID_PREFIX, idPrefix );
		device->SetSubactionPath( GetInstance().GetPathHandLeft() );
		
	}else{
		device->SetType( deInputDevice::edtVRRightHand );
		device->SetName( "Right Hand" );
		id.Format( "%s%scr", OXR_DEVID_PREFIX, idPrefix );
		device->SetSubactionPath( GetInstance().GetPathHandRight() );
	}
	
	device->SetActionPose( oxr.GetAction( pPoseAction( leftHand ) ) );
	if( withOrientationAction ){
		device->SetActionPoseOrientation( oxr.GetAction( pPoseAction2( leftHand ) ) );
	}
	
	device->SetID( id );
	
	device->SetSpacePose( deoxrSpace::Ref::New( new deoxrSpace( *pGetSession(),
		device->GetActionPose(), device->GetSubactionPath(), pDeviceRotation ) ) );
	if( withOrientationAction ){
		device->SetSpacePoseOrientation( deoxrSpace::Ref::New( new deoxrSpace( *pGetSession(),
			device->GetActionPoseOrientation(), device->GetSubactionPath(), pDeviceRotation ) ) );
	}
}

deoxrDeviceComponent *deoxrDeviceProfile::pAddComponent( deoxrDevice &device,
deInputDeviceComponent::eComponentTypes type, const char *name, const char *id,
const char *displayText ){
	const deoxrDeviceComponent::Ref component(
		deoxrDeviceComponent::Ref::New( new deoxrDeviceComponent( device ) ) );
	component->SetID( id );
	component->SetName( name );
	component->SetDisplayText( displayText );
	component->SetType( type );
	component->SetIndex( device.GetComponentCount() );
	device.AddComponent( component );
	return component;
}

deoxrDeviceComponent *deoxrDeviceProfile::pAddComponentTrigger( deoxrDevice &device ){
	return pAddComponent( device, deInputDeviceComponent::ectTrigger, "Trigger", "trigger", "Trigger" );
}

void deoxrDeviceProfile::pAddAxisTrigger( deoxrDevice &device, deoxrDeviceComponent *component ){
	const deoxrDeviceAxis::Ref axis( deoxrDeviceAxis::Ref::New( new deoxrDeviceAxis( device ) ) );
	axis->SetActionAnalog( pInstance.GetOxr().GetAction( deVROpenXR::eiaTriggerAnalog ) );
	axis->SetType( deInputDeviceAxis::eatTrigger );
	axis->SetRange( 0.0f, 1.0f );
	axis->SetCenter( -1.0f );
	axis->SetValue( -1.0f );
	axis->SetName( "Trigger" );
	axis->SetID( "trig" );
	axis->SetDisplayText( "Tri" );
	axis->SetIndex( device.GetAxisCount() );
	axis->SetInputDeviceComponent( component );
	axis->SetFinger(0);
	device.AddAxis( axis );
}

void deoxrDeviceProfile::pAddButtonTrigger(deoxrDevice &device, deoxrDeviceComponent *component,
bool withTouch, bool withAproach){
	const deoxrDeviceButton::Ref button(deoxrDeviceButton::Ref::New(new deoxrDeviceButton(device)));
	button->SetID("trig");
	button->SetName("Trigger");
	button->SetType(deInputDeviceButton::ebtTrigger);
	button->SetDisplayText("Tri");
	button->SetInputDeviceComponent(component);
	button->SetActionPress(pInstance.GetOxr().GetAction(deVROpenXR::eiaTriggerPress));
	if(withTouch){
		button->SetActionTouch(pInstance.GetOxr().GetAction(deVROpenXR::eiaTriggerTouch));
	}
	if(withAproach){
		button->SetActionApproach(pInstance.GetOxr().GetAction(deVROpenXR::eiaTriggerNear));
	}
	button->SetFinger(0);
	button->SetIndex( device.GetButtonCount() );
	device.AddButton( button );
}

void deoxrDeviceProfile::pAddAxisTriggerForce(deoxrDevice &device, deoxrDeviceComponent *component){
	const deoxrDeviceAxis::Ref axis(deoxrDeviceAxis::Ref::New(new deoxrDeviceAxis(device)));
	axis->SetActionAnalog(pInstance.GetOxr().GetAction(deVROpenXR::eiaTriggerForce));
	axis->SetType(deInputDeviceAxis::eatTriggerForce);
	axis->SetRange(0.0f, 1.0f);
	axis->SetCenter(-1.0f);
	axis->SetValue(-1.0f);
	axis->SetName("Trigger Force");
	axis->SetID("trifo");
	axis->SetDisplayText("TriF");
	axis->SetIndex(device.GetAxisCount());
	axis->SetInputDeviceComponent(component);
	device.AddAxis(axis);
}

void deoxrDeviceProfile::pAddAxisTriggerCurl(deoxrDevice &device, deoxrDeviceComponent *component){
	const deoxrDeviceAxis::Ref axis(deoxrDeviceAxis::Ref::New(new deoxrDeviceAxis(device)));
	axis->SetActionAnalog(pInstance.GetOxr().GetAction(deVROpenXR::eiaTriggerCurl));
	axis->SetType(deInputDeviceAxis::eatTriggerCurl);
	axis->SetRange(0.0f, 1.0f);
	axis->SetCenter(-1.0f);
	axis->SetValue(-1.0f);
	axis->SetName("Trigger Curl");
	axis->SetID("tricu");
	axis->SetDisplayText("TriC");
	axis->SetIndex(device.GetAxisCount());
	axis->SetInputDeviceComponent(component);
	device.AddAxis(axis);
}

void deoxrDeviceProfile::pAddAxisTriggerSlide(deoxrDevice &device, deoxrDeviceComponent *component){
	const deoxrDeviceAxis::Ref axis(deoxrDeviceAxis::Ref::New(new deoxrDeviceAxis(device)));
	axis->SetActionAnalog(pInstance.GetOxr().GetAction(deVROpenXR::eiaTriggerSlide));
	axis->SetType(deInputDeviceAxis::eatTriggerSlide);
	axis->SetRange(0.0f, 1.0f);
	axis->SetCenter(-1.0f);
	axis->SetValue(-1.0f);
	axis->SetName("Trigger Slide");
	axis->SetID("trisl");
	axis->SetDisplayText("TriS");
	axis->SetIndex(device.GetAxisCount());
	axis->SetInputDeviceComponent(component);
	device.AddAxis(axis);
}

deoxrDeviceComponent *deoxrDeviceProfile::pAddComponentTrackpad( deoxrDevice &device ){
	return pAddComponent( device, deInputDeviceComponent::ectTouchPad, "TrackPad", "trackpad", "TrackPad" );
}

void deoxrDeviceProfile::pAddAxesTrackpad( deoxrDevice &device, deoxrDeviceComponent *component ){
	static const char *nameAxis[ 2 ] = { "X", "Y" };
	static const char *idAxis[ 2 ] = { "x", "y" };
	decString text;
	int i;
	
	for( i=0; i<2; i++ ){
		const deoxrDeviceAxis::Ref axis( deoxrDeviceAxis::Ref::New( new deoxrDeviceAxis( device ) ) );
		axis->SetActionAnalog( pInstance.GetOxr().GetAction( deVROpenXR::eiaTrackpadAnalog ) );
		axis->SetType( deInputDeviceAxis::eatTouchPad );
		axis->SetComponent( i );
		text.Format( "TrackPad %s", nameAxis[ i ] );
		axis->SetName( text );
		text.Format( "tp%s", idAxis[ i ] );
		axis->SetID( text );
		axis->SetDisplayText( "Pad" );
		axis->SetIndex( device.GetAxisCount() );
		axis->SetDeadZone( axis->GetResolution() );
		axis->SetInputDeviceComponent( component );
		device.AddAxis( axis );
	}
}

void deoxrDeviceProfile::pAddButtonTrackpad( deoxrDevice &device,
deoxrDeviceComponent *component, bool withPress, bool withTouch ){
	const deoxrDeviceButton::Ref button( deoxrDeviceButton::Ref::New( new deoxrDeviceButton( device ) ) );
	button->SetID( "tp" );
	button->SetName( "TrackPad" );
	button->SetType( deInputDeviceButton::ebtTouchPad );
	button->SetDisplayText( "Pad" );
	button->SetInputDeviceComponent( component );
	if( withPress ){
		button->SetActionPress( pInstance.GetOxr().GetAction( deVROpenXR::eiaTrackpadPress ) );
	}
	if( withTouch ){
		button->SetActionTouch( pInstance.GetOxr().GetAction( deVROpenXR::eiaTrackpadTouch ) );
	}
	button->SetIndex( device.GetButtonCount() );
	device.AddButton( button );
}

deoxrDeviceComponent *deoxrDeviceProfile::pAddComponentJoystick( deoxrDevice &device ){
	return pAddComponent( device, deInputDeviceComponent::ectJoystick, "Joystick", "joystick", "Joystick" );
}

void deoxrDeviceProfile::pAddAxesJoystick( deoxrDevice &device, deoxrDeviceComponent *component ){
	static const char *nameAxis[ 2 ] = { "X", "Y" };
	static const char *idAxis[ 2 ] = { "x", "y" };
	decString text;
	int i;
	
	for( i=0; i<2; i++ ){
		const deoxrDeviceAxis::Ref axis( deoxrDeviceAxis::Ref::New( new deoxrDeviceAxis( device ) ) );
		axis->SetActionAnalog( pInstance.GetOxr().GetAction( deVROpenXR::eiaJoystickAnalog ) );
		axis->SetType( deInputDeviceAxis::eatStick );
		axis->SetComponent( i );
		text.Format( "Joystick %s", nameAxis[ i ] );
		axis->SetName( text );
		text.Format( "js%s", idAxis[ i ] );
		axis->SetID( text );
		axis->SetDisplayText( "Joy" );
		axis->SetIndex( device.GetAxisCount() );
		axis->SetDeadZone( axis->GetResolution() );
		axis->SetInputDeviceComponent( component );
		device.AddAxis( axis );
	}
}

void deoxrDeviceProfile::pAddButtonJoystick( deoxrDevice &device,
deoxrDeviceComponent *component, bool withPress, bool withTouch ){
	const deoxrDeviceButton::Ref button( deoxrDeviceButton::Ref::New( new deoxrDeviceButton( device ) ) );
	button->SetID( "js" );
	button->SetName( "Joystick" );
	button->SetType( deInputDeviceButton::ebtStick );
	button->SetDisplayText( "Joy" );
	button->SetInputDeviceComponent( component );
	if( withPress ){
		button->SetActionPress( pInstance.GetOxr().GetAction( deVROpenXR::eiaJoystickPress ) );
	}
	if( withTouch ){
		button->SetActionTouch( pInstance.GetOxr().GetAction( deVROpenXR::eiaJoystickTouch ) );
	}
	button->SetIndex( device.GetButtonCount() );
	device.AddButton( button );
}

deoxrDeviceComponent *deoxrDeviceProfile::pAddComponentThumbrest( deoxrDevice &device ){
	return pAddComponent( device, deInputDeviceComponent::ectGeneric, "Thumbrest", "thumbrest", "Thumbrest" );
}

void deoxrDeviceProfile::pAddButtonThumbrest(deoxrDevice &device, deoxrDeviceComponent *component,
bool withPress, bool withApproach){
	const deoxrDeviceButton::Ref button(deoxrDeviceButton::Ref::New(new deoxrDeviceButton(device)));
	button->SetID("trest");
	button->SetName("Thumbrest");
	button->SetType(deInputDeviceButton::ebtGeneric);
	button->SetDisplayText("ThR");
	button->SetInputDeviceComponent(component);
	button->SetActionTouch(pInstance.GetOxr().GetAction(deVROpenXR::eiaThumbrestTouch));
	if(withPress){
		button->SetActionPress(pInstance.GetOxr().GetAction(deVROpenXR::eiaThumbrestPress));
	}
	if(withApproach){
		button->SetActionApproach(pInstance.GetOxr().GetAction(deVROpenXR::eiaThumbrestNear));
	}
	button->SetIndex(device.GetButtonCount());
	device.AddButton(button);
}

void deoxrDeviceProfile::pAddAxesThumbrestPress(deoxrDevice &device, deoxrDeviceComponent *component){
	const deoxrDeviceAxis::Ref axis(deoxrDeviceAxis::Ref::New(new deoxrDeviceAxis(device)));
	axis->SetActionAnalog(pInstance.GetOxr().GetAction(deVROpenXR::eiaThumbrestPress));
	axis->SetType(deInputDeviceAxis::eatThumbrestPress);
	axis->SetRange(0.0f, 1.0f);
	axis->SetCenter(-1.0f);
	axis->SetValue(-1.0f);
	axis->SetName("Thumbrest Press");
	axis->SetID("trestpr");
	axis->SetDisplayText("ThRPr");
	axis->SetIndex(device.GetAxisCount());
	axis->SetInputDeviceComponent(component);
	device.AddAxis(axis);
}

deoxrDeviceComponent *deoxrDeviceProfile::pAddComponentGrip( deoxrDevice &device ){
	return pAddComponent( device, deInputDeviceComponent::ectGeneric, "Grip", "grip", "Grip" );
}

void deoxrDeviceProfile::pAddAxisGripGrab( deoxrDevice &device, deoxrDeviceComponent *component ){
	const deoxrDeviceAxis::Ref axis( deoxrDeviceAxis::Ref::New( new deoxrDeviceAxis( device ) ) );
	axis->SetActionAnalog( pInstance.GetOxr().GetAction( deVROpenXR::eiaGripGrab ) );
	axis->SetType( deInputDeviceAxis::eatGripGrab );
	axis->SetRange( 0.0f, 1.0f );
	axis->SetCenter( -1.0f );
	axis->SetValue( -1.0f );
	axis->SetName( "Grab" );
	axis->SetID( "gg" );
	axis->SetDisplayText( "Grab" );
	axis->SetIndex( device.GetAxisCount() );
	axis->SetInputDeviceComponent( component );
	device.AddAxis( axis );
}

void deoxrDeviceProfile::pAddAxisGripSqueeze( deoxrDevice &device, deoxrDeviceComponent *component ){
	const deoxrDeviceAxis::Ref axis( deoxrDeviceAxis::Ref::New( new deoxrDeviceAxis( device ) ) );
	axis->SetActionAnalog( pInstance.GetOxr().GetAction( deVROpenXR::eiaGripSqueeze ) );
	axis->SetType( deInputDeviceAxis::eatGripSqueeze );
	axis->SetRange( 0.0f, 1.0f );
	axis->SetCenter( -1.0f );
	axis->SetValue( -1.0f );
	axis->SetName( "Squeeze" );
	axis->SetID( "gs" );
	axis->SetDisplayText( "Squ" );
	axis->SetIndex( device.GetAxisCount() );
	axis->SetInputDeviceComponent( component );
	device.AddAxis( axis );
}

void deoxrDeviceProfile::pAddAxisGripPinch( deoxrDevice &device, deoxrDeviceComponent *component ){
	const deoxrDeviceAxis::Ref axis( deoxrDeviceAxis::Ref::New( new deoxrDeviceAxis( device ) ) );
	axis->SetActionAnalog( pInstance.GetOxr().GetAction( deVROpenXR::eiaGripSqueeze ) );
	axis->SetType( deInputDeviceAxis::eatGripPinch );
	axis->SetRange( 0.0f, 1.0f );
	axis->SetCenter( -1.0f );
	axis->SetValue( -1.0f );
	axis->SetName( "Pinch" );
	axis->SetID( "gp" );
	axis->SetDisplayText( "Pin" );
	axis->SetIndex( device.GetAxisCount() );
	axis->SetInputDeviceComponent( component );
	device.AddAxis( axis );
}

void deoxrDeviceProfile::pAddButtonGrip( deoxrDevice &device, deoxrDeviceComponent *component, bool withTouch ){
	const deoxrDeviceButton::Ref button( deoxrDeviceButton::Ref::New( new deoxrDeviceButton( device ) ) );
	button->SetID( "grip" );
	button->SetName( "Grip" );
	button->SetType( deInputDeviceButton::ebtTrigger );
	button->SetDisplayText( "Grip" );
	button->SetInputDeviceComponent( component );
	button->SetActionPress( pInstance.GetOxr().GetAction( deVROpenXR::eiaGripPress ) );
	if( withTouch ){
		button->SetActionTouch( pInstance.GetOxr().GetAction( deVROpenXR::eiaGripTouch ) );
	}
	button->SetIndex( device.GetButtonCount() );
	device.AddButton( button );
}

void deoxrDeviceProfile::pAddButton( deoxrDevice &device, eButtonAction action,
eButtonLabel label, bool withTouch ){
	deoxrDeviceComponent * const component = pAddComponent( device, deInputDeviceComponent::ectButton,
		pButtonName( label ), pButtonId( label ), pButtonName( label ) );
	
	const deoxrDeviceButton::Ref button( deoxrDeviceButton::Ref::New( new deoxrDeviceButton( device ) ) );
	button->SetID( pButtonId( label ) );
	button->SetName( pButtonName( label ) );
	button->SetDisplayText( pButtonDisplayText( label ) );
	button->SetType( deInputDeviceButton::ebtAction );
	button->SetInputDeviceComponent( component );
	
	switch( action ){
	case ebaPrimary:
		button->SetActionPress(pInstance.GetOxr().GetAction(deVROpenXR::eiaButtonPrimaryPress));
		if(withTouch){
			button->SetActionTouch(pInstance.GetOxr().GetAction(deVROpenXR::eiaButtonPrimaryTouch));
		}
		//button->SetFinger(1);
		break;
		
	case ebaSecondary:
		button->SetActionPress(pInstance.GetOxr().GetAction(deVROpenXR::eiaButtonSecondaryPress));
		if(withTouch){
			button->SetActionTouch(pInstance.GetOxr().GetAction(deVROpenXR::eiaButtonSecondaryTouch));
		}
		//button->SetFinger(2);
		break;
		
	case ebaAuxiliary1:
		button->SetActionPress(pInstance.GetOxr().GetAction(deVROpenXR::eiaButtonAuxiliary1Press));
		if(withTouch){
			button->SetActionTouch(pInstance.GetOxr().GetAction(deVROpenXR::eiaButtonAuxiliary1Touch));
		}
		//button->SetFinger(3);
		break;
		
	case ebaAuxiliary2:
		button->SetActionPress(pInstance.GetOxr().GetAction(deVROpenXR::eiaButtonAuxiliary2Press));
		if(withTouch){
			button->SetActionTouch(pInstance.GetOxr().GetAction(deVROpenXR::eiaButtonAuxiliary2Touch));
		}
		//button->SetFinger(3);
		break;
	}
	
	button->SetIndex( device.GetButtonCount() );
	device.AddButton( button );
}

const char *deoxrDeviceProfile::pButtonName( eButtonLabel label ) const{
	switch( label ){
	case eblA:
		return "A";
		
	case eblB:
		return "B";
		
	case eblX:
		return "X";
		
	case eblY:
		return "Y";
		
	case eblHome:
		return "Home";
		
	case eblSystem:
		return "System";
		
	case eblShoulder:
		return "Shoulder";
		
	case eblBack:
		return "Back";
		
	case eblVolumeUp:
		return "Volume Up";
		
	case eblVolumeDown:
		return "Volume Down";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

const char *deoxrDeviceProfile::pButtonId( eButtonLabel label ) const{
	switch( label ){
	case eblA:
		return "a";
		
	case eblB:
		return "b";
		
	case eblX:
		return "x";
		
	case eblY:
		return "y";
		
	case eblHome:
		return "home";
		
	case eblSystem:
		return "system";
		
	case eblShoulder:
		return "shoulder";
		
	case eblBack:
		return "back";
		
	case eblVolumeUp:
		return "volup";
		
	case eblVolumeDown:
		return "voldown";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

const char *deoxrDeviceProfile::pButtonDisplayText( eButtonLabel label ) const{
	switch( label ){
	case eblA:
		return "A";
		
	case eblB:
		return "B";
		
	case eblX:
		return "X";
		
	case eblY:
		return "Y";
		
	case eblHome:
		return "Home";
		
	case eblSystem:
		return "Sys";
		
	case eblShoulder:
		return "Sho";
		
	case eblBack:
		return "Back";
		
	case eblVolumeUp:
		return "VUp";
		
	case eblVolumeDown:
		return "VDn";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoxrHandTracker *deoxrDeviceProfile::pAddHandTracker(deoxrDevice &device, bool leftHand,
bool withInputSimulation){
	if( ! pInstance.SupportsExtension( deoxrInstance::extEXTHandTracking ) ){
		return nullptr;
	}
	
	deoxrSession &session = pInstance.GetOxr().GetSession();
	if( ! session.GetSystem().GetSupportsHandTracking() ){
		return nullptr;
	}
	
	const deoxrHandTracker::Ref handTracker( deoxrHandTracker::Ref::New(
		new deoxrHandTracker( session, device, leftHand ? XR_HAND_LEFT_EXT : XR_HAND_RIGHT_EXT ) ) );
	
	device.SetHandTracker( handTracker );
	device.SetBoneConfiguration( deInputDevice::ebcHand );
	
	// finger bending and spreading
	deoxrDeviceComponent * const componentHand = device.AddComponent(
		deInputDeviceComponent::ectGeneric, "Hand Pose", "handPose", "Hand Pose" );
	
	struct Data{
		const char *name;
		const char *id;
		const char *displayText;
	};
	const Data bendData[ 5 ] = {
		{ "Bend Thumb", "fb1", "FB1" },
		{ "Bend Index Finger", "fb2", "FB2" },
		{ "Bend Middle Finger", "fb3", "FB3" },
		{ "Bend Ring Finger", "fb4", "FB4" },
		{ "Bend Pinky Finger", "fb5", "FB5" } };
	int i;
	
	for( i=0; i<5; i++ ){
		const deoxrDeviceAxis::Ref axis( deoxrDeviceAxis::Ref::New( new deoxrDeviceAxis( device ) ) );
		axis->SetType( deInputDeviceAxis::eatFingerBend );
		axis->SetRange( 0.0f, 1.0f );
		axis->SetCenter( -1.0f );
		axis->SetValue( -1.0f );
		axis->SetName( bendData[ i ].name );
		axis->SetID( bendData[ i ].id );
		axis->SetDisplayText( bendData[ i ].displayText );
		axis->SetIndex( device.GetAxisCount() );
		axis->SetFinger( i );
		axis->SetInputDeviceComponent( componentHand );
		device.AddAxis( axis );
	}
	
	const Data bendSpread[ 4 ] = {
		{ "Spread Thumb Index Finger", "fs1", "FS1" },
		{ "Spread Index Middle Finger", "fs2", "FS2" },
		{ "Spread Middle Ring Finger", "fs3", "FS3" },
		{ "Spread Ring Pinky Finger", "fs4", "FS4" } };
	
	for( i=0; i<4; i++ ){
		const deoxrDeviceAxis::Ref axis( deoxrDeviceAxis::Ref::New( new deoxrDeviceAxis( device ) ) );
		axis->SetType( deInputDeviceAxis::eatFingerSpread );
		axis->SetRange( 0.0f, 1.0f );
		axis->SetCenter( -1.0f );
		axis->SetValue( -1.0f );
		axis->SetName( bendSpread[ i ].name );
		axis->SetID( bendSpread[ i ].id );
		axis->SetDisplayText( bendSpread[ i ].displayText );
		axis->SetIndex( device.GetAxisCount() );
		axis->SetFinger( i );
		axis->SetInputDeviceComponent( componentHand );
		device.AddAxis( axis );
	}
	
	// two finger input
	if(withInputSimulation){
		const Data tfiData[4] = {
			{"Input Index Finger", "tfi1", "FI1" },
			{"Input Middle Finger", "tfi2", "FI2" },
			{"Input Ring Finger", "tfi3", "FI3" },
			{"Input Pinky Finger", "tfi4", "FI4" }};
		
		for(i=0; i<4; i++){
			const deoxrDeviceAxis::Ref axis(deoxrDeviceAxis::Ref::New(new deoxrDeviceAxis(device)));
			axis->SetType(deInputDeviceAxis::eatTwoFingerTrigger);
			axis->SetRange(0.0f, 1.0f);
			axis->SetCenter(-1.0f);
			axis->SetValue(-1.0f);
			axis->SetName(tfiData[i].name);
			axis->SetID(tfiData[i].id);
			axis->SetDisplayText(tfiData[i].displayText);
			axis->SetFinger(i);
			axis->SetIndex(device.GetAxisCount());
			axis->SetInputDeviceComponent(componentHand);
			device.AddAxis(axis);
			
			const deoxrDeviceButton::Ref button(
				deoxrDeviceButton::Ref::New(new deoxrDeviceButton(device)));
			button->SetType(deInputDeviceButton::ebtTwoFingerTrigger);
			button->SetName(tfiData[i].name);
			button->SetID(tfiData[i].id);
			button->SetDisplayText(tfiData[i].displayText);
			button->SetFinger(i);
			button->SetIndex(device.GetButtonCount());
			button->SetInputDeviceComponent(componentHand);
			device.AddButton(button);
		}
		
		device.SetEnableTwoFingerTriggerSimulation(true);
		
	}else{
		device.SetEnableTwoFingerTriggerSimulation(false);
	}
	
	return handTracker;
}
