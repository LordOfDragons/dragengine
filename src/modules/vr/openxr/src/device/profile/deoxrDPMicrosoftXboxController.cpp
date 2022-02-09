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

#include "deoxrDPMicrosoftXboxController.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPMicrosoftXboxController
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPMicrosoftXboxController::deoxrDPMicrosoftXboxController( deoxrInstance &instance ) :
deoxrDeviceProfile( instance,
	deoxrPath( instance, "/interaction_profiles/microsoft/xbox_controller" ),
	"Microsoft XBox Controller" )
{
}

deoxrDPMicrosoftXboxController::~deoxrDPMicrosoftXboxController(){
}



// Management
///////////////

void deoxrDPMicrosoftXboxController::SuggestBindings(){
	// Valid for user paths:
	// - /user/gamepad
	// 
	// Supported component paths:
	// - /input/menu/click
	// - /input/view/click
	// - /input/a/click
	// - /input/b/click
	// - /input/x/click
	// - /input/y/click
	// - /input/dpad_down/click
	// - /input/dpad_right/click
	// - /input/dpad_up/click
	// - /input/dpad_left/click
	// - /input/shoulder_left/click
	// - /input/shoulder_right/click
	// - /input/thumbstick_left/click
	// - /input/thumbstick_right/click
	// - /input/trigger_left/value
	// - /input/trigger_right/value
	// - /input/thumbstick_left/x
	// - /input/thumbstick_left/y
	// - /input/thumbstick_right/x
	// - /input/thumbstick_right/y
	// - /output/haptic_left
	// - /output/haptic_right
	// - /output/haptic_left_trigger
	// - /output/haptic_right_trigger
	
	/*
	const int bindingCount = 5 * 2;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[ 2 ] = { "/user/hand/left", "/user/hand/right" };
	int i;
	
	for( i=0; i<2; i++ ){
		const decString &basePath = basePathList[ i ];
		
		pAdd( b, deVROpenXR::eiaPose, basePath + "/input/aim/pose" );
		
		pAdd( b, deVROpenXR::eiaGripSqueeze, basePath + "/input/squeeze/click" );
		
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
	*/
}
