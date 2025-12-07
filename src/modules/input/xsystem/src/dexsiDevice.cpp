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
#include <errno.h>

#include "dexsiDevice.h"
#include "dexsiDeviceAxis.h"
#include "dexsiDeviceButton.h"
#include "dexsiDeviceFeedback.h"
#include "dexsiDeviceManager.h"
#include "deXSystemInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/systems/deInputSystem.h>



// Class dexsiDevice
/////////////////////////////

Atom dexsiDevice::pXDeviceTypeAtoms[dexsiDevice::exdtOther];
bool dexsiDevice::pXDeviceTypeAtomsReady = false;

// Constructor, destructor
////////////////////////////

dexsiDevice::dexsiDevice(deXSystemInput &module, eSources source) :
pModule(module),
pIndex(-1),
pSource(source),
pType(deInputDevice::edtMouse),

pXi2Id(0),
pXDeviceType(exdtOther),

pX11FirstKeyCode(0),
pX11KeyCodeCount(0),
pX11KeyCodeMap(NULL),

pDirtyAxesValues(false){
}

dexsiDevice::dexsiDevice(deXSystemInput &module, const XIDeviceInfo &info) :
pModule(module),
pIndex(-1),
pSource(esXInput),
pType(deInputDevice::edtMouse),
pID(decString(XINP_DEVID_PREFIX) + dexsiDeviceManager::NormalizeID(info.name)),
pName(info.name),

pXi2Id(info.deviceid),
pXDeviceType(exdtOther),

pX11FirstKeyCode(0),
pX11KeyCodeCount(0),
pX11KeyCodeMap(NULL),

pDirtyAxesValues(false)
{
#if 0
	pPrepareXDeviceTypeAtoms();
	
	int i;
	for(i=exdtKeyboard; i<exdtOther; i++){
		if(info.type == pXDeviceTypeAtoms[i]){
			pXDeviceType = (eXDeviceTypes)i;
			break;
		}
	}
	
	module.LogInfoFormat("Device: xid=%d id='%s' name='%s' type=%d use=%d", pXi2Id,
		pID.GetString(), pName.GetString(), pXDeviceType, info.use);
	
	const char *ptr = (const char*)info.inputclassinfo;
	for(i=0; i<info.num_classes; i++){
		const XAnyClassInfo &cinfo = *((const XAnyClassInfo *)ptr);
		module.LogInfoFormat("  Class %d: %d (%d)", i, cinfo.c_class, cinfo.length);
		
		if(cinfo.c_class == KeyClass){
			const XKeyInfo &keyinfo = (const XKeyInfo &)cinfo;
			module.LogInfoFormat("    keys=%d codes=%d-%d", keyinfo.num_keys,
				keyinfo.min_keycode, keyinfo.max_keycode);
			
		}else if(cinfo.c_class == ButtonClass){
			const XButtonInfo &buttoninfo = (const XButtonInfo &)cinfo;
			module.LogInfoFormat("    buttons=%d", buttoninfo.num_buttons);
			
		}else if(cinfo.c_class == ValuatorClass){
			const XValuatorInfo &valuatorinfo = (const XValuatorInfo &)cinfo;
			int j;
			for(j=0; j<valuatorinfo.num_axes; j++){
				module.LogInfoFormat("    axis %d: min=%d max=%d", j,
					valuatorinfo.axes[j].min_value, valuatorinfo.axes[j].max_value);
			}
		}
		
		ptr += cinfo.length;
	}
#endif
}

// old interface: joystick interface
// /dev/input/jsX => include linux/joystick,h
// https://www.kernel.org/doc/Documentation/input/joystick-api.txt

dexsiDevice::~dexsiDevice(){
	if(pX11KeyCodeMap){
		delete [] pX11KeyCodeMap;
	}
}



// Management
///////////////

void dexsiDevice::SetIndex(int index){
	pIndex = index;
}

void dexsiDevice::SetType(deInputDevice::eDeviceTypes type){
	pType = type;
}

void dexsiDevice::SetID(const char *id){
	pID = id;
}

void dexsiDevice::SetName(const char *name){
	pName = name;
}

void dexsiDevice::SetDisplayImages(const char *name){
	pDisplayImage = nullptr;
	pDisplayIcons.RemoveAll();
	
	if(!name){
		return;
	}
	
	deImageManager &imageManager = *pModule.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pModule.GetVFS();
	const char * const basePath = "/share/image/device";
	decString filename;
	
	filename.Format("%s/%s/image.png", basePath, name);
	pDisplayImage.TakeOver(imageManager.LoadImage(vfs, filename, "/"));
	
	const int sizes[4] = {128, 64, 32, 16};
	deImage::Ref icon;
	int i;
	
	for(i=0; i<4; i++){
		filename.Format("%s/%s/icon%d.png", basePath, name, sizes[i]);
		icon.TakeOver(imageManager.LoadImage(vfs, filename, "/"));
		pDisplayIcons.Add((deImage*)icon);
	}
}

