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

#include "dexsiDeviceLibEvent.h"
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



// Class dexsiDeviceLibEvent
//////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiDeviceLibEvent::dexsiDeviceLibEvent( deXSystemInput &module, const char *pathDevice ) :
dexsiDevice( module, esLibevdev ),

pEvdevPath( pathDevice ),
pEvdevFile( -1 ),
pEvdevDevice( NULL ),
pEvdevMapRelAxis( NULL ),
pEvdevMapAbsAxis( NULL ),
pEvdevMapKeys( NULL )
{
	// libevdev version
	// https://www.freedesktop.org/software/libevdev/doc/latest/index.html
	// SDL type mapping: https://meghprkh.github.io/blog/2016/06/03/Handling-joysticks-and-gamepads-in-linux/
	
	decString string;
	int i;
	
	// create libevdev device for device at pathDevice
	pEvdevFile = open( pathDevice, O_RDONLY | O_NONBLOCK );
	if( pEvdevFile == -1 ){
		DETHROW_INFO( deeOpenFile, pathDevice );
	}
	
	if( libevdev_new_from_fd( pEvdevFile, &pEvdevDevice ) < 0 ){
		DETHROW_INFO( deeOpenFile, pathDevice );
	}
	
	// retrieve device parameters
	SetName( libevdev_get_name( pEvdevDevice ) );
	
	const char * const unique = libevdev_get_uniq( pEvdevDevice );
	if( unique ){
		string.Format( "%s%d%s", XINP_DEVID_PREFIX, esLibevdev,
			dexsiDeviceManager::NormalizeID( unique ).GetString() );
		
	}else{
		const int bustype = libevdev_get_id_bustype( pEvdevDevice );
		const int vendor = libevdev_get_id_vendor( pEvdevDevice );
		const int product = libevdev_get_id_product( pEvdevDevice );
		const int version = libevdev_get_id_version( pEvdevDevice );
		
		string.Format( "%s%d%04x%04x%04x%04x", XINP_DEVID_PREFIX, esLibevdev,
			bustype, vendor, product, version );
	}
	
	SetID( string );
	
	// try to identify what kind of device this is
	bool hasRelativeAxes = libevdev_has_event_type( pEvdevDevice, EV_REL );
	bool hasAbsoluteAxes = libevdev_has_event_type( pEvdevDevice, EV_ABS );
	bool hasKeys = libevdev_has_event_type( pEvdevDevice, EV_KEY );
// 		bool hasLeftMouse = libevdev_has_event_code( pEvdevDevice, EV_KEY, BTN_LEFT );
	
	/*
	if( hasRelativeAxes && hasLeftMouse ){
		pType = deInputDevice::edtMouse;
		
	}else if( hasAbsoluteAxes ){
		pType = deInputDevice::edtGamepad;
		
	}else{
		pType = deInputDevice::edtKeyboard;
	}*/
	SetType( deInputDevice::edtGamepad );
	SetDisplayImages( "gamepad" );
	
	// relative and absolute axes
	int countAxes = 0;
	
	if( hasRelativeAxes ){
		for( i=0; i<REL_MAX; i++ ){
			if( libevdev_has_event_code( pEvdevDevice, EV_REL, i ) ){
				countAxes++;
			}
		}
	}
	
	if( hasAbsoluteAxes ){
		for( i=0; i<ABS_MAX; i++ ){
			if( libevdev_has_event_code( pEvdevDevice, EV_ABS, i ) ){
				countAxes++;
			}
		}
	}
	
	if( hasRelativeAxes ){
		pEvdevMapRelAxis = new short[ REL_MAX ];
		for( i=0; i<REL_MAX; i++ ){
			pEvdevMapRelAxis[ i ] = -1;
		}
	}
	if( hasAbsoluteAxes ){
		pEvdevMapAbsAxis = new short[ ABS_MAX ];
		for( i=0; i<ABS_MAX; i++ ){
			pEvdevMapAbsAxis[ i ] = -1;
		}
	}
	
	deObjectReference refObject;
	int indexAxis = 0;
	
	if( hasRelativeAxes ){
		for( i=0; i<REL_MAX; i++ ){
			if( ! libevdev_has_event_code( pEvdevDevice, EV_REL, i ) ){
				continue;
			}
			
			refObject.TakeOver( new dexsiDeviceAxis( module ) );
			AddAxis( ( dexsiDeviceAxis* )( deObject* )refObject );
			dexsiDeviceAxis &axis = ( dexsiDeviceAxis& )( deObject& )refObject;
			axis.SetIndex( indexAxis );
			axis.SetName( libevdev_event_code_get_name( EV_REL, i ) );
			axis.SetType( deInputDeviceAxis::eatTouchPad );
			string.Format( "ra%d", i );
			axis.SetID( string );
			axis.SetEvdevCode( i );
			axis.SetAbsolute( false );
			
			string.Format( "%d", indexAxis + 1 );
			axis.SetDisplayText( string );
			
			if( i == 0 ){
				axis.SetDisplayImages( "touchpadX" );
				
			}else if( i == 1 ){
				axis.SetDisplayImages( "touchpadY" );
				
			}else{
				 // "mouseZ"
			}
			
			pEvdevMapRelAxis[ i ] = indexAxis++;
		}
	}
	
	if( hasAbsoluteAxes ){
		int nextGeneric = 0;
		
		for( i=0; i<ABS_MAX; i++ ){
			if( ! libevdev_has_event_code( pEvdevDevice, EV_ABS, i ) ){
				continue;
			}
			
			refObject.TakeOver( new dexsiDeviceAxis( module ) );
			AddAxis( ( dexsiDeviceAxis* )( deObject* )refObject );
			dexsiDeviceAxis &axis = ( dexsiDeviceAxis& )( deObject& )refObject;
			axis.SetIndex( indexAxis );
			axis.SetName( libevdev_event_code_get_name( EV_ABS, i ) );
			axis.SetEvdevCode( i );
			axis.SetAbsolute( true );
			
			axis.SetMinimum( libevdev_get_abs_minimum( pEvdevDevice, i ) );
			axis.SetMaximum( libevdev_get_abs_maximum( pEvdevDevice, i ) );
			axis.SetFuzz( libevdev_get_abs_fuzz( pEvdevDevice, i ) );
			axis.SetFlat( libevdev_get_abs_flat( pEvdevDevice, i ) );
			
			if( axis.GetName().EqualsInsensitive( "abs_x" ) ){
				axis.SetType( deInputDeviceAxis::eatStick );
				axis.SetDisplayImages( "stickX" );
				axis.SetID( "sx0" );
				axis.SetDisplayText( "1" );
				
			}else if( axis.GetName().EqualsInsensitive( "abs_y" ) ){
				axis.SetType( deInputDeviceAxis::eatStick );
				axis.SetDisplayImages( "stickY" );
				axis.SetID( "sy0" );
				axis.SetDisplayText( "1" );
				
			}else if( axis.GetName().EqualsInsensitive( "abs_z" ) ){
				axis.SetType( deInputDeviceAxis::eatTrigger );
				axis.SetDisplayImages( "trigger" );
				axis.SetID( "tr0" );
				axis.SetDisplayText( "1" );
				
			}else if( axis.GetName().EqualsInsensitive( "abs_rx" ) ){
				axis.SetType( deInputDeviceAxis::eatStick );
				axis.SetDisplayImages( "stickX" );
				axis.SetID( "sx1" );
				axis.SetDisplayText( "2" );
				
			}else if( axis.GetName().EqualsInsensitive( "abs_ry" ) ){
				axis.SetType( deInputDeviceAxis::eatStick );
				axis.SetDisplayImages( "stickY" );
				axis.SetID( "sy1" );
				axis.SetDisplayText( "2" );
				
			}else if( axis.GetName().EqualsInsensitive( "abs_rz" ) ){
				axis.SetType( deInputDeviceAxis::eatTrigger );
				axis.SetDisplayImages( "trigger" );
				axis.SetID( "tr1" );
				axis.SetDisplayText( "2" );
				
			}else if( axis.GetName().EqualsInsensitive( "abs_hat0x" ) ){
				axis.SetType( deInputDeviceAxis::eatHat );
				axis.SetDisplayImages( "stickX" );
				axis.SetID( "hx0" );
				axis.SetDisplayText( "1" );
				
			}else if( axis.GetName().EqualsInsensitive( "abs_hat0y" ) ){
				axis.SetType( deInputDeviceAxis::eatHat );
				axis.SetDisplayImages( "stickY" );
				axis.SetID( "hy0" );
				axis.SetDisplayText( "1" );
				
			}else if( axis.GetName().EqualsInsensitive( "abs_hat1x" ) ){
				axis.SetType( deInputDeviceAxis::eatHat );
				axis.SetDisplayImages( "stickX" );
				axis.SetID( "hx1" );
				axis.SetDisplayText( "2" );
				
			}else if( axis.GetName().EqualsInsensitive( "abs_hat1y" ) ){
				axis.SetType( deInputDeviceAxis::eatHat );
				axis.SetDisplayImages( "stickY" );
				axis.SetID( "hy1" );
				axis.SetDisplayText( "2" );
				
			}else{
				axis.SetType( deInputDeviceAxis::eatGeneric );
				axis.SetDisplayImages( "stick" );
				
				string.Format( "aa%d", nextGeneric++ );
				axis.SetID( string );
				
				string.Format( "%d", nextGeneric );
				axis.SetDisplayText( string );
			}
			
			pEvdevMapAbsAxis[ i ] = indexAxis++;
		}
	}
	
	// keys and buttons
	if( hasKeys ){
		int countButtons = 0;
		
		for( i=BTN_MISC; i<KEY_MAX; i++ ){
			if( libevdev_has_event_code( pEvdevDevice, EV_KEY, i ) ){
				countButtons++;
			}
		}
		
		pEvdevMapKeys = new short[ KEY_MAX - BTN_MISC ];
		for( i=BTN_MISC; i<KEY_MAX; i++ ){
			pEvdevMapKeys[ i - BTN_MISC ] = -1;
		}
		
		int indexButton = 0;
		
		int nextGeneric = 0;
		int nextTrigger = 0;
		
		for( i=BTN_MISC; i<KEY_MAX; i++ ){
			if( ! libevdev_has_event_code( pEvdevDevice, EV_KEY, i ) ){
				continue;
			}
			
			refObject.TakeOver( new dexsiDeviceButton( module ) );
			AddButton( ( dexsiDeviceButton* )( deObject* )refObject );
			dexsiDeviceButton &button = ( dexsiDeviceButton& )( deObject& )refObject;
			button.SetName( libevdev_event_code_get_name( EV_KEY, i ) );
			button.SetEvdevCode( i );
			
			// other seen names (but not assigned especially):
			// BTN_MODE
			// BTN_TRIGGER_HAPPY*
			
			if( button.GetName().EqualsInsensitive( "btn_south" ) ){
				button.SetType( deInputDeviceButton::ebtAction );
				button.SetDisplayImages( "button" );
				button.SetID( "baa" );
				button.SetDisplayText( "A" );
				
			}else if( button.GetName().EqualsInsensitive( "btn_east" ) ){
				button.SetType( deInputDeviceButton::ebtAction );
				button.SetDisplayImages( "button" );
				button.SetID( "bab" );
				button.SetDisplayText( "B" );
				
			}else if( button.GetName().EqualsInsensitive( "btn_west" ) ){
				button.SetType( deInputDeviceButton::ebtAction );
				button.SetDisplayImages( "button" );
				button.SetID( "bax" );
				button.SetDisplayText( "X" );
				
			}else if( button.GetName().EqualsInsensitive( "btn_north" ) ){
				button.SetType( deInputDeviceButton::ebtAction );
				button.SetDisplayImages( "button" );
				button.SetID( "bay" );
				button.SetDisplayText( "Y" );
				
			}else if( button.GetName().EqualsInsensitive( "btn_tl" ) ){
				button.SetType( deInputDeviceButton::ebtShoulderLeft );
				button.SetDisplayImages( "button" );
				button.SetID( "basl" );
				button.SetDisplayText( "SL" );
				
			}else if( button.GetName().EqualsInsensitive( "btn_tr" ) ){
				button.SetType( deInputDeviceButton::ebtShoulderRight );
				button.SetDisplayImages( "button" );
				button.SetID( "basr" );
				button.SetDisplayText( "SR" );
				
			}else if( button.GetName().EqualsInsensitive( "btn_select" ) ){
				button.SetType( deInputDeviceButton::ebtSelect );
				button.SetDisplayImages( "button" );
				button.SetID( "basel" );
				button.SetDisplayText( "Select" );
				
			}else if( button.GetName().EqualsInsensitive( "btn_start" ) ){
				button.SetType( deInputDeviceButton::ebtHome );
				button.SetDisplayImages( "button" );
				button.SetID( "basta" );
				button.SetDisplayText( "Start" );
				
			}else if( button.GetName().EqualsInsensitive( "btn_thumbl" ) ){
				button.SetType( deInputDeviceButton::ebtStick );
				button.SetDisplayImages( "button" );
				button.SetID( "bas0" );
				button.SetDisplayText( "StkL" );
				
			}else if( button.GetName().EqualsInsensitive( "btn_thumbr" ) ){
				button.SetType( deInputDeviceButton::ebtStick );
				button.SetDisplayImages( "button" );
				button.SetID( "bas1" );
				button.SetDisplayText( "StkR" );
				
			}else if( button.GetName().BeginsWithInsensitive( "btn_trigger_" ) ){
				button.SetType( deInputDeviceButton::ebtTrigger );
				button.SetDisplayImages( "button" );
				string.Format( "bt%d", nextTrigger++ );
				button.SetID( string );
				string.Format( "T%d", nextTrigger );
				button.SetDisplayText( string );
				
			}else{
				button.SetType( deInputDeviceButton::ebtGeneric );
				button.SetDisplayImages( "button" );
				string.Format( "bg%d", nextGeneric++ );
				button.SetID( string );
				string.Format( "%d", nextGeneric );
				button.SetDisplayText( string );
			}
			
			pEvdevMapKeys[ i - BTN_MISC ] = indexButton++;
		}
	}
	
	/*
	if( libevdev_has_event_type( evdev, EV_SW ) ){
		pModule.LogInfo( "  switches:" );
		for( j=0; j<SW_MAX; j++ ){
			if( libevdev_has_event_code( evdev, EV_SW, j ) ){
				pModule.LogInfoFormat( "  - 0x%x", j );
			}
		}
	}
	
	if( libevdev_has_event_type( evdev, EV_LED ) ){
		pModule.LogInfo( "  led:" );
		for( j=0; j<LED_MAX; j++ ){
			if( libevdev_has_event_code( evdev, EV_LED, j ) ){
				pModule.LogInfoFormat( "  - 0x%x", j );
			}
		}
	}
	*/
	
	/*
	const int slotCount = libevdev_get_num_slots( evdev );
	pModule.LogInfoFormat( "  slotcount=%d", slotCount );
	//INPUT_PROP_BUTTONPAD;
	for( j=0; j<slotCount; j++ ){
	}
	*/
}

