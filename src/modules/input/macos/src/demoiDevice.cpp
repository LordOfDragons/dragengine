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

#include "demoiDevice.h"
#include "demoiDeviceAxis.h"
#include "demoiDeviceButton.h"
#include "deMacOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>


// Class demoiDevice
//////////////////////

// Constructor, destructor
////////////////////////////

demoiDevice::demoiDevice(deMacOSInput &module, eSources source) :
pModule(module),
pIndex(-1),
pSource(source),
pType(deInputDevice::edtMouse),

pButtonCount(0),
pButtons(NULL),
pAxisCount(0),
pAxes(NULL),

pDirtyAxisValues(false){
}

demoiDevice::~demoiDevice(){
	if(pAxes){
		delete [] pAxes;
	}
	if(pButtons){
		delete [] pButtons;
	}
}



// Management
///////////////

void demoiDevice::SetIndex(int index){
    pIndex = index;
}

void demoiDevice::SetType(deInputDevice::eDeviceTypes type){
	pType = type;
}

void demoiDevice::SetID(const char *id){
	pID = id;
}

void demoiDevice::SetName(const char *name){
	pName = name;
}



void demoiDevice::SetButtonCount(int count){
	if(pButtons){
		delete [] pButtons;
		pButtons = NULL;
		pButtonCount = 0;
	}
	
	if(count > 0){
		pButtons = new demoiDeviceButton[count];
		pButtonCount = count;
	}
}

demoiDeviceButton &demoiDevice::GetButtonAt(int index) const{
	if(index < 0 || index >= pButtonCount){
		DETHROW(deeInvalidParam);
	}
	return pButtons[index];
}

demoiDeviceButton *demoiDevice::GetButtonWithID(const char *id) const{
	int i;
	for(i=0; i<pButtonCount; i++){
		if(pButtons[i].GetID() == id){
			return pButtons + i;
		}
	}
	
	return NULL;
}

int demoiDevice::IndexOfButtonWithID(const char *id) const{
	int i;
	for(i=0; i<pButtonCount; i++){
		if(pButtons[i].GetID() == id){
			return i;
		}
	}
	
	return -1;
}

int demoiDevice::IndexOfButtonWithMOCode(int code) const{
    int i;
    for(i=0; i<pButtonCount; i++){
        if(pButtons[i].GetMOCode() == code){
            return i;
        }
    }
    
    return -1;
}



void demoiDevice::SetAxisCount(int count){
	if(pAxes){
		delete [] pAxes;
		pAxes = NULL;
		pAxisCount = 0;
	}
	
	if(count > 0){
		pAxes = new demoiDeviceAxis[count];
		pAxisCount = count;
	}
}

demoiDeviceAxis &demoiDevice::GetAxisAt(int index) const{
	if(index < 0 || index >= pAxisCount){
		DETHROW(deeInvalidParam);
	}
	return pAxes[index];
}

demoiDeviceAxis *demoiDevice::GetAxisWithID(const char *id) const{
	int i;
	for(i=0; i<pAxisCount; i++){
		if(pAxes[i].GetID() == id){
			return pAxes + i;
		}
	}
	
	return NULL;
}

int demoiDevice::IndexOfAxisWithID(const char *id) const{
	int i;
	for(i=0; i<pAxisCount; i++){
		if(pAxes[i].GetID() == id){
			return i;
		}
	}
	
	return -1;
}

int demoiDevice::IndexOfAxisWithMOCode(int code) const{
    int i;
    for(i=0; i<pAxisCount; i++){
        if(pAxes[i].GetMOCode() == code){
            return i;
        }
    }
    
    return -1;
}



void demoiDevice::SetDirtyAxisValues(bool dirty){
    pDirtyAxisValues = dirty;
}



void demoiDevice::GetInfo(deInputDevice &info) const{
	int i;
	
	info.SetID(pID);
	info.SetName(pName);
	info.SetType(pType);
	
	info.SetDisplayImage(NULL);
	info.SetDisplayModel(NULL);
	info.SetDisplaySkin(NULL);
	
	info.SetButtonCount(pButtonCount);
	for(i=0; i<pButtonCount; i++){
		pButtons[i].GetInfo(info.GetButtonAt(i));
	}
	
	info.SetAxisCount(pAxisCount);
	for(i=0; i<pAxisCount; i++){
		pAxes[i].GetInfo(info.GetAxisAt(i));
	}
}

void demoiDevice::Update(){
}

void demoiDevice::SendDirtyAxisEvents(){
    if(! pDirtyAxisValues){
        return;
    }
    
    pDirtyAxisValues = false;
    
    int i;
    for(i=0; i<pAxisCount; i++){
        pAxes[i].SendEvents(*this);
    }
}
