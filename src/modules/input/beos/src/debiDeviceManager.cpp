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
#include <stdlib.h>

#include <device/Joystick.h>

#include "debiDevice.h"
#include "debiDeviceMouse.h"
#include "debiDeviceKeyboard.h"
#include "debiDeviceAxis.h"
#include "debiDeviceButton.h"
#include "debiDeviceJoystick.h"
#include "debiDeviceManager.h"
#include "deBeOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class debiDeviceManager
////////////////////////////

// Constructor, destructor
////////////////////////////

debiDeviceManager::debiDeviceManager(deBeOSInput &module) :
pModule(module)
{
	try{
		pCreateDevices();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

debiDeviceManager::~debiDeviceManager(){
	pCleanUp();
}



// Management
///////////////

void debiDeviceManager::UpdateDeviceList(){
}



debiDevice *debiDeviceManager::GetWithID(const char *id){
	return pDevices.FindOrDefault([&](const debiDevice &device){
		return device.GetID() == id;
	});
}

int debiDeviceManager::IndexOfWithID(const char *id){
	return pDevices.IndexOfMatching([&](const debiDevice &device){
		return device.GetID() == id;
	});
}



void debiDeviceManager::LogDevices(){
	pModule.LogInfo("Input Devices:");
	
	pDevices.Visit([&](const debiDevice &device){
		pModule.LogInfoFormat("- %s (%s) [%d]", device.GetName().GetString(),
			device.GetID().GetString(), device.GetType());
		
		if(device.GetAxes().IsNotEmpty()){
			pModule.LogInfo("  Axes:");
			device.GetAxes().Visit([&](const debiDeviceAxis &axis){
				pModule.LogInfoFormat("    - '%s' (%s) %d .. %d [%d %d]",
					axis.GetName().GetString(), axis.GetID().GetString(),
					axis.GetMinimum(), axis.GetMaximum(), 0, 0);
			});
		}
		
		if(device.GetButtons().IsNotEmpty()){
			pModule.LogInfo("  Buttons:");
			device.GetButtons().VisitIndexed([&](int i, const debiDeviceButton &button){
				pModule.LogInfoFormat("    - '%s' (%s) %d => %d",
				 	button.GetName().GetString(), button.GetID().GetString(),
					button.GetBICode(), i);
			});
		}
	});
}



decString debiDeviceManager::NormalizeID(const char *id){
	if(!id){
		DETHROW(deeInvalidParam);
	}
	
	const int len = strlen(id);
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

void debiDeviceManager::pCleanUp(){
	pDevices.RemoveAll();
}



void debiDeviceManager::pCreateDevices(){
	pMouse = debiDeviceMouse::Ref::New(pModule);
	pMouse->SetIndex(pDevices.GetCount());
	pDevices.Add(pMouse);
	
	pKeyboard = debiDeviceKeyboard::Ref::New(pModule);
	pKeyboard->SetIndex(pDevices.GetCount());
	pDevices.Add(pKeyboard);
	
	pCreateJoystickDevices();
}

void debiDeviceManager::pCreateJoystickDevices(){
	BJoystick joysticks;
	
	const int count = joysticks.CountDevices();
	int i;
	
	char name[B_OS_NAME_LENGTH];
	for(i=0; i<count; i++){
		if(joysticks.GetDeviceName(i, name, sizeof(name)) != B_OK){
			DETHROW(deeInvalidParam);
		}
		
		auto joystick = debiDeviceJoystick::Ref::New(pModule, name);
		joystick->SetIndex(pDevices.GetCount());
		pDevices.Add(joystick);
	}
}
