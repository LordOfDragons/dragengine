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

#include "dewiDevice.h"
#include "dewiDeviceAxis.h"
#include "dewiDeviceButton.h"
#include "dewiDeviceFeedback.h"
#include "dewiDeviceManager.h"
#include "dewiDeviceMouse.h"
#include "dewiDeviceKeyboard.h"
#include "deWindowsInput.h"
#include "dewiDeviceWinRTController.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSWindows.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/threading/deMutexGuard.h>

#include <winrt/Windows.Foundation.Collections.h>


// Class dewiDeviceManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

dewiDeviceManager::dewiDeviceManager(deWindowsInput &module) :
pModule(module)
{
	try{
		pCreateDevices();
		pCreateControllers();

	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dewiDeviceManager::~dewiDeviceManager(){
	pCleanUp();
}



// Management
///////////////

void dewiDeviceManager::UpdateDeviceList(){
}



int dewiDeviceManager::GetCount() const{
	return pDevices.GetCount();
}

dewiDevice *dewiDeviceManager::GetAt(int index) const{
	return (dewiDevice*)pDevices.GetAt(index);
}

dewiDevice *dewiDeviceManager::GetWithID(const char *id) const{
	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dewiDevice * const device = (dewiDevice*)pDevices.GetAt(i);
		if(device->GetID() == id){
			return device;
		}
	}
	
	return NULL;
}

int dewiDeviceManager::IndexOfWithID(const char *id) const{
	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dewiDevice * const device = (dewiDevice*)pDevices.GetAt(i);
		if(device->GetID() == id){
			return i;
		}
	}
	
	return -1;
}

dewiDeviceWinRTController *dewiDeviceManager::GetWithController(wrgi::RawGameController const &controller) const{
	const int count = pDevices.GetCount();
	int i;

	for(i=0; i<count; i++){
		dewiDevice * const device = (dewiDevice*)pDevices.GetAt(i);
		if(device->GetSource() != dewiDevice::esWinRTController){
			continue;
		}

		dewiDeviceWinRTController * const wcd = (dewiDeviceWinRTController*)device;
		if(wcd->GetController() == controller){
			return wcd;
		}
	}

	return nullptr;
}


void dewiDeviceManager::LogDevices() const{
	const int count = pDevices.GetCount();
	int i;
	
	pModule.LogInfo("Input Devices:");
	
	for(i=0; i<count; i++){
		LogDevice(*((dewiDevice*)pDevices.GetAt(i)));
	}
}

void dewiDeviceManager::LogDevice(const dewiDevice &device) const{
	pModule.LogInfoFormat("- '%s' (%s) [%d]", device.GetName().GetString(),
		device.GetID().GetString(), device.GetType());
		
	const int axisCount = device.GetAxisCount();
	int i;
	if(axisCount > 0){
		pModule.LogInfo("  Axes:");
		for(i=0; i<axisCount; i++){
			const dewiDeviceAxis &axis = *device.GetAxisAt(i);
			pModule.LogInfoFormat("    - '%s' (%s)[%d] %d .. %d [%d %d]",
				axis.GetName().GetString(), axis.GetID().GetString(), axis.GetType(),
				axis.GetMinimum(), axis.GetMaximum(), axis.GetFuzz(), axis.GetFlat());
		}
	}
	
	if(device.GetType() == deInputDevice::edtKeyboard){
		pModule.LogInfoFormat("  Buttons: %d", device.GetButtonCount());

	}else{
		const int buttonCount = device.GetButtonCount();
		if(buttonCount > 0){
			pModule.LogInfo("  Buttons:");
			for(i=0; i<buttonCount; i++){
				const dewiDeviceButton &button = *device.GetButtonAt(i);
				pModule.LogInfoFormat("    - '%s' (%s)[%d] %d => %d",
					button.GetName().GetString(), button.GetID().GetString(),
					button.GetType(), button.GetWICode(), i);
			}
		}
	}
}



void dewiDeviceManager::Update(){
	pProcessAddRemoveDevices();

	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((dewiDevice*)pDevices.GetAt(i))->Update();
	}
}



