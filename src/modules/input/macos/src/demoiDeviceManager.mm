/* 
 * Drag[en]gine MacOS Input Module
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

#include "demoiDevice.h"
#include "demoiDeviceMouse.h"
#include "demoiDeviceKeyboard.h"
#include "demoiDeviceManager.h"
#include "demoiDeviceAxis.h"
#include "demoiDeviceButton.h"
#include "deMacOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>

#import <Cocoa/Cocoa.h>


// Class demoiDeviceManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

demoiDeviceManager::demoiDeviceManager( deMacOSInput &module ) :
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

demoiDeviceManager::~demoiDeviceManager(){
	pCleanUp();
}



// Management
///////////////

void demoiDeviceManager::UpdateDeviceList(){
}



int demoiDeviceManager::GetCount() const{
	return pDevices.GetCount();
}

demoiDevice *demoiDeviceManager::GetAt( int index ) const{
	return ( demoiDevice* )pDevices.GetAt( index );
}

demoiDevice *demoiDeviceManager::GetWithID( const char *id ){
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		demoiDevice * const device = ( demoiDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return device;
		}
	}
	
	return NULL;
}

int demoiDeviceManager::IndexOfWithID( const char *id ){
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		demoiDevice * const device = ( demoiDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



void demoiDeviceManager::LogDevices(){
    const int count = pDevices.GetCount();
    int i, j;
    
    pModule.LogInfo( "Input Devices:" );
    
    for( i=0; i<count; i++ ){
        const demoiDevice &device = *( ( demoiDevice* )pDevices.GetAt( i ) );
        pModule.LogInfoFormat( "- '%s' (%s) [%d]", device.GetName().GetString(),
            device.GetID().GetString(), device.GetType() );
        
        const int axisCount = device.GetAxisCount();
        if( axisCount > 0 ){
            pModule.LogInfo( "  Axes:" );
            for( j=0; j<axisCount; j++ ){
                const demoiDeviceAxis &axis = device.GetAxisAt( j );
                pModule.LogInfoFormat( "    - '%s' (%s) %d .. %d [%d %d]",
                    axis.GetName().GetString(), axis.GetID().GetString(), axis.GetMinimum(),
                    axis.GetMaximum(), 0, 0 );
            }
        }
        
        const int buttonCount = device.GetButtonCount();
        if( buttonCount > 0 ){
            pModule.LogInfo( "  Buttons:" );
            for( j=0; j<buttonCount; j++ ){
                const demoiDeviceButton &button = device.GetButtonAt( j );
                pModule.LogInfoFormat( "    - '%s' (%s) %d => %d",
                    button.GetName().GetString(), button.GetID().GetString(),
                    button.GetMOCode(), j );
            }
        }
    }
}



// Private functions
//////////////////////

void demoiDeviceManager::pCleanUp(){
	pDevices.RemoveAll();
	if( pKeyboard ){
		pKeyboard->FreeReference();
	}
	if( pMouse ){
		pMouse->FreeReference();
	}
}



void demoiDeviceManager::pCreateDevices(){
	pMouse = new demoiDeviceMouse( pModule );
	pMouse->SetIndex( pDevices.GetCount() );
    pDevices.Add( pMouse );
    
    pKeyboard = new demoiDeviceKeyboard( pModule );
    pKeyboard->SetIndex( pDevices.GetCount() );
	pDevices.Add( pKeyboard );
}
