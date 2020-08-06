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
#include <String.h>

#include "debiDeviceJoystick.h"
#include "debiDeviceAxis.h"
#include "debiDeviceButton.h"
#include "debiDeviceManager.h"
#include "deBeOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Definitions
////////////////

enum eHatStates{
	ehsCenter,
	ehsUp,
	ehsUpRight,
	ehsRight,
	ehsDownRight,
	ehsDown,
	ehsDownLeft,
	ehsLeft,
	ehsUpLeft
};



// Class debiDeviceJoystick
/////////////////////////////

// Constructor, destructor
////////////////////////////

debiDeviceJoystick::debiDeviceJoystick( deBeOSInput &module, const char *name ) :
debiDevice( module ),
pDevName( name ),
pJoystick( NULL ),
pStateAxisCount( 0 ),
pStateAxis( NULL ),
pStateHatCount( 0 ),
pStateHat( NULL )
{
	deObjectReference refObject;
	decString string;
	BString bname;
	
	const decString normName( debiDeviceManager::NormalizeID( name ) );
	string.Format( "%s%d%s", BEINP_DEVID_PREFIX, debiDevice::esBJoystick, normName.GetString() );
 	SetID( string );
	SetType( deInputDevice::edtGamepad );
	
	try{
		pJoystick = new BJoystick();
		if( pJoystick->Open( name ) == B_ERROR ){
			DETHROW( deeInvalidParam );
		}
		
		if( pJoystick->GetControllerName( &bname ) != B_OK ){
			DETHROW( deeInvalidParam );
		}
		if( bname.Length() == 0 ){
			module.LogWarnFormat( "Bug! GetControllerName() returned empty string for device '%s'", name );
			bname << "BJoy@" << name;
		}
		SetName( bname.String() );
		
		// add axes
		const int countAxes = pJoystick->CountAxes();
		const int countHats = pJoystick->CountHats();
		int indexAxis = 0;
		
		int i;
		for( i=0; i<countAxes; i++ ){
			refObject.TakeOver( new debiDeviceAxis( module ) );
			AddAxis( ( debiDeviceAxis* )( deObject* )refObject );
			debiDeviceAxis &axis = ( debiDeviceAxis& )( deObject& )refObject;
			
			axis.SetIndex( indexAxis );
			axis.SetAbsolute( true );
			string.Format( "axis%d", i );
			axis.SetID( string );
			if( pJoystick->GetAxisNameAt( i, &bname ) != B_OK ){
				DETHROW( deeInvalidParam );
			}
			axis.SetName( bname.String() );
			axis.SetType( deInputDeviceAxis::eatStick );
			axis.SetBICode( i );
			
			string.Format( "%d", indexAxis + 1 );
			axis.SetDisplayText( string );
			
			if( i == 0 ){
				axis.SetDisplayImages( "touchpadX" );
				
			}else if( i == 1 ){
				axis.SetDisplayImages( "touchpadY" );
				
			}else{
				// "mouseZ"
			}
			
			indexAxis++;
		}
		if( countAxes > 0 ){
			pStateAxis = new int16[ countAxes ];
			pStateAxisCount = countAxes;
		}
		
		for( i=0; i<countHats; i++ ){
			refObject.TakeOver( new debiDeviceAxis( module ) );
			AddAxis( ( debiDeviceAxis* )( deObject* )refObject );
			debiDeviceAxis &hatX = ( debiDeviceAxis& )( deObject& )refObject;
			
			refObject.TakeOver( new debiDeviceAxis( module ) );
			AddAxis( ( debiDeviceAxis* )( deObject* )refObject );
			debiDeviceAxis &hatY = ( debiDeviceAxis& )( deObject& )refObject;
			
			if( pJoystick->GetHatNameAt( i, &bname ) != B_OK ){
				DETHROW( deeInvalidParam );
			}
			
			hatX.SetIndex( indexAxis );
			hatX.SetAbsolute( true );
			string.Format( "hat%dX", i );
			hatX.SetID( string );
			string.Format( "%s X", bname.String() );
			hatX.SetName( string );
			hatX.SetType( deInputDeviceAxis::eatHat );
			hatX.SetBICode( i * 2 );
			
			string.Format( "%d", indexAxis + 1 );
			hatX.SetDisplayText( string );
			hatX.SetDisplayImages( "stickX" );
			indexAxis++;
			
			hatY.SetIndex( countAxes + i * 2 + 1 );
			hatY.SetAbsolute( true );
			string.Format( "hat%dY", i );
			hatY.SetID( string );
			string.Format( "%s Y", bname.String() );
			hatY.SetName( string );
			hatY.SetType( deInputDeviceAxis::eatHat );
			hatY.SetBICode( i * 2 + 1 );
			
			string.Format( "%d", indexAxis + 1 );
			hatY.SetDisplayText( string );
			hatY.SetDisplayImages( "stickX" );
			indexAxis++;
		}
		if( countHats > 0 ){
			pStateHat = new uint8[ countHats ];
			pStateHatCount = countHats;
		}
		
		// add buttons
		const int countButtons = pJoystick->CountButtons();
		
		for( i=0; i<countButtons; i++ ){
			refObject.TakeOver( new debiDeviceButton( module ) );
			AddButton( ( debiDeviceButton* )( deObject* )refObject );
			debiDeviceButton &button = ( debiDeviceButton& )( deObject& )refObject;
			
			string.Format( "button%d", i );
			button.SetID( string );
			if( pJoystick->GetButtonNameAt( i, &bname ) != B_OK ){
				DETHROW( deeInvalidParam );
			}
			button.SetName( bname.String() );
			button.SetBICode( i );
			
			string.Format( "%d", i + 1 );
			button.SetDisplayText( string );
			button.SetDisplayImages( "button" );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
    }
}

debiDeviceJoystick::~debiDeviceJoystick(){
	pCleanUp();
}



// Management
///////////////

void debiDeviceJoystick::Update(){
	deBeOSInput &module = GetModule();
	const int deviceIndex = GetIndex();
	
	timeval eventTime;
	gettimeofday( &eventTime, NULL );
	
	// query all values
	if( pJoystick->Update() != B_OK ){
		DETHROW( deeInvalidParam );
	}
	
	const uint32 buttonValues = pJoystick->ButtonValues();
	if( pStateAxisCount > 0 ){
		if( pJoystick->GetAxisValues( pStateAxis ) != B_OK ){
			DETHROW( deeInvalidParam );
		}
	}
	if( pStateHatCount > 0 ){
		if( pJoystick->GetHatValues( pStateHat ) != B_OK ){
			DETHROW( deeInvalidParam );
		}
	}
	
	// update axes
	const int countAxis = GetAxisCount();
	int i;
	
	for( i=0; i<countAxis; i++ ){
		debiDeviceAxis &axis = *GetAxisAt( i );
		float value = axis.GetValue();
		
		switch( axis.GetType() ){
		case deInputDeviceAxis::eatStick:
			value = ( float )pStateAxis[ axis.GetBICode() ] / 32767.0f;
			break;
			
		case deInputDeviceAxis::eatHat:
			if( ( axis.GetBICode() % 2 ) == 0 ){ // X axis
				switch( pStateHat[ axis.GetBICode() / 2 ] ){
				case ehsCenter:
				case ehsUp:
				case ehsDown:
					value = 0.0f;
					break;
					
				case ehsUpLeft:
				case ehsLeft:
				case ehsDownLeft:
					value = -1.0f;
					break;
					
				case ehsUpRight:
				case ehsRight:
				case ehsDownRight:
					value = 1.0f;
					break;
					
				default:
					break;
				}
				
			}else{ // Y axis
				switch( pStateHat[ axis.GetBICode() / 2 ] ){
				case ehsCenter:
				case ehsLeft:
				case ehsRight:
					value = 0.0f;
					break;
					
				case ehsDownLeft:
				case ehsDown:
				case ehsDownRight:
					value = -1.0f;
					break;
					
				case ehsUpLeft:
				case ehsUp:
				case ehsUpRight:
					value = 1.0f;
					break;
					
				default:
					break;
				}
			}
			break;
			
		default:
			break;
		}
		
		axis.ProcessNewValue( *this, value, eventTime );
	}
	
	// update buttons
	const int countButtons = GetButtonCount();
	
	for( i=0; i<countButtons; i++ ){
		debiDeviceButton &button = *GetButtonAt( i );
		
		if( ( buttonValues & ( 1 << button.GetBICode() ) ) == 0 ){
			if( ! button.GetPressed() ){
				continue;
			}
			
			button.SetPressed( false );
			module.AddButtonReleased( deviceIndex, i, eventTime );
			
		}else{
			if( button.GetPressed() ){
				continue;
			}
			
			button.SetPressed( true );
			module.AddButtonPressed( deviceIndex, i, eventTime );
		}
	}
	
	// send dirty axis events
	SendDirtyAxisEvents();
}



// Private Functions
//////////////////////

void debiDeviceJoystick::pCleanUp(){
	if( pStateAxis ){
		delete [] pStateAxis;
	}
	if( pStateHat ){
		delete pStateHat;
	}
	
	if( pJoystick ){
		pJoystick->Close();
		delete pJoystick;
	}
}
