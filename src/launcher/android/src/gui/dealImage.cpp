/* 
 * Drag[en]gine Android Launcher
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

#include "dealImage.h"
#include "dealDisplay.h"
#include "../dealLauncher.h"
#include "../common/exceptions.h"
#include "../common/math/decMath.h"
#include "../common/file/decMemoryFile.h"
#include "../common/file/decMemoryFileReader.h"
#include "../common/utils/decTgaImage.h"
#include "../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealImage
////////////////////

// Constructors, destructors
//////////////////////////////

dealImage::dealImage( dealDisplay &display, const char *filename  ) :
pDisplay( display ),

pTexture( 0 ),
pWidth( 0 ),
pHeight( 0 )
{
	try{
		pLoadImage( filename );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dealImage::~dealImage(){
	pCleanUp();
}



// Management
///////////////



// Private functions
//////////////////////

void dealImage::pCleanUp(){
	if( pTexture != 0 ){
		glDeleteTextures( 1, &pTexture );
	}
}

void dealImage::pLoadImage( const char *filename ){
	decMemoryFile *memoryFileImage = NULL;
	decMemoryFileReader *tgaImageRader = NULL;
	decTgaImage *tgaImage = NULL;
	
	try{
		memoryFileImage = new decMemoryFile( filename );
		pDisplay.GetLauncher().LoadAsset( filename, *memoryFileImage );
		
		tgaImageRader = new decMemoryFileReader( memoryFileImage );
		
		tgaImage = new decTgaImage( *tgaImageRader );
		pCreateTexture( *tgaImage );
		
		delete tgaImage;
		tgaImageRader->FreeReference();
		memoryFileImage->FreeReference();
		
	}catch( const deException & ){
		if( tgaImage ){
			delete tgaImage;
		}
		if( tgaImageRader ){
			tgaImageRader->FreeReference();
		}
		if( memoryFileImage ){
			memoryFileImage->FreeReference();
		}
		throw;
	}
}

void dealImage::pCreateTexture( decTgaImage &tgaImage ){
	if( pTexture != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	glGenTextures( 1, &pTexture );
	if( pTexture == 0 ){
		DETHROW( deeInvalidAction );
	}
	
	pWidth = tgaImage.GetWidth();
	pHeight = tgaImage.GetHeight();
	
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, pTexture );
	
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pWidth, pHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tgaImage.GetPixels() );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	glBindTexture( GL_TEXTURE_2D, 0 );
}
