/* 
 * Drag[en]gine BeOS Input Module
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

debiDeviceMouse::debiDeviceMouse( deBeOSInput &module ) :
debiDevice( module )
{
	decString string;
	
	string.Format( "%s%dm", BEINP_DEVID_PREFIX, debiDevice::esBeOS );
 	SetID( string );
	SetName( "Mouse" );
	SetType( deInputDevice::edtMouse );
	
	// add axis
	SetAxisCount( 4 );
	
	debiDeviceAxis &axisX = GetAxisAt( 0 );
	axisX.SetIndex( 0 );
	axisX.SetAbsolute( false );
	axisX.SetID( "x" );
	axisX.SetName( "X" );
	axisX.SetType( deInputDeviceAxis::eatMouse );
	
	debiDeviceAxis &axisY = GetAxisAt( 1 );
	axisY.SetIndex( 1 );
	axisY.SetAbsolute( false );
	axisY.SetID( "y" );
	axisY.SetName( "Y" );
	axisY.SetType( deInputDeviceAxis::eatMouse );
	
	debiDeviceAxis &wheelY = GetAxisAt( 2 );
	wheelY.SetIndex( 2 );
	wheelY.SetAbsolute( false );
	wheelY.SetID( "wheelY" );
	wheelY.SetName( "Wheel Y" );
	wheelY.SetType( deInputDeviceAxis::eatMouseWheel );
	
	debiDeviceAxis &wheelX = GetAxisAt( 3 );
	wheelX.SetIndex( 3 );
	wheelX.SetAbsolute( false );
	wheelX.SetID( "wheelX" );
	wheelX.SetName( "Wheel X" );
	wheelX.SetType( deInputDeviceAxis::eatMouseWheel );
	
	// add buttons
	int32 buttonCount;
	if( get_mouse_type( &buttonCount ) != B_OK ){
		DETHROW( deeInvalidParam );
	}
	
	SetButtonCount( buttonCount );
	
	mouse_map mmap;
	if( get_mouse_map( &mmap ) != B_OK ){
		DETHROW( deeInvalidParam );
	}
	
    if( buttonCount > 0 ){
        debiDeviceButton &buttonLeft = GetButtonAt( deInputEvent::embcLeft );
        buttonLeft.SetID( "left" );
        buttonLeft.SetBICode( mmap.button[ 0 ] );
        buttonLeft.SetName( "Left" );
        buttonLeft.SetBICode( B_PRIMARY_MOUSE_BUTTON );
    }

    if( buttonCount > 1 ){
        debiDeviceButton &buttonRight = GetButtonAt( deInputEvent::embcRight );
        buttonRight.SetID( "right" );
        buttonRight.SetBICode( mmap.button[ 1 ] );
        buttonRight.SetName( "Right" );
        buttonRight.SetBICode( B_SECONDARY_MOUSE_BUTTON );
    }

    if( buttonCount > 2 ){
        debiDeviceButton &buttonMiddle = GetButtonAt( deInputEvent::embcMiddle );
        buttonMiddle.SetID( "middle" );
        buttonMiddle.SetBICode( mmap.button[ 2 ] );
        buttonMiddle.SetName( "Middle" );
        buttonMiddle.SetBICode( B_TERTIARY_MOUSE_BUTTON );
    }

	int i;
	for( i=3; i<buttonCount; i++ ){
		debiDeviceButton &button = GetButtonAt( i );
		
        string.Format( "aux%d", i - 2 );
        button.SetID( string );
		string.Format( "Aux %d", i - 2 );
        button.SetName( string );
        button.SetBICode( i ); // assuming this is correct.
	}
}

debiDeviceMouse::~debiDeviceMouse(){
}



// Management
///////////////

