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

#include "deInputDeviceComponent.h"
#include "../common/exceptions.h"
#include "../resources/image/deImage.h"



// Class deInputDeviceComponent
//////////////////////////////

deInputDeviceComponent::deInputDeviceComponent() :
pType(ectGeneric){
}

deInputDeviceComponent::deInputDeviceComponent(const deInputDeviceComponent &component) :
pID(component.pID),
pName(component.pName),
pType(component.pType),
pDisplayImage(component.pDisplayImage),
pDisplayIcons(component.pDisplayIcons),
pDisplayText(component.pDisplayText){
}

deInputDeviceComponent::~deInputDeviceComponent(){
}



// Device information
///////////////////////

void deInputDeviceComponent::SetID(const char *id){
	pID = id;
}

void deInputDeviceComponent::SetName(const char *name){
	pName = name;
}

void deInputDeviceComponent::SetType(deInputDeviceComponent:: eComponentTypes type){
	pType = type;
}

void deInputDeviceComponent::SetDisplayImage(deImage *image){
	pDisplayImage = image;
}

int deInputDeviceComponent::GetDisplayIconCount() const{
	return pDisplayIcons.GetCount();
}

deImage *deInputDeviceComponent::GetDisplayIconAt(int index) const{
	return (deImage*)pDisplayIcons.GetAt(index);
}

void deInputDeviceComponent::AddDisplayIcon(deImage *image){
	if(! image){
		DETHROW(deeNullPointer);
	}
	pDisplayIcons.Add(image);
}

void deInputDeviceComponent::SetDisplayText(const char *text){
	pDisplayText = text;
}



// Operators
//////////////

deInputDeviceComponent &deInputDeviceComponent::operator=(const deInputDeviceComponent &component){
	pID = component.pID;
	pName = component.pName;
	pType = component.pType;
	pDisplayImage = component.pDisplayImage;
	pDisplayIcons = component.pDisplayIcons;
	pDisplayText = component.pDisplayText;
	return *this;
}
