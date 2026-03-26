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

#include "deoxrDPBodyTracker.h"
#include "../../deoxrInstance.h"
#include "../../deoxrSystem.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPBodyTracker
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPBodyTracker::deoxrDPBodyTracker(deoxrInstance &instance) :
deoxrDeviceProfile(instance, deoxrPath(), "Body Tracker"){
}

deoxrDPBodyTracker::~deoxrDPBodyTracker() = default;



// Management
///////////////

void deoxrDPBodyTracker::CheckAttached(){
	if(GetInstance().SupportsExtension(deoxrInstance::extFBBodyTracking)
	|| GetInstance().SupportsExtension(deoxrInstance::extMETABodyTrackingFullBody)){
		pAddDevice();
		
	}else{
		pRemoveDevice();
	}
}

void deoxrDPBodyTracker::SuggestBindings(){
}

void deoxrDPBodyTracker::ClearActions(){
	pRemoveDevice();
}



// Private Functions
//////////////////////

void deoxrDPBodyTracker::pAddDevice(){
	if(pDevice){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	pDevice = deoxrDevice::Ref::New(oxr, *this);
	
	decString id;
	
	pDevice->SetType(deInputDevice::edtVRBodyTracker);
	pDevice->SetName("Body Tracker");
	id.Format("%sbt", OXR_DEVID_PREFIX);
	pDevice->SetID(id);
	
	pAddBodyTracker(pDevice);
	
	oxr.GetDevices().Add(pDevice);
}

void deoxrDPBodyTracker::pRemoveDevice(){
	if(!pDevice){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	oxr.GetDevices().Remove(pDevice);
	pDevice.Clear();
}
