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

#include "deoxrDPOculusGoController.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPOculusGoController
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPOculusGoController::deoxrDPOculusGoController( deoxrInstance &instance ) :
deoxrDPBaseTwoHandController( instance,
	deoxrPath( instance, "/interaction_profiles/oculus/go_controller" ),
	"Oculus Go Controller" )
{
}

deoxrDPOculusGoController::~deoxrDPOculusGoController(){
}



// Private Functions
//////////////////////

void deoxrDPOculusGoController::pSuggestBindings(){
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/system/click (may not be available for application use)
	// - /input/trigger/click
	// - /input/back/click
	// - /input/trackpad/x
	// - /input/trackpad/y
	// - /input/trackpad/click
	// - /input/trackpad/touch
	// - /input/grip/pose
	// - /input/aim/pose
	
	const int bindingCount = 8 * 2;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[ 2 ] = { "/user/hand/left", "/user/hand/right" };
	int i;
	
	for( i=0; i<2; i++ ){
		const decString &basePath = basePathList[ i ];
		
		pAdd( b, pPoseAction( i == 0 ), basePath + "/input/grip/pose" );
		pAdd( b, pPoseAction2( i == 0 ), basePath + "/input/aim/pose" );
		
		pAdd( b, deVROpenXR::eiaTriggerPress, basePath + "/input/trigger/click" );
		
		pAdd( b, deVROpenXR::eiaButtonPrimaryPress, basePath + "/input/back/click" );
		
		pAdd( b, deVROpenXR::eiaButtonSecondaryPress, basePath + "/input/system/click" );
		
		pAdd( b, deVROpenXR::eiaTrackpadAnalog, basePath + "/input/trackpad" );
		pAdd( b, deVROpenXR::eiaTrackpadPress, basePath + "/input/trackpad/click" );
		pAdd( b, deVROpenXR::eiaTrackpadTouch, basePath + "/input/trackpad/touch" );
	}
	
	
	GetInstance().SuggestBindings( GetPath(), bindings, bindingCount );
}

void deoxrDPOculusGoController::pAddDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if( device ){
		return;
	}
	
	pCreateDevice( device, left, "ogc_", true );
	
	deoxrDeviceComponent * const trigger = pAddComponentTrigger( device );
	pAddButtonTrigger( device, trigger, false ); // has to be button 0
	
	pAddButton( device, ebaPrimary, eblBack, false ); // has to be button 1
	pAddButton( device, ebaSecondary, eblSystem, false ); // has to be button 2
	
	deoxrDeviceComponent * const trackpad = pAddComponentTrackpad( device );
	pAddAxesTrackpad( device, trackpad );
	pAddButtonTrackpad( device, trackpad, true, true );
	
	pAddHandTracker( device, left );
	
	GetInstance().GetOxr().GetDevices().Add( device );
}
