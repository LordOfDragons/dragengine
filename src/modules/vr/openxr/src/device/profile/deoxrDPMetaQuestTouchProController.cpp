/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deoxrDPMetaQuestTouchProController.h"
#include "../deoxrDeviceAxis.h"
#include "../deoxrDeviceButton.h"
#include "../deoxrDeviceComponent.h"
#include "../deoxrDeviceFeedback.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>


// Class deoxrDPMetaQuestTouchProController
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPMetaQuestTouchProController::deoxrDPMetaQuestTouchProController(deoxrInstance &instance) :
deoxrDPBaseTwoHandController(instance,
	deoxrPath(instance, "/interaction_profiles/facebook/touch_controller_pro"),
	"Meta Quest Touch Pro Controller")
{
}

deoxrDPMetaQuestTouchProController::~deoxrDPMetaQuestTouchProController(){
}



// Private Functions
//////////////////////

bool deoxrDPMetaQuestTouchProController::pProfileEnabled() const{
	return GetInstance().SupportsExtension(deoxrInstance::extFBTouchControllerPro);
}

void deoxrDPMetaQuestTouchProController::pSuggestBindings(){
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// On /user/hand/left only:
	// - /input/x/click
	// - /input/x/touch
	// - /input/y/click
	// - /input/y/touch
	// - /input/menu/click
	// 
	// On /user/hand/right only:
	// - /input/a/click
	// - /input/a/touch
	// - /input/b/click
	// - /input/b/touch
	// - /input/system/click (may not be available for application use)
	// 
	// Both hands:
	// - /input/squeeze/value
	// - /input/trigger/value
	// - /input/trigger/touch
	// - /input/trigger/curl_fb
	// - /input/trigger/slide_fb
	// - /input/trigger/proximity_fb
	// - /input/thumbstick/x
	// - /input/thumbstick/y
	// - /input/thumbstick/click
	// - /input/thumbstick/touch
	// - /input/thumbrest/touch
	// - /input/thumbrest/force
	// - /input/thumb_fb/proximity_fb
	// - /input/grip/pose
	// - /input/aim/pose
	// - /input/stylus_fb/force
	// - /output/haptic
	// - /output/haptic_trigger_fb
	// - /output/haptic_thumb_fb
	
	const bool supportsProximity = GetInstance().SupportsExtension(
		deoxrInstance::extFBTouchControllerProximity);
	
	const int maxBindingCount = 19 * 2 + 5 + 5;

	int bindingCount = 17 * 2 + 5 + 5;
	if(supportsProximity){
		bindingCount += 2 * 2;
	}

	deoxrInstance::sSuggestBinding bindings[maxBindingCount];
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
		
		pAdd(b, deVROpenXR::eiaTriggerPress, basePath + "/input/trigger/value");
		pAdd(b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/trigger/value");
		pAdd(b, deVROpenXR::eiaTriggerTouch, basePath + "/input/trigger/touch");
		pAdd(b, deVROpenXR::eiaTriggerCurl, basePath + "/input/trigger/curl_fb");
		pAdd(b, deVROpenXR::eiaTriggerSlide, basePath + "/input/trigger/slide_fb");
		if(supportsProximity){
			pAdd(b, deVROpenXR::eiaTriggerNear, basePath + "/input/trigger/proximity_fb");
		}
		
		pAdd(b, deVROpenXR::eiaJoystickAnalog, basePath + "/input/thumbstick");
		pAdd(b, deVROpenXR::eiaJoystickPress, basePath + "/input/thumbstick/click");
		pAdd(b, deVROpenXR::eiaJoystickTouch, basePath + "/input/thumbstick/touch");
		
		pAdd(b, deVROpenXR::eiaThumbrestTouch, basePath + "/input/thumbrest/touch");
		pAdd(b, deVROpenXR::eiaThumbrestPress, basePath + "/input/thumbrest/force");
		if(supportsProximity){
			pAdd(b, deVROpenXR::eiaThumbrestNear, basePath + "/input/thumb_fb/proximity_fb");
		}
		
		pAdd(b, deVROpenXR::eiaGripHaptic, basePath + "/output/haptic");
		pAdd(b, deVROpenXR::eiaTriggerHaptic, basePath + "/output/haptic_trigger_fb");
		pAdd(b, deVROpenXR::eiaThumbrestHaptic, basePath + "/output/haptic_thumb_fb");
	}
	
	// left hand
	pAdd(b, deVROpenXR::eiaButtonPrimaryPress, "/user/hand/left/input/x/click");
	pAdd(b, deVROpenXR::eiaButtonPrimaryTouch, "/user/hand/left/input/x/touch");
	
	pAdd(b, deVROpenXR::eiaButtonSecondaryPress, "/user/hand/left/input/y/click");
	pAdd(b, deVROpenXR::eiaButtonSecondaryTouch, "/user/hand/left/input/y/touch");
	
	pAdd(b, deVROpenXR::eiaButtonAuxiliary1Press, "/user/hand/left/input/menu/click");
	
	// right hand
	pAdd(b, deVROpenXR::eiaButtonPrimaryPress, "/user/hand/right/input/a/click");
	pAdd(b, deVROpenXR::eiaButtonPrimaryTouch, "/user/hand/right/input/a/touch");
	
	pAdd(b, deVROpenXR::eiaButtonSecondaryPress, "/user/hand/right/input/b/click");
	pAdd(b, deVROpenXR::eiaButtonSecondaryTouch, "/user/hand/right/input/b/touch");
	
	pAdd(b, deVROpenXR::eiaButtonAuxiliary1Press, "/user/hand/right/input/system/click");
	
	
	GetInstance().SuggestBindings(GetPath(), bindings, bindingCount);
}

void deoxrDPMetaQuestTouchProController::pAddDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if(device){
		return;
	}
	
	const bool supportsProximity = GetInstance().SupportsExtension(
		deoxrInstance::extFBTouchControllerProximity);
	
	pCreateDevice(device, left, "mqtpc_", true);
	
	deoxrDeviceComponent * const trigger = pAddComponentTrigger(device);
	pAddAxisTrigger(device, trigger);
	pAddAxisTriggerCurl(device, trigger);
	pAddAxisTriggerSlide(device, trigger);
	pAddButtonTrigger(device, trigger, true, supportsProximity); // has to be button 0
	
	if(left){
		pAddButton(device, ebaPrimary, eblX, true); // has to be button 1
		pAddButton(device, ebaSecondary, eblY, true); // has to be button 2
		pAddButton(device, ebaAuxiliary1, eblHome, false); // has to be button 3
		
	}else{
		pAddButton(device, ebaPrimary, eblA, true); // has to be button 1
		pAddButton(device, ebaSecondary, eblB, true); // has to be button 2
		pAddButton(device, ebaAuxiliary1, eblSystem, false); // has to be button 3
	}
	
	deoxrDeviceComponent * const joystick = pAddComponentJoystick(device);
	pAddAxesJoystick(device, joystick);
	pAddButtonJoystick(device, joystick, true, true);
	
	deoxrDeviceComponent * const thumbrest = pAddComponentThumbrest(device);
	pAddButtonThumbrest(device, thumbrest, true, supportsProximity);
	pAddAxesThumbrestPress(device, thumbrest);
	
	deoxrDeviceComponent * const grip = pAddComponentGrip(device);
	pAddAxisGripGrab(device, grip);
	pAddButtonGrip(device, grip, false);
	
	GetInstance().GetOxr().GetDevices().Add(device);
}
