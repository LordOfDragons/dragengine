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

#include "deoxrDPHTCViveCosmosControllerInteraction.h"
#include "../deoxrDeviceAxis.h"
#include "../deoxrDeviceButton.h"
#include "../deoxrDeviceComponent.h"
#include "../deoxrDeviceFeedback.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPHTCViveCosmosControllerInteraction
////////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPHTCViveCosmosControllerInteraction::deoxrDPHTCViveCosmosControllerInteraction( deoxrInstance &instance ) :
deoxrDPBaseTwoHandController( instance,
	deoxrPath( instance, "/interaction_profiles/htc/vive_cosmos_controller" ),
	"HTC Vive Cosmos Controller" )
{
}

deoxrDPHTCViveCosmosControllerInteraction::~deoxrDPHTCViveCosmosControllerInteraction(){
}


// Private Functions
//////////////////////

void deoxrDPHTCViveCosmosControllerInteraction::pSuggestBindings(){
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
	// - /input/shoulder/click
	// - /input/squeeze/click
	// - /input/trigger/click
	// - /input/trigger/value
	// - /input/thumbstick/x
	// - /input/thumbstick/y
	// - /input/thumbstick/click
	// - /input/thumbstick/touch
	// - /input/grip/pose
	// - /input/aim/pose
	// - /output/haptic
	
	const int bindingCount = 9 * 2 + 3 + 3;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[ 2 ] = { "/user/hand/left", "/user/hand/right" };
	int i;
	
	// both hands
	for( i=0; i<2; i++ ){
		const decString &basePath = basePathList[ i ];
		
		pAdd( b, pGripPoseAction( i == 0 ), basePath + "/input/aim/pose" );
		
		pAdd( b, deVROpenXR::eiaGripPress, basePath + "/input/squeeze/click" );
		
		pAdd( b, deVROpenXR::eiaButtonAuxiliary1Press, basePath + "/input/shoulder/click" );
		
		pAdd( b, deVROpenXR::eiaTriggerPress, basePath + "/input/trigger/click" );
		pAdd( b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/trigger/value" );
		
		pAdd( b, deVROpenXR::eiaJoystickAnalog, basePath + "/input/thumbstick" );
		pAdd( b, deVROpenXR::eiaJoystickPress, basePath + "/input/thumbstick/click" );
		pAdd( b, deVROpenXR::eiaJoystickTouch, basePath + "/input/thumbstick/touch" );
		
		pAdd( b, deVROpenXR::eiaGripHaptic, basePath + "/output/haptic" );
	}
	
	// left hand
	pAdd( b, deVROpenXR::eiaButtonPrimaryPress, "/user/hand/left/input/x/click" );
	pAdd( b, deVROpenXR::eiaButtonSecondaryPress, "/user/hand/left/input/y/click" );
	pAdd( b, deVROpenXR::eiaButtonAuxiliary2Press, "/user/hand/left/input/menu/click" );
	
	// right hand
	pAdd( b, deVROpenXR::eiaButtonPrimaryPress, "/user/hand/right/input/a/click" );
	pAdd( b, deVROpenXR::eiaButtonSecondaryPress, "/user/hand/right/input/b/click" );
	pAdd( b, deVROpenXR::eiaButtonAuxiliary2Press, "/user/hand/right/input/system/click" );
	
	
	GetInstance().SuggestBindings( GetPath(), bindings, bindingCount );
}

bool deoxrDPHTCViveCosmosControllerInteraction::pProfileEnabled() const{
	return GetInstance().SupportsExtension( deoxrInstance::extHTCViveCosmosControllerInteraction );
}

void deoxrDPHTCViveCosmosControllerInteraction::pAddDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if( device ){
		return;
	}
	
	pCreateDevice( device, left, "htcvcc_" );
	
	deoxrDeviceComponent * const trigger = pAddComponentTrigger( device );
	pAddAxisTrigger( device, trigger );
	pAddButtonTrigger( device, trigger, false ); // has to be button 0
	
	if( left ){
		pAddButton( device, ebaPrimary, eblX, false ); // has to be button 1
		pAddButton( device, ebaSecondary, eblY, false ); // has to be button 2
		pAddButton( device, ebaAuxiliary1, eblShoulder, false ); // has to be button 3
		pAddButton( device, ebaAuxiliary2, eblHome, false ); // has to be button 4
		
	}else{
		pAddButton( device, ebaPrimary, eblA, false ); // has to be button 1
		pAddButton( device, ebaSecondary, eblB, false ); // has to be button 2
		pAddButton( device, ebaAuxiliary1, eblShoulder, false ); // has to be button 3
		pAddButton( device, ebaAuxiliary2, eblSystem, false ); // has to be button 4
	}
	
	deoxrDeviceComponent * const joystick = pAddComponentJoystick( device );
	pAddAxesJoystick( device, joystick );
	pAddButtonJoystick( device, joystick, true, true );
	
	deoxrDeviceComponent * const grip = pAddComponentGrip( device );
	pAddButtonGrip( device, grip, false );
	
	// add device
	GetInstance().GetOxr().GetDevices().Add( device );
}
