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

#include "deoxrDPValveIndexController.h"
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
}



// Management
///////////////

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
	
	const int bindingCount = 16 * 2;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[ 2 ] = { "/user/hand/left", "/user/hand/right" };
	int i;
	
	for( i=0; i<2; i++ ){
		const decString &basePath = basePathList[ i ];
		
		pAdd( b, deVROpenXR::eiaPose, basePath + "/input/aim/pose" );
		
		pAdd( b, deVROpenXR::eiaGripSqueeze, basePath + "/input/squeeze/value" );
		
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
