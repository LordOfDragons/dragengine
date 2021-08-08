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

#include "deovrDevice.h"
#include "deovrDeviceAxis.h"
#include "deVROpenVR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deovrDeviceAxis
//////////////////////////

// Constructor, destructor
////////////////////////////

deovrDeviceAxis::deovrDeviceAxis( deVROpenVR &ovr ) :
pOvr( ovr ),
pIndex( -1 ),
pType( deInputDeviceAxis::eatGeneric ),
pMinimum( -100 ),
pMaximum( 100 ),
pValue( 0.0f ){
}

deovrDeviceAxis::~deovrDeviceAxis(){
}



// Management
///////////////

void deovrDeviceAxis::SetIndex( int index ){
	pIndex = index;
}

void deovrDeviceAxis::SetID( const char *id ){
	pID = id;
}

void deovrDeviceAxis::SetName( const char *name ){
	pName = name;
}

void deovrDeviceAxis::SetType( deInputDeviceAxis::eAxisTypes type ){
	pType = type;
}



void deovrDeviceAxis::SetDisplayImages( const char *name ){
	pDisplayImage = nullptr;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pOvr.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pOvr.GetVFS();
	const char * const basePath = "/share/image/axis";
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

void deovrDeviceAxis::SetDisplayText( const char *text ){
	pDisplayText = text;
}



void deovrDeviceAxis::SetMinimum( int minimum ){
	pMinimum = minimum;
}

void deovrDeviceAxis::SetMaximum( int maximum ){
	pMaximum = maximum;
}



void deovrDeviceAxis::SetValue( float value ){
	value = decMath::clamp( value, -1.0f, 1.0f );
}



void deovrDeviceAxis::GetInfo( deInputDeviceAxis &info ) const{
	int i;
	
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
	
	info.SetDisplayImage( pDisplayImage );
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
}
