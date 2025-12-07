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

#include "deovrDeviceFeedback.h"
#include "deVROpenVR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deovrDeviceFeedback
//////////////////////////////

// Constructor, destructor
////////////////////////////

deovrDeviceFeedback::deovrDeviceFeedback(deVROpenVR &ovr) :
pOvr(ovr),

pIndex(-1),
pType(deInputDeviceFeedback::eftGeneric),
pMaximum(1),
pValue(0.0f){
}

deovrDeviceFeedback::~deovrDeviceFeedback(){
}



// Management
///////////////

void deovrDeviceFeedback::SetIndex(int index){
	pIndex = index;
}

void deovrDeviceFeedback::SetID(const char *id){
	pID = id;
}

void deovrDeviceFeedback::SetName(const char *name){
	pName = name;
}

void deovrDeviceFeedback::SetType(deInputDeviceFeedback::eFeedbackTypes type){
	pType = type;
}

void deovrDeviceFeedback::SetInputDeviceComponent(deovrDeviceComponent *component){
	pInputDeviceComponent = component;
}



void deovrDeviceFeedback::SetDisplayImages(const char *name){
	pDisplayImage = nullptr;
	pDisplayIcons.RemoveAll();
	
	if(! name){
		return;
	}
	
	deImageManager &imageManager = *pOvr.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pOvr.GetVFS();
	const char * const basePath = "/share/image/feedback";
	decString filename;
	
	filename.Format("%s/%s/image.png", basePath, name);
	pDisplayImage.TakeOver(imageManager.LoadImage(vfs, filename, "/"));
	
	const int sizes[4] = {128, 64, 32, 16};
	int i;
	
	for(i=0; i<4; i++){
		filename.Format("%s/%s/icon%d.png", basePath, name, sizes[i]);
		pDisplayIcons.Add(deImage::Ref::New(imageManager.LoadImage(vfs, filename, "/")));
	}
}

void deovrDeviceFeedback::SetDisplayText(const char *text){
	pDisplayText = text;
}



void deovrDeviceFeedback::SetMaximum(int maximum){
	pMaximum = maximum;
}



void deovrDeviceFeedback::SetValue(float value){
	pValue = value;
}



void deovrDeviceFeedback::GetInfo(deInputDeviceFeedback &info) const{
	int i;
	
	info.SetID(pID);
	info.SetName(pName);
	info.SetType(pType);
	info.SetComponent(pInputDeviceComponent ? pInputDeviceComponent->GetID() : "");
	
	info.SetDisplayImage(pDisplayImage);
	for(i=0; i<pDisplayIcons.GetCount(); i++){
		info.AddDisplayIcon((deImage*)pDisplayIcons.GetAt(i));
	}
	info.SetDisplayText(pDisplayText);
}
