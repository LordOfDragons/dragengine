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
