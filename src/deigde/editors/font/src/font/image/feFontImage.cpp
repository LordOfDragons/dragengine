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
#include <stdlib.h>
#include <string.h>

#include "feFontImage.h"
#include "../feFont.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Font Editor"



// Class feFontImage
//////////////////////

// Constructor, destructor
////////////////////////////

feFontImage::feFontImage( deEngine *engine ){
	if( ! engine ) DETHROW( deeInvalidParam );
	
	pEngine = engine;
	pEngImage = NULL;
	pParentFont = NULL;
	
	pWidth = 256;
	pHeight = 256;
	pFilename = "image.png";
	
	pChanged = false;
	pSaved = false;
	
	try{
		pEngImage = pEngine->GetImageManager()->CreateImage( pWidth, pHeight, 1, 4, 8 );
		
		sRGBA8 * const pixels = pEngImage->GetDataRGBA8();
		const int pixelCount = pWidth * pHeight;
		int i;
		for( i=0; i<pixelCount; i++ ){
			pixels[ i ].red = 0;
			pixels[ i ].green = 0;
			pixels[ i ].blue = 0;
			pixels[ i ].alpha = 255;
		}
		
		pEngImage->NotifyImageDataChanged();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}


feFontImage::~feFontImage(){
	pCleanUp();
}



// Management
///////////////

void feFontImage::SetParentFont( feFont* font ){
	pParentFont = font;
}



void feFontImage::SetSize( int width, int height ){
	if( width < 1 || height < 1 ) DETHROW( deeInvalidParam );
	
	deImage *engImage = NULL;
	
	pWidth = width;
	pHeight = height;
	
	try{
		engImage = pEngine->GetImageManager()->CreateImage( pWidth, pHeight, 1, 4, 8 );
		
		if( pEngImage ){
			pEngImage->FreeReference();
		}
		
		pEngImage = engImage;
		engImage->RetainImageData();
		
	}catch( const deException & ){
		if( engImage ) engImage->FreeReference();
		throw;
	}
	
	SetChanged( true );
}

void feFontImage::SetFilename( const char* filename, bool load ){
	if( ! filename ) DETHROW( deeInvalidParam );
	
	if( ! pFilename.Equals( filename ) ){
		pFilename = filename;
		
		if( load ){
			LoadImage();
			
		}else{
			SetSaved( true );
			SetChanged( true );
		}
	}
}

void feFontImage::SetChanged( bool changed ){
	pChanged = changed;
	NotifyImageChanged();
}

void feFontImage::SetSaved( bool saved ){
	pSaved = saved;
}

void feFontImage::LoadImage(){
	if( pEngImage ){
		pEngImage->FreeReference();
		pEngImage = NULL;
	}
	
	if( ! pFilename.IsEmpty() ){
		const char *basePath = "/";
		
		if( pParentFont ){
			basePath = pParentFont->GetDirectoryPath();
		}
		
		if( pParentFont ){
			pParentFont->GetLogger()->LogInfoFormat( LOGSOURCE, "FontImage.LoadImage: basePath='%s' path='%s'", basePath, pFilename.GetString() );
		}
		
		try{
			pEngImage = pEngine->GetImageManager()->LoadImage( pFilename.GetString(), basePath );
			pEngImage->RetainImageData();
			
		}catch( const deException &e ){
			if( pParentFont ){
				pParentFont->GetLogger()->LogException( LOGSOURCE, e );
			}
		}
	}
	
	if( pEngImage ){
		pWidth = pEngImage->GetWidth();
		pHeight = pEngImage->GetHeight();
		
		SetSaved( true );
		SetChanged( false );
		
	}else{
		SetSaved( false );
		SetChanged( true );
	}
}



void feFontImage::NotifyImageChanged(){
	if( pParentFont ){
		pParentFont->SetChanged( true );
		pParentFont->NotifyImageChanged( this );
	}
}



// Private Functions
//////////////////////

void feFontImage::pCleanUp(){
	if( pEngImage ){
		pEngImage->FreeReference();
	}
}
