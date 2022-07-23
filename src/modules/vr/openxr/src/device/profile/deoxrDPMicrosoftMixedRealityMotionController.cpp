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

#include "deoxrDPMicrosoftMixedRealityMotionController.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPMicrosoftMixedRealityMotionController
///////////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPMicrosoftMixedRealityMotionController::deoxrDPMicrosoftMixedRealityMotionController( deoxrInstance &instance ) :
deoxrDPBaseTwoHandController( instance,
	deoxrPath( instance, "/interaction_profiles/microsoft/motion_controller" ),
	"Microsoft Mixed Reality Motion Controller" )
{
}

deoxrDPMicrosoftMixedRealityMotionController::deoxrDPMicrosoftMixedRealityMotionController(
	deoxrInstance &instance, const deoxrPath &path, const char *name ) :
deoxrDPBaseTwoHandController( instance, path, name ){
}

deoxrDPMicrosoftMixedRealityMotionController::~deoxrDPMicrosoftMixedRealityMotionController(){
}



// Private Functions
//////////////////////

void deoxrDPMicrosoftMixedRealityMotionController::pSuggestBindings(){
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/menu/click
	// - /input/squeeze/click
	// - /input/trigger/value
	// - /input/thumbstick/x
	// - /input/thumbstick/y
	// - /input/thumbstick/click
	// - /input/trackpad/x
	// - /input/trackpad/y
	// - /input/trackpad/click
	// - /input/trackpad/touch
	// - /input/grip/pose
	// - /input/aim/pose
	// - /output/haptic
	
	const int bindingCount = 11 * 2;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[ 2 ] = { "/user/hand/left", "/user/hand/right" };
	int i;
	
	for( i=0; i<2; i++ ){
		const decString &basePath = basePathList[ i ];
		
		pAdd( b, pGripPoseAction( i == 0 ), basePath + "/input/aim/pose" );
		
		pAdd( b, deVROpenXR::eiaGripPress, basePath + "/input/squeeze/click" );
		
		pAdd( b, deVROpenXR::eiaTriggerPress, basePath + "/input/trigger/value" );
		pAdd( b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/trigger/value" );
		
		pAdd( b, deVROpenXR::eiaButtonPrimaryPress, basePath + "/input/menu/click" );
		
		pAdd( b, deVROpenXR::eiaJoystickAnalog, basePath + "/input/thumbstick" );
		pAdd( b, deVROpenXR::eiaJoystickPress, basePath + "/input/thumbstick/click" );
		
		pAdd( b, deVROpenXR::eiaTrackpadAnalog, basePath + "/input/trackpad" );
		pAdd( b, deVROpenXR::eiaTrackpadPress, basePath + "/input/trackpad/click" );
		pAdd( b, deVROpenXR::eiaTrackpadTouch, basePath + "/input/trackpad/touch" );
		
		pAdd( b, deVROpenXR::eiaGripHaptic, basePath + "/output/haptic" );
	}
	
	
	GetInstance().SuggestBindings( GetPath(), bindings, bindingCount );
}

const char *deoxrDPMicrosoftMixedRealityMotionController::pDeviceIdPrefix() const{
	return "mmrmc_";
}

void deoxrDPMicrosoftMixedRealityMotionController::pAddDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if( device ){
		return;
	}
	
	pCreateDevice( device, left, pDeviceIdPrefix() );
	
	deoxrDeviceComponent * const trigger = pAddComponentTrigger( device );
	pAddAxisTrigger( device, trigger );
	pAddButtonTrigger( device, trigger, false ); // has to be button 0
	
	pAddButton( device, ebaPrimary, eblHome, false ); // has to be button 1
	
	deoxrDeviceComponent * const joystick = pAddComponentJoystick( device );
	pAddAxesJoystick( device, joystick );
	pAddButtonJoystick( device, joystick, true, false );
	
	deoxrDeviceComponent * const trackpad = pAddComponentTrackpad( device );
	pAddAxesTrackpad( device, trackpad );
	pAddButtonTrackpad( device, trackpad, true, true );
	
	deoxrDeviceComponent * const grip = pAddComponentGrip( device );
	pAddButtonGrip( device, grip, false );
	
	pAddHandTracker( device, left );
	
	GetInstance().GetOxr().GetDevices().Add( device );
}
