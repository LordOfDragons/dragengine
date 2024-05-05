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

dexsiDeviceCoreMouse::dexsiDeviceCoreMouse( deXSystemInput &module ) :
dexsiDevice( module, esX11 )
{
	decString string;
	
	string.Format( "%s%dcm", XINP_DEVID_PREFIX, dexsiDevice::esX11 );
	SetID( string );
	SetName( "Core Mouse" );
	SetType( deInputDevice::edtMouse );
	SetDisplayImages( "mouse" );
	
	// x11 does not provide a way to figure out mouse configuration. using XInput2 should help
	deObjectReference refObject;
	
	refObject.TakeOver( new dexsiDeviceAxis( module ) );
	AddAxis( ( dexsiDeviceAxis* )( deObject* )refObject );
	dexsiDeviceAxis &axisX = ( dexsiDeviceAxis& )( deObject& )refObject;
	axisX.SetIndex( 0 );
	axisX.SetAbsolute( false );
	axisX.SetID( "x" );
	axisX.SetName( "X" );
	axisX.SetType( deInputDeviceAxis::eatMouse );
	axisX.SetDisplayImages( "mouseX" );
	
	refObject.TakeOver( new dexsiDeviceAxis( module ) );
	AddAxis( ( dexsiDeviceAxis* )( deObject* )refObject );
	dexsiDeviceAxis &axisY = ( dexsiDeviceAxis& )( deObject& )refObject;
	axisY.SetIndex( 1 );
	axisY.SetAbsolute( false );
	axisY.SetID( "y" );
	axisY.SetName( "Y" );
	axisY.SetType( deInputDeviceAxis::eatMouse );
	axisY.SetDisplayImages( "mouseY" );
	
	refObject.TakeOver( new dexsiDeviceAxis( module ) );
	AddAxis( ( dexsiDeviceAxis* )( deObject* )refObject );
	dexsiDeviceAxis &wheelY = ( dexsiDeviceAxis& )( deObject& )refObject;
	wheelY.SetIndex( 2 );
	wheelY.SetAbsolute( false );
	wheelY.SetWheelOtherAxis( false );
	wheelY.SetID( "wheelY" );
	wheelY.SetName( "Wheel Y" );
	wheelY.SetType( deInputDeviceAxis::eatMouseWheel );
	wheelY.SetDisplayImages( "mouseY" );
	wheelY.SetDisplayText( "Wheel" );
	
	refObject.TakeOver( new dexsiDeviceAxis( module ) );
	AddAxis( ( dexsiDeviceAxis* )( deObject* )refObject );
	dexsiDeviceAxis &wheelX = ( dexsiDeviceAxis& )( deObject& )refObject;
	wheelX.SetIndex( 3 );
	wheelX.SetAbsolute( false );
	wheelX.SetWheelOtherAxis( true );
	wheelX.SetID( "wheelX" );
	wheelX.SetName( "Wheel X" );
	wheelX.SetType( deInputDeviceAxis::eatMouseWheel );
	wheelX.SetDisplayImages( "mouseX" );
	wheelX.SetDisplayText( "Wheel X" );
	
	refObject.TakeOver( new dexsiDeviceButton( module ) );
	AddButton( ( dexsiDeviceButton* )( deObject* )refObject );
	dexsiDeviceButton &buttonLeft = ( dexsiDeviceButton& )( deObject& )refObject;
	buttonLeft.SetID( "left" );
	buttonLeft.SetName( "Left" );
	buttonLeft.SetDisplayImages( "mouseL" );
	
	refObject.TakeOver( new dexsiDeviceButton( module ) );
	AddButton( ( dexsiDeviceButton* )( deObject* )refObject );
	dexsiDeviceButton &buttonRight = ( dexsiDeviceButton& )( deObject& )refObject;
	buttonRight.SetID( "right" );
	buttonRight.SetName( "Right" );
	buttonRight.SetDisplayImages( "mouseR" );
	
	refObject.TakeOver( new dexsiDeviceButton( module ) );
	AddButton( ( dexsiDeviceButton* )( deObject* )refObject );
	dexsiDeviceButton &buttonMiddle = ( dexsiDeviceButton& )( deObject& )refObject;
	buttonMiddle.SetID( "middle" );
	buttonMiddle.SetName( "Middle" );
	buttonMiddle.SetDisplayImages( "mouseM" );
	
	refObject.TakeOver( new dexsiDeviceButton( module ) );
	AddButton( ( dexsiDeviceButton* )( deObject* )refObject );
	dexsiDeviceButton &buttonBackward = ( dexsiDeviceButton& )( deObject& )refObject;
	buttonBackward.SetID( "backward" );
	buttonBackward.SetName( "Backward" );
	buttonBackward.SetDisplayImages( "mouseX" );
	buttonBackward.SetDisplayText( "B" );
	
	refObject.TakeOver( new dexsiDeviceButton( module ) );
	AddButton( ( dexsiDeviceButton* )( deObject* )refObject );
	dexsiDeviceButton &buttonForward = ( dexsiDeviceButton& )( deObject& )refObject;
	buttonForward.SetID( "forward" );
	buttonForward.SetName( "Forward" );
	buttonForward.SetDisplayImages( "mouseX" );
	buttonForward.SetDisplayText( "F" );
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

