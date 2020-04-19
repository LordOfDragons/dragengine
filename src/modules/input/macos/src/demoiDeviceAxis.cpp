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

#include <stdlib.h>

#include "deMacOSInput.h"
#include "demoiDevice.h"
#include "demoiDeviceAxis.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/input/deInputDeviceAxis.h>



// Class demoiDeviceAxis
//////////////////////////

// Constructor, destructor
////////////////////////////

demoiDeviceAxis::demoiDeviceAxis() :
pIndex( -1 ),
pType( deInputDeviceAxis::eatGeneric ),
pMinimum( -100 ),
pMaximum( 100 ),
pAbsolute( true ),
pWheelOtherAxis( false ),

pValue( 0.0f ),
pChangedValue( 0.0f ),

pMOCode( 0 ){
}

demoiDeviceAxis::~demoiDeviceAxis(){
}



// Management
///////////////

void demoiDeviceAxis::SetIndex( int index ){
    pIndex = index;
}

void demoiDeviceAxis::SetID( const char *id ){
    pID = id;
}

void demoiDeviceAxis::SetName( const char *name ){
	pName = name;
}

void demoiDeviceAxis::SetType( deInputDeviceAxis::eAxisTypes type ){
    pType = type;
}



void demoiDeviceAxis::SetMinimum( int minimum ){
    pMinimum = minimum;
}

void demoiDeviceAxis::SetMaximum( int maximum ){
    pMaximum = maximum;
}

void demoiDeviceAxis::SetAbsolute( bool absolute ){
    pAbsolute = absolute;
}

void demoiDeviceAxis::SetWheelOtherAxis( bool otherAxis ){
    pWheelOtherAxis = otherAxis;
}



void demoiDeviceAxis::SetValue( float value ){
    if( pAbsolute ){
        value = decMath::clamp( value, -1.0f, 1.0f );
    }
    pValue = value;
}



void demoiDeviceAxis::SetMOCode( int code ){
    pMOCode = code;
}



void demoiDeviceAxis::GetInfo( deInputDeviceAxis &info ) const{
	info.SetID( pID );
	info.SetName( pName );
    info.SetType( pType );
}

void demoiDeviceAxis::SendEvents( demoiDevice &device ){
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
