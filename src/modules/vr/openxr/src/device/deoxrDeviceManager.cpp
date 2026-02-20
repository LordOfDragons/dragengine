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



deoxrDevice *deoxrDeviceManager::GetWithID(const char *id) const{
	return pDevices.FindOrDefault([&](const deoxrDevice &device){
		return device.GetID() == id;
	});
}

int deoxrDeviceManager::IndexOfWithID(const char *id) const{
	return pDevices.IndexOfMatching([&](const deoxrDevice &device){
		return device.GetID() == id;
	});
}

void deoxrDeviceManager::Add(deoxrDevice *device){
	DEASSERT_NOTNULL(device)
	
	device->SetIndex(pDevices.GetCount());
	pDevices.AddOrThrow(device);
	
	pOxr.LogInfoFormat("Input Device Added: id='%s' type=%d axes=%d buttons=%d feedbacks=%d",
		device->GetID().GetString(), device->GetType(), device->GetAxes().GetCount(),
		device->GetButtons().GetCount(), device->GetFeedbacks().GetCount());
	LogDevice(*device);
	
	pNotifyAttachedDetached = true;
}

void deoxrDeviceManager::Remove(deoxrDevice *device){
	const int index = pDevices.IndexOf(device);
	DEASSERT_TRUE(index != -1)
	
	pOxr.LogInfoFormat("Input Device Removed: id='%s'", device->GetID().GetString());
	
	pDevices.RemoveFrom(index);
	
	pDevices.VisitIndexed([&](int i, deoxrDevice &d){
		d.SetIndex(i);
	});
	
	pNotifyAttachedDetached = true;
}

void deoxrDeviceManager::TrackDeviceStates(){
	pDevices.Visit([](deoxrDevice &device){
		device.TrackStates();
	});
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
	pDevices.Visit([](deoxrDevice &device){
		device.ReferenceSpaceChanged();
	});
}



void deoxrDeviceManager::LogDevices(){
	pOxr.LogInfo("Input Devices:");
	
	pDevices.Visit([&](const deoxrDevice &device){
		pOxr.LogInfoFormat("- '%s' (%s) [%d]", device.GetName().GetString(),
			device.GetID().GetString(), device.GetType());
		
		if(device.GetComponents().IsNotEmpty()){
			pOxr.LogInfo("  Components:");
			device.GetComponents().Visit([&](const deoxrDeviceComponent &component){
				pOxr.LogInfoFormat("    - '%s' (%s)", component.GetName().GetString(),
					component.GetID().GetString());
			});
		}
		
		if(device.GetAxes().IsNotEmpty()){
			pOxr.LogInfo("  Axes:");
			device.GetAxes().Visit([&](const deoxrDeviceAxis &axis){
				pOxr.LogInfoFormat("    - '%s' (%s) [%s]", axis.GetName().GetString(),
					axis.GetID().GetString(), axis.GetInputDeviceComponent()
						? axis.GetInputDeviceComponent()->GetID().GetString() : "");
			});
		}
		
		if(device.GetButtons().IsNotEmpty()){
			pOxr.LogInfo("  Buttons:");
			device.GetButtons().VisitIndexed([&](int i, const deoxrDeviceButton &button){
				pOxr.LogInfoFormat("    - '%s' (%s) [%s] => %d",
					button.GetName().GetString(), button.GetID().GetString(), button.GetInputDeviceComponent()
						? button.GetInputDeviceComponent()->GetID().GetString() : "", i);
			});
		}
	});
}

void deoxrDeviceManager::LogDevice(const deoxrDevice &device){
	pOxr.LogInfoFormat("- '%s' (%s) [%d]", device.GetName().GetString(),
		device.GetID().GetString(), device.GetType());
	
	if(device.GetComponents().IsNotEmpty()){
		pOxr.LogInfo("  Components:");
		device.GetComponents().Visit([&](const deoxrDeviceComponent &component){
			pOxr.LogInfoFormat("    - '%s' (%s)", component.GetName().GetString(),
				component.GetID().GetString());
		});
	}
	
	if(device.GetAxes().IsNotEmpty()){
		pOxr.LogInfo("  Axes:");
		device.GetAxes().Visit([&](const deoxrDeviceAxis &axis){
			pOxr.LogInfoFormat("    - '%s' (%s) [%s]", axis.GetName().GetString(),
				axis.GetID().GetString(), axis.GetInputDeviceComponent()
					? axis.GetInputDeviceComponent()->GetID().GetString() : "");
		});
	}
	
	if(device.GetButtons().IsNotEmpty()){
		pOxr.LogInfo("  Buttons:");
		device.GetButtons().VisitIndexed([&](int i, const deoxrDeviceButton &button){
			pOxr.LogInfoFormat("    - '%s' (%s) [%s]%s%s => %d",
				button.GetName().GetString(),
				button.GetID().GetString(),
				button.GetInputDeviceComponent()
					? button.GetInputDeviceComponent()->GetID().GetString() : "",
				button.GetActionTouch() ? " {touchable}" : "",
				button.GetActionNear() ? " {near}" : "",
				i);
		});
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
