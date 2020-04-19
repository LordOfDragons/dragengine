/* 
 * Drag[en]gine MacOS Input Module
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

#include "demoiDevice.h"
#include "demoiDeviceMouse.h"
#include "demoiDeviceAxis.h"
#include "demoiDeviceButton.h"
#include "deMacOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>


// Class demoiDeviceMouse
///////////////////////////

// Constructor, destructor
////////////////////////////

demoiDeviceMouse::demoiDeviceMouse( deMacOSInput &module ) :
demoiDevice( module, esMO )
{
    decString string;
    
    string.Format( "%s%dm", MOINP_DEVID_PREFIX, demoiDevice::esMO );
    SetID( string );
    SetName( "Mouse" );
    SetType( deInputDevice::edtMouse );
    
    // TODO figure out how to figure out mouse configuration on MacOS
    
    // axes
    SetAxisCount( 4 );
    
    demoiDeviceAxis &axisX = GetAxisAt( 0 );
    axisX.SetAbsolute( false );
    axisX.SetID( "x" );
    axisX.SetName( "X" );
    axisX.SetType( deInputDeviceAxis::eatMouse );
    
    demoiDeviceAxis &axisY = GetAxisAt( 1 );
    axisY.SetAbsolute( false );
    axisY.SetID( "y" );
    axisY.SetName( "Y" );
    axisY.SetType( deInputDeviceAxis::eatMouse );
    
    demoiDeviceAxis &wheelY = GetAxisAt( 2 );
    wheelY.SetAbsolute( false );
    wheelY.SetWheelOtherAxis( false );
    wheelY.SetID( "wheelY" );
    wheelY.SetName( "Wheel Y" );
    wheelY.SetType( deInputDeviceAxis::eatMouseWheel );
    
    demoiDeviceAxis &wheelX = GetAxisAt( 3 );
    wheelX.SetAbsolute( false );
    wheelX.SetWheelOtherAxis( true );
    wheelX.SetID( "wheelX" );
    wheelX.SetName( "Wheel X" );
    wheelX.SetType( deInputDeviceAxis::eatMouseWheel );
    
    // buttons
    SetButtonCount( 5 );
    
    demoiDeviceButton &buttonLeft = GetButtonAt( deInputEvent::embcLeft );
    buttonLeft.SetID( "left" );
    buttonLeft.SetName( "Left" );
    
    demoiDeviceButton &buttonRight = GetButtonAt( deInputEvent::embcRight );
    buttonRight.SetID( "right" );
    buttonRight.SetName( "Right" );
    
    demoiDeviceButton &buttonMiddle = GetButtonAt( deInputEvent::embcMiddle );
    buttonMiddle.SetID( "middle" );
    buttonMiddle.SetName( "Middle" );
    
    demoiDeviceButton &buttonBackward = GetButtonAt( 3 );
    buttonBackward.SetID( "backward" );
    buttonBackward.SetName( "Backward" );
    
    demoiDeviceButton &buttonForward = GetButtonAt( 4 );
    buttonForward.SetID( "forward" );
    buttonForward.SetName( "Forward" );
}

demoiDeviceMouse::~demoiDeviceMouse(){
}



// Management
///////////////
