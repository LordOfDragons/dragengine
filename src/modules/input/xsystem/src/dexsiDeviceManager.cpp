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
#include <linux/limits.h>

#include "dexsiDevice.h"
#include "dexsiDeviceAxis.h"
#include "dexsiDeviceButton.h"
#include "dexsiDeviceFeedback.h"
#include "dexsiDeviceManager.h"
#include "dexsiDeviceCoreMouse.h"
#include "dexsiDeviceCoreKeyboard.h"
#include "dexsiDeviceLibEvent.h"
#include "deXSystemInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class dexsiDeviceManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiDeviceManager::dexsiDeviceManager(deXSystemInput &module) :
pModule(module),
pInotifyFd(-1),
pInotifyWatchEvdev(-1),
pInotifyBufferLen(1024 * (sizeof(inotify_event) + 16)),
pInotifyBuffer(new uint8_t[pInotifyBufferLen]),
pTimeoutDelayProbeDevices(0.0f)
{
	try{
		pCreateDevices();
		pFindPrimaryDevices();
		pStartWatchEvdev();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dexsiDeviceManager::~dexsiDeviceManager(){
	pCleanUp();
}



// Management
///////////////

void dexsiDeviceManager::UpdateDeviceList(){
}



int dexsiDeviceManager::GetCount() const{
	return pDevices.GetCount();
}

dexsiDevice *dexsiDeviceManager::GetAt(int index) const{
	return (dexsiDevice*)pDevices.GetAt(index);
}

dexsiDevice *dexsiDeviceManager::GetWithID(const char *id){
	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dexsiDevice * const device = (dexsiDevice*)pDevices.GetAt(i);
		if(device->GetID() == id){
			return device;
		}
	}
	
	return NULL;
}

int dexsiDeviceManager::IndexOfWithID(const char *id){
	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dexsiDevice * const device = (dexsiDevice*)pDevices.GetAt(i);
		if(device->GetID() == id){
			return i;
		}
	}
	
	return -1;
}



void dexsiDeviceManager::Update(){
	pUpdateDelayProbeDevices();
	pUpdateWatchEvdev();
	
	const int deviceCount = pDevices.GetCount();
	int i;
	for(i=0; i<deviceCount; i++){
		((dexsiDevice*)pDevices.GetAt(i))->Update();
	}
}

void dexsiDeviceManager::LogDevices(){
	pModule.LogInfo("Input Devices:");
	
	const int count = pDevices.GetCount();
	int i;
	for(i=0; i<count; i++){
		LogDevice(*((dexsiDevice*)pDevices.GetAt(i)));
	}
}

void dexsiDeviceManager::LogDevice(const dexsiDevice &device){
	pModule.LogInfoFormat("- '%s' (%s) [%d]", device.GetName().GetString(),
		device.GetID().GetString(), device.GetType());
	
	const int axisCount = device.GetAxisCount();
	int i;
	if(axisCount > 0){
		pModule.LogInfo("  Axes:");
		for(i=0; i<axisCount; i++){
			const dexsiDeviceAxis &axis = *device.GetAxisAt(i);
			pModule.LogInfoFormat("    - '%s' (%s)[%d] %d .. %d [%d %d]",
				axis.GetName().GetString(), axis.GetID().GetString(), axis.GetType(),
				axis.GetMinimum(), axis.GetMaximum(), axis.GetFuzz(), axis.GetFlat());
		}
	}
	
	const int buttonCount = device.GetButtonCount();
	if(buttonCount > 0){
		if(device.GetType() == deInputDevice::edtKeyboard){
			pModule.LogInfoFormat("  Keys: %d", buttonCount);
			
		}else{
			pModule.LogInfo("  Buttons:");
			for(i=0; i<buttonCount; i++){
				const dexsiDeviceButton &button = *device.GetButtonAt(i);
				pModule.LogInfoFormat("    - '%s' (%s)[%d] %d => %d",
					button.GetName().GetString(), button.GetID().GetString(),
					button.GetType(), button.GetEvdevCode(), i);
			}
		}
	}
}



