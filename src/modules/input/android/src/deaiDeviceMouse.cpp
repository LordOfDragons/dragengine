/* 
 * Drag[en]gine Android Input Module
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

#include "deaiDeviceMouse.h"
#include "deaiDeviceAxis.h"
#include "deaiDeviceButton.h"
#include "deaiDeviceFeedback.h"
#include "deaiDeviceManager.h"
#include "deAndroidInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class deaiDeviceMouse
//////////////////////////

// Constructor, destructor
////////////////////////////

deaiDeviceMouse::deaiDeviceMouse( deAndroidInput &module ) :
deaiDevice( module, esAndroid )
{
	decString string;
	
	string.Format( "%s%dmouse", AINP_DEVID_PREFIX, deaiDevice::esAndroid );
	SetID( string );
	SetName( "Mouse" );
	SetType( deInputDevice::edtMouse );
	
	SetAxisCount( 4 );
	
	deaiDeviceAxis &axisX = GetAxisAt( 0 );
	axisX.SetAbsolute( false );
	axisX.SetID( "x" );
	axisX.SetName( "X" );
	axisX.SetType( deInputDeviceAxis::eatMouse );
	
	deaiDeviceAxis &axisY = GetAxisAt( 1 );
	axisY.SetAbsolute( false );
	axisY.SetID( "y" );
	axisY.SetName( "Y" );
	axisY.SetType( deInputDeviceAxis::eatMouse );
	
	deaiDeviceAxis &scrollY = GetAxisAt( 2 );
	scrollY.SetAbsolute( false );
	scrollY.SetWheelOtherAxis( false );
	scrollY.SetID( "scrollY" );
	scrollY.SetName( "Scroll Y" );
	scrollY.SetType( deInputDeviceAxis::eatMouseWheel );
	
	deaiDeviceAxis &scrollX = GetAxisAt( 3 );
	scrollX.SetAbsolute( false );
	scrollX.SetWheelOtherAxis( true );
	scrollX.SetID( "scrollX" );
	scrollX.SetName( "Scroll X" );
	scrollX.SetType( deInputDeviceAxis::eatMouseWheel );
	
	SetButtonCount( 5 );
	
	deaiDeviceButton &buttonLeft = GetButtonAt( deInputEvent::embcLeft );
	buttonLeft.SetID( "left" );
	buttonLeft.SetName( "Left" );
	buttonLeft.SetAICode( AMOTION_EVENT_BUTTON_PRIMARY );
	
	deaiDeviceButton &buttonRight = GetButtonAt( deInputEvent::embcRight );
	buttonRight.SetID( "right" );
	buttonRight.SetName( "Right" );
	buttonRight.SetAICode( AMOTION_EVENT_BUTTON_SECONDARY );
	
	deaiDeviceButton &buttonMiddle = GetButtonAt( deInputEvent::embcMiddle );
	buttonMiddle.SetID( "middle" );
	buttonMiddle.SetName( "Middle" );
	buttonMiddle.SetAICode( AMOTION_EVENT_BUTTON_TERTIARY );
	
	deaiDeviceButton &buttonBackward = GetButtonAt( 3 );
	buttonBackward.SetID( "backward" );
	buttonBackward.SetName( "Backward" );
	buttonBackward.SetAICode( AMOTION_EVENT_BUTTON_BACK );
	
	deaiDeviceButton &buttonForward = GetButtonAt( 4 );
	buttonForward.SetID( "forward" );
	buttonForward.SetName( "Forward" );
	buttonForward.SetAICode( AMOTION_EVENT_BUTTON_FORWARD );
}

deaiDeviceMouse::~deaiDeviceMouse(){
}



// Management
///////////////



// Private Functions
//////////////////////

