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
#include "demoiDeviceAxis.h"
#include "demoiDeviceButton.h"
#include "deMacOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>


// Class demoiDevice
//////////////////////

// Constructor, destructor
////////////////////////////

demoiDevice::demoiDevice( deMacOSInput &module, eSources source ) :
pModule( module ),
pIndex( -1 ),
pSource( source ),
pType( deInputDevice::edtMouse ),

pButtonCount( 0 ),
pButtons( NULL ),
pAxisCount( 0 ),
pAxes( NULL ),

pDirtyAxisValues( false ){
}

demoiDevice::~demoiDevice(){
	if( pAxes ){
		delete [] pAxes;
	}
	if( pButtons ){
		delete [] pButtons;
	}
}



// Management
///////////////

void demoiDevice::SetIndex( int index ){
    pIndex = index;
}

void demoiDevice::SetType( deInputDevice::eDeviceTypes type ){
	pType = type;
}

void demoiDevice::SetID( const char *id ){
	pID = id;
}

void demoiDevice::SetName( const char *name ){
	pName = name;
}



void demoiDevice::SetButtonCount( int count ){
	if( pButtons ){
		delete [] pButtons;
		pButtons = NULL;
		pButtonCount = 0;
	}
	
	if( count > 0 ){
		pButtons = new demoiDeviceButton[ count ];
		pButtonCount = count;
	}
}

demoiDeviceButton &demoiDevice::GetButtonAt( int index ) const{
	if( index < 0 || index >= pButtonCount ){
		DETHROW( deeInvalidParam );
	}
	return pButtons[ index ];
}

demoiDeviceButton *demoiDevice::GetButtonWithID( const char *id ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetID() == id ){
			return pButtons + i;
		}
	}
	
	return NULL;
}

int demoiDevice::IndexOfButtonWithID( const char *id ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}

int demoiDevice::IndexOfButtonWithMOCode( int code ) const{
    int i;
    for( i=0; i<pButtonCount; i++ ){
        if( pButtons[ i ].GetMOCode() == code ){
            return i;
        }
    }
    
    return -1;
}



void demoiDevice::SetAxisCount( int count ){
	if( pAxes ){
		delete [] pAxes;
		pAxes = NULL;
		pAxisCount = 0;
	}
	
	if( count > 0 ){
		pAxes = new demoiDeviceAxis[ count ];
		pAxisCount = count;
	}
}

demoiDeviceAxis &demoiDevice::GetAxisAt( int index ) const{
	if( index < 0 || index >= pAxisCount ){
		DETHROW( deeInvalidParam );
	}
	return pAxes[ index ];
}

demoiDeviceAxis *demoiDevice::GetAxisWithID( const char *id ) const{
	int i;
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetID() == id ){
			return pAxes + i;
		}
	}
	
	return NULL;
}

int demoiDevice::IndexOfAxisWithID( const char *id ) const{
	int i;
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}

int demoiDevice::IndexOfAxisWithMOCode( int code ) const{
    int i;
    for( i=0; i<pAxisCount; i++ ){
        if( pAxes[ i ].GetMOCode() == code ){
            return i;
        }
    }
    
    return -1;
}



void demoiDevice::SetDirtyAxisValues( bool dirty ){
    pDirtyAxisValues = dirty;
}



void demoiDevice::GetInfo( deInputDevice &info ) const{
	int i;
	
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
	
	info.SetDisplayImage( NULL );
	info.SetDisplayModel( NULL );
	info.SetDisplaySkin( NULL );
	
	info.SetButtonCount( pButtonCount );
	for( i=0; i<pButtonCount; i++ ){
		pButtons[ i ].GetInfo( info.GetButtonAt( i ) );
	}
	
	info.SetAxisCount( pAxisCount );
	for( i=0; i<pAxisCount; i++ ){
		pAxes[ i ].GetInfo( info.GetAxisAt( i ) );
	}
}

void demoiDevice::Update(){
}

void demoiDevice::SendDirtyAxisEvents(){
    if( ! pDirtyAxisValues ){
        return;
    }
    
    pDirtyAxisValues = false;
    
    int i;
    for( i=0; i<pAxisCount; i++ ){
        pAxes[ i ].SendEvents( *this );
    }
}
