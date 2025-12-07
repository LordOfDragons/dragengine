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
#include <errno.h>

#include "dewiDeviceMouse.h"
#include "dewiDeviceAxis.h"
#include "dewiDeviceButton.h"
#include "dewiDeviceFeedback.h"
#include "dewiDeviceManager.h"
#include "deWindowsInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSWindows.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class dewiDeviceMouse
//////////////////////////

// Constructor, destructor
////////////////////////////

dewiDeviceMouse::dewiDeviceMouse( deWindowsInput &module ) :
dewiDevice( module, esWindows )
{
	decString string;
	
	string.Format( "%s%dcm", WINP_DEVID_PREFIX, esWindows );
	SetID( string );
	SetName( " Mouse" );
	SetType( deInputDevice::edtMouse );
	
	const dewiDeviceAxis::Ref axisX(dewiDeviceAxis::Ref::NewWith(module));
	AddAxis(axisX);
	axisX->SetIndex( 0 );
	axisX->SetAbsolute( false );
	axisX->SetID( "x" );
	axisX->SetName( "X" );
	axisX->SetType( deInputDeviceAxis::eatMouse );
	axisX->SetDisplayImages( "mouseX" );
	
	const dewiDeviceAxis::Ref axisY(dewiDeviceAxis::Ref::NewWith(module));
	AddAxis(axisY);
	axisY->SetIndex( 1 );
	axisY->SetAbsolute( false );
	axisY->SetID( "y" );
	axisY->SetName( "Y" );
	axisY->SetType( deInputDeviceAxis::eatMouse );
	axisY->SetDisplayImages( "mouseY" );
	
	const dewiDeviceAxis::Ref wheelY(dewiDeviceAxis::Ref::NewWith(module));
	AddAxis(wheelY);
	wheelY->SetIndex( 2 );
	wheelY->SetAbsolute( false );
	wheelY->SetWheelOtherAxis( false );
	wheelY->SetID( "wheelY" );
	wheelY->SetName( "Wheel Y" );
	wheelY->SetType( deInputDeviceAxis::eatMouseWheel );
	wheelY->SetDisplayImages( "mouseY" );
	wheelY->SetDisplayText( "Wheel" );
	
	/*
	const dewiDeviceAxis::Ref wheelX(dewiDeviceAxis::Ref::NewWith(module));
	AddAxis(wheelX);
	wheelX->SetAbsolute( false );
	wheelX->SetWheelOtherAxis( true );
	wheelX->SetID( "wheelX" );
	wheelX->SetName( "Wheel X" );
	wheelX->SetType( deInputDeviceAxis::eatMouseWheel );
	wheelX->SetDisplayImages( "mouseX" );
	wheelX->SetDisplayText( "Wheel X" );
	*/
	
	const dewiDeviceButton::Ref buttonLeft(dewiDeviceButton::Ref::NewWith(module));
	AddButton(buttonLeft);
	buttonLeft->SetID( "left" );
	buttonLeft->SetName( "Left" );
	buttonLeft->SetDisplayImages( "mouseL" );
	
	const dewiDeviceButton::Ref buttonRight(dewiDeviceButton::Ref::NewWith(module));
	AddButton(buttonRight);
	buttonRight->SetID( "right" );
	buttonRight->SetName( "Right" );
	buttonRight->SetDisplayImages( "mouseR" );
	
	const dewiDeviceButton::Ref buttonMiddle(dewiDeviceButton::Ref::NewWith(module));
	AddButton(buttonMiddle);
	buttonMiddle->SetID( "middle" );
	buttonMiddle->SetName( "Middle" );
	buttonMiddle->SetDisplayImages( "mouseM" );
	
	const dewiDeviceButton::Ref buttonBackward(dewiDeviceButton::Ref::NewWith(module));
	AddButton(buttonBackward);
	buttonBackward->SetID( "backward" );
	buttonBackward->SetName( "Backward" );
	buttonBackward->SetDisplayImages( "mouseX" );
	buttonBackward->SetDisplayText( "B" );
	
	const dewiDeviceButton::Ref buttonForward(dewiDeviceButton::Ref::NewWith(module));
	AddButton(buttonForward);
	buttonForward->SetID( "forward" );
	buttonForward->SetName( "Forward" );
	buttonForward->SetDisplayImages( "mouseX" );
	buttonForward->SetDisplayText( "F" );
}

dewiDeviceMouse::~dewiDeviceMouse(){
}



// Management
///////////////

void dewiDeviceMouse::Update(){
	SendDirtyAxisEvents();
}



// Private Functions
//////////////////////

