/* 
 * Drag[en]gine Windows Input Module
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
#include <errno.h>

#include "dewiDevice.h"
#include "dewiDeviceAxis.h"
#include "dewiDeviceButton.h"
#include "dewiDeviceFeedback.h"
#include "dewiDeviceManager.h"
#include "deWindowsInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/systems/deInputSystem.h>



// Class dewiDevice
/////////////////////

// Constructor, destructor
////////////////////////////

dewiDevice::dewiDevice( deWindowsInput &module, eSources source ) :
pModule( module ),
pIndex( -1 ),
pSource( source ),
pType( deInputDevice::edtMouse ),

pDirtyAxesValues( false ){
}

dewiDevice::~dewiDevice(){
}



// Management
///////////////

void dewiDevice::SetIndex( int index ){
	pIndex = index;
}

void dewiDevice::SetType( deInputDevice::eDeviceTypes type ){
	pType = type;
}

void dewiDevice::SetID( const char *id ){
	pID = id;
}

void dewiDevice::SetName( const char *name ){
	pName = name;
}

void dewiDevice::SetDisplayImages( const char *name ){
	pDisplayImage = NULL;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pModule.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pModule.GetVFS();
	const char * const basePath = "/share/image/device";
	decString filename;
	
	filename.Format( "%s/%s/image.png", basePath, name );
	pDisplayImage.TakeOver( imageManager.LoadImage( vfs, filename, "/" ) );
	
	const int sizes[ 4 ] = {128, 64, 32, 16};
	deImageReference icon;
	int i;
	
	for( i=0; i<4; i++ ){
		filename.Format( "%s/%s/icon%d.png", basePath, name, sizes[ i ] );
		icon.TakeOver( imageManager.LoadImage( vfs, filename, "/" ) );
		pDisplayIcons.Add( ( deImage* )icon );
	}
}

void dewiDevice::SetDisplayText( const char *text ){
	pDisplayText = text;
}



int dewiDevice::GetButtonCount() const{
	return pButtons.GetCount();
}

void dewiDevice::AddButton( dewiDeviceButton *button ){
	if( ! button ){
		DETHROW( deeNullPointer );
	}
	pButtons.Add( button );
}

dewiDeviceButton *dewiDevice::GetButtonAt( int index ) const{
	return ( dewiDeviceButton* )pButtons.GetAt( index );
}

dewiDeviceButton *dewiDevice::GetButtonWithID( const char *id ) const{
	const int count = pButtons.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		dewiDeviceButton * const button = ( dewiDeviceButton* )pButtons.GetAt( i );
		if( button->GetID() == id ){
			return button;
		}
	}
	
	return NULL;
}

dewiDeviceButton *dewiDevice::GetButtonWithWICode( int code ) const{
	const int count = pButtons.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		dewiDeviceButton * const button = ( dewiDeviceButton* )pButtons.GetAt( i );
		if( button->GetWICode() == code ){
			return button;
		}
	}
	
	return NULL;
}

int dewiDevice::IndexOfButtonWithID( const char *id ) const{
	const int count = pButtons.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		dewiDeviceButton &button = *( ( dewiDeviceButton* )pButtons.GetAt( i ) );
		if( button.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}

int dewiDevice::IndexOfButtonWithWICode( int code ) const{
	const int count = pButtons.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		dewiDeviceButton &button = *( ( dewiDeviceButton* )pButtons.GetAt( i ) );
		if( button.GetWICode() == code ){
			return i;
		}
	}
	
	return -1;
}



int dewiDevice::GetAxisCount() const{
	return pAxes.GetCount();
}

void dewiDevice::AddAxis( dewiDeviceAxis *axis ){
	if( ! axis ){
		DETHROW( deeNullPointer );
	}
	pAxes.Add( axis );
}

dewiDeviceAxis *dewiDevice::GetAxisAt( int index ) const{
	return ( dewiDeviceAxis* )pAxes.GetAt( index );
}

dewiDeviceAxis *dewiDevice::GetAxisWithID( const char *id ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		dewiDeviceAxis * const axis = ( dewiDeviceAxis* )pAxes.GetAt( i );
		if( axis->GetID() == id ){
			return axis;
		}
	}
	
	return NULL;
}

dewiDeviceAxis *dewiDevice::GetAxisWithWICode( int code ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		dewiDeviceAxis * const axis = ( dewiDeviceAxis* )pAxes.GetAt( i );
		if( axis->GetWICode() == code ){
			return axis;
		}
	}
	
	return NULL;
}

int dewiDevice::IndexOfAxisWithID( const char *id ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		dewiDeviceAxis &axis = *( ( dewiDeviceAxis* )pAxes.GetAt( i ) );
		if( axis.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}

int dewiDevice::IndexOfAxisWithWICode( int code ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		dewiDeviceAxis &axis = *( ( dewiDeviceAxis* )pAxes.GetAt( i ) );
		if( axis.GetWICode() == code ){
			return i;
		}
	}
	
	return -1;
}



int dewiDevice::GetFeedbackCount() const{
	return pFeedbacks.GetCount();
}

void dewiDevice::AddFeedback( dewiDeviceFeedback *feedback ){
	if( ! feedback ){
		DETHROW( deeNullPointer );
	}
	pFeedbacks.Add( feedback );
}

dewiDeviceFeedback *dewiDevice::GetFeedbackAt( int index ) const{
	return ( dewiDeviceFeedback* )pFeedbacks.GetAt( index );
}

dewiDeviceFeedback *dewiDevice::GetFeedbackWithID( const char *id ) const{
	const int count = pFeedbacks.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		dewiDeviceFeedback * const feedback = ( dewiDeviceFeedback* )pFeedbacks.GetAt( i );
		if( feedback->GetID() == id ){
			return feedback;
		}
	}
	
	return NULL;
}

int dewiDevice::IndexOfFeedbackWithID( const char *id ) const{
	const int count = pFeedbacks.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		dewiDeviceFeedback * const feedback = ( dewiDeviceFeedback* )pFeedbacks.GetAt( i );
		if( feedback->GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



void dewiDevice::SetDirtyAxesValues( bool dirty ){
	pDirtyAxesValues = dirty;
}



void dewiDevice::GetInfo( deInputDevice &info ) const{
	int i;
	
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
	info.SetDisplayImage( pDisplayImage );
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
	
	info.SetDisplayModel( NULL );
	info.SetDisplaySkin( NULL );
	
	const int buttonCount = pButtons.GetCount();
	info.SetButtonCount( buttonCount );
	for( i=0; i<buttonCount; i++ ){
		( ( dewiDeviceButton* )pButtons.GetAt( i ) )->GetInfo( info.GetButtonAt( i ) );
	}
	
	const int axisCount = pAxes.GetCount();
	info.SetAxisCount( axisCount );
	for( i=0; i<axisCount; i++ ){
		( ( dewiDeviceAxis* )pAxes.GetAt( i ) )->GetInfo( info.GetAxisAt( i ) );
	}
	
	const int feedbackCount = pFeedbacks.GetCount();
	info.SetFeedbackCount( feedbackCount );
	for( i=0; i<feedbackCount; i++ ){
		( ( dewiDeviceFeedback* )pFeedbacks.GetAt( i ) )->GetInfo( info.GetFeedbackAt( i ) );
	}
}

void dewiDevice::Update(){
}

void dewiDevice::SendDirtyAxisEvents(){
	if( ! pDirtyAxesValues ){
		return;
	}
	
	pDirtyAxesValues = false;
	
	const int axisCount = pAxes.GetCount();
	int i;
	for( i=0; i<axisCount; i++ ){
		( ( dewiDeviceAxis* )pAxes.GetAt( i ) )->SendEvents( *this );
	}
}
