/* 
 * Drag[en]gine OpenVR VR Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deovrDevice.h"
#include "deovrDeviceAxis.h"
#include "deovrDeviceButton.h"
#include "deovrDeviceFeedback.h"
#include "deovrDeviceManager.h"
#include "deVROpenVR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>



// Class deovrDeviceManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deovrDeviceManager::deovrDeviceManager( deVROpenVR &ovr ) :
pOvr( ovr ){
}

deovrDeviceManager::~deovrDeviceManager(){
}



// Management
///////////////

void deovrDeviceManager::Clear(){
	pDevices.RemoveAll();
}

void deovrDeviceManager::UpdateDeviceList(){
}



int deovrDeviceManager::GetCount() const{
	return pDevices.GetCount();
}

deovrDevice *deovrDeviceManager::GetAt( int index ) const{
	return ( deovrDevice* )pDevices.GetAt( index );
}

deovrDevice *deovrDeviceManager::GetWithID( const char *id ){
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deovrDevice * const device = ( deovrDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return device;
		}
	}
	
	return nullptr;
}

int deovrDeviceManager::IndexOfWithID( const char *id ){
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deovrDevice * const device = ( deovrDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



void deovrDeviceManager::LogDevices(){
	const int count = pDevices.GetCount();
	int i, j;
	
	pOvr.LogInfo( "Input Devices:" );
	
	for( i=0; i<count; i++ ){
		const deovrDevice &device = *( ( deovrDevice* )pDevices.GetAt( i ) );
		pOvr.LogInfoFormat( "- '%s' (%s) [%d]", device.GetName().GetString(),
			device.GetID().GetString(), device.GetType() );
		
		const int axisCount = device.GetAxisCount();
		if( axisCount > 0 ){
			pOvr.LogInfo( "  Axes:" );
			for( j=0; j<axisCount; j++ ){
				const deovrDeviceAxis &axis = *device.GetAxisAt( j );
				pOvr.LogInfoFormat( "    - '%s' (%s) %d .. %d", axis.GetName().GetString(),
					axis.GetID().GetString(), axis.GetMinimum(), axis.GetMaximum() );
			}
		}
		
		const int buttonCount = device.GetButtonCount();
		if( buttonCount > 0 ){
			pOvr.LogInfo( "  Buttons:" );
			for( j=0; j<buttonCount; j++ ){
				const deovrDeviceButton &button = *device.GetButtonAt( j );
				pOvr.LogInfoFormat( "    - '%s' (%s) => %d",
					button.GetName().GetString(), button.GetID().GetString(), j );
			}
		}
	}
}



decString deovrDeviceManager::NormalizeID( const char *id ){
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
