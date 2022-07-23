/* 
 * Drag[en]gine OpenXR VR Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "deoxrDeviceComponent.h"
#include "deoxrDevice.h"
#include "../deVROpenXR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputDeviceComponent.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deoxrDeviceComponent
////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDeviceComponent::deoxrDeviceComponent( deoxrDevice &device ) :
pDevice( device ),
pIndex( -1 ),
pType( deInputDeviceComponent::ectGeneric ){
}

deoxrDeviceComponent::~deoxrDeviceComponent(){
}



// Management
///////////////

void deoxrDeviceComponent::SetIndex( int index ){
	pIndex = index;
}

void deoxrDeviceComponent::SetID( const char *id ){
	pID = id;
}

void deoxrDeviceComponent::SetName( const char *name ){
	pName = name;
}

void deoxrDeviceComponent::SetType( deInputDeviceComponent::eComponentTypes type ){
	pType = type;
}



void deoxrDeviceComponent::SetDisplayImages( const char *name ){
	pDisplayImage = nullptr;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pDevice.GetOxr().GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pDevice.GetOxr().GetVFS();
	const char * const basePath = "/share/image/component";
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

void deoxrDeviceComponent::SetDisplayImages( const deoxrDeviceComponent &component ){
	pDisplayImage = component.pDisplayImage;
	pDisplayIcons = component.pDisplayIcons;
}

void deoxrDeviceComponent::SetDisplayText( const char *text ){
	pDisplayText = text;
}



void deoxrDeviceComponent::GetInfo( deInputDeviceComponent &info ) const{
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
	
	info.SetDisplayImage( pDisplayImage );
	
	int i;
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
}
