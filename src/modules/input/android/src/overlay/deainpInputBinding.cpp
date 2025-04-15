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

#include <stdlib.h>

#include "deainpInputBinding.h"
#include "../deAndroidInput.h"
#include "../deainpDevice.h"
#include "../deainpDeviceAxis.h"
#include "../deainpDeviceButton.h"
#include "../deainpDeviceManager.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class deainpInputBinding
///////////////////////////

// Constructor, destructor
////////////////////////////

deainpInputBinding::deainpInputBinding() :
pInvertAxis( false ),
pDeviceIndex( -1 ),
pAxisIndex( -1 ),
pButtonIndex( -1 ){
}

deainpInputBinding::deainpInputBinding( const char *deviceID, const char *buttonID ) :
pDeviceID( deviceID ),
pButtonID( buttonID ),
pInvertAxis( false ),
pDeviceIndex( -1 ),
pAxisIndex( -1 ),
pButtonIndex( -1 ){
}

deainpInputBinding::deainpInputBinding( const char *deviceID, const char *axisID, bool invertAxis ) :
pDeviceID( deviceID ),
pAxisID( axisID ),
pInvertAxis( invertAxis ),
pDeviceIndex( -1 ),
pAxisIndex( -1 ),
pButtonIndex( -1 ){
}

deainpInputBinding::deainpInputBinding( const deainpInputBinding &binding ) :
pDeviceID( binding.pDeviceID ),
pAxisID( binding.pAxisID ),
pButtonID( binding.pButtonID ),
pInvertAxis( binding.pInvertAxis ),
pDeviceIndex( binding.pDeviceIndex ),
pAxisIndex( binding.pAxisIndex ),
pButtonIndex( binding.pButtonIndex ){
}

deainpInputBinding::~deainpInputBinding(){
}



// Management
///////////////

void deainpInputBinding::SetDeviceID( const char *id ){
	pDeviceID = id;
}

void deainpInputBinding::SetAxisID( const char *id ){
	pAxisID = id;
}

void deainpInputBinding::SetButtonID( const char *id ){
	pButtonID = id;
}

void deainpInputBinding::SetInvertAxis( bool invertAxis ){
	pInvertAxis = invertAxis;
}



void deainpInputBinding::UpdateIndices( const deainpDeviceManager &devices ){
	pDeviceIndex = devices.IndexOfWithID( pDeviceID );
	
	if( pDeviceIndex != -1 ){
		const deainpDevice &device = *devices.GetAt( pDeviceIndex );
		pAxisIndex = device.IndexOfAxisWithID( pAxisID );
		pButtonIndex = device.IndexOfButtonWithID( pButtonID );
		
	}else{
		pAxisIndex = -1;
		pButtonIndex = -1;
	}
}

void deainpInputBinding::ChangeButtonState( deAndroidInput &androidInput, bool pressed ) const{
	if( pButtonIndex == -1 ){
		return;
	}
	
	const deainpDevice &device = *androidInput.GetDevices().GetAt( pDeviceIndex );
	const timeval timestamp = androidInput.TimeValNow();
	
	deainpDeviceButton &button = device.GetButtonAt( pButtonIndex );
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

bool deainpInputBinding::operator==( const deainpInputBinding &binding ) const{
	return pDeviceID == binding.pDeviceID
		&& pAxisID == binding.pAxisID
		&& pButtonID == binding.pButtonID
		&& pInvertAxis == binding.pInvertAxis;
}

deainpInputBinding &deainpInputBinding::operator=( const deainpInputBinding &binding ){
	pDeviceID = binding.pDeviceID;
	pAxisID = binding.pAxisID;
	pButtonID = binding.pButtonID;
	pInvertAxis = binding.pInvertAxis;
	pDeviceIndex = binding.pDeviceIndex;
	pAxisIndex = binding.pAxisIndex;
	pButtonIndex = binding.pButtonIndex;
	return *this;
}
