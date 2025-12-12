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

#include "deoxrDPHMD.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"
#include "../../deoxrFaceTracker.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPHMD
/////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPHMD::deoxrDPHMD(deoxrInstance &instance) :
deoxrDeviceProfile(instance, deoxrPath(instance, "/interaction_profiles/dragengine/hmd"), "HMD"){
}

deoxrDPHMD::~deoxrDPHMD(){
	pRemoveDevice();
}



// Management
///////////////

void deoxrDPHMD::CheckAttached(){
	deoxrSession * const session = GetInstance().GetOxr().GetSession();
	
	if(session){
		pAddDevice();
		
	}else{
		pRemoveDevice();
	}
}

void deoxrDPHMD::SuggestBindings(){
}

void deoxrDPHMD::ClearActions(){
	pRemoveDevice();
}



// Protected
//////////////

void deoxrDPHMD::pAddDevice(){
	if(pDevice){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	pDevice.TakeOver(new deoxrDevice(oxr, *this));
	pDevice->SetType(deInputDevice::edtVRHMD);
	pDevice->SetName("HMD");
	
	decString id;
	id.Format("%shmd", OXR_DEVID_PREFIX);
	pDevice->SetID(id);
	
	if(GetInstance().SupportsExtension(deoxrInstance::extHTCFacialTracking)){
		deoxrSession &session = oxr.GetSession();
		if(session.GetSystem().GetSupportsFaceEyeTracking()
		|| session.GetSystem().GetSupportsFaceLipTracking()){
			pDevice->SetFaceTracker(deoxrFaceTracker::Ref::New(session));
		}
	}
	
	oxr.GetDevices().Add(pDevice);
}

void deoxrDPHMD::pRemoveDevice(){
	if(!pDevice){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	oxr.GetDevices().Remove(pDevice);
	pDevice = nullptr;
}
