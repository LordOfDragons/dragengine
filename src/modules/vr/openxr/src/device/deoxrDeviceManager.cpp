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

#include "deoxrDevice.h"
#include "deoxrDeviceAxis.h"
#include "deoxrDeviceButton.h"
#include "deoxrDeviceFeedback.h"
#include "deoxrDeviceComponent.h"
#include "deoxrDeviceManager.h"
#include "../deVROpenXR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deVRSystem.h>



// Class deoxrDeviceManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDeviceManager::deoxrDeviceManager(deVROpenXR &oxr) :
pOxr(oxr),
pNotifyAttachedDetached(false){
}

deoxrDeviceManager::~deoxrDeviceManager(){
}



// Management
///////////////

void deoxrDeviceManager::Clear(){
	if(pDevices.GetCount() > 0){
		pNotifyAttachedDetached = true;
	}
	
	pDevices.RemoveAll();
}



int deoxrDeviceManager::GetCount() const{
	return pDevices.GetCount();
}

deoxrDevice *deoxrDeviceManager::GetAt(int index) const{
	return (deoxrDevice*)pDevices.GetAt(index);
}

deoxrDevice *deoxrDeviceManager::GetWithID(const char *id) const{
	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoxrDevice * const device = (deoxrDevice*)pDevices.GetAt(i);
		if(device->GetID() == id){
			return device;
		}
	}
	
	return nullptr;
}

int deoxrDeviceManager::IndexOfWithID(const char *id) const{
	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoxrDevice * const device = (deoxrDevice*)pDevices.GetAt(i);
		if(device->GetID() == id){
			return i;
		}
	}
	
	return -1;
}

void deoxrDeviceManager::Add(deoxrDevice *device){
	if(!device){
		DETHROW_INFO(deeNullPointer, "device");
	}
	
	device->SetIndex(pDevices.GetCount());
	pDevices.Add(device);
	
	pOxr.LogInfoFormat("Input Device Added: id='%s' type=%d axes=%d buttons=%d feedbacks=%d",
		device->GetID().GetString(), device->GetType(), device->GetAxisCount(),
		device->GetButtonCount(), device->GetFeedbackCount());
	LogDevice(*device);
	
	pNotifyAttachedDetached = true;
}

void deoxrDeviceManager::Remove(deoxrDevice *device){
	const int index = pDevices.IndexOf(device);
	if(index == -1){
		DETHROW_INFO(deeInvalidParam, "device not in list");
	}
	
	pOxr.LogInfoFormat("Input Device Removed: id='%s'", device->GetID().GetString());
	
	pDevices.RemoveFrom(index);
	
	const int count = pDevices.GetCount();
	int i;
	for(i=index; i<count; i++){
		GetAt(i)->SetIndex(i);
	}
	
	pNotifyAttachedDetached = true;
}

void deoxrDeviceManager::TrackDeviceStates(){
	const int count = pDevices.GetCount();
	int i;
	for(i=0; i<count; i++){
		GetAt(i)->TrackStates();
	}
}

void deoxrDeviceManager::CheckNotifyAttachedDetached(){
	if(!pNotifyAttachedDetached){
		return;
	}
	
	pNotifyAttachedDetached = false;
	
	deInputEvent event;
	event.SetType(deInputEvent::eeDevicesAttachedDetached);
	event.SetSource(deInputEvent::esVR);
	pOxr.InputEventSetTimestamp(event);
	pOxr.GetGameEngine()->GetVRSystem()->GetEventQueue().AddEvent(event);
}

void deoxrDeviceManager::ReferenceSpaceChanged(){
	const int count = pDevices.GetCount();
	int i;
	for(i=0; i<count; i++){
		GetAt(i)->ReferenceSpaceChanged();
	}
}



