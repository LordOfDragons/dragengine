/* 
 * Drag[en]gine IGDE
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeIcon.h"
#include "../native/toolkit.h"
#include "../../engine/igdeEngineController.h"
#include "../../environment/igdeEnvironment.h"
#include "../../module/igdeEditorModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/image/deImageManager.h>



// Class igdeIcon
///////////////////

// Constructor, destructor
////////////////////////////

igdeIcon::igdeIcon( deImage &image ) :
pNativeIcon( NULL ),
pSize( image.GetWidth(), image.GetHeight() )
{
	pNativeIcon = igdeNativeIcon::CreateNativeIcon( image );
}

igdeIcon::igdeIcon( deImage &image, int width, int height ) :
pNativeIcon( NULL ),
pSize( width, height )
{
	pNativeIcon = igdeNativeIcon::CreateNativeIcon( image );
	igdeNativeIcon::Scale( pSize, pNativeIcon );
}

igdeIcon *igdeIcon::LoadPNG( igdeEnvironment &environment, const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileReaderReference reader;
	reader.TakeOver( environment.GetFileSystemIGDE()->
		OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
	
	void * const native = igdeNativeIcon::CreateNativeIconPNG( reader );
	
	try{
		return new igdeIcon( native, igdeNativeIcon::GetSize( native ) );
		
	}catch( const deException & ){
		igdeNativeIcon::DestroyNativeIcon( native );
		throw;
	}
}

igdeIcon *igdeIcon::LoadPNG( const igdeEditorModule &editor, const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decPath path;
	path.SetFromUnix( "/data/modules" );
	path.AddComponent( editor.GetEditorDirectory() );
	path.AddUnixPath( filename );
	
	decBaseFileReaderReference reader;
	reader.TakeOver( editor.GetEnvironment().GetFileSystemIGDE()->OpenFileForReading( path ) );
	void * const native = igdeNativeIcon::CreateNativeIconPNG( reader );
	
	try{
		return new igdeIcon( native, igdeNativeIcon::GetSize( native ) );
		
	}catch( const deException & ){
		igdeNativeIcon::DestroyNativeIcon( native );
		throw;
	}
}

igdeIcon *igdeIcon::LoadImage( igdeEnvironment &environment, const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	deImageReference image;
	image.TakeOver( environment.GetEngineController()->GetEngine()->
		GetImageManager()->LoadImage( filename, "/" ) );
	void * const native = igdeNativeIcon::CreateNativeIcon( image );
	
	try{
		return new igdeIcon( native, igdeNativeIcon::GetSize( native ) );
		
	}catch( const deException & ){
		igdeNativeIcon::DestroyNativeIcon( native );
		throw;
	}
}

igdeIcon::igdeIcon( void *nativeIcon, const decPoint &size ) :
pNativeIcon( nativeIcon ), pSize( size ){
}

igdeIcon::~igdeIcon(){
	if( pNativeIcon ){
		igdeNativeIcon::DestroyNativeIcon( pNativeIcon );
	}
}


// Management
///////////////

igdeIcon *igdeIcon::Scale( const decPoint &size ) const{
	if( ! ( size >= decPoint( 1, 1 ) ) || ! pNativeIcon ){
		DETHROW( deeInvalidParam );
	}
	
	void * const nativeScaled = igdeNativeIcon::DuplicateNativeIcon( pNativeIcon );
	igdeNativeIcon::Scale( size, nativeScaled );
	
	return new igdeIcon( nativeScaled, size );
}

void igdeIcon::Update( deImage &image ){
	if( pNativeIcon ){
		igdeNativeIcon::UpdatePixels( pNativeIcon, image );
	}
}
