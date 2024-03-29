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

#include "deoxrDPMSFTHandInteraction.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPMSFTHandInteraction
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPMSFTHandInteraction::deoxrDPMSFTHandInteraction( deoxrInstance &instance ) :
deoxrDPBaseTwoHandController( instance,
	deoxrPath( instance, "/interaction_profiles/microsoft/hand_interaction" ),
	"MSFT Hand Controller" )
{
}

deoxrDPMSFTHandInteraction::~deoxrDPMSFTHandInteraction(){
}



// Private Functions
//////////////////////

void deoxrDPMSFTHandInteraction::pSuggestBindings(){
	// Valid for top level user path:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/select/value
	// - /input/squeeze/value
	// - /input/aim/pose
	// - /input/grip/pose
	
	const int bindingCount = 5 * 2;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[ 2 ] = { "/user/hand/left", "/user/hand/right" };
	int i;
	
	for( i=0; i<2; i++ ){
		const decString &basePath = basePathList[ i ];
		
		pAdd( b, pGripPoseAction( i == 0 ), basePath + "/input/aim/pose" );
		
		pAdd( b, deVROpenXR::eiaGripPress, basePath + "/input/squeeze/value" );
		pAdd( b, deVROpenXR::eiaGripGrab, basePath + "/input/squeeze/value" );
		
		pAdd( b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/select/value" );
		pAdd( b, deVROpenXR::eiaTriggerPress, basePath + "/input/select/value" );
	}
	
	
	GetInstance().SuggestBindings( GetPath(), bindings, bindingCount );
}

bool deoxrDPMSFTHandInteraction::pProfileEnabled() const{
	return GetInstance().SupportsExtension( deoxrInstance::extMSFTHandInteraction );
}

void deoxrDPMSFTHandInteraction::pAddDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if( device ){
		return;
	}
	
	pCreateDevice( device, left, "msfthc_" );
	
	deoxrDeviceComponent * const trigger = pAddComponentTrigger( device );
	pAddAxisTrigger( device, trigger );
	pAddButtonTrigger( device, trigger, false ); // has to be button 0
	
	deoxrDeviceComponent * const grip = pAddComponentGrip( device );
	pAddAxesGripGrab( device, grip );
	pAddButtonGrip( device, grip, false );
	
	pAddHandTracker( device, left );
	
	GetInstance().GetOxr().GetDevices().Add( device );
}