void deoxrDeviceManager::LogDevices(){
	const int count = pDevices.GetCount();
	int i, j;
	
	pOxr.LogInfo("Input Devices:");
	
	for(i=0; i<count; i++){
		const deoxrDevice &device = *((deoxrDevice*)pDevices.GetAt(i));
		pOxr.LogInfoFormat("- '%s' (%s) [%d]", device.GetName().GetString(),
			device.GetID().GetString(), device.GetType());
		
		const int componentCount = device.GetComponentCount();
		if(componentCount > 0){
			pOxr.LogInfo("  Components:");
			for(j=0; j<componentCount; j++){
				const deoxrDeviceComponent &component = *device.GetComponentAt(j);
				pOxr.LogInfoFormat("    - '%s' (%s)", component.GetName().GetString(),
					component.GetID().GetString());
			}
		}
		
		const int axisCount = device.GetAxisCount();
		if(axisCount > 0){
			pOxr.LogInfo("  Axes:");
			for(j=0; j<axisCount; j++){
				const deoxrDeviceAxis &axis = *device.GetAxisAt(j);
				pOxr.LogInfoFormat("    - '%s' (%s) [%s]", axis.GetName().GetString(),
					axis.GetID().GetString(), axis.GetInputDeviceComponent()
						? axis.GetInputDeviceComponent()->GetID().GetString() : "");
			}
		}
		
		const int buttonCount = device.GetButtonCount();
		if(buttonCount > 0){
			pOxr.LogInfo("  Buttons:");
			for(j=0; j<buttonCount; j++){
				const deoxrDeviceButton &button = *device.GetButtonAt(j);
				pOxr.LogInfoFormat("    - '%s' (%s) [%s] => %d",
					button.GetName().GetString(), button.GetID().GetString(), button.GetInputDeviceComponent()
						? button.GetInputDeviceComponent()->GetID().GetString() : "", j);
			}
		}
	}
}

void deoxrDeviceManager::LogDevice(const deoxrDevice &device){
	pOxr.LogInfoFormat("- '%s' (%s) [%d]", device.GetName().GetString(),
		device.GetID().GetString(), device.GetType());
	
	const int componentCount = device.GetComponentCount();
	int i;
	if(componentCount > 0){
		pOxr.LogInfo("  Components:");
		for(i=0; i<componentCount; i++){
			const deoxrDeviceComponent &component = *device.GetComponentAt(i);
			pOxr.LogInfoFormat("    - '%s' (%s)", component.GetName().GetString(),
				component.GetID().GetString());
		}
	}
	
	const int axisCount = device.GetAxisCount();
	if(axisCount > 0){
		pOxr.LogInfo("  Axes:");
		for(i=0; i<axisCount; i++){
			const deoxrDeviceAxis &axis = *device.GetAxisAt(i);
			pOxr.LogInfoFormat("    - '%s' (%s) [%s]", axis.GetName().GetString(),
				axis.GetID().GetString(), axis.GetInputDeviceComponent()
					? axis.GetInputDeviceComponent()->GetID().GetString() : "");
		}
	}
	
	const int buttonCount = device.GetButtonCount();
	if(buttonCount > 0){
		pOxr.LogInfo("  Buttons:");
		for(i=0; i<buttonCount; i++){
			const deoxrDeviceButton &button = *device.GetButtonAt(i);
			pOxr.LogInfoFormat("    - '%s' (%s) [%s]%s%s => %d",
				button.GetName().GetString(),
				button.GetID().GetString(),
				button.GetInputDeviceComponent()
					? button.GetInputDeviceComponent()->GetID().GetString() : "",
				button.GetActionTouch() ? " {touchable}" : "",
				button.GetActionNear() ? " {near}" : "",
				i);
		}
	}
}



decString deoxrDeviceManager::NormalizeID(const char *id){
	if(!id){
		DETHROW(deeInvalidParam);
	}
	
	const int len = (int)strlen(id);
	if(len == 0){
		return decString();
	}
	
	decString nid;
	nid.Set(' ', len);
	
	int i;
	for(i=0; i<len; i++){
		if((id[i]  >= 'A' && id[i] <= 'Z')
		|| (id[i] >= 'a' && id[i] <= 'z')
		|| (id[i] >= '0' && id[i] <= '9')
		|| id[i] == '_'){
			nid[i] = id[i];
			
		}else{
			nid[i] = '_';
		}
	}
	
	return nid;
}



// Private functions
//////////////////////
