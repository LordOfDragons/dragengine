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

#include "deoxrDPHTCViveFocus3ControllerInteraction.h"
#include "../deoxrDeviceAxis.h"
#include "../deoxrDeviceButton.h"
#include "../deoxrDeviceComponent.h"
#include "../deoxrDeviceFeedback.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPHTCViveFocus3ControllerInteraction
////////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPHTCViveFocus3ControllerInteraction::deoxrDPHTCViveFocus3ControllerInteraction( deoxrInstance &instance ) :
deoxrDPBaseTwoHandController( instance,
	deoxrPath( instance, "/interaction_profiles/htc/vive_focus3_controller" ),
	"HTC Vive Focus3 Controller" )
{
}

deoxrDPHTCViveFocus3ControllerInteraction::~deoxrDPHTCViveFocus3ControllerInteraction(){
}



// Private Functions
//////////////////////

void deoxrDPHTCViveFocus3ControllerInteraction::pSuggestBindings(){
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// On /user/hand/left only:
	// - /input/x/click
	// - /input/y/click
	// - /input/menu/click
	// 
	// On /user/hand/right only:
	// - /input/a/click
	// - /input/b/click
	// - /input/system/click (may not be available for application use)
	// 
	// On Both Hands:
	// - /input/squeeze/click
	// - /input/squeeze/touch
	// - /input/trigger/click
	// - /input/trigger/touch
	// - /input/trigger/value
	// - /input/thumbstick/x
	// - /input/thumbstick/y
	// - /input/thumbstick/click
	// - /input/thumbstick/touch
	// - /input/thumbrest/touch
	// - /input/grip/pose
	// - /input/aim/pose
	// - /output/haptic
	
	const int bindingCount = 11 * 2 + 3 + 3;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[ 2 ] = { "/user/hand/left", "/user/hand/right" };
	int i;
	
	// both hands
	for( i=0; i<2; i++ ){
		const decString &basePath = basePathList[ i ];
		
		pAdd( b, pGripPoseAction( i == 0 ), basePath + "/input/aim/pose" );
		
		pAdd( b, deVROpenXR::eiaGripPress, basePath + "/input/squeeze/click" );
		pAdd( b, deVROpenXR::eiaGripTouch, basePath + "/input/squeeze/touch" );
		
		pAdd( b, deVROpenXR::eiaTriggerPress, basePath + "/input/trigger/click" );
		pAdd( b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/trigger/value" );
		pAdd( b, deVROpenXR::eiaTriggerTouch, basePath + "/input/trigger/touch" );
		
		pAdd( b, deVROpenXR::eiaJoystickAnalog, basePath + "/input/thumbstick" );
		pAdd( b, deVROpenXR::eiaJoystickPress, basePath + "/input/thumbstick/click" );
		pAdd( b, deVROpenXR::eiaJoystickTouch, basePath + "/input/thumbstick/touch" );
		
		pAdd( b, deVROpenXR::eiaThumbrestTouch, basePath + "/input/thumbrest/touch" );
		
		pAdd( b, deVROpenXR::eiaGripHaptic, basePath + "/output/haptic" );
	}
	
	// left hand
	pAdd( b, deVROpenXR::eiaButtonPrimaryPress, "/user/hand/left/input/x/click" );
	pAdd( b, deVROpenXR::eiaButtonSecondaryPress, "/user/hand/left/input/y/click" );
	pAdd( b, deVROpenXR::eiaButtonAuxiliary1Press, "/user/hand/left/input/menu/click" );
	
	// right hand
	pAdd( b, deVROpenXR::eiaButtonPrimaryPress, "/user/hand/right/input/a/click" );
	pAdd( b, deVROpenXR::eiaButtonSecondaryPress, "/user/hand/right/input/b/click" );
	pAdd( b, deVROpenXR::eiaButtonAuxiliary1Press, "/user/hand/right/input/system/click" );
	
	
	GetInstance().SuggestBindings( GetPath(), bindings, bindingCount );
}

bool deoxrDPHTCViveFocus3ControllerInteraction::pProfileEnabled() const{
	return GetInstance().SupportsExtension( deoxrInstance::extHTCViveFocus3ControllerInteraction );
}

void deoxrDPHTCViveFocus3ControllerInteraction::pAddDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if( device ){
		return;
	}
	
	pCreateDevice( device, left, "htcvfc_" );
	
	deoxrDeviceComponent * const trigger = pAddComponentTrigger( device );
	pAddAxisTrigger( device, trigger );
	pAddButtonTrigger( device, trigger, true ); // has to be button 0
	
	if( left ){
		pAddButton( device, ebaPrimary, eblX, false ); // has to be button 1
		pAddButton( device, ebaSecondary, eblY, false ); // has to be button 2
		pAddButton( device, ebaAuxiliary1, eblHome, false ); // has to be button 3
		
	}else{
		pAddButton( device, ebaPrimary, eblA, false ); // has to be button 1
		pAddButton( device, ebaSecondary, eblB, false ); // has to be button 2
		pAddButton( device, ebaAuxiliary1, eblSystem, false ); // has to be button 3
	}
	
	deoxrDeviceComponent * const trackpad = pAddComponentTrackpad( device );
	pAddAxesTrackpad( device, trackpad );
	pAddButtonTrackpad( device, trackpad, true, true );
	
	deoxrDeviceComponent * const thumbrest = pAddComponentThumbrest( device );
	pAddButtonThumbrest( device, thumbrest );
	
	deoxrDeviceComponent * const grip = pAddComponentGrip( device );
	pAddButtonGrip( device, grip, true );
	
	pAddHandTracker( device, left );
	
	GetInstance().GetOxr().GetDevices().Add( device );
}
