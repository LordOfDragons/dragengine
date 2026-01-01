/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deoxrDPMndxDevSpace.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>


// Class deoxrDPMndxDevSpace::Device
//////////////////////////////////////

deoxrDPMndxDevSpace::Device::Device(deoxrDPMndxDevSpace &aprofile,
	const XrXDevPropertiesMNDX &info, XrXDevIdMNDX aid) :
profile(aprofile),
name(info.name),
serial(info.serial),
id(aid){
}

deoxrDPMndxDevSpace::Device::~Device(){
	if(device){
		profile.GetInstance().GetOxr().GetDevices().Remove(device);
	}
}


// Class deoxrDPMndxDevSpace
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPMndxDevSpace::deoxrDPMndxDevSpace(deoxrInstance &instance) :
deoxrDeviceProfile(instance, {}, "Mndx XDev Space"),
pTimeoutCheckAttached(3.0f),
pCurGeneration{}{
}


// Management
///////////////

void deoxrDPMndxDevSpace::OnSessionStateChanged(){
	if(GetInstance().GetOxr().GetSessionState() == XR_SESSION_STATE_FOCUSED){
		pTimerCheckAttached.Reset();
		pTimeoutCheckAttached = 0.0f;
	}
}

void deoxrDPMndxDevSpace::OnActionsSynced(){
	if(GetInstance().GetOxr().GetSessionState() != XR_SESSION_STATE_FOCUSED){
		return;
	}
	
	pTimeoutCheckAttached -= pTimerCheckAttached.GetElapsedTime();
	if(pTimeoutCheckAttached > 0.0f){
		return;
	}
	
	pTimeoutCheckAttached = 3.0f;
	CheckAttached();
}

void deoxrDPMndxDevSpace::CheckAttached(){
	const deoxrInstance &instance = GetInstance();
	if(!instance.SupportsExtension(deoxrInstance::extMNDXXDevSpace)){
		return;
	}
	
	deoxrSession &session = *pGetSession();
	if(!session.GetRunning()){
		pDevices.RemoveAll();
		return;
	}
	
	XrXDevListMNDX list = XR_NULL_HANDLE;
	XrXDevIdMNDX *xdevs = nullptr;
	
	try{
		XrCreateXDevListInfoMNDX createInfo = {XR_TYPE_CREATE_XDEV_LIST_INFO_MNDX};
		OXR_CHECK(instance.xrCreateXDevListMNDX(session.GetSession(), &createInfo, &list));
		
		uint64_t generation{};
		OXR_CHECK(instance.xrGetXDevListGenerationNumberMNDX(list, &generation));
		if(generation == pCurGeneration){
			instance.xrDestroyXDevListMNDX(list);
			return;
		}
		
		pCurGeneration = generation;
		
		deVROpenXR &oxr = instance.GetOxr();
		oxr.LogInfo("XDevSpace: List changed");
		uint32_t i, count = 0;
		OXR_CHECK(instance.xrEnumerateXDevsMNDX(list, 0, &count, nullptr));
		
		xdevs = new XrXDevIdMNDX[count];
		OXR_CHECK(instance.xrEnumerateXDevsMNDX(list, count, &count, xdevs));
		
		const decObjectOrderedSet oldDevices(pDevices);
		pDevices.RemoveAll();
		
		oxr.LogInfoFormat("XDevSpace: %d devices:", (int)count);
		for(i=0; i<count; i++){
			XrGetXDevInfoMNDX info{XR_TYPE_GET_XDEV_INFO_MNDX};
			info.id = xdevs[i];
			
			XrXDevPropertiesMNDX props = {XR_TYPE_XDEV_PROPERTIES_MNDX};
			OXR_CHECK(instance.xrGetXDevPropertiesMNDX(list, &info, &props));
			oxr.LogInfoFormat("- %d: name='%s' serial='%s' space=%c",
				(int)i, props.name, props.serial, props.canCreateSpace == XR_TRUE ? 'y' : 'n');
			
			if(!props.canCreateSpace){
				continue;
			}
			
			//Device * const oldDevice = pFindDeviceById(oldDevices, xdevs[i]);
			Device * const oldDevice = pFindDeviceBySerial(oldDevices, props.serial);
			if(oldDevice){
				pDevices.Add(oldDevice);
				continue;
			}
			
			const Device::Ref device(Device::Ref::New(*this, props, xdevs[i]));
			const decString serialLower(device->serial.GetLower());
			const decString nameLower(device->name.GetLower());
			
			if(serialLower.BeginsWith("lhr-") && nameLower.FindString("tracker") != -1){
				pAddDeviceTracker(list, device);
			}
		}
		
		instance.xrDestroyXDevListMNDX(list);
		
	}catch(const deException &e){
		if(xdevs){
			delete[] xdevs;
		}
		if(list != XR_NULL_HANDLE){
			instance.xrDestroyXDevListMNDX(list);
		}
		return;
	}
}

