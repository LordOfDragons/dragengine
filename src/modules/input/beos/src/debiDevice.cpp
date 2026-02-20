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

#include "debiDevice.h"
#include "debiDeviceAxis.h"
#include "debiDeviceButton.h"
#include "deBeOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/systems/deInputSystem.h>



// Class debiDevice
/////////////////////

// Constructor, destructor
////////////////////////////

debiDevice::debiDevice(deBeOSInput &module) :
pModule(module),
pIndex(-1),
pSource(esBeOS),
pType(deInputDevice::edtMouse),
pDirtyAxesValues(false){
}

debiDevice::~debiDevice(){
}



// Management
///////////////

void debiDevice::SetIndex(int index){
	pIndex = index;
}

void debiDevice::SetType(deInputDevice::eDeviceTypes type){
	pType = type;
}

void debiDevice::SetID(const char *id){
	pID = id;
}

void debiDevice::SetName(const char *name){
	pName = name;
}

void debiDevice::SetDisplayImages(const char *name){
	pDisplayImage = NULL;
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

void debiDevice::SetDisplayText(const char *text){
	pDisplayText = text;
}



debiDeviceButton *debiDevice::GetButtonWithID(const char *id) const{
	const int count = pButtons.GetCount();
	int i;
	for(i=0; i<count; i++){
		debiDeviceButton * const button = (debiDeviceButton*)pButtons.GetAt(i);
		if(button->GetID() == id){
			return button;
		}
	}
	return NULL;
}

int debiDevice::IndexOfButtonWithID(const char *id) const{
	const int count = pButtons.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(((debiDeviceButton*)pButtons.GetAt(i))->GetID() == id){
			return i;
		}
	}
	return -1;
}

int debiDevice::IndexOfButtonWithBICode(int code) const{
	const int count = pButtons.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(((debiDeviceButton*)pButtons.GetAt(i))->GetBICode() == code){
			return i;
		}
	}
	return -1;
}

void debiDevice::AddButton(debiDeviceButton *button){
	if(!button){
		DETHROW(deeNullPointer);
	}
	pButtons.Add(button);
}



debiDeviceAxis *debiDevice::GetAxisWithID(const char *id) const{
	const int count = pAxes.GetCount();
	int i;
	for(i=0; i<count; i++){
		debiDeviceAxis * const axis = (debiDeviceAxis*)pAxes.GetAt(i);
		if(axis->GetID() == id){
			return axis;
		}
	}
	return NULL;
}

int debiDevice::IndexOfAxisWithID(const char *id) const{
	const int count = pAxes.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(((debiDeviceAxis*)pAxes.GetAt(i))->GetID() == id){
			return i;
		}
	}
	return -1;
}

int debiDevice::IndexOfAxisWithBICode(int code) const{
	const int count = pAxes.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(((debiDeviceAxis*)pAxes.GetAt(i))->GetBICode() == code){
			return i;
		}
	}
	return -1;
}

void debiDevice::AddAxis(debiDeviceAxis *axis){
	if(!axis){
		DETHROW(deeNullPointer);
	}
	pAxes.Add(axis);
}



void debiDevice::SetDirtyAxesValues(bool dirty){
	pDirtyAxesValues = dirty;
}



void debiDevice::GetInfo(deInputDevice &info) const{
	int i;
	
	info.SetID(pID);
	info.SetName(pName);
	info.SetType(pType);
	info.SetDisplayImage(pDisplayImage);
	pDisplayIcons.Visit([&](deImage *icon){
		info.AddDisplayIcon(icon);
	});
	info.SetDisplayText(pDisplayText);
	
	info.SetDisplayModel(NULL);
	info.SetDisplaySkin(NULL);
	
	info.SetButtonCount(pButtons.GetCount());
	pButtons.VisitIndexed([&](int i, const debiDeviceButton &button){
		button.GetInfo(info.GetButtons().GetAt(i));
	});
	
	info.SetAxisCount(pAxes.GetCount());
	pAxes.VisitIndexed([&](int i, const debiDeviceAxis &axis){
		axis.GetInfo(info.GetAxes().GetAt(i));
	});
}

void debiDevice::Update(){
}

void debiDevice::SendDirtyAxisEvents(){
	if(!pDirtyAxesValues){
		return;
	}
	
	pDirtyAxesValues = false;
	
	pAxes.Visit([&](debiDeviceAxis &axis){
		axis.SendEvents(*this);
	});
}
