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

#include "igdeNativeFoxIcon.h"
#include "../../../resources/igdeIcon.h"
#include "../../../../engine/igdeEngineController.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/image/deImageManager.h>



// Class igdeNativeFoxIcon
////////////////////////////

// Constructor, destructor
////////////////////////////

void *igdeNativeFoxIcon::CreateNativeIcon( deImage &image ){
	if( image.GetBitCount() != 8 ){
		DETHROW( deeInvalidParam );
	}
	
	const int height = image.GetHeight();
	const int width = image.GetWidth();
	FXColor *nativeImageData = NULL;
	FXIcon *nativeIcon = NULL;
	
	try{
		// we have to create the pixel buffer the image is going to take ownership of.
		// we have to use fox allocation routines for this to work correctly
		if( ! FXCALLOC( &nativeImageData, FXColor, width * height ) ){
			DETHROW( deeOutOfMemory );
		}
		
		// copy pixel data from image to the allocated fox pixel buffer
		CopyPixelData( image, nativeImageData );
		
		// create icon. using pixelData=NULL the constructor creates the pixel data.
		// see the table in the comment below for the possible combinations of
		// parameters and the resulting behavior.
		// 
		// using IMAGE_SHMI and IMAGE_SHMP is only recommended for large images.
		// this call produces mostly small images so no need for this
		nativeIcon = new FXIcon( FXApp::instance(), nativeImageData,
			FXRGB( 255, 255, 0 ), IMAGE_OWNED | IMAGE_KEEP, width, height );
		
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
		nativeIcon->create();
		
	}catch( const deException & ){
		if( nativeIcon ){
			delete nativeIcon;
		}
		if( nativeImageData ){
			FXFREE( &nativeImageData );
		}
		throw;
	}
	
	return nativeIcon;
}

void *igdeNativeFoxIcon::CreateNativeIconPNG( decBaseFileReader &reader ){
	FXPNGIcon *nativeIcon = NULL;
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
		
		nativeIcon = new FXPNGIcon( FXApp::instance(), 0, FXRGB(192,192,192), IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP );
		#ifdef FOX_OLD_MEMORY_STREAM
		if( ! stream.open( FX::FXStreamLoad, ( FXuval )imageSize, ( FXuchar* )imageData ) ){
		#else
		if( ! stream.open( FX::FXStreamLoad, ( FXuchar* )imageData, ( FXuval )imageSize, false ) ){
		#endif
			DETHROW( deeInvalidParam );
		}
		if( ! nativeIcon->loadPixels( stream ) ){
			DETHROW_INFO( deeInvalidFileFormat, reader.GetFilename() );
		}
		
		delete [] imageData;
		imageData = NULL;
		
		nativeIcon->create();
		
	}catch( const deException & ){
		if( nativeIcon ){
			delete nativeIcon;
		}
		if( imageData ){
			delete [] imageData;
		}
		throw;
	}
	
	return nativeIcon;
}

void *igdeNativeFoxIcon::DuplicateNativeIcon( void *native ){
	const FXIcon * const sourceIcon = ( FXIcon* )native;
	const int height = sourceIcon->getHeight();
	const int width = sourceIcon->getWidth();
	const int pixelCount = width * height;
	
	FXColor *imageData = NULL;
	if( ! FXCALLOC( &imageData, FXColor, pixelCount ) ){
		DETHROW( deeOutOfMemory );
	}
	memcpy( imageData, sourceIcon->getData(), sizeof( FXColor ) * pixelCount );
	
	return new FXIcon( FXApp::instance(), imageData, 0, IMAGE_OWNED | IMAGE_KEEP, width, height );
}

void igdeNativeFoxIcon::DestroyNativeIcon( void *native ){
	delete ( FXIcon* )native;
}



// Management
///////////////

decPoint igdeNativeFoxIcon::GetSize( void *native ){
	const FXIcon &icon = *( ( FXIcon* )native );
	return decPoint( icon.getWidth(), icon.getHeight() );
}

void igdeNativeFoxIcon::Scale( const decPoint &size, void *native ){
	FXIcon &icon = *( ( FXIcon* )native );
	const decPoint iconSize( icon.getWidth(), icon.getHeight() );
	if( ! ( iconSize == size ) ){
		icon.scale( size.x, size.y, 1 );
	}
}

void igdeNativeFoxIcon::UpdatePixels( void *native, deImage &image ){
	FXIcon &icon = *( ( FXIcon* )native );
	const decPoint iconSize( icon.getWidth(), icon.getHeight() );
	
	if( image.GetWidth() != iconSize.x || image.GetHeight() != iconSize.y || image.GetBitCount() != 8 ){
		DETHROW( deeInvalidParam );
	}
	
	CopyPixelData( image, icon.getData() ); // getData() is valid because of IMAGE_KEEP
	
	icon.render();
}



void igdeNativeFoxIcon::CopyPixelData( deImage &image, FXColor *foxData ){
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
