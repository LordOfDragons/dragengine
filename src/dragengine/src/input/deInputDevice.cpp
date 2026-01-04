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

#include "deInputDevice.h"
#include "deInputDeviceAxis.h"
#include "deInputDeviceButton.h"
#include "deInputDeviceFeedback.h"
#include "deInputDeviceComponent.h"
#include "../common/exceptions.h"
#include "../resources/model/deModel.h"
#include "../resources/skin/deSkin.h"
#include "../resources/image/deImage.h"



// Class deInputDevice
////////////////////////

deInputDevice::deInputDevice() :
pType(edtGeneric),
pBoneConfiguration(ebcNone),
pSupportsFaceEyeExpressions(false),
pSupportsFaceMouthExpressions(false),
pUsingHandInteraction(false){
}

deInputDevice::~deInputDevice(){
	pCleanUp();
}



// Device information
///////////////////////

void deInputDevice::SetID(const char *id){
	pID = id;
}

void deInputDevice::SetName(const char *name){
	pName = name;
}

void deInputDevice::SetType(eDeviceTypes type){
	pType = type;
}

void deInputDevice::SetDisplayModel(deModel *model){
	pDisplayModel = model;
}

void deInputDevice::SetDisplaySkin(deSkin *skin){
	pDisplaySkin = skin;
}

void deInputDevice::SetDisplayImage(deImage *image){
	pDisplayImage = image;
}



void deInputDevice::AddDisplayIcon(deImage *image){
	DEASSERT_NOTNULL(image)
	pDisplayIcons.Add(image);
}

void deInputDevice::SetDisplayText(const char * text){
	pDisplayText = text;
}

void deInputDevice::SetBoneConfiguration(eBoneConfigurations configuration){
	pBoneConfiguration = configuration;
}

const decVector &deInputDevice::GetFingerTipOffset(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index <= 4)
	return pFingerTipOffset[index];
}

void deInputDevice::SetFingerTipOffset(int index, const decVector &offset){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index <= 4)
	pFingerTipOffset[index] = offset;
}

void deInputDevice::SetHandRig(deRig *rig){
	pHandRig = rig;
}

void deInputDevice::SetSupportsFaceEyeExpressions(bool supportsFaceEyeExpressions){
	pSupportsFaceEyeExpressions = supportsFaceEyeExpressions;
}

void deInputDevice::SetSupportsFaceMouthExpressions(bool supportsFaceMouthExpressions){
	pSupportsFaceMouthExpressions = supportsFaceMouthExpressions;
}

void deInputDevice::SetUsingHandInteraction(bool usingHandInteraction){
	pUsingHandInteraction = usingHandInteraction;
}

void deInputDevice::SetVRModel(deModel *model){
	pVRModel = model;
}

void deInputDevice::SetVRSkin(deSkin *skin){
	pVRSkin = skin;
}



// Buttons
////////////

void deInputDevice::SetButtonCount(int count){
	pButtons = decTList<deInputDeviceButton>(count, deInputDeviceButton());
}

int deInputDevice::IndexOfButtonWithID(const char *id) const{
	return pButtons.IndexOfMatching([&](const deInputDeviceButton &button){
		return button.GetID() == id;
	});
}



// Axes
/////////

void deInputDevice::SetAxisCount(int count){
	pAxes = decTList<deInputDeviceAxis>(count, deInputDeviceAxis());
}

int deInputDevice::IndexOfAxisWithID(const char *id) const{
	return pAxes.IndexOfMatching([&](const deInputDeviceAxis &axis){
		return axis.GetID() == id;
	});
}



// Feedbacks
//////////////

void deInputDevice::SetFeedbackCount(int count){
	pFeedbacks = decTList<deInputDeviceFeedback>(count, deInputDeviceFeedback());
}

int deInputDevice::IndexOfFeedbackWithID(const char *id) const{
	return pFeedbacks.IndexOfMatching([&](const deInputDeviceFeedback &feedback){
		return feedback.GetID() == id;
	});
}



// Components
//////////////

void deInputDevice::SetComponentCount(int count){
	pComponents = decTList<deInputDeviceComponent>(count, deInputDeviceComponent());
}

int deInputDevice::IndexOfComponentWithID(const char *id) const{
	return pComponents.IndexOfMatching([&](const deInputDeviceComponent &component){
		return component.GetID() == id;
	});
}



// Privat functions
/////////////////////

void deInputDevice::pCleanUp(){
}
