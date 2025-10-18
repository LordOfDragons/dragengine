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

#include "dewiDeviceWinRTController.h"
#include "dewiDeviceAxis.h"
#include "dewiDeviceButton.h"
#include "dewiDeviceFeedback.h"
#include "dewiDeviceManager.h"
#include "deWindowsInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSWindows.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>

#include <winrt/Windows.System.Power.h>

namespace wrsp = winrt::Windows::System::Power;



// Class dewiDeviceWinRTController
////////////////////////////////////

// Constructor, destructor
////////////////////////////

dewiDeviceWinRTController::dewiDeviceWinRTController(
	deWindowsInput &module, wrgi::RawGameController const &controller ) :
dewiDevice( module, esWinRTController ),
pController( controller ),
pGamepad( wrgi::Gamepad::FromGameController( controller ) ),
pReadingButtonSize( 0 ),
pReadingTime(0),
pBatteryReport({})
{
	decString string;

	string.Format( "%s%d%s", WINP_DEVID_PREFIX, esWinRTController,
		dewiDeviceManager::NormalizeID( deOSWindows::WideToUtf8(
			controller.NonRoamableId().c_str() ) ).GetString() );
	SetID( string );
	SetName( deOSWindows::WideToUtf8( controller.DisplayName().c_str() ) );

	if( pGamepad ){
		SetType( deInputDevice::edtGamepad );
		SetDisplayImages( "gamepad" );

	}else{
		SetType( deInputDevice::edtGeneric );
	}

	// add axes
	int i, count = controller.AxisCount();

	for( i=0; i<count; i++ ){
		const dewiDeviceAxis::Ref axis( dewiDeviceAxis::Ref::New( new dewiDeviceAxis( module ) ) );
		axis->SetIndex( GetAxisCount() );
		axis->SetAbsolute( true );
		axis->SetMinimum( -1000 );
		axis->SetMaximum( 1000 );

		if( pGamepad && count >= 6 ){
			switch( i ){
			case 0:
				axis->SetType( deInputDeviceAxis::eatStick );
				axis->SetDisplayImages( "stickX" );
				axis->SetID( "sxl" );
				axis->SetDisplayText( "L" );
				axis->SetName( "Left Stick X" );
				axis->SetFlat( 100 );
				axis->SetWinRTReadingIndexAxis( 1 );
				break;

			case 1:
				axis->SetType( deInputDeviceAxis::eatStick );
				axis->SetDisplayImages( "stickY" );
				axis->SetID( "syl" );
				axis->SetDisplayText( "L" );
				axis->SetName( "Left Stick Y" );
				axis->SetFlat( 100 );
				axis->SetWinRTReadingIndexAxis( 0 );
				axis->SetWinRTInverseAxis( true );
				break;

			case 2:
				axis->SetType( deInputDeviceAxis::eatStick );
				axis->SetDisplayImages( "stickX" );
				axis->SetID( "sxr" );
				axis->SetDisplayText( "R" );
				axis->SetName( "Right Stick X" );
				axis->SetFlat( 100 );
				axis->SetWinRTReadingIndexAxis( 3 );
				break;

			case 3:
				axis->SetType( deInputDeviceAxis::eatStick );
				axis->SetDisplayImages( "stickY" );
				axis->SetID( "syr" );
				axis->SetDisplayText( "R" );
				axis->SetName( "Right Stick Y" );
				axis->SetFlat( 100 );
				axis->SetWinRTReadingIndexAxis( 2 );
				axis->SetWinRTInverseAxis( true );
				break;

			case 4:
				axis->SetType( deInputDeviceAxis::eatTrigger );
				axis->SetDisplayImages( "trigger" );
				axis->SetID( "trl" );
				axis->SetDisplayText( "L" );
				axis->SetName( "Left Trigger" );
				axis->SetWinRTReadingIndexAxis( 4 );
				break;

			case 5:
				axis->SetType( deInputDeviceAxis::eatTrigger );
				axis->SetDisplayImages( "trigger" );
				axis->SetID( "trr" );
				axis->SetDisplayText( "R" );
				axis->SetName( "Right Trigger" );
				axis->SetWinRTReadingIndexAxis( 5 );
				break;

			default:
				axis->SetType( deInputDeviceAxis::eatGeneric );
				axis->SetDisplayImages( "stick" );
				string.Format( "aa%d", i );
				axis->SetID( string );
				string.Format( "%d", i );
				axis->SetDisplayText( string );
				string.Format( "Axis %d", i );
				axis->SetName( string );
				axis->SetWinRTReadingIndexAxis( i );
			}

		}else{
			axis->SetType( deInputDeviceAxis::eatGeneric );
			axis->SetDisplayImages( "stick" );
			string.Format( "aa%d", i );
			axis->SetID( string );
			string.Format( "%d", i );
			axis->SetDisplayText( string );
			string.Format( "Axis %d", i );
			axis->SetName( string );
			axis->SetWinRTReadingIndexAxis( i );
		}

		AddAxis( axis );
	}

	// add arrow keys as first switch
	dewiDeviceAxis::Ref axisDPadX(dewiDeviceAxis::Ref::New(new dewiDeviceAxis(module)));
	axisDPadX->SetIndex(GetAxisCount());
	axisDPadX->SetAbsolute(true);
	axisDPadX->SetMinimum(-1);
	axisDPadX->SetMaximum(1);
	axisDPadX->SetType(deInputDeviceAxis::eatHat);
	axisDPadX->SetDisplayImages("stickX");
	axisDPadX->SetID("dpadx");
	axisDPadX->SetName("DPad X");

	dewiDeviceAxis::Ref axisDPadY(dewiDeviceAxis::Ref::New(new dewiDeviceAxis(module)));
	axisDPadY->SetIndex(GetAxisCount() + 1);
	axisDPadY->SetAbsolute(true);
	axisDPadY->SetMinimum(-1);
	axisDPadY->SetMaximum(1);
	axisDPadY->SetType(deInputDeviceAxis::eatHat);
	axisDPadY->SetDisplayImages("stickY");
	axisDPadY->SetID("dpady");
	axisDPadY->SetName("DPad Y");

	count = controller.ButtonCount();

	for(i=0; i<count; i++){
		switch(controller.GetButtonLabel(i)){
		case wrgi::GameControllerButtonLabel::XboxUp:
		case wrgi::GameControllerButtonLabel::Up:
			axisDPadY->SetWinRTReadingButtonNegative(i);
			break;

		case wrgi::GameControllerButtonLabel::XboxDown:
		case wrgi::GameControllerButtonLabel::Down:
			axisDPadY->SetWinRTReadingButtonPositive(i);
			break;

		case wrgi::GameControllerButtonLabel::XboxLeft:
		case wrgi::GameControllerButtonLabel::Left:
			axisDPadX->SetWinRTReadingButtonNegative(i);
			break;

		case wrgi::GameControllerButtonLabel::XboxRight:
		case wrgi::GameControllerButtonLabel::Right:
			axisDPadX->SetWinRTReadingButtonPositive(i);
			break;
			
		default:
			break;
		}
	}

	if(axisDPadX->GetWinRTReadingButtonPositive() != -1 && axisDPadX->GetWinRTReadingButtonNegative() != -1){
		AddAxis(axisDPadX);

	}else{
		axisDPadX = nullptr;
	}

	if(axisDPadY->GetWinRTReadingButtonPositive() != -1 && axisDPadY->GetWinRTReadingButtonNegative() != -1){
		AddAxis(axisDPadY);

	}else{
		axisDPadY = nullptr;
	}

	// add switches
	count = controller.SwitchCount();

	for( i=0; i<count; i++ ){
		const dewiDeviceAxis::Ref axis( dewiDeviceAxis::Ref::New( new dewiDeviceAxis( module ) ) );
		axis->SetIndex( GetAxisCount() );
		axis->SetAbsolute( true );
		axis->SetMinimum( -1 );
		axis->SetMaximum( 1 );
		axis->SetType( deInputDeviceAxis::eatHat );
		axis->SetWinRTReadingIndexSwitch( i );
		
		if( ( i % 2 ) == 0 ){
			axis->SetDisplayImages( "stickX" );
			string.Format( "hx%d", i / 2 );
			axis->SetID( string );
			axis->SetWinRTReadingDirectionSwitch( 0 );
			string.Format( "Hat %d X", i );
			axis->SetName( string );
			
		}else{
			axis->SetDisplayImages( "stickY" );
			string.Format( "hy%d", i / 2 );
			axis->SetID( string );
			axis->SetWinRTReadingDirectionSwitch( -1 );
			string.Format( "Hat %d Y", i );
			axis->SetName( string );
		}

		string.Format( "H%d", ( i / 2 ) + 1 );
		axis->SetDisplayText( string );

		AddAxis( axis );
	}

	// add buttons
	count = controller.ButtonCount();

	for( i=0; i<count; i++ ){
		const dewiDeviceButton::Ref button( dewiDeviceButton::Ref::New( new dewiDeviceButton( module ) ) );
		
		button->SetWinRTReadingIndex( i );

		switch( controller.GetButtonLabel( i ) ){
		case wrgi::GameControllerButtonLabel::XboxBack:
		case wrgi::GameControllerButtonLabel::Back:
			button->SetName( "Back" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "baback" );
			button->SetDisplayText( "Back" );
			break;

		case wrgi::GameControllerButtonLabel::XboxStart:
		case wrgi::GameControllerButtonLabel::Start:
			button->SetName( "Start" );
			button->SetType( deInputDeviceButton::ebtHome );
			button->SetDisplayImages( "button" );
			button->SetID( "bastart" );
			button->SetDisplayText( "Start" );
			break;

		case wrgi::GameControllerButtonLabel::XboxMenu:
		case wrgi::GameControllerButtonLabel::Menu:
			button->SetName( "Menu" );
			button->SetType( deInputDeviceButton::ebtHome );
			button->SetDisplayImages( "button" );
			button->SetID( "bamenu" );
			button->SetDisplayText( "Menu" );
			break;

		case wrgi::GameControllerButtonLabel::XboxView:
		case wrgi::GameControllerButtonLabel::View:
			button->SetName( "View" );
			button->SetType( deInputDeviceButton::ebtSelect );
			button->SetDisplayImages( "button" );
			button->SetID( "baview" );
			button->SetDisplayText( "View" );
			break;

		case wrgi::GameControllerButtonLabel::XboxUp:
		case wrgi::GameControllerButtonLabel::Up:
			if(axisDPadY){
				continue;
			}

			button->SetName( "Up" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "baup" );
			button->SetDisplayText( "Up" );
			break;

		case wrgi::GameControllerButtonLabel::XboxDown:
		case wrgi::GameControllerButtonLabel::Down:
			if(axisDPadY){
				continue;
			}

			button->SetName( "Down" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "badown" );
			button->SetDisplayText( "Down" );
			break;

		case wrgi::GameControllerButtonLabel::XboxLeft:
		case wrgi::GameControllerButtonLabel::Left:
			if(axisDPadX){
				continue;
			}

			button->SetName( "Left" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "baleft" );
			button->SetDisplayText( "Left" );
			break;

		case wrgi::GameControllerButtonLabel::XboxRight:
		case wrgi::GameControllerButtonLabel::Right:
			if(axisDPadX){
				continue;
			}

			button->SetName("Right");
			button->SetType(deInputDeviceButton::ebtGeneric);
			button->SetDisplayImages("button");
			button->SetID("baright");
			button->SetDisplayText("Right");
			break;

		case wrgi::GameControllerButtonLabel::XboxA:
		case wrgi::GameControllerButtonLabel::LetterA:
			button->SetName( "A" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "baa" );
			button->SetDisplayText( "A" );
			break;

		case wrgi::GameControllerButtonLabel::XboxB:
		case wrgi::GameControllerButtonLabel::LetterB:
			button->SetName( "B" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "bab" );
			button->SetDisplayText( "B" );
			break;

		case wrgi::GameControllerButtonLabel::XboxX:
		case wrgi::GameControllerButtonLabel::LetterX:
			button->SetName( "X" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "bax" );
			button->SetDisplayText( "X" );
			break;

		case wrgi::GameControllerButtonLabel::XboxY:
		case wrgi::GameControllerButtonLabel::LetterY:
			button->SetName( "Y" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "bay" );
			button->SetDisplayText( "Y" );
			break;

		case wrgi::GameControllerButtonLabel::Cross:
			button->SetName( "Cross" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "bacross" );
			button->SetDisplayText( "X" );
			break;

		case wrgi::GameControllerButtonLabel::Circle:
			button->SetName( "Circle" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "bacircle" );
			button->SetDisplayText( "O" );
			break;

		case wrgi::GameControllerButtonLabel::Square:
			button->SetName( "Square" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "basquare" );
			button->SetDisplayText( "S" );
			break;

		case wrgi::GameControllerButtonLabel::Triangle:
			button->SetName( "Triangle" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "batriangle" );
			button->SetDisplayText( "T" );
			break;

		case wrgi::GameControllerButtonLabel::XboxLeftBumper:
		case wrgi::GameControllerButtonLabel::LeftBumper:
			button->SetName( "Shoulder Left" );
			button->SetType( deInputDeviceButton::ebtShoulderLeft );
			button->SetDisplayImages( "button" );
			button->SetID( "bashl" );
			button->SetDisplayText( "SL" );
			break;

		case wrgi::GameControllerButtonLabel::XboxRightBumper:
		case wrgi::GameControllerButtonLabel::RightBumper:
			button->SetName( "Shoulder Right" );
			button->SetType( deInputDeviceButton::ebtShoulderRight );
			button->SetDisplayImages( "button" );
			button->SetID( "bashr" );
			button->SetDisplayText( "SR" );
			break;

		case wrgi::GameControllerButtonLabel::XboxLeftTrigger:
		case wrgi::GameControllerButtonLabel::LeftTrigger:
			button->SetName( "Trigger Left" );
			button->SetType( deInputDeviceButton::ebtTrigger );
			button->SetDisplayImages( "button" );
			button->SetID( "batl" );
			button->SetDisplayText( "TL" );
			break;

		case wrgi::GameControllerButtonLabel::XboxRightTrigger:
		case wrgi::GameControllerButtonLabel::RightTrigger:
			button->SetName( "Trigger Right" );
			button->SetType( deInputDeviceButton::ebtTrigger );
			button->SetDisplayImages( "button" );
			button->SetID( "batr" );
			button->SetDisplayText( "TR" );
			break;

		case wrgi::GameControllerButtonLabel::XboxLeftStickButton:
		case wrgi::GameControllerButtonLabel::LeftStickButton:
			button->SetName( "Stick Left" );
			button->SetType( deInputDeviceButton::ebtStick );
			button->SetDisplayImages( "button" );
			button->SetID( "bastl" );
			button->SetDisplayText( "StL" );
			break;

		case wrgi::GameControllerButtonLabel::XboxRightStickButton:
		case wrgi::GameControllerButtonLabel::RightStickButton:
			button->SetName( "Stick Right" );
			button->SetType( deInputDeviceButton::ebtStick );
			button->SetDisplayImages( "button" );
			button->SetID( "bastr" );
			button->SetDisplayText( "StR" );
			break;

		case wrgi::GameControllerButtonLabel::XboxPaddle1:
		case wrgi::GameControllerButtonLabel::Paddle1:
			button->SetName( "Paddle 1" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "bap1" );
			button->SetDisplayText( "P1" );
			break;

		case wrgi::GameControllerButtonLabel::XboxPaddle2:
		case wrgi::GameControllerButtonLabel::Paddle2:
			button->SetName( "Paddle 2" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "bap2" );
			button->SetDisplayText( "P2" );
			break;

		case wrgi::GameControllerButtonLabel::XboxPaddle3:
		case wrgi::GameControllerButtonLabel::Paddle3:
			button->SetName( "Paddle 3" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "bap3" );
			button->SetDisplayText( "P3" );
			break;

		case wrgi::GameControllerButtonLabel::XboxPaddle4:
		case wrgi::GameControllerButtonLabel::Paddle4:
			button->SetName( "Paddle 4" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "bap4" );
			button->SetDisplayText( "P4" );
			break;

		case wrgi::GameControllerButtonLabel::Mode:
			button->SetName( "Mode" );
			button->SetType( deInputDeviceButton::ebtHome );
			button->SetDisplayImages( "button" );
			button->SetID( "bamode" );
			button->SetDisplayText( "Mode" );
			break;

		case wrgi::GameControllerButtonLabel::Select:
			button->SetName( "Select" );
			button->SetType( deInputDeviceButton::ebtSelect );
			button->SetDisplayImages( "button" );
			button->SetID( "basel" );
			button->SetDisplayText( "Select" );
			break;

		case wrgi::GameControllerButtonLabel::Options:
			button->SetName( "Options" );
			button->SetType( deInputDeviceButton::ebtSelect );
			button->SetDisplayImages( "button" );
			button->SetID( "baopt" );
			button->SetDisplayText( "Opt" );
			break;

		case wrgi::GameControllerButtonLabel::Share:
			button->SetName( "Share" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "bashare" );
			button->SetDisplayText( "Share" );
			break;

		case wrgi::GameControllerButtonLabel::LetterC:
			button->SetName( "C" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "bac" );
			button->SetDisplayText( "C" );
			break;

		case wrgi::GameControllerButtonLabel::LetterL:
			button->SetName( "L" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "bal" );
			button->SetDisplayText( "L" );
			break;

		case wrgi::GameControllerButtonLabel::LetterR:
			button->SetName( "R" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "bar" );
			button->SetDisplayText( "R" );
			break;

		case wrgi::GameControllerButtonLabel::LetterZ:
			button->SetName( "Z" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "baz" );
			button->SetDisplayText( "Z" );
			break;

		case wrgi::GameControllerButtonLabel::Left1:
			button->SetName( "Left 1" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "baleft1" );
			button->SetDisplayText( "Left1" );
			break;

		case wrgi::GameControllerButtonLabel::Left2:
			button->SetName( "Left 2" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "baleft2" );
			button->SetDisplayText( "Left2" );
			break;

		case wrgi::GameControllerButtonLabel::Left3:
			button->SetName( "Left 3" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "baleft3" );
			button->SetDisplayText( "Left3" );
			break;

		case wrgi::GameControllerButtonLabel::Right1:
			button->SetName( "Right 1" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "baright1" );
			button->SetDisplayText( "Right1" );
			break;

		case wrgi::GameControllerButtonLabel::Right2:
			button->SetName( "Right 2" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "baright2" );
			button->SetDisplayText( "Right2" );
			break;

		case wrgi::GameControllerButtonLabel::Right3:
			button->SetName( "Right 3" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "baright3" );
			button->SetDisplayText( "Right3" );
			break;

		case wrgi::GameControllerButtonLabel::Plus:
			button->SetName( "+" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "baplus" );
			button->SetDisplayText( "+" );
			break;

		case wrgi::GameControllerButtonLabel::Minus:
			button->SetName( "-" );
			button->SetType( deInputDeviceButton::ebtAction );
			button->SetDisplayImages( "button" );
			button->SetID( "baminus" );
			button->SetDisplayText( "-" );
			break;

		case wrgi::GameControllerButtonLabel::DownLeftArrow:
			button->SetName( "Down-Left Arrow" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "badla" );
			button->SetDisplayText( "DLA" );
			break;

		case wrgi::GameControllerButtonLabel::DialLeft:
			button->SetName( "Dial Left" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "badiall" );
			button->SetDisplayText( "DialL" );
			break;

		case wrgi::GameControllerButtonLabel::DialRight:
			button->SetName( "Dial Right" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "badialr" );
			button->SetDisplayText( "DialR" );
			break;

		case wrgi::GameControllerButtonLabel::Suspension:
			button->SetName( "Suspension" );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			button->SetID( "basusp" );
			button->SetDisplayText( "Susp" );
			break;

		default:
			string.Format( "Button %d", i );
			button->SetName( string );
			button->SetType( deInputDeviceButton::ebtGeneric );
			button->SetDisplayImages( "button" );
			string.Format( "bg%d", i );
			button->SetID( string );
			string.Format( "%d", i );
			button->SetDisplayText( string );
		}

		AddButton( button );
	}

	// if battery charge is supported add battery level axis
	const wrdp::BatteryReport batteryReport = controller.TryGetBatteryReport();
	if(batteryReport){
		if( batteryReport.RemainingCapacityInMilliwattHours() && batteryReport.FullChargeCapacityInMilliwattHours() ){
			const dewiDeviceAxis::Ref axis( dewiDeviceAxis::Ref::New( new dewiDeviceAxis( module ) ) );
			axis->SetIndex( GetAxisCount() );
			axis->SetType( deInputDeviceAxis::eatBatteryLevel );
			axis->SetID( "batlvl" );
			axis->SetAbsolute( true );
			axis->SetMinimum( 0 );
			axis->SetMaximum( 100 );
			axis->SetName( "Battery Level" );
			axis->SetDisplayText( "Bat" );
			axis->SetIsBatteryLevel( true );

			AddAxis( axis );
		}

		if( batteryReport.Status() != wrsp::BatteryStatus::NotPresent ){
			const dewiDeviceButton::Ref button( dewiDeviceButton::Ref::New( new dewiDeviceButton( module ) ) );
			button->SetType( deInputDeviceButton::ebtBatteryCharging );
			button->SetID( "batcha" );
			button->SetName( "Battery Charging" );
			button->SetDisplayText( "BatCh" );
			button->SetIsBatteryCharging( true );

			AddButton( button );
		}
	}

	// allocate arrays
	if( controller.AxisCount() > 0 ){
		pReadingAxis.assign( controller.AxisCount(), 0.0 );
	}
	if( controller.SwitchCount() > 0 ){
		pReadingSwitch.assign( controller.SwitchCount(), wrgi::GameControllerSwitchPosition::Center );
	}
	if( controller.ButtonCount() > 0 ){
		pReadingButton = std::make_unique<bool[]>( controller.ButtonCount() );
		pReadingButtonSize = controller.ButtonCount();
	}
}

