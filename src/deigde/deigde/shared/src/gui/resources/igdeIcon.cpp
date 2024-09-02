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
