/* 
 * Drag[en]gine X System Input Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

