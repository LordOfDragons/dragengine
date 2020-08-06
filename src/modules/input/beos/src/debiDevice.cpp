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
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/image/deImageManager.h>
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
pDirtyAxesValues( false ){
}

debiDevice::~debiDevice(){
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

void debiDevice::SetDisplayImages( const char *name ){
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

void debiDevice::SetDisplayText( const char *text ){
	pDisplayText = text;
}



int debiDevice::GetButtonCount() const{
	return pButtons.GetCount();
}

debiDeviceButton *debiDevice::GetButtonAt( int index ) const{
	return ( debiDeviceButton* )pButtons.GetAt( index );
}

debiDeviceButton *debiDevice::GetButtonWithID( const char *id ) const{
	const int count = pButtons.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		debiDeviceButton * const button = ( debiDeviceButton* )pButtons.GetAt( i );
		if( button->GetID() == id ){
			return button;
		}
	}
	return NULL;
}

int debiDevice::IndexOfButtonWithID( const char *id ) const{
	const int count = pButtons.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( ( ( debiDeviceButton* )pButtons.GetAt( i ) )->GetID() == id ){
			return i;
		}
	}
	return -1;
}

int debiDevice::IndexOfButtonWithBICode( int code ) const{
	const int count = pButtons.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( ( ( debiDeviceButton* )pButtons.GetAt( i ) )->GetBICode() == code ){
			return i;
		}
	}
	return -1;
}

void debiDevice::AddButton( debiDeviceButton *button ){
	if( ! button ){
		DETHROW( deeNullPointer );
	}
	pButtons.Add( button );
}



int debiDevice::GetAxisCount() const{
	return pAxes.GetCount();
}

debiDeviceAxis *debiDevice::GetAxisAt( int index ) const{
	return ( debiDeviceAxis* )pAxes.GetAt( index );
}

debiDeviceAxis *debiDevice::GetAxisWithID( const char *id ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		debiDeviceAxis * const axis = ( debiDeviceAxis* )pAxes.GetAt( i );
		if( axis->GetID() == id ){
			return axis;
		}
	}
	return NULL;
}

int debiDevice::IndexOfAxisWithID( const char *id ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( ( ( debiDeviceAxis* )pAxes.GetAt( i ) )->GetID() == id ){
			return i;
		}
	}
	return -1;
}

int debiDevice::IndexOfAxisWithBICode( int code ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( ( ( debiDeviceAxis* )pAxes.GetAt( i ) )->GetBICode() == code ){
			return i;
		}
	}
	return -1;
}

void debiDevice::AddAxis( debiDeviceAxis *axis ){
	if( ! axis ){
		DETHROW( deeNullPointer );
	}
	pAxes.Add( axis );
}



void debiDevice::SetDirtyAxesValues( bool dirty ){
	pDirtyAxesValues = dirty;
}



void debiDevice::GetInfo( deInputDevice &info ) const{
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
		( ( debiDeviceButton* )pButtons.GetAt( i ) )->GetInfo( info.GetButtonAt( i ) );
	}
	
	const int axisCount = pAxes.GetCount();
	info.SetAxisCount( axisCount );
	for( i=0; i<axisCount; i++ ){
		( ( debiDeviceAxis* )pAxes.GetAt( i ) )->GetInfo( info.GetAxisAt( i ) );
	}
}

void debiDevice::Update(){
}

void debiDevice::SendDirtyAxisEvents(){
	if( ! pDirtyAxesValues ){
		return;
	}
	
	pDirtyAxesValues = false;
	
	const int axisCount = pAxes.GetCount();
	int i;
	for( i=0; i<axisCount; i++ ){
		( ( debiDeviceAxis* )pAxes.GetAt( i ) )->SendEvents( *this );
	}
}
