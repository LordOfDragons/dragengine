/* 
 * Drag[en]gine OpenVR VR Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
#include <string.h>

#include "deovrDevice.h"
#include "deovrDeviceAxis.h"
#include "deovrDeviceButton.h"
#include "deovrDeviceFeedback.h"
#include "deovrDeviceManager.h"
#include "deVROpenVR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deovrDevice
//////////////////////

// Constructor, destructor
////////////////////////////

deovrDevice::deovrDevice( deVROpenVR &ovr ) :
pOvr( ovr ),
pIndex( -1 ),
pType( deInputDevice::edtGeneric ){
}

deovrDevice::~deovrDevice(){
}



// Management
///////////////

void deovrDevice::SetIndex( int index ){
	pIndex = index;
}

void deovrDevice::SetType( deInputDevice::eDeviceTypes type ){
	pType = type;
}

void deovrDevice::SetID( const char *id ){
	pID = id;
}

void deovrDevice::SetName( const char *name ){
	pName = name;
}

void deovrDevice::SetDisplayImages( const char *name ){
	pDisplayImage = nullptr;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pOvr.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pOvr.GetVFS();
	const char * const basePath = "/share/image/device";
	decString filename;
	
	filename.Format( "%s/%s/image.png", basePath, name );
	pDisplayImage.TakeOver( imageManager.LoadImage( vfs, filename, "/" ) );
	
	const int sizes[ 4 ] = {128, 64, 32, 16};
	int i;
	
	for( i=0; i<4; i++ ){
		filename.Format( "%s/%s/icon%d.png", basePath, name, sizes[ i ] );
		pDisplayIcons.Add( deImage::Ref::New( imageManager.LoadImage( vfs, filename, "/" ) ) );
	}
}

void deovrDevice::SetDisplayText( const char *text ){
	pDisplayText = text;
}



int deovrDevice::GetButtonCount() const{
	return pButtons.GetCount();
}

void deovrDevice::AddButton( deovrDeviceButton *button ){
	if( ! button ){
		DETHROW( deeNullPointer );
	}
	pButtons.Add( button );
}

deovrDeviceButton *deovrDevice::GetButtonAt( int index ) const{
	return ( deovrDeviceButton* )pButtons.GetAt( index );
}

deovrDeviceButton *deovrDevice::GetButtonWithID( const char *id ) const{
	const int count = pButtons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deovrDeviceButton * const button = ( deovrDeviceButton* )pButtons.GetAt( i );
		if( button->GetID() == id ){
			return button;
		}
	}
	
	return nullptr;
}

int deovrDevice::IndexOfButtonWithID( const char *id ) const{
	const int count = pButtons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deovrDeviceButton &button = *( ( deovrDeviceButton* )pButtons.GetAt( i ) );
		if( button.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



int deovrDevice::GetAxisCount() const{
	return pAxes.GetCount();
}

void deovrDevice::AddAxis( deovrDeviceAxis *axis ){
	if( ! axis ){
		DETHROW( deeNullPointer );
	}
	pAxes.Add( axis );
}

deovrDeviceAxis *deovrDevice::GetAxisAt( int index ) const{
	return ( deovrDeviceAxis* )pAxes.GetAt( index );
}

deovrDeviceAxis *deovrDevice::GetAxisWithID( const char *id ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deovrDeviceAxis * const axis = ( deovrDeviceAxis* )pAxes.GetAt( i );
		if( axis->GetID() == id ){
			return axis;
		}
	}
	
	return nullptr;
}

int deovrDevice::IndexOfAxisWithID( const char *id ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const deovrDeviceAxis &axis = *( ( deovrDeviceAxis* )pAxes.GetAt( i ) );
		if( axis.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



int deovrDevice::GetFeedbackCount() const{
	return pFeedbacks.GetCount();
}

void deovrDevice::AddFeedback( deovrDeviceFeedback *feedback ){
	if( ! feedback ){
		DETHROW( deeNullPointer );
	}
	pFeedbacks.Add( feedback );
}

deovrDeviceFeedback *deovrDevice::GetFeedbackAt( int index ) const{
	return ( deovrDeviceFeedback* )pFeedbacks.GetAt( index );
}

int deovrDevice::IndexOfFeedbackWithID( const char *id ) const{
	const int count = pFeedbacks.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const deovrDeviceFeedback &feedback = *( ( deovrDeviceFeedback* )pFeedbacks.GetAt( i ) );
		if( feedback.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



void deovrDevice::GetInfo( deInputDevice &info ) const{
	int i;
	
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
	info.SetDisplayImage( pDisplayImage );
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
	
	info.SetDisplayModel( nullptr );
	info.SetDisplaySkin( nullptr );
	
	const int buttonCount = pButtons.GetCount();
	info.SetButtonCount( buttonCount );
	for( i=0; i<buttonCount; i++ ){
		( ( deovrDeviceButton* )pButtons.GetAt( i ) )->GetInfo( info.GetButtonAt( i ) );
	}
	
	const int axisCount = pAxes.GetCount();
	info.SetAxisCount( axisCount );
	for( i=0; i<axisCount; i++ ){
		( ( deovrDeviceAxis* )pAxes.GetAt( i ) )->GetInfo( info.GetAxisAt( i ) );
	}
	
	const int feedbackCount = pFeedbacks.GetCount();
	info.SetFeedbackCount( feedbackCount );
	for( i=0; i<feedbackCount; i++ ){
		( ( deovrDeviceFeedback* )pFeedbacks.GetAt( i ) )->GetInfo( info.GetFeedbackAt( i ) );
	}
}

void deovrDevice::Update(){
}



// Private Functions
//////////////////////
