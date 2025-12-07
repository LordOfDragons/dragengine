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

#include "deInputDeviceFeedback.h"
#include "../common/exceptions.h"
#include "../common/math/decMath.h"
#include "../resources/image/deImage.h"



// Class deInputDeviceFeedback
////////////////////////////////

deInputDeviceFeedback::deInputDeviceFeedback() :
pType(eftGeneric){
}

deInputDeviceFeedback::deInputDeviceFeedback(const deInputDeviceFeedback &feedback) :
pID(feedback.pID),
pName(feedback.pName),
pType(feedback.pType),
pDisplayImage(feedback.pDisplayImage),
pDisplayIcons(feedback.pDisplayIcons),
pDisplayText(feedback.pDisplayText){
}

deInputDeviceFeedback::~deInputDeviceFeedback(){
}



// Device information
///////////////////////

void deInputDeviceFeedback::SetID(const char *id){
	pID = id;
}

void deInputDeviceFeedback::SetName(const char *name){
	pName = name;
}

void deInputDeviceFeedback::SetType(eFeedbackTypes type){
	pType = type;
}

void deInputDeviceFeedback::SetComponent(const char *component){
	pComponent = component;
}

void deInputDeviceFeedback::SetDisplayImage(deImage *image){
	pDisplayImage = image;
}

int deInputDeviceFeedback::GetDisplayIconCount() const{
	return pDisplayIcons.GetCount();
}

deImage *deInputDeviceFeedback::GetDisplayIconAt(int index) const{
	return (deImage*)pDisplayIcons.GetAt(index);
}

void deInputDeviceFeedback::AddDisplayIcon(deImage *image){
	if(!image){
		DETHROW(deeNullPointer);
	}
	pDisplayIcons.Add(image);
}

void deInputDeviceFeedback::SetDisplayText(const char *text){
	pDisplayText = text;
}



// Operators
//////////////

deInputDeviceFeedback &deInputDeviceFeedback::operator=(const deInputDeviceFeedback &feedback){
	pID = feedback.pID;
	pName = feedback.pName;
	pType = feedback.pType;
	pComponent = feedback.pComponent;
	pDisplayImage = feedback.pDisplayImage;
	pDisplayIcons = feedback.pDisplayIcons;
	pDisplayText = feedback.pDisplayText;
	return *this;
}