decString dewiDeviceManager::NormalizeID(const char *id){
	DEASSERT_NOTNULL(id)
	
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

dewiDeviceManager::sEventHandlerController::sEventHandlerController(dewiDeviceManager *manager) :
pManager(manager)
{
	wrgi::RawGameController::RawGameControllerAdded(
		{get_strong(), &sEventHandlerController::pOnControllerAdded});
	wrgi::RawGameController::RawGameControllerRemoved(
		{get_strong(), &sEventHandlerController::pOnControllerRemoved});
}

void dewiDeviceManager::sEventHandlerController::DropManager(){
	const deMutexGuard guard(pMutex);
	pManager = nullptr;
}

void dewiDeviceManager::sEventHandlerController::pOnControllerAdded(
wrf::IInspectable const &sender, wrgi::RawGameController const &controller){
	const deMutexGuard guard(pMutex);
	if(pManager){
		pManager->pOnControllerAdded(controller);
	}
}

void dewiDeviceManager::sEventHandlerController::pOnControllerRemoved(
wrf::IInspectable const &sender, wrgi::RawGameController const &controller){
	const deMutexGuard guard(pMutex);
	if(pManager){
		pManager->pOnControllerRemoved(controller);
	}
}



void dewiDeviceManager::pCleanUp(){
	if(pEventHandlerController){
		pEventHandlerController->DropManager();
		pEventHandlerController = nullptr;
	}
}

void dewiDeviceManager::pCreateDevices(){
	pMouse.TakeOver(new dewiDeviceMouse(pModule));
	pMouse->SetIndex(pDevices.GetCount());
	pDevices.Add(pMouse);
	
	pKeyboard.TakeOver(new dewiDeviceKeyboard(pModule));
	pKeyboard->SetIndex(pDevices.GetCount());
	pDevices.Add(pKeyboard);
}

void dewiDeviceManager::pCreateControllers(){
	winrt::init_apartment();
	
	const deMutexGuard guard(pMutex);
	for(wrgi::RawGameController const& controller : wrgi::RawGameController::RawGameControllers()){
		if(GetWithController(controller)){
			continue;
		}
		
		const dewiDeviceWinRTController::Ref device(dewiDeviceWinRTController::Ref::New(
			new dewiDeviceWinRTController(pModule, controller)));

		if(device->GetType() != deInputDevice::edtGeneric){
			device->SetIndex(pDevices.GetCount());
			pDevices.Add(device);
		}
	}

	pEventHandlerController = winrt::make_self<sEventHandlerController>(this);
}

void dewiDeviceManager::pProcessAddRemoveDevices(){
	const deMutexGuard guard(pMutex);
	bool changed = false;

	std::vector<wrgi::RawGameController>::const_iterator iter;
	for(iter=pRemoveControllers.cbegin(); iter!=pRemoveControllers.cend(); iter++){
		wrgi::RawGameController const &controller = *iter;

		dewiDevice * const device = GetWithController(controller);
		if(! device){
			continue;
		}

		pModule.LogInfoFormat("Controller removed: %s", device->GetName().GetString());

		pDevices.Remove(device);
		changed = true;
	}
	pRemoveControllers.clear();

	for(iter=pAddControllers.cbegin(); iter!=pAddControllers.cend(); iter++){
		wrgi::RawGameController const &controller = *iter;

		if(GetWithController(controller)){
			continue;
		}

		const dewiDeviceWinRTController::Ref device(dewiDeviceWinRTController::Ref::New(
			new dewiDeviceWinRTController(pModule, controller)));

		if(device->GetType() == deInputDevice::edtGeneric){
			continue;
		}

		device->SetIndex(pDevices.GetCount());

		pModule.LogInfoFormat("Controller added: %s", device->GetName().GetString());
		LogDevice(device);

		pDevices.Add(device);
		changed = true;
	}
	pAddControllers.clear();
	
	if(changed){
		pUpdateDeviceIndices();
		pModule.AddDevicesAttachedDetached(timeGetTime());
	}
}

void dewiDeviceManager::pUpdateDeviceIndices(){
	const int count = pDevices.GetCount();
	int i;
	for(i=0; i<count; i++){
		((dewiDevice*)pDevices.GetAt(i))->SetIndex(i);
	}
}

void dewiDeviceManager::pOnControllerAdded(wrgi::RawGameController const &controller){
	const deMutexGuard guard(pMutex);
	pAddControllers.push_back(controller);
}

void dewiDeviceManager::pOnControllerRemoved(wrgi::RawGameController const &controller){
	const deMutexGuard guard(pMutex);
	pRemoveControllers.push_back(controller);
}