dexsiDeviceLibEvent::~dexsiDeviceLibEvent(){
	if( pEvdevMapKeys ){
		delete [] pEvdevMapKeys;
	}
	if( pEvdevMapAbsAxis ){
		delete [] pEvdevMapAbsAxis;
	}
	if( pEvdevMapRelAxis ){
		delete [] pEvdevMapRelAxis;
	}
	if( pEvdevDevice ){
		libevdev_free( pEvdevDevice );
	}
	if( pEvdevFile != -1 ){
		close( pEvdevFile );
	}
}



// Management
///////////////

void dexsiDeviceLibEvent::Update(){
	deXSystemInput &module = GetModule();
	const int deviceIndex = GetIndex();
	int flag = LIBEVDEV_READ_FLAG_NORMAL;
	input_event ev;
	
	while( true ){
		const int result = libevdev_next_event( pEvdevDevice, flag, &ev );
		
		if( result == -EAGAIN ){
			break; // no more events
			
		}else if( result == LIBEVDEV_READ_STATUS_SYNC ){
			flag = LIBEVDEV_READ_FLAG_SYNC;
			continue;
			
		}else{
			switch( ev.type ){
			case EV_REL:
				if( pEvdevMapRelAxis && ev.code >= 0 && ev.code < REL_MAX
				&& pEvdevMapRelAxis[ ev.code ] != -1 ){
					GetAxisAt( pEvdevMapRelAxis[ ev.code ] )->EvdevProcessEvent( *this, ev );
				}
				break;
				
			case EV_ABS:
				if( pEvdevMapAbsAxis && ev.code >= 0 && ev.code < ABS_MAX
				&& pEvdevMapAbsAxis[ ev.code ] != -1 ){
					GetAxisAt( pEvdevMapAbsAxis[ ev.code ] )->EvdevProcessEvent( *this, ev );
				}
				break;
				
			case EV_KEY:
				if( pEvdevMapKeys && ev.code >= BTN_MISC && ev.code < KEY_MAX
				&& pEvdevMapKeys[ ev.code - BTN_MISC ] != -1 ){
					const int code = pEvdevMapKeys[ ev.code - BTN_MISC ];
					
					switch( ev.value ){
					case 0: // release
						GetButtonAt( code )->SetPressed( false );
						module.AddButtonReleased( deviceIndex, code, ev.time );
						break;
						
					case 1: // press
						GetButtonAt( code )->SetPressed( true );
						module.AddButtonPressed( deviceIndex, code, ev.time );
						break;
						
					case 2: // auto-repeat (or auto-fire)
						GetButtonAt( code )->SetPressed( true );
						module.AddButtonReleased( deviceIndex, code, ev.time );
						module.AddButtonPressed( deviceIndex, code, ev.time );
						break;
					}
				}
				break;
				
			default:
				break;
			}
		}
	}
	
	SendDirtyAxisEvents();
}


// Private Functions
//////////////////////

