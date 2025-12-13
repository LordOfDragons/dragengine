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

#include "deAndroidInput.h"
#include "deainpDevice.h"
#include "deainpDeviceAxis.h"
#include "deainpDeviceButton.h"
#include "deainpDeviceFeedback.h"
#include "deainpDeviceManager.h"
#include "deainpDeviceMouse.h"
#include "deainpDeviceKeyboard.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSAndroid.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class deainpDeviceManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deainpDeviceManager::deainpDeviceManager(deAndroidInput &module) :
pModule(module)
{
	try{
		pCreateDevices();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deainpDeviceManager::~deainpDeviceManager(){
	pCleanUp();
}



// Management
///////////////

void deainpDeviceManager::UpdateDeviceList(){
}



int deainpDeviceManager::GetCount() const{
	return pDevices.GetCount();
}

deainpDevice *deainpDeviceManager::GetAt(int index) const{
	return (deainpDevice*)pDevices.GetAt(index);
}

deainpDevice *deainpDeviceManager::GetWithID(const char *id) const{
	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deainpDevice * const device = (deainpDevice*)pDevices.GetAt(i);
		if(device->GetID() == id){
			return device;
		}
	}
	
	return NULL;
}

int deainpDeviceManager::IndexOfWithID(const char *id) const{
	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deainpDevice * const device = (deainpDevice*)pDevices.GetAt(i);
		if(device->GetID() == id){
			return i;
		}
	}
	
	return -1;
}



void deainpDeviceManager::LogDevices(){
	const int count = pDevices.GetCount();
	int i, j;
	
	pModule.LogInfo("Input Devices:");
	
	for(i=0; i<count; i++){
		const deainpDevice &device = *((deainpDevice*)pDevices.GetAt(i));
		pModule.LogInfoFormat("- '%s' (%s) [%d]", device.GetName().GetString(),
			device.GetID().GetString(), device.GetType());
		
		const int axisCount = device.GetAxisCount();
		if(axisCount > 0){
			pModule.LogInfo("  Axes:");
			for(j=0; j<axisCount; j++){
				const deainpDeviceAxis &axis = device.GetAxisAt(j);
				pModule.LogInfoFormat("    - '%s' (%s) %d .. %d [%d %d]",
					axis.GetName().GetString(), axis.GetID().GetString(), axis.GetMinimum(),
					axis.GetMaximum(), axis.GetFuzz(), axis.GetFlat());
			}
		}
		
		const int buttonCount = device.GetButtonCount();
		if(buttonCount > 0){
			pModule.LogInfo("  Buttons:");
			for(j=0; j<buttonCount; j++){
				const deainpDeviceButton &button = device.GetButtonAt(j);
				pModule.LogInfoFormat("    - '%s' (%s) %d => %d",
					button.GetName().GetString(), button.GetID().GetString(),
					button.GetAICode(), j);
			}
		}
	}
}



// Private functions
//////////////////////

void deainpDeviceManager::pCleanUp(){
	pDevices.RemoveAll();
}

void deainpDeviceManager::pCreateDevices(){
	pMouse = deainpDeviceMouse::Ref::New(pModule);
	pMouse->SetIndex(pDevices.GetCount());
	pDevices.Add(pMouse);
	
	pKeyboard = deainpDeviceKeyboard::Ref::New(pModule);
	pKeyboard->SetIndex(pDevices.GetCount());
	pDevices.Add(pKeyboard);
}
