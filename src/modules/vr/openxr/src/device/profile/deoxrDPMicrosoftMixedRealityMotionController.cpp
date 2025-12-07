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

#include "deoxrDPMicrosoftMixedRealityMotionController.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPMicrosoftMixedRealityMotionController
///////////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPMicrosoftMixedRealityMotionController::deoxrDPMicrosoftMixedRealityMotionController(deoxrInstance &instance) :
deoxrDPBaseTwoHandController(instance,
	deoxrPath(instance, "/interaction_profiles/microsoft/motion_controller"),
	"Microsoft Mixed Reality Motion Controller")
{
}

deoxrDPMicrosoftMixedRealityMotionController::deoxrDPMicrosoftMixedRealityMotionController(
	deoxrInstance &instance, const deoxrPath &path, const char *name) :
deoxrDPBaseTwoHandController(instance, path, name){
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
	
	const int bindingCount = 12 * 2;
	deoxrInstance::sSuggestBinding bindings[bindingCount];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[2] = {"/user/hand/left", "/user/hand/right"};
	int i;
	
	for(i=0; i<2; i++){
		const decString &basePath = basePathList[i];
		
		pAdd(b, pPoseAction(i == 0), basePath + "/input/grip/pose");
		pAdd(b, pPoseAction2(i == 0), basePath + "/input/aim/pose");
		
		pAdd(b, deVROpenXR::eiaGripPress, basePath + "/input/squeeze/click");
		
		pAdd(b, deVROpenXR::eiaTriggerPress, basePath + "/input/trigger/value");
		pAdd(b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/trigger/value");
		
		pAdd(b, deVROpenXR::eiaButtonPrimaryPress, basePath + "/input/menu/click");
		
		pAdd(b, deVROpenXR::eiaJoystickAnalog, basePath + "/input/thumbstick");
		pAdd(b, deVROpenXR::eiaJoystickPress, basePath + "/input/thumbstick/click");
		
		pAdd(b, deVROpenXR::eiaTrackpadAnalog, basePath + "/input/trackpad");
		pAdd(b, deVROpenXR::eiaTrackpadPress, basePath + "/input/trackpad/click");
		pAdd(b, deVROpenXR::eiaTrackpadTouch, basePath + "/input/trackpad/touch");
		
		pAdd(b, deVROpenXR::eiaGripHaptic, basePath + "/output/haptic");
	}
	
	
	GetInstance().SuggestBindings(GetPath(), bindings, bindingCount);
}

const char *deoxrDPMicrosoftMixedRealityMotionController::pDeviceIdPrefix() const{
	return "mmrmc_";
}

void deoxrDPMicrosoftMixedRealityMotionController::pAddDevice(bool left){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if(device){
		return;
	}
	
	pCreateDevice(device, left, pDeviceIdPrefix(), true);
	
	deoxrDeviceComponent * const trigger = pAddComponentTrigger(device);
	pAddAxisTrigger(device, trigger);
	pAddButtonTrigger(device, trigger, false); // has to be button 0
	
	pAddButton(device, ebaPrimary, eblHome, false); // has to be button 1
	
	deoxrDeviceComponent * const joystick = pAddComponentJoystick(device);
	pAddAxesJoystick(device, joystick);
	pAddButtonJoystick(device, joystick, true, false);
	
	deoxrDeviceComponent * const trackpad = pAddComponentTrackpad(device);
	pAddAxesTrackpad(device, trackpad);
	pAddButtonTrackpad(device, trackpad, true, true);
	
	deoxrDeviceComponent * const grip = pAddComponentGrip(device);
	pAddButtonGrip(device, grip, false);
	
	pAddHandTracker(device, left, true);
	
	GetInstance().GetOxr().GetDevices().Add(device);
}
