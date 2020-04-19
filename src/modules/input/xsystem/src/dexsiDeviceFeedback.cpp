/* 
 * Drag[en]gine X System Input Module
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

#include <stdlib.h>

#include "dexsiDeviceFeedback.h"
#include "deXSystemInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/image/deImageManager.h>



// Class dexsiDeviceFeedback
//////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiDeviceFeedback::dexsiDeviceFeedback( deXSystemInput &module ) :
pModule( module ),

pIndex( -1 ),
pType( deInputDeviceFeedback::eftGeneric ),
pMaximum( 1 ),
pValue( 0.0f ),
pEvdevCode( 0 ){
}

dexsiDeviceFeedback::~dexsiDeviceFeedback(){
}



// Management
///////////////

void dexsiDeviceFeedback::SetIndex( int index ){
	pIndex = index;
}

void dexsiDeviceFeedback::SetID( const char *id ){
	pID = id;
}

void dexsiDeviceFeedback::SetName( const char *name ){
	pName = name;
}

void dexsiDeviceFeedback::SetType( deInputDeviceFeedback::eFeedbackTypes type ){
	pType = type;
}



void dexsiDeviceFeedback::SetDisplayImages( const char *name ){
	pDisplayImage = NULL;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pModule.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pModule.GetVFS();
	const char * const basePath = "/share/image/feedback";
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

void dexsiDeviceFeedback::SetDisplayText( const char *text ){
	pDisplayText = text;
}



void dexsiDeviceFeedback::SetMaximum( int maximum ){
	pMaximum = maximum;
}



void dexsiDeviceFeedback::SetValue( float value ){
	pValue = value;
}



void dexsiDeviceFeedback::SetEvdevCode( int code ){
	pEvdevCode = code;
}



void dexsiDeviceFeedback::GetInfo( deInputDeviceFeedback &info ) const{
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
