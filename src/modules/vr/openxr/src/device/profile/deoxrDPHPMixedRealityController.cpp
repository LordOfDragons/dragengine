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

#include "deoxrDPHPMixedRealityController.h"
#include "../deoxrDeviceAxis.h"
#include "../deoxrDeviceButton.h"
#include "../deoxrDeviceComponent.h"
#include "../deoxrDeviceFeedback.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPHPMixedRealityController
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPHPMixedRealityController::deoxrDPHPMixedRealityController(deoxrInstance &instance) :
deoxrDPBaseTwoHandController(instance,
	deoxrPath(instance, "/interaction_profiles/hp/mixed_reality_controller"),
	"HP Mixed Reality Controller")
{
}

deoxrDPHPMixedRealityController::~deoxrDPHPMixedRealityController(){
}



// Private Functions
//////////////////////

void deoxrDPHPMixedRealityController::pSuggestBindings(){
	// Valid for the user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// On /user/hand/left only
	// - /input/x/click
	// - /input/y/click
	// 
	// On /user/hand/right only:
	// - /input/a/click
	// - /input/b/click
	// 
	// On both hands:
	// - /input/menu/click
	// - /input/squeeze/value
	// - /input/trigger/value
	// - /input/thumbstick/x
	// - /input/thumbstick/y
	// - /input/thumbstick/click
	// - /input/grip/pose
	// - /input/aim/pose
	// - /output/haptic
	
	const int bindingCount = 10 * 2 + 2 + 2;
	deoxrInstance::sSuggestBinding bindings[bindingCount];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[2] = {"/user/hand/left", "/user/hand/right"};
	int i;
	
	// both hands
	for(i=0; i<2; i++){
		const decString &basePath = basePathList[i];
		
		pAdd(b, pPoseAction(i == 0), basePath + "/input/grip/pose");
		pAdd(b, pPoseAction2(i == 0), basePath + "/input/aim/pose");
		
		pAdd(b, deVROpenXR::eiaGripPress, basePath + "/input/squeeze/value");
		pAdd(b, deVROpenXR::eiaGripGrab, basePath + "/input/squeeze/value");
		
		pAdd(b, deVROpenXR::eiaButtonAuxiliary1Press, basePath + "/input/menu/click");
		
		pAdd(b, deVROpenXR::eiaTriggerPress, basePath + "/input/trigger/value");
		pAdd(b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/trigger/value");
		
		pAdd(b, deVROpenXR::eiaJoystickAnalog, basePath + "/input/thumbstick");
		pAdd(b, deVROpenXR::eiaJoystickPress, basePath + "/input/thumbstick/click");
		
		pAdd(b, deVROpenXR::eiaGripHaptic, basePath + "/output/haptic");
	}
	
	// left hand
	pAdd(b, deVROpenXR::eiaButtonPrimaryPress, "/user/hand/left/input/x/click");
	pAdd(b, deVROpenXR::eiaButtonSecondaryPress, "/user/hand/left/input/y/click");
	
	// right hand
	pAdd(b, deVROpenXR::eiaButtonPrimaryPress, "/user/hand/right/input/a/click");
	pAdd(b, deVROpenXR::eiaButtonSecondaryPress, "/user/hand/right/input/b/click");
	
	
	GetInstance().SuggestBindings(GetPath(), bindings, bindingCount);
}

bool deoxrDPHPMixedRealityController::pProfileEnabled() const{
	return GetInstance().SupportsExtension(deoxrInstance::extEXTHPMixedRealityController);
}

void deoxrDPHPMixedRealityController::pAddDevice(bool left){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if(device){
		return;
	}
	
	pCreateDevice(device, left, "hpmrc_", true);
	
	deoxrDeviceComponent * const trigger = pAddComponentTrigger(device);
	pAddAxisTrigger(device, trigger);
	pAddButtonTrigger(device, trigger, false); // has to be button 0
	
	if(left){
		pAddButton(device, ebaPrimary, eblX, false); // has to be button 1
		pAddButton(device, ebaSecondary, eblY, false); // has to be button 2
		
	}else{
		pAddButton(device, ebaPrimary, eblA, false); // has to be button 1
		pAddButton(device, ebaSecondary, eblB, false); // has to be button 2
	}
	
	pAddButton(device, ebaAuxiliary1, eblHome, false); // has to be button 3
	
	deoxrDeviceComponent * const joystick = pAddComponentJoystick(device);
	pAddAxesJoystick(device, joystick);
	pAddButtonJoystick(device, joystick, true, false);
	
	deoxrDeviceComponent * const grip = pAddComponentGrip(device);
	pAddAxisGripGrab(device, grip);
	pAddButtonGrip(device, grip, false);
	
	pAddHandTracker(device, left, true);
	
	GetInstance().GetOxr().GetDevices().Add(device);
}
