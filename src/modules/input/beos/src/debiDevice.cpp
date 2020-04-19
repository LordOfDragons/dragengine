/* 
 * Drag[en]gine BeOS Input Module
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

#include "debiDevice.h"
#include "debiDeviceAxis.h"
#include "debiDeviceButton.h"
#include "deBeOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class debiDevice
/////////////////////

// Constructor, destructor
////////////////////////////

debiDevice::debiDevice( deBeOSInput &module ) :
pModule( module ),
pIndex( -1 ),
pSource( esBeOS ),
pType( deInputDevice::edtMouse ),

pButtonCount( 0 ),
pButtons( NULL ),

pAxisCount( 0 ),
pAxes( NULL ),

pDirtyAxesValues( false ){
}

debiDevice::~debiDevice(){
	if( pAxes ){
		delete [] pAxes;
	}
	if( pButtons ){
		delete [] pButtons;
	}
}



// Management
///////////////

void debiDevice::SetIndex( int index ){
	pIndex = index;
}

void debiDevice::SetType( deInputDevice::eDeviceTypes type ){
	pType = type;
}

void debiDevice::SetID( const char *id ){
	pID = id;
}

void debiDevice::SetName( const char *name ){
	pName = name;
}



void debiDevice::SetButtonCount( int count ){
	if( pButtons ){
		delete [] pButtons;
		pButtons = NULL;
		pButtonCount = 0;
	}
	
	if( count > 0 ){
		pButtons = new debiDeviceButton[ count ];
		pButtonCount = count;
	}
}

debiDeviceButton &debiDevice::GetButtonAt( int index ) const{
	if( index < 0 || index >= pButtonCount ){
		DETHROW( deeInvalidParam );
	}
	return pButtons[ index ];
}

debiDeviceButton *debiDevice::GetButtonWithID( const char *id ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetID() == id ){
			return pButtons + i;
		}
	}
	
	return NULL;
}

int debiDevice::IndexOfButtonWithID( const char *id ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}

int debiDevice::IndexOfButtonWithBICode( int code ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetBICode() == code ){
			return i;
		}
	}
	
	return -1;
}



void debiDevice::SetAxisCount( int count ){
	if( pAxes ){
		delete [] pAxes;
		pAxes = NULL;
		pAxisCount = 0;
	}
	
	if( count > 0 ){
		pAxes = new debiDeviceAxis[ count ];
		pAxisCount = count;
	}
}

debiDeviceAxis &debiDevice::GetAxisAt( int index ) const{
	if( index < 0 || index >= pAxisCount ){
		DETHROW( deeInvalidParam );
	}
	return pAxes[ index ];
}

debiDeviceAxis *debiDevice::GetAxisWithID( const char *id ) const{
	int i;
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetID() == id ){
			return pAxes + i;
		}
	}
	
	return NULL;
}

int debiDevice::IndexOfAxisWithID( const char *id ) const{
	int i;
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}

int debiDevice::IndexOfAxisWithBICode( int code ) const{
	int i;
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetBICode() == code ){
			return i;
		}
	}
	
	return -1;
}



void debiDevice::SetDirtyAxesValues( bool dirty ){
	pDirtyAxesValues = dirty;
}



void debiDevice::GetInfo( deInputDevice &info ) const{
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

void debiDevice::Update(){
}

void debiDevice::SendDirtyAxisEvents(){
	if( ! pDirtyAxesValues ){
		return;
	}
	
	pDirtyAxesValues = false;
	
	int i;
	for( i=0; i<pAxisCount; i++ ){
		pAxes[ i ].SendEvents( *this );
	}
}