decString dexsiDeviceManager::NormalizeID(const char *id){
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

void dexsiDeviceManager::pCleanUp(){
	pStopWatchEvdev();
	delete [] pInotifyBuffer;
}



// general extension information: // https://www.x.org/wiki/guide/extensions

void dexsiDeviceManager::pCreateXInputDevices(){
	// from XInput extension add what is not core
	// 
	// NOTE core mouse and keyboard parameters can only be queried using XInput
	// 
	// PROBLEM for some reason this doesn't work and causes segfaults. documentation is
	//         complicated and does not give correct information on how this is all
	//         supposed to work.
	// 
	// possible solution is XInput2 which might be better designed than the previous version
	// https://www.x.org/releases/X11R7.7/doc/inputproto/XI2proto.txt
	return;
	
	// check if XInput in the required version is present
	Display * const display = pModule.GetOSUnix()->GetDisplay();
	
	int extOpcode = -1;
	int extEvent;
	int extError;
	if(!XQueryExtension(display, "XInputExtension", &extOpcode, &extEvent, &extError)){
		DETHROW(deeInvalidParam);
	}
	
	int extOpcodeMajor = 2;
	int extOpcodeMinor = 2;
	if(XIQueryVersion(display, &extOpcodeMajor, &extOpcodeMinor) != Success){
		DETHROW(deeInvalidParam);
	}
	
	// what the?! using XIQueryDevice causes XCloseDisplay to segfault. happens on both
	// XInput and XInput2. is there some hidden magic required to not segfault?
	int countDevices;
	XIDeviceInfo * const devices = XIQueryDevice(display, XIAllDevices, &countDevices); // XIAllMasterDevices
	
	
	XIFreeDeviceInfo(devices);
	
/*
typedef struct{
	int                 deviceid;
	char                *name;
	int                 use;
	int                 attachment;
	Bool                enabled;
	int                 num_classes;
	XIAnyClassInfo      **classes;
} XIDeviceInfo;

The use and attachment fields specify the type of the device
and the current attachment or pairing.
- If use is XIMasterPointer, the device is a master pointer and
  attachment specifies the deviceid of the paired master
  keyboard.
- If use is XIMasterKeyboard, the device is a master keyboard,
  and the attachment field specifies the paired master pointer.
- If use is XISlavePointer, the device is a slave device and
  currently attached to the master pointer specified in
  attachement.
- If use is XISlaveKeyboard, the device is a slave device an
  currently attached to the master keyboard specified in
  attachment.
- If use is XIFloatingSlave, the device is a slave device
  currently not attached to any master device. The value of the
  attachment field for floating slave devices is undefined.

typedef struct{
	int         type;
	int         sourceid;
} XIAnyClassInfo;



A device may have zero or one XIButtonClass, denoting the
device's capability to send button events.

typedef struct{
    int           mask_len;
    unsigned char *mask;
} XIButtonState;

typedef struct{
    int         type;
    int         sourceid;
    int         num_buttons;
    Atom        *labels;
    XIButtonState state;
} XIButtonClassInfo;

labels is a list of num_buttons Atoms specifying the button
labels for this device. If the label is not None, then the
label specifies the type of button in physical device order
(i.e. as the buttons are numbered on the physical input
device).



A device may have zero or one XIKeyClass, denoting the device's
capability to send key events.

typedef struct{
    int         type;
    int         sourceid;
    int         num_keycodes;
    int         *keycodes;
} XIKeyClassInfo;

// RPTD: should be possible to convert KeyCode to KeySym then use XKeysymToString



A device may have zero or more XIValuatorClass, denoting the
device's capability to send coordinates.

typedef struct{
    int         type;
    int         sourceid;
    int         number;
    Atom        label;
    double      min;
    double      max;
    double      value;
    int         resolution;
    int         mode;
} XIValuatorInfo;

If the label field is not None, the value of label is an Atom
describing the axis.

min and max are the minimum and maximum values allowed on this
axis. If both are zero, no minumum or maximum values are set on
this device. value is the current value of this axis.

The resolution field specifies the resolution of the device in
units/m.

The mode specifies the mode of this axis. If the mode is
XIModeAbsolute this axis sends absolute coordinates. If the
mode is XIModeRelative, this device sends relative coordinates.
*/
	
	
	
	
#if 0
	Display * const display = pModule.GetOSUnix()->GetDisplay();
	XDeviceInfo *xdevices = NULL;
	dexsiDevice::Ref device;
	int i, countDevices;
	
	try{
		xdevices = XListInputDevices(display, &countDevices);
		if(!xdevices){
			return;
		}
		
		for(i=0; i<countDevices; i++){
			if(xdevices[i].use == IsXPointer){
				pModule.LogInfo("  -> Core Pointer");
				
			}else if(xdevices[i].use == IsXKeyboard){
				pModule.LogInfo("  -> Core Keyboard");
				
			}else if(xdevices[i].use == IsXExtensionDevice){
				pModule.LogInfo("  -> Extension Device");
			}
			// device can only be added if it is core or extension device. all other uses
			// are not useful since we can not open the device
			
			device.TakeOverWith(pModule, xdevices[i]);
		}
		
	}catch(const deException &){
		if(xdevices){
			XFreeDeviceList(xdevices);
		}
		throw;
	}
	
	XFreeDeviceList(xdevices);
#endif
	
	// old interface: joystick interface
	// /dev/input/jsX => include linux/joystick,h
	// https://www.kernel.org/doc/Documentation/input/joystick-api.txt
}

void dexsiDeviceManager::pCreateEvdevDevices(){
	// from libevdev add gamepads and joysticks
	// https://www.freedesktop.org/software/libevdev/doc/latest/index.html
	// SDL type mapping: https://meghprkh.github.io/blog/2016/06/03/Handling-joysticks-and-gamepads-in-linux/
	const char * const basePath[] = {"/dev/input/event%d", "/dev/event%d"};
	decString pathDevice;
	int i, bp;
	
	for(bp=0; bp<2; bp++){
		for(i=0; i<32; i++){
			pathDevice.Format(basePath[bp], i);
			
			try{
				const dexsiDeviceLibEvent::Ref device(dexsiDeviceLibEvent::Ref::New(
						new dexsiDeviceLibEvent(pModule, pathDevice)));
				
				// devices without buttons and axes are not interesting to us. this weeds out
				// non-input devices like audio devices
				if(device->GetButtonCount() == 0 && device->GetAxisCount() == 0){
					continue;
				}
				
				// we can end up with duplicates for whatever reason
				if(GetWithID(device->GetID())){
					continue;
				}
				
				switch(device->GetType()){
				case deInputDevice::edtGamepad:
				case deInputDevice::edtTouchpad:
					device->SetIndex(pDevices.GetCount());
					pDevices.Add(device);
					break;
					
				default:
					break;
				}
				
			}catch(const deException &){
				// ignore
			}
		}
	}
}



void dexsiDeviceManager::pCreateDevices(){
	pX11CoreMouse.TakeOver(new dexsiDeviceCoreMouse(pModule));
	pX11CoreMouse->SetIndex(pDevices.GetCount());
	pDevices.Add(pX11CoreMouse);
	
	pX11CoreKeyboard.TakeOver(new dexsiDeviceCoreKeyboard(pModule));
	pX11CoreKeyboard->SetIndex(pDevices.GetCount());
	pDevices.Add(pX11CoreKeyboard);
	
	pCreateXInputDevices();
	pCreateEvdevDevices();
}

void dexsiDeviceManager::pFindPrimaryDevices(){
	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dexsiDevice * const device = (dexsiDevice*)pDevices.GetAt(i);
		
		switch(device->GetType()){
		case deInputDevice::edtMouse:
			if(!pPrimaryMouse){
				pPrimaryMouse = device;
			}
			break;
			
		case deInputDevice::edtKeyboard:
			if(!pPrimaryKeyboard){
				pPrimaryKeyboard = device;
			}
			break;
			
		default:
			break;
		}
	}
	
	if(!pPrimaryMouse){
		pModule.LogInfo("No mouse device found");
	}
	if(!pPrimaryKeyboard){
		pModule.LogInfo("No keyboard device found");
	}
}

