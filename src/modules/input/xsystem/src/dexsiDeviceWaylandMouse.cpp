/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include <dragengine/dragengine_configuration.h>
#if defined(OS_UNIX_X11) && defined(OS_UNIX_WAYLAND)

#include <stdio.h>
#include <string.h>

#include "dexsiDeviceWaylandMouse.h"
#include "dexsiDeviceAxis.h"
#include "dexsiDeviceButton.h"
#include "dexsiDeviceFeedback.h"
#include "deXSystemInput.h"

#include <dragengine/input/deInputDevice.h>


// Class dexsiDeviceWaylandMouse
//////////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiDeviceWaylandMouse::dexsiDeviceWaylandMouse(deXSystemInput &module) :
dexsiDevice(module, esWayland)
{
	decString string;
	string.Format("%s%dwm", XINP_DEVID_PREFIX, dexsiDevice::esWayland);
	SetID(string);
	SetName("Wayland Mouse");
	SetType(deInputDevice::edtMouse);
	SetDisplayImages("mouse");
	
	auto axisX = dexsiDeviceAxis::Ref::New(module);
	AddAxis(axisX);
	axisX->SetIndex(0);
	axisX->SetAbsolute(false);
	axisX->SetID("x");
	axisX->SetName("X");
	axisX->SetType(deInputDeviceAxis::eatMouse);
	axisX->SetDisplayImages("mouseX");
	
	auto axisY = dexsiDeviceAxis::Ref::New(module);
	AddAxis(axisY);
	axisY->SetIndex(1);
	axisY->SetAbsolute(false);
	axisY->SetID("y");
	axisY->SetName("Y");
	axisY->SetType(deInputDeviceAxis::eatMouse);
	axisY->SetDisplayImages("mouseY");
	
	auto wheelY = dexsiDeviceAxis::Ref::New(module);
	AddAxis(wheelY);
	wheelY->SetIndex(2);
	wheelY->SetAbsolute(false);
	wheelY->SetWheelOtherAxis(false);
	wheelY->SetID("wheelY");
	wheelY->SetName("Wheel Y");
	wheelY->SetType(deInputDeviceAxis::eatMouseWheel);
	wheelY->SetDisplayImages("mouseY");
	wheelY->SetDisplayText("Wheel");
	
	auto wheelX = dexsiDeviceAxis::Ref::New(module);
	AddAxis(wheelX);
	wheelX->SetIndex(3);
	wheelX->SetAbsolute(false);
	wheelX->SetWheelOtherAxis(true);
	wheelX->SetID("wheelX");
	wheelX->SetName("Wheel X");
	wheelX->SetType(deInputDeviceAxis::eatMouseWheel);
	wheelX->SetDisplayImages("mouseX");
	wheelX->SetDisplayText("Wheel X");
	
	auto buttonLeft = dexsiDeviceButton::Ref::New(module);
	AddButton(buttonLeft);
	buttonLeft->SetID("left");
	buttonLeft->SetName("Left");
	buttonLeft->SetDisplayImages("mouseL");
	
	auto buttonRight = dexsiDeviceButton::Ref::New(module);
	AddButton(buttonRight);
	buttonRight->SetID("right");
	buttonRight->SetName("Right");
	buttonRight->SetDisplayImages("mouseR");
	
	auto buttonMiddle = dexsiDeviceButton::Ref::New(module);
	AddButton(buttonMiddle);
	buttonMiddle->SetID("middle");
	buttonMiddle->SetName("Middle");
	buttonMiddle->SetDisplayImages("mouseM");
	
	auto buttonBackward = dexsiDeviceButton::Ref::New(module);
	AddButton(buttonBackward);
	buttonBackward->SetID("backward");
	buttonBackward->SetName("Backward");
	buttonBackward->SetDisplayImages("mouseX");
	buttonBackward->SetDisplayText("B");
	
	auto buttonForward = dexsiDeviceButton::Ref::New(module);
	AddButton(buttonForward);
	buttonForward->SetID("forward");
	buttonForward->SetName("Forward");
	buttonForward->SetDisplayImages("mouseX");
	buttonForward->SetDisplayText("F");
}

dexsiDeviceWaylandMouse::~dexsiDeviceWaylandMouse() = default;


// Management
///////////////

void dexsiDeviceWaylandMouse::Update(){
	SendDirtyAxisEvents();
}

#endif // OS_UNIX_X11 && OS_UNIX_WAYLAND
