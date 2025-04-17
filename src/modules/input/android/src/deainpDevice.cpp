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

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "deainpDevice.h"
#include "deainpDeviceAxis.h"
#include "deainpDeviceButton.h"
#include "deainpDeviceFeedback.h"
#include "deainpDeviceManager.h"
#include "deAndroidInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class deainpDevice
/////////////////////

// Constructor, destructor
////////////////////////////

deainpDevice::deainpDevice( deAndroidInput &module, eSources source ) :
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

deainpDevice::~deainpDevice(){
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

void deainpDevice::SetIndex( int index ){
	pIndex = index;
}

void deainpDevice::SetType( deInputDevice::eDeviceTypes type ){
	pType = type;
}

void deainpDevice::SetID( const char *id ){
	pID = id;
}

void deainpDevice::SetName( const char *name ){
	pName = name;
}



void deainpDevice::SetButtonCount( int count ){
	if( pButtons ){
		delete [] pButtons;
		pButtons = NULL;
		pButtonCount = 0;
	}
	
	if( count > 0 ){
		pButtons = new deainpDeviceButton[ count ];
		pButtonCount = count;
	}
}

deainpDeviceButton &deainpDevice::GetButtonAt( int index ) const{
	if( index < 0 || index >= pButtonCount ){
		DETHROW( deeInvalidParam );
	}
	return pButtons[ index ];
}

deainpDeviceButton *deainpDevice::GetButtonWithID( const char *id ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetID() == id ){
			return pButtons + i;
		}
	}
	
	return NULL;
}

deainpDeviceButton *deainpDevice::GetButtonWithAICode( int code ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetAICode() == code ){
			return pButtons + i;
		}
	}
	
	return NULL;
}

int deainpDevice::IndexOfButtonWithID( const char *id ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}

int deainpDevice::IndexOfButtonWithKeyCode( deInputEvent::eKeyCodes code ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetKeyCode() == code ){
			return i;
		}
	}
	
	return -1;
}

int deainpDevice::IndexOfButtonWithAICode( int code ) const{
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetAICode() == code ){
			return i;
		}
	}
	
	return -1;
}



void deainpDevice::SetAxisCount( int count ){
	if( pAxes ){
		delete [] pAxes;
		pAxes = NULL;
		pAxisCount = 0;
	}
	
	if( count > 0 ){
		pAxes = new deainpDeviceAxis[ count ];
		pAxisCount = count;
	}
}

deainpDeviceAxis &deainpDevice::GetAxisAt( int index ) const{
	if( index < 0 || index >= pAxisCount ){
		DETHROW( deeInvalidParam );
	}
	return pAxes[ index ];
}

deainpDeviceAxis *deainpDevice::GetAxisWithID( const char *id ) const{
	int i;
	
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetID() == id ){
			return pAxes + i;
		}
	}
	
	return NULL;
}

deainpDeviceAxis *deainpDevice::GetAxisWithAICode( int code ) const{
	int i;
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetAICode() == code ){
			return pAxes + i;
		}
	}
	
	return NULL;
}

int deainpDevice::IndexOfAxisWithID( const char *id ) const{
	int i;
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}

int deainpDevice::IndexOfAxisWithAICode( int code ) const{
	int i;
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetAICode() == code ){
			return i;
		}
	}
	
	return -1;
}



void deainpDevice::SetFeedbackCount( int count ){
	if( pFeedbacks ){
		delete [] pFeedbacks;
		pFeedbacks = NULL;
		pFeedbackCount = 0;
	}
	
	if( count > 0 ){
		pFeedbacks = new deainpDeviceFeedback[ count ];
		pFeedbackCount = count;
	}
}

deainpDeviceFeedback &deainpDevice::GetFeedbackAt( int index ) const{
	if( index < 0 || index >= pFeedbackCount ){
		DETHROW( deeInvalidParam );
	}
	return pFeedbacks[ index ];
}

deainpDeviceFeedback *deainpDevice::GetFeedbackWithID( const char *id ) const{
	int i;
	for( i=0; i<pFeedbackCount; i++ ){
		if( pFeedbacks[ i ].GetID() == id ){
			return pFeedbacks + i;
		}
	}
	
	return NULL;
}

int deainpDevice::IndexOfFeedbackWithID( const char *id ) const{
	int i;
	for( i=0; i<pFeedbackCount; i++ ){
		if( pFeedbacks[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



void deainpDevice::SetDirtyAxesValues( bool dirty ){
	pDirtyAxesValues = dirty;
}



void deainpDevice::GetInfo( deInputDevice &info ) const{
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

void deainpDevice::Update(){
}

void deainpDevice::SendDirtyAxisEvents(){
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
