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

#include <stdlib.h>

#include "deaiInputBinding.h"
#include "../deAndroidInput.h"
#include "../deaiDevice.h"
#include "../deaiDeviceAxis.h"
#include "../deaiDeviceButton.h"
#include "../deaiDeviceManager.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class deaiInputBinding
///////////////////////////

// Constructor, destructor
////////////////////////////

deaiInputBinding::deaiInputBinding() :
pInvertAxis( false ),
pDeviceIndex( -1 ),
pAxisIndex( -1 ),
pButtonIndex( -1 ){
}

deaiInputBinding::deaiInputBinding( const char *deviceID, const char *buttonID ) :
pDeviceID( deviceID ),
pButtonID( buttonID ),
pInvertAxis( false ),
pDeviceIndex( -1 ),
pAxisIndex( -1 ),
pButtonIndex( -1 ){
}

deaiInputBinding::deaiInputBinding( const char *deviceID, const char *axisID, bool invertAxis ) :
pDeviceID( deviceID ),
pAxisID( axisID ),
pInvertAxis( invertAxis ),
pDeviceIndex( -1 ),
pAxisIndex( -1 ),
pButtonIndex( -1 ){
}

deaiInputBinding::deaiInputBinding( const deaiInputBinding &binding ) :
pDeviceID( binding.pDeviceID ),
pAxisID( binding.pAxisID ),
pButtonID( binding.pButtonID ),
pInvertAxis( binding.pInvertAxis ),
pDeviceIndex( binding.pDeviceIndex ),
pAxisIndex( binding.pAxisIndex ),
pButtonIndex( binding.pButtonIndex ){
}

deaiInputBinding::~deaiInputBinding(){
}



// Management
///////////////

void deaiInputBinding::SetDeviceID( const char *id ){
	pDeviceID = id;
}

void deaiInputBinding::SetAxisID( const char *id ){
	pAxisID = id;
}

void deaiInputBinding::SetButtonID( const char *id ){
	pButtonID = id;
}

void deaiInputBinding::SetInvertAxis( bool invertAxis ){
	pInvertAxis = invertAxis;
}



void deaiInputBinding::UpdateIndices( const deaiDeviceManager &devices ){
	pDeviceIndex = devices.IndexOfWithID( pDeviceID );
	
	if( pDeviceIndex != -1 ){
		const deaiDevice &device = *devices.GetAt( pDeviceIndex );
		pAxisIndex = device.IndexOfAxisWithID( pAxisID );
		pButtonIndex = device.IndexOfButtonWithID( pButtonID );
		
	}else{
		pAxisIndex = -1;
		pButtonIndex = -1;
	}
}

void deaiInputBinding::ChangeButtonState( deAndroidInput &androidInput, bool pressed ) const{
	if( pButtonIndex == -1 ){
		return;
	}
	
	const deaiDevice &device = *androidInput.GetDevices().GetAt( pDeviceIndex );
	const timeval timestamp = androidInput.TimeValNow();
	
	deaiDeviceButton &button = device.GetButtonAt( pButtonIndex );
	button.SetPressed( pressed );
	
	switch( device.GetType() ){
	case deInputDevice::edtMouse:
		if( pressed ){
			androidInput.AddMousePress( pDeviceIndex, pButtonIndex, 0, timestamp );
			
		}else{
			androidInput.AddMouseRelease( pDeviceIndex, pButtonIndex, 0, timestamp );
		}
		break;
		
	case deInputDevice::edtKeyboard:
		if( pressed ){
			androidInput.AddKeyPress( pDeviceIndex, pButtonIndex,
				button.GetAIChar(), button.GetKeyCode(), timestamp );
			
		}else{
			androidInput.AddKeyRelease( pDeviceIndex, pButtonIndex,
				button.GetAIChar(), button.GetKeyCode(), timestamp );
		}
		break;
		
	default:
		if( pressed ){
			androidInput.AddButtonPressed( pDeviceIndex,
				pButtonIndex, timestamp );
			
		}else{
			androidInput.AddButtonReleased( pDeviceIndex,
				pButtonIndex, timestamp );
		}
	}
}



// Operators
//////////////

bool deaiInputBinding::operator==( const deaiInputBinding &binding ) const{
	return pDeviceID == binding.pDeviceID
		&& pAxisID == binding.pAxisID
		&& pButtonID == binding.pButtonID
		&& pInvertAxis == binding.pInvertAxis;
}

deaiInputBinding &deaiInputBinding::operator=( const deaiInputBinding &binding ){
	pDeviceID = binding.pDeviceID;
	pAxisID = binding.pAxisID;
	pButtonID = binding.pButtonID;
	pInvertAxis = binding.pInvertAxis;
	pDeviceIndex = binding.pDeviceIndex;
	pAxisIndex = binding.pAxisIndex;
	pButtonIndex = binding.pButtonIndex;
	return *this;
}