void dexsiDevice::SetDisplayText(const char *text){
	pDisplayText = text;
}



int dexsiDevice::GetButtonCount() const{
	return pButtons.GetCount();
}

void dexsiDevice::AddButton(dexsiDeviceButton *button){
	DEASSERT_NOTNULL(button)
	pButtons.Add(button);
}

dexsiDeviceButton *dexsiDevice::GetButtonAt(int index) const{
	return (dexsiDeviceButton*)pButtons.GetAt(index);
}

dexsiDeviceButton *dexsiDevice::GetButtonWithID(const char *id) const{
	const int count = pButtons.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dexsiDeviceButton * const button = (dexsiDeviceButton*)pButtons.GetAt(i);
		if(button->GetID() == id){
			return button;
		}
	}
	
	return NULL;
}

dexsiDeviceButton *dexsiDevice::GetButtonWithX11Code(int code) const{
	const int count = pButtons.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dexsiDeviceButton * const button = (dexsiDeviceButton*)pButtons.GetAt(i);
		if(button->GetX11Code() == code){
			return button;
		}
	}
	
	return NULL;
}

int dexsiDevice::IndexOfButtonWithID(const char *id) const{
	const int count = pButtons.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const dexsiDeviceButton &button = *((dexsiDeviceButton*)pButtons.GetAt(i));
		if(button.GetID() == id){
			return i;
		}
	}
	
	return -1;
}

int dexsiDevice::IndexOfButtonWithX11Code(int code) const{
	const int count = pButtons.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const dexsiDeviceButton &button = *((dexsiDeviceButton*)pButtons.GetAt(i));
		if(button.GetX11Code() == code){
			return i;
		}
	}
	
	return -1;
}



int dexsiDevice::GetAxisCount() const{
	return pAxes.GetCount();
}

void dexsiDevice::AddAxis(dexsiDeviceAxis *axis){
	DEASSERT_NOTNULL(axis)
	pAxes.Add(axis);
}

dexsiDeviceAxis *dexsiDevice::GetAxisAt(int index) const{
	return (dexsiDeviceAxis*)pAxes.GetAt(index);
}

dexsiDeviceAxis *dexsiDevice::GetAxisWithID(const char *id) const{
	const int count = pAxes.GetCount();
	int i;
	for(i=0; i<count; i++){
		dexsiDeviceAxis * const axis = (dexsiDeviceAxis*)pAxes.GetAt(i);
		if(axis->GetID() == id){
			return axis;
		}
	}
	
	return NULL;
}

dexsiDeviceAxis *dexsiDevice::GetAxisWithX11Code(int code) const{
	const int count = pAxes.GetCount();
	int i;
	for(i=0; i<count; i++){
		dexsiDeviceAxis * const axis = (dexsiDeviceAxis*)pAxes.GetAt(i);
		if(axis->GetX11Code() == code){
			return axis;
		}
	}
	
	return NULL;
}

int dexsiDevice::IndexOfAxisWithID(const char *id) const{
	const int count = pAxes.GetCount();
	int i;
	for(i=0; i<count; i++){
		const dexsiDeviceAxis &axis = *((dexsiDeviceAxis*)pAxes.GetAt(i));
		if(axis.GetID() == id){
			return i;
		}
	}
	
	return -1;
}

int dexsiDevice::IndexOfAxisWithX11Code(int code) const{
	const int count = pAxes.GetCount();
	int i;
	for(i=0; i<count; i++){
		const dexsiDeviceAxis &axis = *((dexsiDeviceAxis*)pAxes.GetAt(i));
		if(axis.GetX11Code() == code){
			return i;
		}
	}
	
	return -1;
}



int dexsiDevice::GetFeedbackCount() const{
	return pFeedbacks.GetCount();
}

void dexsiDevice::AddFeedback(dexsiDeviceFeedback *feedback){
	if(!feedback){
		DETHROW(deeNullPointer);
	}
	pFeedbacks.Add(feedback);
}

dexsiDeviceFeedback *dexsiDevice::GetFeedbackAt(int index) const{
	return (dexsiDeviceFeedback*)pFeedbacks.GetAt(index);
}

dexsiDeviceFeedback *dexsiDevice::GetFeedbackWithID(const char *id) const{
	const int count = pFeedbacks.GetCount();
	int i;
	for(i=0; i<count; i++){
		dexsiDeviceFeedback * const feedback = (dexsiDeviceFeedback*)pFeedbacks.GetAt(i);
		if(feedback->GetID() == id){
			return feedback;
		}
	}
	
	return NULL;
}