dewiDeviceWinRTController::~dewiDeviceWinRTController(){
}



// Management
///////////////

void dewiDeviceWinRTController::Update(){
	// GetCurrentReading returns an uint64_t timestamp but the documentation does not tell
	// what time unit it uses so not usable. sticking to timeGetTime(). better than nothing
	pController.GetCurrentReading(
		winrt::array_view<bool>( pReadingButton.get(), pReadingButton.get() + pReadingButtonSize ),
		pReadingSwitch, pReadingAxis );
	pReadingTime = timeGetTime();

	
	pBatteryReport = pController.TryGetBatteryReport();

	int i, count = GetAxisCount();
	for( i=0; i<count; i++ ){
		GetAxisAt( i )->WinRTReading( *this );
	}

	count = GetButtonCount();
	for( i=0; i<count; i++ ){
		GetButtonAt( i )->WinRTReading( *this );
	}

	SendDirtyAxisEvents();
}

double dewiDeviceWinRTController::GetReadingAxis( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pReadingAxis.size() )

	return pReadingAxis[ index ];
}

wrgi::GameControllerSwitchPosition dewiDeviceWinRTController::GetReadingSwitch( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pReadingSwitch.size() )

	return pReadingSwitch[ index ];
}

bool dewiDeviceWinRTController::GetReadingButton( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pReadingButtonSize )

	return pReadingButton[ index ];
}



// Private Functions
//////////////////////
