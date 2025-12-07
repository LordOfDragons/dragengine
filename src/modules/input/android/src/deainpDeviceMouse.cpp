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

#include "deainpDeviceMouse.h"
#include "deainpDeviceAxis.h"
#include "deainpDeviceButton.h"
#include "deainpDeviceFeedback.h"
#include "deainpDeviceManager.h"
#include "deAndroidInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class deainpDeviceMouse
//////////////////////////

// Constructor, destructor
////////////////////////////

deainpDeviceMouse::deainpDeviceMouse(deAndroidInput &module) :
deainpDevice(module, esAndroid)
{
	decString string;
	
	string.Format("%s%dmouse", AINP_DEVID_PREFIX, deainpDevice::esAndroid);
	SetID(string);
	SetName("Mouse");
	SetType(deInputDevice::edtMouse);
	
	SetAxisCount(4);
	
	deainpDeviceAxis &axisX = GetAxisAt(0);
	axisX.SetAbsolute(false);
	axisX.SetID("x");
	axisX.SetName("X");
	axisX.SetType(deInputDeviceAxis::eatMouse);
	
	deainpDeviceAxis &axisY = GetAxisAt(1);
	axisY.SetAbsolute(false);
	axisY.SetID("y");
	axisY.SetName("Y");
	axisY.SetType(deInputDeviceAxis::eatMouse);
	
	deainpDeviceAxis &scrollY = GetAxisAt(2);
	scrollY.SetAbsolute(false);
	scrollY.SetWheelOtherAxis(false);
	scrollY.SetID("scrollY");
	scrollY.SetName("Scroll Y");
	scrollY.SetType(deInputDeviceAxis::eatMouseWheel);
	
	deainpDeviceAxis &scrollX = GetAxisAt(3);
	scrollX.SetAbsolute(false);
	scrollX.SetWheelOtherAxis(true);
	scrollX.SetID("scrollX");
	scrollX.SetName("Scroll X");
	scrollX.SetType(deInputDeviceAxis::eatMouseWheel);
	
	SetButtonCount(5);
	
	deainpDeviceButton &buttonLeft = GetButtonAt(deInputEvent::embcLeft);
	buttonLeft.SetID("left");
	buttonLeft.SetName("Left");
	buttonLeft.SetAICode(AMOTION_EVENT_BUTTON_PRIMARY);
	
	deainpDeviceButton &buttonRight = GetButtonAt(deInputEvent::embcRight);
	buttonRight.SetID("right");
	buttonRight.SetName("Right");
	buttonRight.SetAICode(AMOTION_EVENT_BUTTON_SECONDARY);
	
	deainpDeviceButton &buttonMiddle = GetButtonAt(deInputEvent::embcMiddle);
	buttonMiddle.SetID("middle");
	buttonMiddle.SetName("Middle");
	buttonMiddle.SetAICode(AMOTION_EVENT_BUTTON_TERTIARY);
	
	deainpDeviceButton &buttonBackward = GetButtonAt(3);
	buttonBackward.SetID("backward");
	buttonBackward.SetName("Backward");
	buttonBackward.SetAICode(AMOTION_EVENT_BUTTON_BACK);
	
	deainpDeviceButton &buttonForward = GetButtonAt(4);
	buttonForward.SetID("forward");
	buttonForward.SetName("Forward");
	buttonForward.SetAICode(AMOTION_EVENT_BUTTON_FORWARD);
}

deainpDeviceMouse::~deainpDeviceMouse(){
}



// Management
///////////////



// Private Functions
//////////////////////

