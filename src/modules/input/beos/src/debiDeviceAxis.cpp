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

#include "debiDevice.h"
#include "debiDeviceAxis.h"
#include "deBeOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class debiDeviceAxis
/////////////////////////

// Constructor, destructor
////////////////////////////

debiDeviceAxis::debiDeviceAxis(deBeOSInput &module) :
pModule(module),
pIndex(-1),
pType(deInputDeviceAxis::eatGeneric),
pMinimum(-100),
pMaximum(100),
pFuzz(0),
pFlat(0),
pAbsolute(true),
pWheelOtherAxis(false),
pWheelChange(0),

pValue(0.0f),
pChangedValue(0.0f),

pBICode(-1){
}

debiDeviceAxis::~debiDeviceAxis(){
}



// Management
///////////////

void debiDeviceAxis::SetIndex(int index){
	pIndex = index;
}

void debiDeviceAxis::SetID(const char *id){
	pID = id;
}

void debiDeviceAxis::SetName(const char *name){
	pName = name;
}

void debiDeviceAxis::SetType(deInputDeviceAxis::eAxisTypes type){
	pType = type;
}



void debiDeviceAxis::SetDisplayImages(const char *name){
	pDisplayImage = NULL;
	pDisplayIcons.RemoveAll();
	
	if(!name){
		return;
	}
	
	deImageManager &imageManager = *pModule.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pModule.GetVFS();
	const char * const basePath = "/share/image/axis";
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

void debiDeviceAxis::SetDisplayText(const char *text){
	pDisplayText = text;
}



void debiDeviceAxis::SetMinimum(int minimum){
	pMinimum = minimum;
}

void debiDeviceAxis::SetMaximum(int maximum){
	pMaximum = maximum;
}

void debiDeviceAxis::SetFuzz(int fuzz){
	pFuzz = fuzz;
}

void debiDeviceAxis::SetFlat(int flat){
	pFlat = flat;
}

void debiDeviceAxis::SetAbsolute(bool absolute){
	pAbsolute = absolute;
}

void debiDeviceAxis::SetWheelOtherAxis(bool otherAxis){
	pWheelOtherAxis = otherAxis;
}

void debiDeviceAxis::SetWheelChange(int change, int modifiers, const timeval &eventTime){
	pWheelChange = change;
	pLastModifiers = modifiers;
	pLastEventTime = eventTime;
}

void debiDeviceAxis::IncrementWheelChange(int amount, int modifiers, const timeval &eventTime){
	pWheelChange += amount;
	pLastModifiers = modifiers;
	pLastEventTime = eventTime;
}



void debiDeviceAxis::SetValue(float value){
	if(pAbsolute){
		value = decMath::clamp(value, -1.0f, 1.0f);
	}
	pValue = value;
}



void debiDeviceAxis::SetBICode(int code){
	pBICode = code;
}



void debiDeviceAxis::GetInfo(deInputDeviceAxis &info) const{
	int i;
	
	info.SetID(pID);
	info.SetName(pName);
	info.SetType(pType);
	
	info.SetDisplayImage(pDisplayImage);
	for(i=0; i<pDisplayIcons.GetCount(); i++){
		info.AddDisplayIcon((deImage*)pDisplayIcons.GetAt(i));
	}
	info.SetDisplayText(pDisplayText);
}

void debiDeviceAxis::ProcessNewValue(debiDevice &device, float newValue, const timeval &eventTime){
	if(pAbsolute){
		pChangedValue = decMath::clamp(newValue, -1.0f, 1.0f);
		
	}else{
		pChangedValue += newValue;
	}
	
	if(fabsf(pChangedValue - pValue) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pLastEventTime = eventTime;
	device.SetDirtyAxesValues(true);
}


void debiDeviceAxis::SendEvents(debiDevice &device){
	if(pAbsolute){
		if(fabsf(pChangedValue - pValue) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		pValue = pChangedValue;
		device.GetModule().AddAxisChanged(device.GetIndex(), pIndex, pValue, pLastEventTime);
		
	}else if(pType == deInputDeviceAxis::eatMouseWheel){
		if(pWheelChange == 0){
			return;
		}
		
		const int change = pWheelChange;
		pWheelChange = 0;
		
		if(pWheelOtherAxis){
			device.GetModule().AddMouseWheelChanged(device.GetIndex(), pIndex,
				change, 0, pLastModifiers, pLastEventTime);
			
		}else{
			device.GetModule().AddMouseWheelChanged(device.GetIndex(), pIndex,
				0, change, pLastModifiers, pLastEventTime);
		}
		
	}else{
		if(fabsf(pChangedValue) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		pValue = pChangedValue;
		pChangedValue = 0.0f;
		
		device.GetModule().AddAxisChanged(device.GetIndex(), pIndex, pValue, pLastEventTime);
	}
}
