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
deoxrDPBaseTwoHandController( instance,
	deoxrPath( instance, "/interaction_profiles/valve/index_controller" ),
	"Valve Index Controller" )
{
}

deoxrDPValveIndexController::~deoxrDPValveIndexController(){
}



// Private Functions
//////////////////////

void deoxrDPValveIndexController::pSuggestBindings(){
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
	
	const int bindingCount = 18 * 2;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[ 2 ] = { "/user/hand/left", "/user/hand/right" };
	int i;
	
	for( i=0; i<2; i++ ){
		const decString &basePath = basePathList[ i ];
		
		pAdd( b, pGripPoseAction( i == 0 ), basePath + "/input/aim/pose" );
		
		pAdd( b, deVROpenXR::eiaGripPress, basePath + "/input/squeeze/value" );
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

void deoxrDPValveIndexController::pAddDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if( device ){
		return;
	}
	
	pCreateDevice( device, left, "ic_" );
	
	deoxrDeviceComponent * const trigger = pAddComponentTrigger( device );
	pAddAxisTrigger( device, trigger );
	pAddButtonTrigger( device, trigger, true ); // has to be button 0
	
	pAddButton( device, ebaPrimary, eblA, true ); // has to be button 1
	pAddButton( device, ebaSecondary, eblB, true ); // has to be button 2
	
	deoxrDeviceComponent * const joystick = pAddComponentJoystick( device );
	pAddAxesJoystick( device, joystick );
	pAddButtonJoystick( device, joystick, true, true );
	
	deoxrDeviceComponent * const trackpad = pAddComponentTrackpad( device );
	pAddAxesTrackpad( device, trackpad );
	pAddButtonTrackpad( device, trackpad, true, true );
	
	deoxrDeviceComponent * const grip = pAddComponentGrip( device );
	pAddAxisGripGrab( device, grip );
	pAddAxisGripSqueeze( device, grip );
// 	pAddAxesGripPinch( device, grip );
	pAddButtonGrip( device, grip, false );
	
	pAddHandTracker( device, left );
	
	GetInstance().GetOxr().GetDevices().Add( device );
}
