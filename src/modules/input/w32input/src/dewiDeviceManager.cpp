/* 
 * Drag[en]gine Windows Input Module
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

#include "dewiDevice.h"
#include "dewiDeviceAxis.h"
#include "dewiDeviceButton.h"
#include "dewiDeviceFeedback.h"
#include "dewiDeviceManager.h"
#include "dewiDeviceMouse.h"
#include "dewiDeviceKeyboard.h"
#include "deWindowsInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSWindows.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class dewiDeviceManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

dewiDeviceManager::dewiDeviceManager( deWindowsInput &module ) :
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

dewiDeviceManager::~dewiDeviceManager(){
	pCleanUp();
}



// Management
///////////////

void dewiDeviceManager::UpdateDeviceList(){
}



int dewiDeviceManager::GetCount() const{
	return pDevices.GetCount();
}

dewiDevice *dewiDeviceManager::GetAt( int index ) const{
	return ( dewiDevice* )pDevices.GetAt( index );
}

dewiDevice *dewiDeviceManager::GetWithID( const char *id ){
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dewiDevice * const device = ( dewiDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return device;
		}
	}
	
	return NULL;
}

int dewiDeviceManager::IndexOfWithID( const char *id ){
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dewiDevice * const device = ( dewiDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



void dewiDeviceManager::LogDevices(){
	const int count = pDevices.GetCount();
	int i, j;
	
	pModule.LogInfo( "Input Devices:" );
	
	for( i=0; i<count; i++ ){
		const dewiDevice &device = *( ( dewiDevice* )pDevices.GetAt( i ) );
		pModule.LogInfoFormat( "- '%s' (%s) [%d]", device.GetName().GetString(),
			device.GetID().GetString(), device.GetType() );
		
		const int axisCount = device.GetAxisCount();
		if( axisCount > 0 ){
			pModule.LogInfo( "  Axes:" );
			for( j=0; j<axisCount; j++ ){
				const dewiDeviceAxis &axis = *device.GetAxisAt( j );
				pModule.LogInfoFormat( "    - '%s' (%s) %d .. %d [%d %d]",
					axis.GetName().GetString(), axis.GetID().GetString(), axis.GetMinimum(),
					axis.GetMaximum(), axis.GetFuzz(), axis.GetFlat() );
			}
		}
		
		const int buttonCount = device.GetButtonCount();
		if( buttonCount > 0 ){
			pModule.LogInfo( "  Buttons:" );
			for( j=0; j<buttonCount; j++ ){
				const dewiDeviceButton &button = *device.GetButtonAt( j );
				pModule.LogInfoFormat( "    - '%s' (%s) %d => %d",
					button.GetName().GetString(), button.GetID().GetString(),
					button.GetWICode(), j );
			}
		}
	}
}



decString dewiDeviceManager::NormalizeID( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int len = ( int )strlen( id );
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

void dewiDeviceManager::pCleanUp(){
	pDevices.RemoveAll();
	if( pKeyboard ){
		pKeyboard->FreeReference();
	}
	if( pMouse ){
		pMouse->FreeReference();
	}
}



void dewiDeviceManager::pCreateDevices(){
	pMouse = new dewiDeviceMouse( pModule );
	pMouse->SetIndex( pDevices.GetCount() );
	pDevices.Add( pMouse );
	
	pKeyboard = new dewiDeviceKeyboard( pModule );
	pKeyboard->SetIndex( pDevices.GetCount() );
	pDevices.Add( pKeyboard );
}
