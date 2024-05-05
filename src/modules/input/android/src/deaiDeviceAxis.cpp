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

#include "deaiDevice.h"
#include "deaiDeviceAxis.h"
#include "deAndroidInput.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class deaiDeviceAxis
//////////////////////////

// Constructor, destructor
////////////////////////////

deaiDeviceAxis::deaiDeviceAxis() :
pIndex( -1 ),
pType( deInputDeviceAxis::eatGeneric ),
pMinimum( -100 ),
pMaximum( 100 ),
pFuzz( 0 ),
pFlat( 0 ),
pAbsolute( true ),
pWheelOtherAxis( false ),

pValue( 0.0f ),
pChangedValue( 0.0f ),

pAICode( -1 ){
}

deaiDeviceAxis::~deaiDeviceAxis(){
}



// Management
///////////////

void deaiDeviceAxis::SetIndex( int index ){
	pIndex = index;
}

void deaiDeviceAxis::SetID( const char *id ){
	pID = id;
}

void deaiDeviceAxis::SetName( const char *name ){
	pName = name;
}

void deaiDeviceAxis::SetType( deInputDeviceAxis::eAxisTypes type ){
	pType = type;
}


void deaiDeviceAxis::SetMinimum( int minimum ){
	pMinimum = minimum;
}

void deaiDeviceAxis::SetMaximum( int maximum ){
	pMaximum = maximum;
}

void deaiDeviceAxis::SetFuzz( int fuzz ){
	pFuzz = fuzz;
}

void deaiDeviceAxis::SetFlat( int flat ){
	pFlat = flat;
}

void deaiDeviceAxis::SetAbsolute( bool absolute ){
	pAbsolute = absolute;
}

void deaiDeviceAxis::SetWheelOtherAxis( bool otherAxis ){
	pWheelOtherAxis = otherAxis;
}



void deaiDeviceAxis::SetValue( float value ){
	if( pAbsolute ){
		value = decMath::clamp( value, -1.0f, 1.0f );
	}
	pValue = value;
}



void deaiDeviceAxis::SetAICode( int code ){
	pAICode = code;
}



void deaiDeviceAxis::GetInfo( deInputDeviceAxis &info ) const{
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
}

void deaiDeviceAxis::SendEvents( deaiDevice &device ){
	if( pAbsolute ){
		if( fabsf( pChangedValue - pValue ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		pValue = pChangedValue;
		device.GetModule().AddAxisChanged( device.GetIndex(), pIndex, pValue, pLastEventTime );
		
	}else{
		if( fabsf( pChangedValue ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		pValue = pChangedValue;
		pChangedValue = 0.0f;
		device.GetModule().AddAxisChanged( device.GetIndex(), pIndex, pValue, pLastEventTime );
	}
}