void deoxrDPMndxDevSpace::SuggestBindings(){
}

void deoxrDPMndxDevSpace::OnSessionEnd(){
	pDevices.RemoveAll();
}


// Private Functions
//////////////////////

deoxrDPMndxDevSpace::Device *deoxrDPMndxDevSpace::pFindDeviceById(XrXDevIdMNDX id) const{
	return pFindDeviceById(pDevices, id);
}

deoxrDPMndxDevSpace::Device *deoxrDPMndxDevSpace::pFindDeviceById(
const decObjectOrderedSet & list, XrXDevIdMNDX id) const{
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		Device * const device = (Device*)list.GetAt(i);
		if(device->id == id){
			return device;
		}
	}
	
	return nullptr;
}

deoxrDPMndxDevSpace::Device *deoxrDPMndxDevSpace::pFindDeviceBySerial(
const decObjectOrderedSet &list, const char *serial) const{
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		Device * const device = (Device*)list.GetAt(i);
		if(device->serial == serial){
			return device;
		}
	}
	
	return nullptr;
}

decString deoxrDPMndxDevSpace::pSanitizedSerial(const decString &serial) const{
	const int len = serial.GetLength();
	decString sanitized;
	int i;
	
	sanitized.Set(0, len);
	const char * const a = serial.GetString();
	char * const b = sanitized.GetMutableString();
	
	for(i=0; i<len; i++){
		const char c = a[i];
		if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-'){
			b[i] = c;
			
		}else{
			b[i] = '_';
		}
	}
	
	return sanitized;
}

void deoxrDPMndxDevSpace::pAddDeviceTracker(XrXDevListMNDX list, const Device::Ref &device){
	const deoxrInstance &instance = GetInstance();
	deoxrSession &session = *pGetSession();
	deVROpenXR &oxr = instance.GetOxr();
	
	device->device = deoxrDevice::Ref::New(oxr, *this);
	
	device->device->SetType(deInputDevice::edtVRTracker);
	device->device->SetName(device->name);
	
	decString id;
	id.Format("%sxds_%s", OXR_DEVID_PREFIX, pSanitizedSerial(device->serial).GetString());
	device->device->SetID(id);
	
	XrCreateXDevSpaceInfoMNDX createSpaceInfo{XR_TYPE_CREATE_XDEV_SPACE_INFO_MNDX};
	createSpaceInfo.xdevList = list;
	createSpaceInfo.offset = {{0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
	createSpaceInfo.id = device->id;
	
	XrSpace space = XR_NULL_HANDLE;
	OXR_CHECK(instance.xrCreateXDevSpaceMNDX(session.GetSession(), &createSpaceInfo, &space));
	device->device->SetSpacePose(deoxrSpace::Ref::New(session, space));
	
	oxr.GetDevices().Add(device->device);
	pDevices.Add(device);
}
