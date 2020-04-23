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
	deObjectReference refObject;
	
	refObject.TakeOver( new debiDeviceAxis( module ) );
	AddAxis( ( debiDeviceAxis* )( deObject* )refObject );
	debiDeviceAxis &axisX = ( debiDeviceAxis& )( deObject& )refObject;
	axisX.SetIndex( 0 );
	axisX.SetAbsolute( false );
	axisX.SetID( "x" );
	axisX.SetName( "X" );
	axisX.SetType( deInputDeviceAxis::eatMouse );
	axisX.SetDisplayImages( "mouseX" );
	
	refObject.TakeOver( new debiDeviceAxis( module ) );
	AddAxis( ( debiDeviceAxis* )( deObject* )refObject );
	debiDeviceAxis &axisY = ( debiDeviceAxis& )( deObject& )refObject;
	axisY.SetIndex( 1 );
	axisY.SetAbsolute( false );
	axisY.SetID( "y" );
	axisY.SetName( "Y" );
	axisY.SetType( deInputDeviceAxis::eatMouse );
	axisX.SetDisplayImages( "mouseY" );
	
	refObject.TakeOver( new debiDeviceAxis( module ) );
	AddAxis( ( debiDeviceAxis* )( deObject* )refObject );
	debiDeviceAxis &wheelY = ( debiDeviceAxis& )( deObject& )refObject;
	wheelY.SetIndex( 2 );
	wheelY.SetAbsolute( false );
	wheelY.SetID( "wheelY" );
	wheelY.SetName( "Wheel Y" );
	wheelY.SetType( deInputDeviceAxis::eatMouseWheel );
	wheelY.SetDisplayImages( "mouseY" );
	wheelY.SetDisplayText( "Wheel" );
	
	refObject.TakeOver( new debiDeviceAxis( module ) );
	AddAxis( ( debiDeviceAxis* )( deObject* )refObject );
	debiDeviceAxis &wheelX = ( debiDeviceAxis& )( deObject& )refObject;
	wheelX.SetIndex( 3 );
	wheelX.SetAbsolute( false );
	wheelX.SetID( "wheelX" );
	wheelX.SetName( "Wheel X" );
	wheelX.SetType( deInputDeviceAxis::eatMouseWheel );
	wheelX.SetDisplayImages( "mouseX" );
	wheelX.SetDisplayText( "Wheel" );
	
	// add buttons
	int32 buttonCount;
	if( get_mouse_type( &buttonCount ) != B_OK ){
		DETHROW( deeInvalidParam );
	}
	
	mouse_map mmap;
	if( get_mouse_map( &mmap ) != B_OK ){
		DETHROW( deeInvalidParam );
	}
	
	if( buttonCount > 0 ){
		refObject.TakeOver( new debiDeviceButton( module ) );
		AddButton( ( debiDeviceButton* )( deObject* )refObject );
		debiDeviceButton &buttonLeft = ( debiDeviceButton& )( deObject& )refObject;
		buttonLeft.SetID( "left" );
		buttonLeft.SetBICode( mmap.button[ 0 ] );
		buttonLeft.SetName( "Left" );
		buttonLeft.SetBICode( B_PRIMARY_MOUSE_BUTTON );
		buttonLeft.SetDisplayImages( "mouseL" );
	}
	
	if( buttonCount > 1 ){
		refObject.TakeOver( new debiDeviceButton( module ) );
		AddButton( ( debiDeviceButton* )( deObject* )refObject );
		debiDeviceButton &buttonRight = ( debiDeviceButton& )( deObject& )refObject;
		buttonRight.SetID( "right" );
		buttonRight.SetBICode( mmap.button[ 1 ] );
		buttonRight.SetName( "Right" );
		buttonRight.SetBICode( B_SECONDARY_MOUSE_BUTTON );
		buttonRight.SetDisplayImages( "mouseR" );
	}
	
	if( buttonCount > 2 ){
		refObject.TakeOver( new debiDeviceButton( module ) );
		AddButton( ( debiDeviceButton* )( deObject* )refObject );
		debiDeviceButton &buttonMiddle = ( debiDeviceButton& )( deObject& )refObject;
		buttonMiddle.SetID( "middle" );
		buttonMiddle.SetBICode( mmap.button[ 2 ] );
		buttonMiddle.SetName( "Middle" );
		buttonMiddle.SetBICode( B_TERTIARY_MOUSE_BUTTON );
		buttonMiddle.SetDisplayImages( "mouseM" );
	}
	
	int i;
	for( i=3; i<buttonCount; i++ ){
		refObject.TakeOver( new debiDeviceButton( module ) );
		AddButton( ( debiDeviceButton* )( deObject* )refObject );
		debiDeviceButton &button = ( debiDeviceButton& )( deObject& )refObject;
		
		string.Format( "aux%d", i - 2 );
		button.SetID( string );
		string.Format( "Aux %d", i - 2 );
		button.SetName( string );
		button.SetBICode( i ); // assuming this is correct.
		button.SetDisplayImages( "mouseX" );
		string.Format( "%d", i + 1 );
		button.SetDisplayText( string );
	}
}

debiDeviceMouse::~debiDeviceMouse(){
}



// Management
///////////////

void debiDeviceMouse::Update(){
	SendDirtyAxisEvents();
}
