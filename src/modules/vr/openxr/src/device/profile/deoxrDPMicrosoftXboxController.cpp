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

#include "deoxrDPMicrosoftXboxController.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPMicrosoftXboxController
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPMicrosoftXboxController::deoxrDPMicrosoftXboxController(deoxrInstance &instance) :
deoxrDeviceProfile(instance,
	deoxrPath(instance, "/interaction_profiles/microsoft/xbox_controller"),
	"Microsoft XBox Controller")
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
	deoxrInstance::sSuggestBinding bindings[bindingCount];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[2] = {"/user/hand/left", "/user/hand/right"};
	int i;
	
	for(i=0; i<2; i++){
		const decString &basePath = basePathList[i];
		
		pAdd(b, deVROpenXR::eiaPose, basePath + "/input/aim/pose");
		
		pAdd(b, deVROpenXR::eiaGripSqueeze, basePath + "/input/squeeze/click");
		
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
	*/
}
