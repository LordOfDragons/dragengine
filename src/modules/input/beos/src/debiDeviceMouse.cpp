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

#include <View.h>

#include "debiDeviceMouse.h"
#include "debiDeviceAxis.h"
#include "debiDeviceButton.h"
#include "deBeOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class debiDeviceMouse
//////////////////////////

// Constructor, destructor
////////////////////////////

debiDeviceMouse::debiDeviceMouse(deBeOSInput &module) :
debiDevice(module)
{
	decString string;
	
	string.Format("%s%dm", BEINP_DEVID_PREFIX, debiDevice::esBeOS);
 	SetID(string);
	SetName("Mouse");
	SetType(deInputDevice::edtMouse);
	
	// add axis
	const debiDeviceAxis::Ref axisX(debiDeviceAxis::Ref::NewWith(module));
	AddAxis(axisX);
	axisX->SetIndex(0);
	axisX->SetAbsolute(false);
	axisX->SetID("x");
	axisX->SetName("X");
	axisX->SetType(deInputDeviceAxis::eatMouse);
	axisX->SetDisplayImages("mouseX");
	
	const debiDeviceAxis::Ref axisY(debiDeviceAxis::Ref::New(module));
	AddAxis(axisY);
	axisY->SetIndex(1);
	axisY->SetAbsolute(false);
	axisY->SetID("y");
	axisY->SetName("Y");
	axisY->SetType(deInputDeviceAxis::eatMouse);
	axisY->SetDisplayImages("mouseY");
	
	const debiDeviceAxis::Ref wheelY(debiDeviceAxis::Ref::New(module));
	AddAxis(wheelY);
	wheelY->SetIndex(2);
	wheelY->SetAbsolute(false);
	wheelY->SetID("wheelY");
	wheelY->SetName("Wheel Y");
	wheelY->SetType(deInputDeviceAxis::eatMouseWheel);
	wheelY->SetDisplayImages("mouseY");
	wheelY->SetDisplayText("Wheel");
	
	const debiDeviceAxis::Ref wheelX(debiDeviceAxis::Ref::New(module));
	AddAxis(wheelX);
	wheelX->SetIndex(3);
	wheelX->SetAbsolute(false);
	wheelX->SetID("wheelX");
	wheelX->SetName("Wheel X");
	wheelX->SetType(deInputDeviceAxis::eatMouseWheel);
	wheelX->SetDisplayImages("mouseX");
	wheelX->SetDisplayText("Wheel");
	
	// add buttons
	int32 buttonCount = 3;
	// no more working. there is a constant B_MAX_MOUSE_BUTTONS but mmap.button[i] returns
	// just 1<<i which is kinda pointless. assuming 3 buttons and wait until this is fixed
	/*if( get_mouse_type( &buttonCount ) != B_OK ){
		DETHROW(deeInvalidParam);
	}*/
	
	mouse_map mmap;
	if(get_mouse_map(&mmap) != B_OK){
		DETHROW(deeInvalidParam);
	}
	
	if(buttonCount > 0){
		debiDeviceButton::Ref button(debiDeviceButton::Ref::New(module));
		AddButton(button);
		button->SetID("left");
		button->SetBICode(mmap.button[0]);
		button->SetName("Left");
		button->SetBICode(B_PRIMARY_MOUSE_BUTTON);
		button->SetDisplayImages("mouseL");
	}
	
	if(buttonCount > 1){
		debiDeviceButton::Ref button(debiDeviceButton::Ref::New(module));
		AddButton(button);
		button->SetID("right");
		button->SetBICode(mmap.button[1]);
		button->SetName("Right");
		button->SetBICode(B_SECONDARY_MOUSE_BUTTON);
		button->SetDisplayImages("mouseR");
	}
	
	if(buttonCount > 2){
		debiDeviceButton::Ref button(debiDeviceButton::Ref::New(module));
		AddButton(button);
		button->SetID("middle");
		button->SetBICode(mmap.button[2]);
		button->SetName("Middle");
		button->SetBICode(B_TERTIARY_MOUSE_BUTTON);
		button->SetDisplayImages("mouseM");
	}
	
	int i;
	for(i=3; i<buttonCount; i++){
		debiDeviceButton::Ref button(debiDeviceButton::Ref::New(module));
		AddButton(button);
		
		string.Format("aux%d", i - 2);
		button->SetID(string);
		string.Format("Aux %d", i - 2);
		button->SetName(string);
		button->SetBICode(i); // assuming this is correct.
		button->SetDisplayImages("mouseX");
		string.Format("%d", i + 1);
		button->SetDisplayText(string);
	}
}

debiDeviceMouse::~debiDeviceMouse(){
}



// Management
///////////////

void debiDeviceMouse::Update(){
	SendDirtyAxisEvents();
}
