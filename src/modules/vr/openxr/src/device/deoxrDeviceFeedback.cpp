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

#include "deoxrDeviceFeedback.h"
#include "../deVROpenXR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deoxrDeviceFeedback
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDeviceFeedback::deoxrDeviceFeedback( deVROpenXR &oxr ) :
pOxr( oxr ),

pIndex( -1 ),
pType( deInputDeviceFeedback::eftGeneric ),
pMaximum( 1 ),
pValue( 0.0f ){
}

deoxrDeviceFeedback::~deoxrDeviceFeedback(){
}



// Management
///////////////

void deoxrDeviceFeedback::SetIndex( int index ){
	pIndex = index;
}

void deoxrDeviceFeedback::SetID( const char *id ){
	pID = id;
}

void deoxrDeviceFeedback::SetName( const char *name ){
	pName = name;
}

void deoxrDeviceFeedback::SetType( deInputDeviceFeedback::eFeedbackTypes type ){
	pType = type;
}

void deoxrDeviceFeedback::SetInputDeviceComponent( deoxrDeviceComponent *component ){
	pInputDeviceComponent = component;
}



void deoxrDeviceFeedback::SetDisplayImages( const char *name ){
	pDisplayImage = nullptr;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pOxr.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pOxr.GetVFS();
	const char * const basePath = "/share/image/feedback";
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

void deoxrDeviceFeedback::SetDisplayText( const char *text ){
	pDisplayText = text;
}



void deoxrDeviceFeedback::SetMaximum( int maximum ){
	pMaximum = maximum;
}



void deoxrDeviceFeedback::SetValue( float value ){
	pValue = value;
}



void deoxrDeviceFeedback::GetInfo( deInputDeviceFeedback &info ) const{
	int i;
	
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
	info.SetComponent( pInputDeviceComponent ? pInputDeviceComponent->GetID().GetString() : "" );
	
	info.SetDisplayImage( pDisplayImage );
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
}
