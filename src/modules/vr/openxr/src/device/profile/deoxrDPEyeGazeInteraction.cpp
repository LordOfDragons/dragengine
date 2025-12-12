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

#include "deoxrDPEyeGazeInteraction.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class deoxrDPEyeGazeInteraction
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPEyeGazeInteraction::deoxrDPEyeGazeInteraction(deoxrInstance &instance) :
deoxrDeviceProfile(instance,
	deoxrPath(instance, "/interaction_profiles/ext/eye_gaze_interaction"),
	"Eye Gaze Interaction"){
}

deoxrDPEyeGazeInteraction::~deoxrDPEyeGazeInteraction(){
}



// Management
///////////////

void deoxrDPEyeGazeInteraction::CheckAttached(){
	if(GetInstance().SupportsExtension(deoxrInstance::extEXTEyeGazeInteraction)){
		pAddDevice();
		
	}else{
		pRemoveDevice();
	}
}

void deoxrDPEyeGazeInteraction::SuggestBindings(){
	if(!GetInstance().SupportsExtension(deoxrInstance::extEXTEyeGazeInteraction)){
		return;
	}
	
	// Valid for user paths:
	// - /user/eyes_ext
	// 
	// Supported component paths:
	// - /input/gaze_ext/pose
	
	const int bindingCount = 1;
	deoxrInstance::sSuggestBinding bindings[bindingCount];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	pAdd(b, deVROpenXR::eiaPose, "/user/eyes_ext/input/gaze_ext/pose");
	
	GetInstance().SuggestBindings(GetPath(), bindings, bindingCount);
}

void deoxrDPEyeGazeInteraction::ClearActions(){
	pRemoveDevice();
}



// Private Functions
//////////////////////

void deoxrDPEyeGazeInteraction::pAddDevice() {
	if(pDevice){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	pDevice.TakeOver(new deoxrDevice(oxr, *this));
	
	decString id;
	
	pDevice->SetType(deInputDevice::edtVREyeTracker);
	pDevice->SetName("Eye Tracker");
	id.Format("%segi", OXR_DEVID_PREFIX);
	
	pDevice->SetActionPose(oxr.GetAction(deVROpenXR::eiaPose));
	pDevice->SetID(id);
	
	pDevice->SetSpacePose(deoxrSpace::Ref::New(*pGetSession(),
		pDevice->GetActionPose(), pDevice->GetSubactionPath(), GetDeviceRotation()));
	
	GetInstance().GetOxr().GetDevices().Add(pDevice);
}

void deoxrDPEyeGazeInteraction::pRemoveDevice() {
	if(!pDevice){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	oxr.GetDevices().Remove(pDevice);
	pDevice = nullptr;
}
