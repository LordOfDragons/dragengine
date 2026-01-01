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
#include <errno.h>

#include "dexsiDeviceCoreMouse.h"
#include "dexsiDeviceAxis.h"
#include "dexsiDeviceButton.h"
#include "dexsiDeviceFeedback.h"
#include "dexsiDeviceManager.h"
#include "deXSystemInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class dexsiDeviceCoreMouse
///////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiDeviceCoreMouse::dexsiDeviceCoreMouse(deXSystemInput &module) :
dexsiDevice(module, esX11)
{
	decString string;
	
	string.Format("%s%dcm", XINP_DEVID_PREFIX, dexsiDevice::esX11);
	SetID(string);
	SetName("Core Mouse");
	SetType(deInputDevice::edtMouse);
	SetDisplayImages("mouse");
	
	// x11 does not provide a way to figure out mouse configuration. using XInput2 should help
	const dexsiDeviceAxis::Ref axisX(dexsiDeviceAxis::Ref::New(module));
	AddAxis(axisX);
	axisX->SetIndex(0);
	axisX->SetAbsolute(false);
	axisX->SetID("x");
	axisX->SetName("X");
	axisX->SetType(deInputDeviceAxis::eatMouse);
	axisX->SetDisplayImages("mouseX");
	
	const dexsiDeviceAxis::Ref axisY(dexsiDeviceAxis::Ref::New(module));
	AddAxis(axisY);
	axisY->SetIndex(1);
	axisY->SetAbsolute(false);
	axisY->SetID("y");
	axisY->SetName("Y");
	axisY->SetType(deInputDeviceAxis::eatMouse);
	axisY->SetDisplayImages("mouseY");
	
	const dexsiDeviceAxis::Ref wheelY(dexsiDeviceAxis::Ref::New(module));
	AddAxis(wheelY);
	wheelY->SetIndex(2);
	wheelY->SetAbsolute(false);
	wheelY->SetWheelOtherAxis(false);
	wheelY->SetID("wheelY");
	wheelY->SetName("Wheel Y");
	wheelY->SetType(deInputDeviceAxis::eatMouseWheel);
	wheelY->SetDisplayImages("mouseY");
	wheelY->SetDisplayText("Wheel");
	
	const dexsiDeviceAxis::Ref wheelX(dexsiDeviceAxis::Ref::New(module));
	AddAxis(wheelX);
	wheelX->SetIndex(3);
	wheelX->SetAbsolute(false);
	wheelX->SetWheelOtherAxis(true);
	wheelX->SetID("wheelX");
	wheelX->SetName("Wheel X");
	wheelX->SetType(deInputDeviceAxis::eatMouseWheel);
	wheelX->SetDisplayImages("mouseX");
	wheelX->SetDisplayText("Wheel X");
	
	const dexsiDeviceButton::Ref buttonLeft(dexsiDeviceButton::Ref::New(module));
	AddButton(buttonLeft);
	buttonLeft->SetID("left");
	buttonLeft->SetName("Left");
	buttonLeft->SetDisplayImages("mouseL");
	
	const dexsiDeviceButton::Ref buttonRight(dexsiDeviceButton::Ref::New(module));
	AddButton(buttonRight);
	buttonRight->SetID("right");
	buttonRight->SetName("Right");
	buttonRight->SetDisplayImages("mouseR");
	
	const dexsiDeviceButton::Ref buttonMiddle(dexsiDeviceButton::Ref::New(module));
	AddButton(buttonMiddle);
	buttonMiddle->SetID("middle");
	buttonMiddle->SetName("Middle");
	buttonMiddle->SetDisplayImages("mouseM");
	
	const dexsiDeviceButton::Ref buttonBackward(dexsiDeviceButton::Ref::New(module));
	AddButton(buttonBackward);
	buttonBackward->SetID("backward");
	buttonBackward->SetName("Backward");
	buttonBackward->SetDisplayImages("mouseX");
	buttonBackward->SetDisplayText("B");
	
	const dexsiDeviceButton::Ref buttonForward(dexsiDeviceButton::Ref::New(module));
	AddButton(buttonForward);
	buttonForward->SetID("forward");
	buttonForward->SetName("Forward");
	buttonForward->SetDisplayImages("mouseX");
	buttonForward->SetDisplayText("F");
}

dexsiDeviceCoreMouse::~dexsiDeviceCoreMouse(){
}



// Management
///////////////

void dexsiDeviceCoreMouse::Update(){
	SendDirtyAxisEvents();
}



// Private Functions
//////////////////////

