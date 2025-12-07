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

#include "deoxrDeviceProfile.h"
#include "deoxrDeviceProfileManager.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDeviceProfileManager
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDeviceProfileManager::deoxrDeviceProfileManager(){
}

deoxrDeviceProfileManager::~deoxrDeviceProfileManager(){
}



// Management
///////////////

int deoxrDeviceProfileManager::GetCount() const{
	return pProfiles.GetCount();
}

deoxrDeviceProfile *deoxrDeviceProfileManager::GetAt(int index) const{
	return (deoxrDeviceProfile*)pProfiles.GetAt(index);
}

void deoxrDeviceProfileManager::Add(deoxrDeviceProfile *profile){
	if(! profile){
		DETHROW_INFO(deeNullPointer, "profile");
	}
	pProfiles.Add(profile);
}

void deoxrDeviceProfileManager::RemoveAll(){
	pProfiles.RemoveAll();
}

void deoxrDeviceProfileManager::AllOnActionsSynced(){
	const int count = pProfiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoxrDeviceProfile*)pProfiles.GetAt(i))->OnActionsSynced();
	}
}

void deoxrDeviceProfileManager::AllOnSessionStateChanged(){
	const int count = pProfiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoxrDeviceProfile*)pProfiles.GetAt(i))->OnSessionStateChanged();
	}
}

void deoxrDeviceProfileManager::CheckAllAttached(){
	const int count = pProfiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoxrDeviceProfile*)pProfiles.GetAt(i))->CheckAttached();
	}
}

void deoxrDeviceProfileManager::ClearActions(){
	const int count = pProfiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoxrDeviceProfile*)pProfiles.GetAt(i))->ClearActions();
	}
}

void deoxrDeviceProfileManager::RemoveDevice(deInputDevice::eDeviceTypes type){
	const int count = pProfiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoxrDeviceProfile*)pProfiles.GetAt(i))->RemoveDevice(type);
	}
}

void deoxrDeviceProfileManager::OnSessionEnd(){
	const int count = pProfiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoxrDeviceProfile*)pProfiles.GetAt(i))->OnSessionEnd();
	}
}
