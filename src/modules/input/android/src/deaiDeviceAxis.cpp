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
