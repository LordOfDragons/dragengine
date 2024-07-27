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
	pAddAxisGripGrab( device, grip );
	pAddButtonGrip( device, grip, false );
	
	pAddHandTracker( device, left );
	
	GetInstance().GetOxr().GetDevices().Add( device );
}
