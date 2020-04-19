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

#include "deAndroidInput.h"
#include "deaiDevice.h"
#include "deaiDeviceAxis.h"
#include "deaiDeviceButton.h"
#include "deaiDeviceFeedback.h"
#include "deaiDeviceManager.h"
#include "deaiDeviceMouse.h"
#include "deaiDeviceKeyboard.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSAndroid.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class deaiDeviceManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deaiDeviceManager::deaiDeviceManager( deAndroidInput &module ) :
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

deaiDeviceManager::~deaiDeviceManager(){
	pCleanUp();
}



// Management
///////////////

void deaiDeviceManager::UpdateDeviceList(){
}



int deaiDeviceManager::GetCount() const{
	return pDevices.GetCount();
}

deaiDevice *deaiDeviceManager::GetAt( int index ) const{
	return ( deaiDevice* )pDevices.GetAt( index );
}

deaiDevice *deaiDeviceManager::GetWithID( const char *id ) const{
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deaiDevice * const device = ( deaiDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return device;
		}
	}
	
	return NULL;
}

int deaiDeviceManager::IndexOfWithID( const char *id ) const{
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deaiDevice * const device = ( deaiDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



void deaiDeviceManager::LogDevices(){
	const int count = pDevices.GetCount();
	int i, j;
	
	pModule.LogInfo( "Input Devices:" );
	
	for( i=0; i<count; i++ ){
		const deaiDevice &device = *( ( deaiDevice* )pDevices.GetAt( i ) );
		pModule.LogInfoFormat( "- '%s' (%s) [%d]", device.GetName().GetString(),
			device.GetID().GetString(), device.GetType() );
		
		const int axisCount = device.GetAxisCount();
		if( axisCount > 0 ){
			pModule.LogInfo( "  Axes:" );
			for( j=0; j<axisCount; j++ ){
				const deaiDeviceAxis &axis = device.GetAxisAt( j );
				pModule.LogInfoFormat( "    - '%s' (%s) %d .. %d [%d %d]",
					axis.GetName().GetString(), axis.GetID().GetString(), axis.GetMinimum(),
					axis.GetMaximum(), axis.GetFuzz(), axis.GetFlat() );
			}
		}
		
		const int buttonCount = device.GetButtonCount();
		if( buttonCount > 0 ){
			pModule.LogInfo( "  Buttons:" );
			for( j=0; j<buttonCount; j++ ){
				const deaiDeviceButton &button = device.GetButtonAt( j );
				pModule.LogInfoFormat( "    - '%s' (%s) %d => %d",
					button.GetName().GetString(), button.GetID().GetString(),
					button.GetAICode(), j );
			}
		}
	}
}



// Private functions
//////////////////////

void deaiDeviceManager::pCleanUp(){
	pDevices.RemoveAll();
	if( pKeyboard ){
		pKeyboard->FreeReference();
	}
	if( pMouse ){
		pMouse->FreeReference();
	}
}

void deaiDeviceManager::pCreateDevices(){
	pMouse = new deaiDeviceMouse( pModule );
	pMouse->SetIndex( pDevices.GetCount() );
	pDevices.Add( pMouse );
	
	pKeyboard = new deaiDeviceKeyboard( pModule );
	pKeyboard->SetIndex( pDevices.GetCount() );
	pDevices.Add( pKeyboard );
}