int dexsiDevice::IndexOfFeedbackWithID(const char *id) const{
	const int count = pFeedbacks.GetCount();
	int i;
	for(i=0; i<count; i++){
		const dexsiDeviceFeedback &feedback = *((dexsiDeviceFeedback*)pFeedbacks.GetAt(i));
		if(feedback.GetID() == id){
			return i;
		}
	}
	
	return -1;
}



void dexsiDevice::ResetX11KeyCodeMap(int firstKeyCode, int keyCodeCount){
	if(firstKeyCode < 0 || keyCodeCount < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(pX11KeyCodeMap){
		delete [] pX11KeyCodeMap;
		pX11KeyCodeMap = NULL;
	}
	
	pX11FirstKeyCode = firstKeyCode;
	pX11KeyCodeCount = keyCodeCount;
	
	if(keyCodeCount > 0){
		pX11KeyCodeMap = new int[keyCodeCount];
		
		int i;
		for(i=0; i<pX11KeyCodeCount; i++){
			pX11KeyCodeMap[i] = -1;
		}
	}
}

void dexsiDevice::SetLookupX11KeyCode(int keyCode, int button){
	if(keyCode < pX11FirstKeyCode || keyCode >= pX11FirstKeyCode + pX11KeyCodeCount){
		DETHROW(deeInvalidParam);
	}
	
	pX11KeyCodeMap[keyCode - pX11FirstKeyCode] = button;
}

int dexsiDevice::LookupX11KeyCode(int keyCode) const{
	if(keyCode < pX11FirstKeyCode || keyCode >= pX11FirstKeyCode + pX11KeyCodeCount){
		return -1;
	}
	
	return pX11KeyCodeMap[keyCode - pX11FirstKeyCode];
}



void dexsiDevice::SetDirtyAxesValues(bool dirty){
	pDirtyAxesValues = dirty;
}



void dexsiDevice::GetInfo(deInputDevice &info) const{
	int i;
	
	info.SetID(pID);
	info.SetName(pName);
	info.SetType(pType);
	info.SetDisplayImage(pDisplayImage);
	for(i=0; i<pDisplayIcons.GetCount(); i++){
		info.AddDisplayIcon((deImage*)pDisplayIcons.GetAt(i));
	}
	info.SetDisplayText(pDisplayText);
	
	info.SetDisplayModel(NULL);
	info.SetDisplaySkin(NULL);
	
	const int buttonCount = pButtons.GetCount();
	info.SetButtonCount(buttonCount);
	for(i=0; i<buttonCount; i++){
		((dexsiDeviceButton*)pButtons.GetAt(i))->GetInfo(info.GetButtonAt(i));
	}
	
	const int axisCount = pAxes.GetCount();
	info.SetAxisCount(axisCount);
	for(i=0; i<axisCount; i++){
		((dexsiDeviceAxis*)pAxes.GetAt(i))->GetInfo(info.GetAxisAt(i));
	}
	
	const int feedbackCount = pFeedbacks.GetCount();
	info.SetFeedbackCount(feedbackCount);
	for(i=0; i<feedbackCount; i++){
		((dexsiDeviceFeedback*)pFeedbacks.GetAt(i))->GetInfo(info.GetFeedbackAt(i));
	}
}

void dexsiDevice::Update(){
}

void dexsiDevice::SendDirtyAxisEvents(){
	if(!pDirtyAxesValues){
		return;
	}
	
	pDirtyAxesValues = false;
	
	const int axisCount = pAxes.GetCount();
	int i;
	for(i=0; i<axisCount; i++){
		((dexsiDeviceAxis*)pAxes.GetAt(i))->SendEvents(*this);
	}
}



// Private Functions
//////////////////////

void dexsiDevice::pPrepareXDeviceTypeAtoms(){
	if(pXDeviceTypeAtomsReady){
		return;
	}
	
	/*
	const char * const names[exdtOther] = {
		XI_KEYBOARD,
		XI_MOUSE,
		XI_TABLET,
		XI_TOUCHSCREEN,
		XI_TOUCHPAD,
		XI_BARCODE,
		XI_BUTTONBOX,
		XI_KNOB_BOX,
		XI_ONE_KNOB,
		XI_NINE_KNOB,
		XI_TRACKBALL,
		XI_QUADRATURE,
		XI_ID_MODULE,
		XI_SPACEBALL,
		XI_DATAGLOVE,
		XI_EYETRACKER,
		XI_CURSORKEYS,
		XI_FOOTMOUSE,
		XI_JOYSTICK
	};
	
	if(!XInternAtoms(pModule.GetOSUnix()->GetDisplay(), (char**)names,
	exdtOther, false, &pXDeviceTypeAtoms[0])){
		DETHROW(deeInvalidParam);
	}
	*/
	
	pXDeviceTypeAtomsReady = true;
}
