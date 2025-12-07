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

#include "dewiDeviceFeedback.h"
#include "deWindowsInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class dewiDeviceFeedback
/////////////////////////////

// Constructor, destructor
////////////////////////////

dewiDeviceFeedback::dewiDeviceFeedback(deWindowsInput &module) :
pModule(module),
pIndex(-1),
pType(deInputDeviceFeedback::eftGeneric),
pMaximum(1),
pValue(0.0f){
}

dewiDeviceFeedback::~dewiDeviceFeedback(){
}



// Management
///////////////

void dewiDeviceFeedback::SetIndex(int index){
	pIndex = index;
}

void dewiDeviceFeedback::SetID(const char *id){
	pID = id;
}

void dewiDeviceFeedback::SetName(const char *name){
	pName = name;
}

void dewiDeviceFeedback::SetType(deInputDeviceFeedback::eFeedbackTypes type){
	pType = type;
}



void dewiDeviceFeedback::SetDisplayImages(const char *name){
	pDisplayImage = NULL;
	pDisplayIcons.RemoveAll();
	
	if(!name){
		return;
	}
	
	deImageManager &imageManager = *pModule.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pModule.GetVFS();
	const char * const basePath = "/share/image/feedback";
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

void dewiDeviceFeedback::SetDisplayText(const char *text){
	pDisplayText = text;
}



void dewiDeviceFeedback::SetMaximum(int maximum){
	pMaximum = maximum;
}



void dewiDeviceFeedback::SetValue(float value){
	pValue = value;
}



void dewiDeviceFeedback::GetInfo(deInputDeviceFeedback &info) const{
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
