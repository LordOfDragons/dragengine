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

#include "deovrDeviceComponent.h"
#include "deovrDevice.h"
#include "deVROpenVR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputDeviceComponent.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deovrDeviceComponent
////////////////////////////

// Constructor, destructor
////////////////////////////

deovrDeviceComponent::deovrDeviceComponent(deovrDevice &device) :
pDevice(device),
pIndex(-1),
pType(deInputDeviceComponent::ectGeneric){
}

deovrDeviceComponent::~deovrDeviceComponent(){
}



// Management
///////////////

void deovrDeviceComponent::SetIndex(int index){
	pIndex = index;
}

void deovrDeviceComponent::SetID(const char *id){
	pID = id;
}

void deovrDeviceComponent::SetName(const char *name){
	pName = name;
}

void deovrDeviceComponent::SetType(deInputDeviceComponent::eComponentTypes type){
	pType = type;
}



void deovrDeviceComponent::SetDisplayImages(const char *name){
	pDisplayImage = nullptr;
	pDisplayIcons.RemoveAll();
	
	if(!name){
		return;
	}
	
	deImageManager &imageManager = *pDevice.GetOvr().GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pDevice.GetOvr().GetVFS();
	const char * const basePath = "/share/image/component";
	decString filename;
	
	filename.Format("%s/%s/image.png", basePath, name);
	pDisplayImage = imageManager.LoadImage(vfs, filename, "/");
	
	const int sizes[4] = {128, 64, 32, 16};
	int i;
	
	for(i=0; i<4; i++){
		filename.Format("%s/%s/icon%d.png", basePath, name, sizes[i]);
		pDisplayIcons.Add(imageManager.LoadImage(vfs, filename, "/"));
	}
}

void deovrDeviceComponent::SetDisplayImages(const deovrDeviceComponent &component){
	pDisplayImage = component.pDisplayImage;
	pDisplayIcons = component.pDisplayIcons;
}

void deovrDeviceComponent::SetDisplayText(const char *text){
	pDisplayText = text;
}



void deovrDeviceComponent::GetInfo(deInputDeviceComponent &info) const{
	info.SetID(pID);
	info.SetName(pName);
	info.SetType(pType);
	
	info.SetDisplayImage(pDisplayImage);
	
	int i;
	for(i=0; i<pDisplayIcons.GetCount(); i++){
		info.AddDisplayIcon((deImage*)pDisplayIcons.GetAt(i));
	}
	info.SetDisplayText(pDisplayText);
}
