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
	debiDeviceAxis::Ref axisX(debiDeviceAxis::Ref::NewWith(module));
	AddAxis( axisX );
	axisX->SetIndex( 0 );
	axisX->SetAbsolute( false );
	axisX->SetID( "x" );
	axisX->SetName( "X" );
	axisX->SetType( deInputDeviceAxis::eatMouse );
	axisX->SetDisplayImages( "mouseX" );
	
	refObject.TakeOver( new debiDeviceAxis( module ) );
	AddAxis( refObject );
	debiDeviceAxis &axisY = ( debiDeviceAxis& )( deObject& )refObject;
	axisY.SetIndex( 1 );
	axisY.SetAbsolute( false );
	axisY.SetID( "y" );
	axisY.SetName( "Y" );
	axisY.SetType( deInputDeviceAxis::eatMouse );
	axisY.SetDisplayImages( "mouseY" );
	
	refObject.TakeOver( new debiDeviceAxis( module ) );
	AddAxis( refObject );
	debiDeviceAxis &wheelY = ( debiDeviceAxis& )( deObject& )refObject;
	wheelY.SetIndex( 2 );
	wheelY.SetAbsolute( false );
	wheelY.SetID( "wheelY" );
	wheelY.SetName( "Wheel Y" );
	wheelY.SetType( deInputDeviceAxis::eatMouseWheel );
	wheelY.SetDisplayImages( "mouseY" );
	wheelY.SetDisplayText( "Wheel" );
	
	refObject.TakeOver( new debiDeviceAxis( module ) );
	AddAxis( refObject );
	debiDeviceAxis &wheelX = ( debiDeviceAxis& )( deObject& )refObject;
	wheelX.SetIndex( 3 );
	wheelX.SetAbsolute( false );
	wheelX.SetID( "wheelX" );
	wheelX.SetName( "Wheel X" );
	wheelX.SetType( deInputDeviceAxis::eatMouseWheel );
	wheelX.SetDisplayImages( "mouseX" );
	wheelX.SetDisplayText( "Wheel" );
	
	// add buttons
	int32 buttonCount = 3;
	// no more working. there is a constant B_MAX_MOUSE_BUTTONS but mmap.button[i] returns
	// just 1<<i which is kinda pointless. assuming 3 buttons and wait until this is fixed
	/*if( get_mouse_type( &buttonCount ) != B_OK ){
		DETHROW( deeInvalidParam );
	}*/
	
	mouse_map mmap;
	if( get_mouse_map( &mmap ) != B_OK ){
		DETHROW( deeInvalidParam );
	}
	
	if( buttonCount > 0 ){
		refObject.TakeOver( new debiDeviceButton( module ) );
		AddButton( refObject );
		debiDeviceButton &buttonLeft = ( debiDeviceButton& )( deObject& )refObject;
		buttonLeft.SetID( "left" );
		buttonLeft.SetBICode( mmap.button[ 0 ] );
		buttonLeft.SetName( "Left" );
		buttonLeft.SetBICode( B_PRIMARY_MOUSE_BUTTON );
		buttonLeft.SetDisplayImages( "mouseL" );
	}
	
	if( buttonCount > 1 ){
		refObject.TakeOver( new debiDeviceButton( module ) );
		AddButton( refObject );
		debiDeviceButton &buttonRight = ( debiDeviceButton& )( deObject& )refObject;
		buttonRight.SetID( "right" );
		buttonRight.SetBICode( mmap.button[ 1 ] );
		buttonRight.SetName( "Right" );
		buttonRight.SetBICode( B_SECONDARY_MOUSE_BUTTON );
		buttonRight.SetDisplayImages( "mouseR" );
	}
	
	if( buttonCount > 2 ){
		refObject.TakeOver( new debiDeviceButton( module ) );
		AddButton( refObject );
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
		AddButton( refObject );
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
