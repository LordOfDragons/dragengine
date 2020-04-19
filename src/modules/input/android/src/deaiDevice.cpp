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

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "deaiDevice.h"
#include "deaiDeviceAxis.h"
#include "deaiDeviceButton.h"
#include "deaiDeviceFeedback.h"
#include "deaiDeviceManager.h"
#include "deAndroidInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class deaiDevice
/////////////////////

// Constructor, destructor
////////////////////////////

deaiDevice::deaiDevice( deAndroidInput &module, eSources source ) :
pModule( module ),
pIndex( -1 ),
pSource( source ),
pType( deInputDevice::edtMouse ),

pButtonCount( 0 ),
pButtons( NULL ),
pAxisCount( 0 ),
pAxes( NULL ),
pFeedbackCount( 0 ),
pFeedbacks( NULL ),

pDirtyAxesValues( false ){
}

deaiDevice::~deaiDevice(){
	if( pFeedbacks ){
		delete [] pFeedbacks;
	}
	if( pAxes ){
		delete [] pAxes;
	}
	if( pButtons ){
		delete [] pButtons;
	}
}



// Management
///////////////

void deaiDevice::SetIndex( int index ){
	pIndex = index;
}

void deaiDevice::SetType( deInputDevice::eDeviceTypes type ){
	pType = type;
}

void deaiDevice::SetID( const char *id ){
	pID = id;
}

void deaiDevice::SetName( const char *name ){
	pName = name;
}



void deaiDevice::SetButtonCount( int count ){
	if( pButtons ){
		delete [] pButtons;
		pButtons = NULL;
		pButtonCount = 0;
	}
	
	if( count > 0 ){
		pButtons = new deaiDeviceButton[ count ];
		pButtonCount = count;
	}
}

deaiDeviceButton &deaiDevice::GetButtonAt( int index ) const{
	if( index < 0 || index >= pButtonCount ){
		DETHROW( deeInvalidParam );
	}
	return pButtons[ index ];
}

deaiDeviceButton *deaiDevice::GetButtonWithID( const char *id ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetID() == id ){
			return pButtons + i;
		}
	}
	
	return NULL;
}

deaiDeviceButton *deaiDevice::GetButtonWithAICode( int code ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetAICode() == code ){
			return pButtons + i;
		}
	}
	
	return NULL;
}

int deaiDevice::IndexOfButtonWithID( const char *id ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}

int deaiDevice::IndexOfButtonWithKeyCode( deInputEvent::eKeyCodes code ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetKeyCode() == code ){
			return i;
		}
	}
	
	return -1;
}

int deaiDevice::IndexOfButtonWithAICode( int code ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetAICode() == code ){
			return i;
		}
	}
	
	return -1;
}



void deaiDevice::SetAxisCount( int count ){
	if( pAxes ){
		delete [] pAxes;
		pAxes = NULL;
		pAxisCount = 0;
	}
	
	if( count > 0 ){
		pAxes = new deaiDeviceAxis[ count ];
		pAxisCount = count;
	}
}

deaiDeviceAxis &deaiDevice::GetAxisAt( int index ) const{
	if( index < 0 || index >= pAxisCount ){
		DETHROW( deeInvalidParam );
	}
	return pAxes[ index ];
}

deaiDeviceAxis *deaiDevice::GetAxisWithID( const char *id ) const{
	int i;
	
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetID() == id ){
			return pAxes + i;
		}
	}
	
	return NULL;
}

deaiDeviceAxis *deaiDevice::GetAxisWithAICode( int code ) const{
	int i;
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetAICode() == code ){
			return pAxes + i;
		}
	}
	
	return NULL;
}

int deaiDevice::IndexOfAxisWithID( const char *id ) const{
	int i;
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}

int deaiDevice::IndexOfAxisWithAICode( int code ) const{
	int i;
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetAICode() == code ){
			return i;
		}
	}
	
	return -1;
}



void deaiDevice::SetFeedbackCount( int count ){
	if( pFeedbacks ){
		delete [] pFeedbacks;
		pFeedbacks = NULL;
		pFeedbackCount = 0;
	}
	
	if( count > 0 ){
		pFeedbacks = new deaiDeviceFeedback[ count ];
		pFeedbackCount = count;
	}
}

deaiDeviceFeedback &deaiDevice::GetFeedbackAt( int index ) const{
	if( index < 0 || index >= pFeedbackCount ){
		DETHROW( deeInvalidParam );
	}
	return pFeedbacks[ index ];
}

deaiDeviceFeedback *deaiDevice::GetFeedbackWithID( const char *id ) const{
	int i;
	for( i=0; i<pFeedbackCount; i++ ){
		if( pFeedbacks[ i ].GetID() == id ){
			return pFeedbacks + i;
		}
	}
	
	return NULL;
}

int deaiDevice::IndexOfFeedbackWithID( const char *id ) const{
	int i;
	for( i=0; i<pFeedbackCount; i++ ){
		if( pFeedbacks[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



void deaiDevice::SetDirtyAxesValues( bool dirty ){
	pDirtyAxesValues = dirty;
}



void deaiDevice::GetInfo( deInputDevice &info ) const{
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
	
	info.SetFeedbackCount( pFeedbackCount );
	for( i=0; i<pFeedbackCount; i++ ){
		pFeedbacks[ i ].GetInfo( info.GetFeedbackAt( i ) );
	}
}

void deaiDevice::Update(){
}

void deaiDevice::SendDirtyAxisEvents(){
	if( ! pDirtyAxesValues ){
		return;
	}
	
	pDirtyAxesValues = false;
	
	int i;
	for( i=0; i<pAxisCount; i++ ){
		pAxes[ i ].SendEvents( *this );
	}
}



// Private Functions
//////////////////////
