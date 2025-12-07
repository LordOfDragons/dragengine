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

#include "deoxrDPHandInteraction.h"
#include "../deoxrDeviceAxis.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPHandInteraction
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPHandInteraction::deoxrDPHandInteraction( deoxrInstance &instance ) :
deoxrDPBaseTwoHandController( instance,
	deoxrPath( instance, "/interaction_profiles/ext/hand_interaction_ext" ),
	"Hand Interaction" )
{
	SetDeviceRotation( decVector( DEG2RAD * -90.0f, 0.0f, 0.0f ) );
}

deoxrDPHandInteraction::~deoxrDPHandInteraction(){
}


// Private Functions
//////////////////////

bool deoxrDPHandInteraction::pProfileEnabled() const{
	return GetInstance().SupportsExtension(deoxrInstance::extEXTHandInteraction);
}

void deoxrDPHandInteraction::pSuggestBindings(){
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/aim/pose
	// - /input/grip/pose
	// - /input/pinch_ext/pose
	// - /input/poke_ext/pose
	// - /input/pinch_ext/value
	// - /input/pinch_ext/ready_ext
	// - /input/aim_activate_ext/value
	// - /input/aim_activate_ext/ready_ext
	// - /input/grasp_ext/value
	// - /input/grasp_ext/ready_ext
	
	const int bindingCountPerHand = 10;
	const int bindingCount = bindingCountPerHand * 2;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	int usedBindingCount = 0;
	
	const deoxrPath basePathList[ 2 ] = { pPathHandLeft, pPathHandRight };
	int i;
	
	for( i=0; i<2; i++ ){
		const decString &basePath = basePathList[ i ];
		
		pAdd( b, pPoseAction( i == 0 ), basePath + "/input/grip/pose" );
		
		pAdd( b, deVROpenXR::eiaTriggerPress, basePath + "/input/pinch_ext/value" );
		pAdd( b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/pinch_ext/value" );
		pAdd( b, deVROpenXR::eiaTriggerTouch, basePath + "/input/pinch_ext/ready_ext" );
		
		pAdd( b, deVROpenXR::eiaGripPress, basePath + "/input/grasp_ext/value" );
		pAdd( b, deVROpenXR::eiaGripGrab, basePath + "/input/grasp_ext/value" );
		pAdd( b, deVROpenXR::eiaGripTouch, basePath + "/input/grasp_ext/ready_ext" );
		
		pAdd( b, deVROpenXR::eiaGesturePinch, basePath + "/input/pinch_ext/value" );
		pAdd( b, deVROpenXR::eiaGestureAim, basePath + "/input/aim_activate_ext/value" );
		pAdd( b, deVROpenXR::eiaGestureGrasp, basePath + "/input/grasp_ext/value" );

		usedBindingCount += bindingCountPerHand;
	}
	
	if( usedBindingCount > 0 ){
		GetInstance().SuggestBindings( GetPath(), bindings, usedBindingCount );
	}
}

void deoxrDPHandInteraction::pAddDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if( device ){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	pCreateDevice( device, left, "hi_", false );
	
	// controller simulation
	deoxrDeviceComponent * const trigger = pAddComponentTrigger( device );
	pAddAxisTrigger( device, trigger );
	pAddButtonTrigger( device, trigger, true ); // has to be button 0
	
	deoxrDeviceComponent * const grip = pAddComponentGrip( device );
	pAddAxisGripGrab( device, grip );
	pAddButtonGrip( device, grip, true );
	
	// gestures
	deoxrDeviceComponent * const gesture = device->AddComponent(
		deInputDeviceComponent::ectGesture, "Hand Gesture", "handGesture", "Hand Gesture" );
	
	deoxrDeviceAxis::Ref axis( deoxrDeviceAxis::Ref::NewWith(device) );
	axis->SetActionAnalog( oxr.GetAction( deVROpenXR::eiaGesturePinch ) );
	axis->SetType( deInputDeviceAxis::eatGesture );
	axis->SetRange( 0.0f, 1.0f );
	axis->SetCenter( -1.0f );
	axis->SetValue( -1.0f );
	axis->SetName( "Pinch" );
	axis->SetID( "ghpinch" );
	axis->SetDisplayText( "Pinch" );
	axis->SetIndex( device->GetAxisCount() );
	axis->SetInputDeviceComponent( gesture );
	device->AddAxis( axis );
	
	axis.TakeOver( new deoxrDeviceAxis( device ) );
	axis->SetActionAnalog( oxr.GetAction( deVROpenXR::eiaGestureAim ) );
	axis->SetType( deInputDeviceAxis::eatGesture );
	axis->SetRange( 0.0f, 1.0f );
	axis->SetCenter( -1.0f );
	axis->SetValue( -1.0f );
	axis->SetName( "Aim" );
	axis->SetID( "ghaim" );
	axis->SetDisplayText( "Aim" );
	axis->SetIndex( device->GetAxisCount() );
	axis->SetInputDeviceComponent( gesture );
	device->AddAxis( axis );
	
	axis.TakeOver( new deoxrDeviceAxis( device ) );
	axis->SetActionAnalog( oxr.GetAction( deVROpenXR::eiaGestureGrasp ) );
	axis->SetType( deInputDeviceAxis::eatGesture );
	axis->SetRange( 0.0f, 1.0f );
	axis->SetCenter( -1.0f );
	axis->SetValue( -1.0f );
	axis->SetName( "Grasp" );
	axis->SetID( "ghgrasp" );
	axis->SetDisplayText( "Grasp" );
	axis->SetIndex( device->GetAxisCount() );
	axis->SetInputDeviceComponent( gesture );
	device->AddAxis( axis );
	
	// hand tracking
	pAddHandTracker(device, left, true);
	device->SetEnableTwoFingerTriggerSimulation(false);
	
	oxr.GetDevices().Add( device );
}
