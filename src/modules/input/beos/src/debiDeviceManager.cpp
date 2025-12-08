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
pModule(module),
pMouse(NULL),
pKeyboard(NULL)
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



int debiDeviceManager::GetCount() const{
	return pDevices.GetCount();
}

debiDevice *debiDeviceManager::GetAt(int index) const{
	return (debiDevice*)pDevices.GetAt(index);
}

debiDevice *debiDeviceManager::GetWithID(const char *id){
	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		debiDevice * const device = (debiDevice*)pDevices.GetAt(i);
		if(device->GetID() == id){
			return device;
		}
	}
	
	return NULL;
}

int debiDeviceManager::IndexOfWithID(const char *id){
	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		debiDevice * const device = (debiDevice*)pDevices.GetAt(i);
		if(device->GetID() == id){
			return i;
		}
	}
	
	return -1;
}



void debiDeviceManager::LogDevices(){
	const int count = pDevices.GetCount();
	int i, j;
	
	pModule.LogInfo("Input Devices:");
	
	for(i=0; i<count; i++){
		const debiDevice &device = *((debiDevice*)pDevices.GetAt(i));
		pModule.LogInfoFormat("- %s (%s) [%d]", device.GetName().GetString(),
			device.GetID().GetString(), device.GetType());
		
		const int axisCount = device.GetAxisCount();
		if(axisCount > 0){
			pModule.LogInfo("  Axes:");
			for(j=0; j<axisCount; j++){
				const debiDeviceAxis &axis = *device.GetAxisAt(j);
				pModule.LogInfoFormat("    - '%s' (%s) %d .. %d [%d %d]",
					axis.GetName().GetString(), axis.GetID().GetString(), axis.GetMinimum(),
					axis.GetMaximum(), 0, 0);
			}
		}
		
		const int buttonCount = device.GetButtonCount();
		if(buttonCount > 0){
			pModule.LogInfo("  Buttons:");
			for(j=0; j<buttonCount; j++){
				const debiDeviceButton &button = *device.GetButtonAt(j);
				pModule.LogInfoFormat("    - '%s' (%s) %d => %d",
					button.GetName().GetString(), button.GetID().GetString(),
					button.GetBICode(), j);
			}
		}
	}
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
	pMouse.TakeOver(new debiDeviceMouse(pModule));
	pMouse->SetIndex(pDevices.GetCount());
	pDevices.Add(pMouse);
	
	pKeyboard.TakeOver(new debiDeviceKeyboard(pModule));
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
		
		debiDeviceJoystick *joystick = NULL;
		try{
			joystick = new debiDeviceJoystick(pModule, name);
			joystick->SetIndex(pDevices.GetCount());
			pDevices.Add(joystick);
			joystick->FreeReference();
			
		}catch(const deException &){
			if(joystick){
				joystick->FreeReference();
			}
			throw;
		}
	}
}
