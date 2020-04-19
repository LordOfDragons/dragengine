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

static void pCopyPixelData( deImage &image, FXColor *foxData );

// Constructor, destructor
////////////////////////////

igdeIcon::igdeIcon( deImage &image ) :
pNativeIcon( NULL ),
pSize( image.GetWidth(), image.GetHeight() )
{
	pCreateFromImage( image );
}

igdeIcon::igdeIcon( deImage &image, int width, int height ) :
pNativeIcon( NULL ),
pSize( width, height )
{
	FXIcon * const foxIcon = ( FXIcon* )pCreateFromImage( image );
	if( width != foxIcon->getWidth() || height != foxIcon->getHeight() ){
		foxIcon->scale( width, height, 1 );
	}
	pNativeIcon = foxIcon;
}

igdeIcon *igdeIcon::LoadPNG( igdeEnvironment &environment, const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileReaderReference reader;
	reader.TakeOver( environment.GetFileSystemIGDE()->OpenFileForReading(
		decPath::CreatePathUnix( filename ) ) );
	FXIcon * const icon = ( FXIcon* )pCreateFromPNG( reader );
	
	try{
		return new igdeIcon( icon, decPoint( icon->getWidth(), icon->getHeight() ) );
		
	}catch( const deException & ){
		delete icon;
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
	FXIcon * const icon = ( FXIcon* )pCreateFromPNG( reader );
	
	try{
		return new igdeIcon( icon, decPoint( icon->getWidth(), icon->getHeight() ) );
		
	}catch( const deException & ){
		delete icon;
		throw;
	}
}

igdeIcon *igdeIcon::LoadImage( igdeEnvironment &environment, const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	deImageReference image;
	image.TakeOver( environment.GetEngineController()->GetEngine()
		->GetImageManager()->LoadImage( filename, "/" ) );
	FXIcon * const icon = ( FXIcon* )pCreateFromImage( image );
	
	try{
		return new igdeIcon( icon, decPoint( icon->getWidth(), icon->getHeight() ) );
		
	}catch( const deException & ){
		delete icon;
		throw;
	}
}

igdeIcon::igdeIcon( void *nativeIcon, const decPoint &size ) : pNativeIcon( nativeIcon ), pSize( size ){
}

igdeIcon::~igdeIcon(){
	if( pNativeIcon ){
		delete ( FXIcon* )pNativeIcon;
	}
}


// Management
///////////////

igdeIcon *igdeIcon::Scale( const decPoint &size ) const{
	if( ! ( size >= decPoint( 1, 1 ) ) ){
		DETHROW( deeInvalidParam );
	}
	
	const FXIcon * const sourceIcon = ( FXIcon* )GetNativeIcon();
	if( ! sourceIcon ){
		DETHROW( deeInvalidParam );
	}
	
	FXColor *imageData = NULL;
	if( ! FXCALLOC( &imageData, FXColor, pSize.x * pSize.y ) ){
		DETHROW( deeOutOfMemory );
	}
	memcpy( imageData, sourceIcon->getData(), sizeof( FXColor ) * ( pSize.x * pSize.y ) );
	
	FXIcon * const icon = new FXIcon( FXApp::instance(), imageData, 0,
		IMAGE_OWNED | IMAGE_KEEP, pSize.x, pSize.y );
	if( size != pSize ){
		icon->scale( size.x, size.y, 1 );
	}
	
	return new igdeIcon( icon, size );
}

void igdeIcon::Update( deImage &image ){
	if( ! pNativeIcon ){
		return;
	}
	
	FXIcon &foxIcon = *( ( FXIcon* )pNativeIcon );
	
	if( image.GetWidth() != pSize.x || image.GetHeight() != pSize.y || image.GetBitCount() != 8 ){
		DETHROW( deeInvalidParam );
	}
	
	pCopyPixelData( image, foxIcon.getData() ); // getData() is valid because of IMAGE_KEEP
	
	foxIcon.render();
}



// Private Functions
//////////////////////

void *igdeIcon::pCreateFromImage( deImage &image ){
	if( image.GetBitCount() != 8 ){
		DETHROW( deeInvalidParam );
	}
	
	const int height = image.GetHeight();
	const int width = image.GetWidth();
	FXColor *foxImageData = NULL;
	FXIcon *foxIcon = NULL;
	
	try{
		// we have to create the pixel buffer the image is going to take ownership of.
		// we have to use fox allocation routines for this to work correctly
		if( ! FXCALLOC( &foxImageData, FXColor, width * height ) ){
			DETHROW( deeOutOfMemory );
		}
		
		// copy pixel data from image to the allocated fox pixel buffer
		pCopyPixelData( image, foxImageData );
		
		// create icon. using pixelData=NULL the constructor creates the pixel data.
		// see the table in the comment below for the possible combinations of
		// parameters and the resulting behavior.
		// 
		// using IMAGE_SHMI and IMAGE_SHMP is only recommended for large images.
		// this call produces mostly small images so no need for this
		foxIcon = new FXIcon( FXApp::instance(), foxImageData, FXRGB( 255, 255, 0 ),
			IMAGE_OWNED | IMAGE_KEEP, width, height );
		
		// up to this point only the pixel data exists. calling create() creates the server
		// side data actually used for rendering later on. because IMAGE_KEEP is not used
		// in the constructor the pixel data is discarded after the server side resources
		// have been created. because IMAGE_OWNED is not used in the constructor the pixel
		// data is deleted.
		// 
		// the diffrent parameter used in the constructor affect the behavior like this:
		// | pixelBuffer | IMAGE_OWNED | IMAGE_KEEP | constructor |  after create   | destructor |
		// | ----------- | ----------- | ---------- | ----------- | --------------- | ---------- |
		// |     NULL    |     false   |    false   |   create    | delete set null |   nothing  |
		// |     NULL    |     true    |    false   |   create    | delete set null |   nothing  |
		// |     NULL    |     false   |    true    |   create    |     nothing     |   delete   |
		// |     NULL    |     true    |    true    |   create    |     nothing     |   delete   |
		// |   provided  |     false   |    false   |   nothing   |     set null    |   nothing  |
		// |   provided  |     true    |    false   |   nothing   | delete set null |   nothing  |
		// |   provided  |     false   |    true    |   nothing   |     nothing     |   nothing  |
		// |   provided  |     true    |    true    |   nothing   |     nothing     |   delete   |
		// 
		// we use pixelBuffer=NULL, IMAGE_OWNED=true and IMAGE_KEEP=false. this way the pixel
		// buffer is created by the icon resource and deleted after creating the server side
		// resources.
		// 
		// if we plan to modify the image we need to use pixelBuffer=NULL, IMAGE_OWNED=true
		// and IMAGE_KEEP=false. then we can call render again after changing the content
		foxIcon->create();
		
	}catch( const deException & ){
		if( foxIcon ){
			delete foxIcon;
		}
		if( foxImageData ){
			FXFREE( &foxImageData );
		}
		throw;
	}
	
	return foxIcon;
}

void *igdeIcon::pCreateFromPNG( decBaseFileReader &reader ){
	FXPNGIcon *foxIcon = NULL;
	char *imageData = NULL;
	FXMemoryStream stream;
	int imageSize = 0;
	
	try{
		imageSize = reader.GetLength();
		if( imageSize == 0 ){
			DETHROW_INFO( deeInvalidFileFormat, reader.GetFilename() );
		}
		
		imageData = new char[ imageSize ];
		reader.Read( imageData, imageSize );
		
		foxIcon = new FXPNGIcon( FXApp::instance(), 0, FXRGB(192,192,192), IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP );
		#ifdef FOX_OLD_MEMORY_STREAM
		if( ! stream.open( FX::FXStreamLoad, ( FXuval )imageSize, ( FXuchar* )imageData ) ){
		#else
		if( ! stream.open( FX::FXStreamLoad, ( FXuchar* )imageData, ( FXuval )imageSize, false ) ){
		#endif
			DETHROW( deeInvalidParam );
		}
		if( ! foxIcon->loadPixels( stream ) ){
			DETHROW_INFO( deeInvalidFileFormat, reader.GetFilename() );
		}
		
		delete [] imageData;
		imageData = NULL;
		
		foxIcon->create();
		
	}catch( const deException & ){
		if( foxIcon ){
			delete foxIcon;
		}
		if( imageData ){
			delete [] imageData;
		}
		throw;
	}
	
	return foxIcon;
}

void pCopyPixelData( deImage &image, FXColor *foxData ){
	const int height = image.GetHeight();
	const int width = image.GetWidth();
	const int pixelCount = width * height;
	int i;
	
	if( image.GetComponentCount() == 1 ){
		image.RetainImageData();
		const sGrayscale8 * const dataIn = image.GetDataGrayscale8();
		for( i=0; i<pixelCount; i++ ){
			foxData[ i ] = FXRGBA( dataIn[ i ].value, dataIn[ i ].value, dataIn[ i ].value, 255 );
		}
		image.ReleaseImageData();
		
	}else if( image.GetComponentCount() == 3 ){
		image.RetainImageData();
		const sRGB8 * const dataIn = image.GetDataRGB8();
		for( i=0; i<pixelCount; i++ ){
			foxData[ i ] = FXRGBA( dataIn[ i ].red, dataIn[ i ].green, dataIn[ i ].blue, 255 );
		}
		image.ReleaseImageData();
		
	}else if( image.GetComponentCount() == 4 ){
		image.RetainImageData();
		const sRGBA8 * const dataIn = image.GetDataRGBA8();
		for( i=0; i<pixelCount; i++ ){
			foxData[ i ] = FXRGBA( dataIn[ i ].red, dataIn[ i ].green, dataIn[ i ].blue, dataIn[ i ].alpha );
		}
		image.ReleaseImageData();
		
	}else{
		DETHROW( deeInvalidParam );
	}
}
