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

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "deainpDevice.h"
#include "deainpDeviceAxis.h"
#include "deainpDeviceButton.h"
#include "deainpDeviceFeedback.h"
#include "deainpDeviceManager.h"
#include "deAndroidInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDeviceAxis.h>
#include <dragengine/input/deInputDeviceButton.h>
#include <dragengine/input/deInputDeviceComponent.h>
#include <dragengine/input/deInputDeviceFeedback.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class deainpDevice
/////////////////////

// Constructor, destructor
////////////////////////////

deainpDevice::deainpDevice(deAndroidInput &module, eSources source) :
pModule(module),
pIndex(-1),
pSource(source),
pType(deInputDevice::edtMouse),

pDirtyAxesValues(false){
}

deainpDevice::~deainpDevice() = default;



// Management
///////////////

void deainpDevice::SetIndex(int index){
	pIndex = index;
}

void deainpDevice::SetType(deInputDevice::eDeviceTypes type){
	pType = type;
}

void deainpDevice::SetID(const char *id){
	pID = id;
}

void deainpDevice::SetName(const char *name){
	pName = name;
}



deainpDeviceButton *deainpDevice::GetButtonWithID(const char *id){
	deainpDeviceButton *found = nullptr;
	return pButtons.Find(found, [&](const deainpDeviceButton &button){
		return button.GetID() == id;
	}) ? found : nullptr;
}

deainpDeviceButton *deainpDevice::GetButtonWithAICode(int code){
	deainpDeviceButton *found = nullptr;
	return pButtons.Find(found, [&](const deainpDeviceButton &button){
		return button.GetAICode() == code;
	}) ? found : nullptr;
}

int deainpDevice::IndexOfButtonWithID(const char *id) const{
	return pButtons.IndexOfMatching([&](const deainpDeviceButton &button){
		return button.GetID() == id;
	});
}

int deainpDevice::IndexOfButtonWithKeyCode(deInputEvent::eKeyCodes code) const{
	return pButtons.IndexOfMatching([&](const deainpDeviceButton &button){
		return button.GetKeyCode() == code;
	});
}

int deainpDevice::IndexOfButtonWithAICode(int code) const{
	return pButtons.IndexOfMatching([&](const deainpDeviceButton &button){
		return button.GetAICode() == code;
	});
}



deainpDeviceAxis *deainpDevice::GetAxisWithID(const char *id){
	deainpDeviceAxis *found = nullptr;
	return pAxes.Find(found, [&](const deainpDeviceAxis &axis){
		return axis.GetID() == id;
	}) ? found : nullptr;
}

deainpDeviceAxis *deainpDevice::GetAxisWithAICode(int code){
	deainpDeviceAxis *found = nullptr;
	return pAxes.Find(found, [&](const deainpDeviceAxis &axis){
		return axis.GetAICode() == code;
	}) ? found : nullptr;
}

int deainpDevice::IndexOfAxisWithID(const char *id) const{
	return pAxes.IndexOfMatching([&](const deainpDeviceAxis &axis){
		return axis.GetID() == id;
	});
}

int deainpDevice::IndexOfAxisWithAICode(int code) const{
	return pAxes.IndexOfMatching([&](const deainpDeviceAxis &axis){
		return axis.GetAICode() == code;
	});
}



deainpDeviceFeedback *deainpDevice::GetFeedbackWithID(const char *id){
	deainpDeviceFeedback *found = nullptr;
	return pFeedbacks.Find(found, [&](const deainpDeviceFeedback &feedback){
		return feedback.GetID() == id;
	}) ? found : nullptr;
}

int deainpDevice::IndexOfFeedbackWithID(const char *id) const{
	return pFeedbacks.IndexOfMatching([&](const deainpDeviceFeedback &feedback){
		return feedback.GetID() == id;
	});
}



void deainpDevice::SetDirtyAxesValues(bool dirty){
	pDirtyAxesValues = dirty;
}



void deainpDevice::GetInfo(deInputDevice &info) const{
	int i;
	
	info.SetID(pID);
	info.SetName(pName);
	info.SetType(pType);
	
	info.SetDisplayImage(NULL);
	info.SetDisplayModel(NULL);
	info.SetDisplaySkin(NULL);
	
	info.SetButtonCount(pButtons.GetCount());
	for(i=0; i<pButtons.GetCount(); i++){
		pButtons[i].GetInfo(info.GetButtons().GetAt(i));
	}
	
	info.SetAxisCount(pAxes.GetCount());
	for(i=0; i<pAxes.GetCount(); i++){
		pAxes[i].GetInfo(info.GetAxes().GetAt(i));
	}
	
	info.SetFeedbackCount(pFeedbacks.GetCount());
	for(i=0; i<pFeedbacks.GetCount(); i++){
		pFeedbacks[i].GetInfo(info.GetFeedbacks().GetAt(i));
	}
}

void deainpDevice::Update(){
}

void deainpDevice::SendDirtyAxisEvents(){
	if(!pDirtyAxesValues){
		return;
	}
	
	pDirtyAxesValues = false;
	
	pAxes.Visit([&](deainpDeviceAxis &axis){
		axis.SendEvents(*this);
	});
}



// Private Functions
//////////////////////