void dexsiDeviceManager::pStartWatchEvdev(){
	pInotifyFd = inotify_init1(IN_NONBLOCK);
	if(pInotifyFd == -1){
		pModule.LogWarn("Failed monitoring event device directory (1)");
		return;
	}
	
	pInotifyWatchEvdev = inotify_add_watch(pInotifyFd, "/dev/input", IN_CREATE | IN_DELETE);
	if(pInotifyWatchEvdev == -1){
		pModule.LogWarn("Failed monitoring event device directory (2)");
		return;
	}
	
	pModule.LogInfo("Watching event device directory");
}

void dexsiDeviceManager::pStopWatchEvdev(){
	if(pInotifyWatchEvdev >= 0){
		inotify_rm_watch(pInotifyFd, pInotifyWatchEvdev);
		pInotifyWatchEvdev = -1;
	}
	
	if(pInotifyFd >= 0){
		close(pInotifyFd);
		pInotifyFd = -1;
	}
}

void dexsiDeviceManager::pUpdateWatchEvdev(){
	const ssize_t length = read(pInotifyFd, pInotifyBuffer, pInotifyBufferLen);
	if(length <= 0){
		return;
	}
	
	bool devicesChanged = false;
	ssize_t position = 0;
	
	while(position < length){
		const inotify_event &event = *((inotify_event*)(pInotifyBuffer + position));
		
		if(event.len > 0){
			if((event.mask & IN_CREATE) == IN_CREATE){
				if((event.mask & IN_ISDIR) != IN_ISDIR){
					const decString fname(event.name);
					if(fname.BeginsWith("event")){
						pEvdevAppeared(decString("/dev/input/") + fname);
					}
				}
				
			}else if((event.mask & IN_DELETE) == IN_DELETE){
				if((event.mask & IN_ISDIR) != IN_ISDIR){
					const decString fname(event.name);
					if(fname.BeginsWith("event")){
						devicesChanged |= pEvdevDisappeared(decString("/dev/input/") + fname);
					}
				}
			}
		}
		
		position += sizeof(inotify_event) + event.len;
	}
	
	if(devicesChanged){
		pUpdateDeviceIndices();
		
		timeval eventTime;
		gettimeofday(&eventTime, NULL);
		pModule.AddDeviceAttachedDetached(eventTime);
	}
}

