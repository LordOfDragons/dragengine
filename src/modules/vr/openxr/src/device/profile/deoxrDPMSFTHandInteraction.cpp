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

deoxrDPMSFTHandInteraction::deoxrDPMSFTHandInteraction(deoxrInstance &instance) :
deoxrDPBaseTwoHandController(instance,
	deoxrPath(instance, "/interaction_profiles/microsoft/hand_interaction"),
	"MSFT Hand Controller")
{
	SetDeviceRotation(decVector(DEG2RAD * -90.0f, 0.0f, 0.0f));
}

deoxrDPMSFTHandInteraction::~deoxrDPMSFTHandInteraction(){
}


// Private Functions
//////////////////////

void deoxrDPMSFTHandInteraction::pSuggestBindings(){
	// Valid for user path:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/aim/pose
	// - /input/grip/pose
	// - /input/select/value
	// - /input/squeeze/value
	
	const int bindingCountPerHand = 5;
	const int bindingCount = bindingCountPerHand * 2;
	deoxrInstance::sSuggestBinding bindings[bindingCount];
	deoxrInstance::sSuggestBinding *b = bindings;
	int usedBindingCount = 0;
	
	const deoxrPath basePathList[2] = {pPathHandLeft, pPathHandRight};
	int i;
	
	for(i=0; i<2; i++){
		const decString &basePath = basePathList[i];
		
		pAdd(b, pPoseAction(i == 0), basePath + "/input/grip/pose");
		
		pAdd(b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/select/value");
		pAdd(b, deVROpenXR::eiaGripGrab, basePath + "/input/squeeze/value");
		
		pAdd(b, deVROpenXR::eiaGesturePinch, basePath + "/input/select/value");
		pAdd(b, deVROpenXR::eiaGestureGrasp, basePath + "/input/squeeze/value");
		
		usedBindingCount += bindingCountPerHand;
	}
	
	if(usedBindingCount > 0){
		GetInstance().SuggestBindings(GetPath(), bindings, usedBindingCount);
	}
}

bool deoxrDPMSFTHandInteraction::pProfileEnabled() const{
	return GetInstance().SupportsExtension(deoxrInstance::extMSFTHandInteraction);
}

void deoxrDPMSFTHandInteraction::pAddDevice(bool left){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if(device){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	deoxrDeviceButton::Ref button;
	deoxrDeviceAxis::Ref axis;
	
	pCreateDevice(device, left, "msfthi_", false);
	
	// controller simulation
	deoxrDeviceComponent * const trigger = pAddComponentTrigger(device);
	pAddAxisTrigger(device, trigger);
	
	button.TakeOverWith(device);
	button->SetID("trig");
	button->SetName("Trigger");
	button->SetType(deInputDeviceButton::ebtTrigger);
	button->SetDisplayText("Tri");
	button->SetInputDeviceComponent(trigger);
	button->SetFakeFromAxis(device->GetAxisAt(device->GetAxisCount() - 1));
	button->SetIndex(device->GetButtonCount());
	device->AddButton(button); // has to be button 0
	
	deoxrDeviceComponent * const grip = pAddComponentGrip(device);
	pAddAxisGripGrab(device, grip);
	
	button.TakeOverWith(device);
	button->SetID("grip");
	button->SetName("Grip");
	button->SetType(deInputDeviceButton::ebtTrigger);
	button->SetDisplayText("Grip");
	button->SetInputDeviceComponent(grip);
	button->SetFakeFromAxis(device->GetAxisAt(device->GetAxisCount() - 1));
	button->SetIndex(device->GetButtonCount());
	device->AddButton(button);
	
	// gestures
	deoxrDeviceComponent * const gesture = device->AddComponent(
		deInputDeviceComponent::ectGesture, "Hand Gesture", "handGesture", "Hand Gesture");
	
	axis.TakeOverWith(device);
	axis->SetActionAnalog(oxr.GetAction(deVROpenXR::eiaGesturePinch));
	axis->SetType(deInputDeviceAxis::eatGesture);
	axis->SetRange(0.0f, 1.0f);
	axis->SetCenter(-1.0f);
	axis->SetValue(-1.0f);
	axis->SetName("Pinch");
	axis->SetID("ghpinch");
	axis->SetDisplayText("Pinch");
	axis->SetIndex(device->GetAxisCount());
	axis->SetInputDeviceComponent(gesture);
	device->AddAxis(axis);
	
	axis.TakeOverWith(device);
	axis->SetActionAnalog(oxr.GetAction(deVROpenXR::eiaGestureGrasp));
	axis->SetType(deInputDeviceAxis::eatGesture);
	axis->SetRange(0.0f, 1.0f);
	axis->SetCenter(-1.0f);
	axis->SetValue(-1.0f);
	axis->SetName("Grasp");
	axis->SetID("ghgrasp");
	axis->SetDisplayText("Grasp");
	axis->SetIndex(device->GetAxisCount());
	axis->SetInputDeviceComponent(gesture);
	device->AddAxis(axis);
	
	// hand tracking
	pAddHandTracker(device, left, true);
	device->SetEnableTwoFingerTriggerSimulation(false);
	
	oxr.GetDevices().Add(device);
}
