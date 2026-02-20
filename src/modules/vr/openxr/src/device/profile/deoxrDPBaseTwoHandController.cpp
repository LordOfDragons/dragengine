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

#include "deoxrDPBaseTwoHandController.h"
#include "../deoxrDeviceAxis.h"
#include "../deoxrDeviceButton.h"
#include "../deoxrDeviceComponent.h"
#include "../deoxrDeviceFeedback.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPBaseTwoHandController
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPBaseTwoHandController::deoxrDPBaseTwoHandController(
	deoxrInstance &instance, const deoxrPath &path, const char *name) :
deoxrDeviceProfile(instance, path, name),
pPathHandLeft(instance.GetPathHandLeft()),
pPathHandRight(instance.GetPathHandRight()){
}

deoxrDPBaseTwoHandController::~deoxrDPBaseTwoHandController(){
	pRemoveDevice(true);
	pRemoveDevice(false);
}



// Management
///////////////

void deoxrDPBaseTwoHandController::CheckAttached(){
	const bool enabled = pProfileEnabled();
	
	if(enabled && pMatchesProfile(pPathHandLeft)){
		if(!pDeviceLeft){
			GetInstance().GetOxr().GetDeviceProfiles().RemoveDevice(deInputDevice::edtVRLeftHand);
		}
		
		pAddDevice(true);
		
	}else{
		pRemoveDevice(true);
	}
	
	if(enabled && pMatchesProfile(pPathHandRight)){
		if(!pDeviceRight){
			GetInstance().GetOxr().GetDeviceProfiles().RemoveDevice(deInputDevice::edtVRRightHand);
		}
		
		pAddDevice(false);
		
	}else{
		pRemoveDevice(false);
	}
}

void deoxrDPBaseTwoHandController::SuggestBindings(){
	if(pProfileEnabled()){
		pSuggestBindings();
	}
}

void deoxrDPBaseTwoHandController::ClearActions(){
	pRemoveDevice(true);
	pRemoveDevice(false);
}

void deoxrDPBaseTwoHandController::RemoveDevice(deInputDevice::eDeviceTypes type){
	if(type == deInputDevice::edtVRLeftHand && pDeviceLeft){
		pRemoveDevice(true);
	}
	if(type == deInputDevice::edtVRRightHand && pDeviceRight){
		pRemoveDevice(false);
	}
}



// Private Functions
//////////////////////

bool deoxrDPBaseTwoHandController::pProfileEnabled() const{
	return true;
}

void deoxrDPBaseTwoHandController::pRemoveDevice(bool left){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if(!device){
		return;
	}
	
	GetInstance().GetOxr().GetDevices().Remove(device);
	device = nullptr;
}
