/* 
 * Drag[en]gine IGDE Font Editor
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
#include <string.h>

#include "feUFontImportImage.h"
#include "../../font/feFont.h"
#include "../../font/image/feFontImage.h"

#include <dragengine/resources/image/deImage.h>
#include <dragengine/common/exceptions.h>



// Class feUFontImportImage
/////////////////////////////

// Constructor, destructor
////////////////////////////

feUFontImportImage::feUFontImportImage( feFontImage *fontImage, deImage *newImage, const char *newPath ){
	if( ! newImage || ! newPath ) DETHROW( deeInvalidParam );
	
	if( newImage->GetComponentCount() != 4 ) DETHROW( deeInvalidParam );
	
	deImage &oldImage = *fontImage->GetEngineImage();
	
	pImage = NULL;
	
	pOldWidth = oldImage.GetWidth();
	pOldHeight = oldImage.GetHeight();
	pOldColor = NULL;
	
	pNewWidth = newImage->GetWidth();
	pNewHeight = newImage->GetHeight();
	pNewColor = NULL;
	
	pOldPath = fontImage->GetFilename();
	pNewPath = newPath;
	
	SetShortInfo( "Font import image" );
	SetMemoryConsumption( sizeof( feUFontImportImage ) + sizeof( decColor ) * ( pOldWidth * pOldHeight + pNewWidth * pNewHeight ) );
	
	newImage->RetainImageData();
	
	try{
		pOldColor = new decColor[ pOldWidth * pOldHeight ];
		if( ! pOldColor ) DETHROW( deeOutOfMemory );
		pStoreColors( oldImage, pOldColor );
		
		pNewColor = new decColor[ pNewWidth * pNewHeight ];
		if( ! pNewColor ) DETHROW( deeOutOfMemory );
		pStoreColors( *newImage, pNewColor );
		
	}catch( const deException & ){
		newImage->ReleaseImageData();
		pCleanUp();
		throw;
	}
	
	pImage = fontImage;
	fontImage->AddReference();
}

feUFontImportImage::~feUFontImportImage(){
	pCleanUp();
}



// Management
///////////////

void feUFontImportImage::Undo(){
	pImage->SetSize( pOldWidth, pOldHeight );
	pImage->SetFilename( pOldPath.GetString(), false );
	pRestoreColors( *pImage->GetEngineImage(), pOldColor );
}

void feUFontImportImage::Redo(){
	pImage->SetSize( pNewWidth, pNewHeight );
	pImage->SetFilename( pNewPath.GetString(), false );
	pRestoreColors( *pImage->GetEngineImage(), pNewColor );
}



// Private
////////////

void feUFontImportImage::pCleanUp(){
	if( pNewColor ) delete [] pNewColor;
	if( pOldColor ) delete [] pOldColor;
	
	if( pImage ) pImage->FreeReference();
}

void feUFontImportImage::pStoreColors( deImage &image, decColor *colors ){
	int p, pixelCount = image.GetWidth() * image.GetHeight();
	int bitCount = image.GetBitCount();
	float factor;
	
	if( bitCount == 8 ){
		sRGBA8 *data = image.GetDataRGBA8();
		factor = 1.0f / 255.0f;
		
		for( p=0; p<pixelCount; p++ ){
			const sRGBA8 &pixel = data[ p ];
			
			colors[ p ].r = ( float )pixel.red * factor;
			colors[ p ].g = ( float )pixel.green * factor;
			colors[ p ].b = ( float )pixel.blue * factor;
			colors[ p ].a = ( float )pixel.alpha * factor;
		}
		
	}else if( bitCount == 16 ){
		sRGBA16 *data = image.GetDataRGBA16();
		factor = 1.0f / 65535.0f;
		
		for( p=0; p<pixelCount; p++ ){
			const sRGBA16 &pixel = data[ p ];
			
			colors[ p ].r = ( float )pixel.red * factor;
			colors[ p ].g = ( float )pixel.green * factor;
			colors[ p ].b = ( float )pixel.blue * factor;
			colors[ p ].a = ( float )pixel.alpha * factor;
		}
		
	}else{ // bitCount == 32
		memcpy( pNewColor, image.GetDataRGBA32(), sizeof( decColor ) * pixelCount );
	}
}

void feUFontImportImage::pRestoreColors( deImage &image, decColor *colors ){
	int p, pixelCount = image.GetWidth() * image.GetHeight();
	int bitCount = image.GetBitCount();
	float factor;
	
	if( bitCount == 8 ){
		sRGBA8 *data = image.GetDataRGBA8();
		factor = 255.0f;
		
		for( p=0; p<pixelCount; p++ ){
			sRGBA8 &pixel = data[ p ];
			
			pixel.red = ( unsigned char )( colors[ p ].r * factor );
			pixel.green = ( unsigned char )( colors[ p ].g * factor );
			pixel.blue = ( unsigned char )( colors[ p ].b * factor );
			pixel.alpha = ( unsigned char )( colors[ p ].a * factor );
		}
		
	}else if( bitCount == 16 ){
		sRGBA16 *data = image.GetDataRGBA16();
		factor = 65535.0f;
		
		for( p=0; p<pixelCount; p++ ){
			sRGBA16 &pixel = data[ p ];
			
			pixel.red = ( unsigned short )( colors[ p ].r * factor );
			pixel.green = ( unsigned short )( colors[ p ].g * factor );
			pixel.blue = ( unsigned short )( colors[ p ].b * factor );
			pixel.alpha = ( unsigned short )( colors[ p ].a * factor );
		}
		
	}else{ // bitCount == 32
		memcpy( image.GetDataRGBA32(), pNewColor, sizeof( decColor ) * pixelCount );
	}
	
	image.NotifyImageDataChanged();
}
