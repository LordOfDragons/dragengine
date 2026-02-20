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
#include <errno.h>

#include "dewiDevice.h"
#include "dewiDeviceAxis.h"
#include "dewiDeviceButton.h"
#include "dewiDeviceFeedback.h"
#include "dewiDeviceManager.h"
#include "deWindowsInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/systems/deInputSystem.h>



// Class dewiDevice
/////////////////////

// Constructor, destructor
////////////////////////////

dewiDevice::dewiDevice(deWindowsInput &module, eSources source) :
pModule(module),
pIndex(-1),
pSource(source),
pType(deInputDevice::edtMouse),

pDirtyAxesValues(false){
}

dewiDevice::~dewiDevice(){
}



// Management
///////////////

void dewiDevice::SetIndex(int index){
	pIndex = index;
}

void dewiDevice::SetType(deInputDevice::eDeviceTypes type){
	pType = type;
}

void dewiDevice::SetID(const char *id){
	pID = id;
}

void dewiDevice::SetName(const char *name){
	pName = name;
}

void dewiDevice::SetDisplayImages(const char *name){
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
	pDisplayImage = imageManager.LoadImage(vfs, filename, "/");
	
	const int sizes[4] = {128, 64, 32, 16};
	deImage::Ref icon;
	int i;
	
	for(i=0; i<4; i++){
		filename.Format("%s/%s/icon%d.png", basePath, name, sizes[i]);
		pDisplayIcons.Add(imageManager.LoadImage(vfs, filename, "/"));
	}
}

void dewiDevice::SetDisplayText(const char *text){
	pDisplayText = text;
}



void dewiDevice::AddButton(dewiDeviceButton *button){
	if(!button){
		DETHROW(deeNullPointer);
	}
	pButtons.Add(button);
}

dewiDeviceButton *dewiDevice::GetButtonWithID(const char *id) const{
	return pButtons.FindOrDefault([&](const dewiDeviceButton &button){
		return button.GetID() == id;
	});
}

dewiDeviceButton *dewiDevice::GetButtonWithWICode(int code) const{
	return pButtons.FindOrDefault([&](const dewiDeviceButton &button){
		return button.GetWICode() == code;
	});
}

int dewiDevice::IndexOfButtonWithID(const char *id) const{
	return pButtons.IndexOfMatching([&](const dewiDeviceButton &button){
		return button.GetID() == id;
	});
}

int dewiDevice::IndexOfButtonWithWICode(int code) const{
	return pButtons.IndexOfMatching([&](const dewiDeviceButton &button){
		return button.GetWICode() == code;
	});
}



void dewiDevice::AddAxis(dewiDeviceAxis *axis){
	if(!axis){
		DETHROW(deeNullPointer);
	}
	pAxes.Add(axis);
}

dewiDeviceAxis *dewiDevice::GetAxisWithID(const char *id) const{
	return pAxes.FindOrDefault([&](const dewiDeviceAxis &axis){
		return axis.GetID() == id;
	});
}

dewiDeviceAxis *dewiDevice::GetAxisWithWICode(int code) const{
	return pAxes.FindOrDefault([&](const dewiDeviceAxis &axis){
		return axis.GetWICode() == code;
	});
}

int dewiDevice::IndexOfAxisWithID(const char *id) const{
	return pAxes.IndexOfMatching([&](const dewiDeviceAxis &axis){
		return axis.GetID() == id;
	});
}

int dewiDevice::IndexOfAxisWithWICode(int code) const{
	return pAxes.IndexOfMatching([&](const dewiDeviceAxis &axis){
		return axis.GetWICode() == code;
	});
}



void dewiDevice::AddFeedback(dewiDeviceFeedback *feedback){
	if(!feedback){
		DETHROW(deeNullPointer);
	}
	pFeedbacks.Add(feedback);
}

dewiDeviceFeedback *dewiDevice::GetFeedbackWithID(const char *id) const{
	return pFeedbacks.FindOrDefault([&](const dewiDeviceFeedback &feedback){
		return feedback.GetID() == id;
	});
}

int dewiDevice::IndexOfFeedbackWithID(const char *id) const{
	return pFeedbacks.IndexOfMatching([&](const dewiDeviceFeedback &feedback){
		return feedback.GetID() == id;
	});
}



void dewiDevice::SetDirtyAxesValues(bool dirty){
	pDirtyAxesValues = dirty;
}



void dewiDevice::GetInfo(deInputDevice &info) const{
	info.SetID(pID);
	info.SetName(pName);
	info.SetType(pType);
	info.SetDisplayImage(pDisplayImage);
	pDisplayIcons.Visit([&](deImage *icon){
		info.AddDisplayIcon(icon);
	});
	info.SetDisplayText(pDisplayText);
	
	info.SetDisplayModel(nullptr);
	info.SetDisplaySkin(nullptr);
	
	info.SetButtonCount(pButtons.GetCount());
	pButtons.VisitIndexed([&](int i, const dewiDeviceButton &button){
		button.GetInfo(info.GetButtons().GetAt(i));
	});
	
	info.SetAxisCount(pAxes.GetCount());
	pAxes.VisitIndexed([&](int i, const dewiDeviceAxis &axis){
		axis.GetInfo(info.GetAxes().GetAt(i));
	});
	
	info.SetFeedbackCount(pFeedbacks.GetCount());
	pFeedbacks.VisitIndexed([&](int i, const dewiDeviceFeedback &feedback){
		feedback.GetInfo(info.GetFeedbacks().GetAt(i));
	});
}

void dewiDevice::Update(){
}

void dewiDevice::SendDirtyAxisEvents(){
	if(!pDirtyAxesValues){
		return;
	}
	
	pDirtyAxesValues = false;
	
	pAxes.Visit([&](dewiDeviceAxis &axis){
		axis.SendEvents(*this);
	});
}
