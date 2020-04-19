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
#include <stdlib.h>

#include <device/Joystick.h>

#include "debiDevice.h"
#include "debiDeviceMouse.h"
#include "debiDeviceKeyboard.h"
#include "debiDeviceAxis.h"
#include "debiDeviceButton.h"
#include "debiDeviceJoystick.h"
#include "debiDeviceManager.h"
#include "deBeOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class debiDeviceManager
////////////////////////////

// Constructor, destructor
////////////////////////////

debiDeviceManager::debiDeviceManager( deBeOSInput &module ) :
pModule( module ),
pMouse( NULL ),
pKeyboard( NULL )
{
	try{
		pCreateDevices();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

debiDeviceManager::~debiDeviceManager(){
	pCleanUp();
}



// Management
///////////////

void debiDeviceManager::UpdateDeviceList(){
}



int debiDeviceManager::GetCount() const{
	return pDevices.GetCount();
}

debiDevice *debiDeviceManager::GetAt( int index ) const{
	return ( debiDevice* )pDevices.GetAt( index );
}

debiDevice *debiDeviceManager::GetWithID( const char *id ){
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		debiDevice * const device = ( debiDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return device;
		}
	}
	
	return NULL;
}

int debiDeviceManager::IndexOfWithID( const char *id ){
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		debiDevice * const device = ( debiDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



void debiDeviceManager::LogDevices(){
	const int count = pDevices.GetCount();
	int i, j;
	
	pModule.LogInfo( "Input Devices:" );
	
	for( i=0; i<count; i++ ){
		const debiDevice &device = *( ( debiDevice* )pDevices.GetAt( i ) );
		pModule.LogInfoFormat( "- %s (%s) [%d]", device.GetName().GetString(),
			device.GetID().GetString(), device.GetType() );
		
		const int axisCount = device.GetAxisCount();
		if( axisCount > 0 ){
			pModule.LogInfo( "  Axes:" );
			for( j=0; j<axisCount; j++ ){
				const debiDeviceAxis &axis = device.GetAxisAt( j );
				pModule.LogInfoFormat( "    - '%s' (%s) %d .. %d [%d %d]",
					axis.GetName().GetString(), axis.GetID().GetString(), axis.GetMinimum(),
					axis.GetMaximum(), 0, 0 );
			}
		}
		
		const int buttonCount = device.GetButtonCount();
		if( buttonCount > 0 ){
			pModule.LogInfo( "  Buttons:" );
			for( j=0; j<buttonCount; j++ ){
				const debiDeviceButton &button = device.GetButtonAt( j );
				pModule.LogInfoFormat( "    - '%s' (%s) %d => %d",
					button.GetName().GetString(), button.GetID().GetString(),
					button.GetBICode(), j );
			}
		}
	}
}



decString debiDeviceManager::NormalizeID( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int len = strlen( id );
	if( len == 0 ){
		return decString();
	}
	
	decString nid;
	nid.Set( ' ', len );
	
	int i;
	for( i=0; i<len; i++ ){
		if( ( id[ i ]  >= 'A' && id[ i ] <= 'Z' )
		|| ( id[ i ] >= 'a' && id[ i ] <= 'z' )
		|| ( id[ i ] >= '0' && id[ i ] <= '9' )
		|| id[ i ] == '_' ){
			nid[ i ] = id[ i ];
			
		}else{
			nid[ i ] = '_';
		}
	}
	
	return nid;
}



// Private functions
//////////////////////

void debiDeviceManager::pCleanUp(){
	pDevices.RemoveAll();
	if( pKeyboard ){
		pKeyboard->FreeReference();
	}
	if( pMouse ){
		pMouse->FreeReference();
	}
}



void debiDeviceManager::pCreateDevices(){
	pMouse = new debiDeviceMouse( pModule );
	pMouse->SetIndex( pDevices.GetCount() );
	pDevices.Add( pMouse );
	
	pKeyboard = new debiDeviceKeyboard( pModule );
	pKeyboard->SetIndex( pDevices.GetCount() );
	pDevices.Add( pKeyboard );
	
	pCreateJoystickDevices();
}

void debiDeviceManager::pCreateJoystickDevices(){
	BJoystick joysticks;
	
	const int count = joysticks.CountDevices();
	int i;
	
	char name[ B_OS_NAME_LENGTH ];
	for( i=0; i<count; i++ ){
		if( joysticks.GetDeviceName( i, name, sizeof( name ) ) != B_OK ){
			DETHROW( deeInvalidParam );
		}
		
		debiDeviceJoystick *joystick = NULL;
		try{
			joystick = new debiDeviceJoystick( pModule, name );
			joystick->SetIndex( pDevices.GetCount() );
			pDevices.Add( joystick );
			joystick->FreeReference();
			
		}catch( const deException & ){
			if( joystick ){
				joystick->FreeReference();
			}
			throw;
		}
	}
}