void dexsiDeviceManager::pEvdevAppeared(const decString &path){
	pModule.LogInfoFormat("Event device file appeared: %s", path.GetString());
	
	if(!pDelayProbeDevices.Has(path)){
		pDelayProbeDevices.Add(path);
		pTimeoutDelayProbeDevices = 2.0f;
		pTimerDelayProbeDevices.Reset();
	}
}

bool dexsiDeviceManager::pEvdevDisappeared(const decString &path){
	pModule.LogInfoFormat("Event device file disappeared: %s", path.GetString());
	
	const int count = pDevices.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dexsiDevice * const device = ((dexsiDevice*)pDevices.GetAt(i));
		if(device->GetSource() != dexsiDevice::esLibevdev){
			continue;
		}
		
		const dexsiDeviceLibEvent &evd = *(dexsiDeviceLibEvent*)device;
		if(evd.GetEvdevPath() != path){
			continue;
		}
		
		pModule.LogInfoFormat("Device deatached: %s", evd.GetID().GetString());
		pDevices.Remove(device);
		return true;
	}
	
	return false;
}



void dexsiDeviceManager::pUpdateDelayProbeDevices(){
	const int count = pDelayProbeDevices.GetCount();
	if(count == 0){
		return;
	}
	
	pTimeoutDelayProbeDevices -= pTimerDelayProbeDevices.GetElapsedTime();
	if(pTimeoutDelayProbeDevices > 0.0f){
		return;
	}
	
	pTimeoutDelayProbeDevices = 0.0f;
	
	bool devicesChanged = false;
	int i;
	
	for(i=0; i<count; i++){
		devicesChanged |= pProbeDevice(pDelayProbeDevices.GetAt(i));
	}
	pDelayProbeDevices.RemoveAll();
	
	if(devicesChanged){
		timeval eventTime;
		gettimeofday(&eventTime, NULL);
		pModule.AddDeviceAttachedDetached(eventTime);
	}
}

bool dexsiDeviceManager::pProbeDevice(const decString &path){
	pModule.LogInfoFormat("Probing event device file: %s", path.GetString());
	
	try{
		const dexsiDeviceLibEvent::Ref device(dexsiDeviceLibEvent::Ref::New(
			new dexsiDeviceLibEvent(pModule, path)));
		
		switch(device->GetType()){
		case deInputDevice::edtGamepad:
			device->SetIndex(pDevices.GetCount());
			pDevices.Add(device);
			pModule.LogInfoFormat("Device attached: %s", device->GetID().GetString());
			LogDevice(device);
			return true;
			
		default:
			pModule.LogInfoFormat("Unsupported device type: %d", device->GetType());
			break;
		}
		
	}catch(const deException &e){
		pModule.LogException(e);
	}
	
	return false;
}

void dexsiDeviceManager::pUpdateDeviceIndices(){
	const int count = pDevices.GetCount();
	int i;
	for(i=0; i<count; i++){
		((dexsiDevice*)pDevices.GetAt(i))->SetIndex(i);
	}
}
