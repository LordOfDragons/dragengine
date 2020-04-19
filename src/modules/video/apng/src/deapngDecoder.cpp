/* 
 * Drag[en]gine Animated PNG Video Module
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

#include "deapngDecoder.h"
#include "deapngReader.h"
#include "deVideoApng.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/resources/image/deImage.h>



// Class deapngDecoder
////////////////////////

// Constructor, destructor
////////////////////////////

deapngDecoder::deapngDecoder( deVideoApng &module, decBaseFileReader *file ) :
deBaseVideoDecoder( file ),
pModule( module ),
pReader( NULL )
{
	module.LogInfoFormat( "Create decoder for %s", file->GetFilename() );
	
	try{
		pReader = new deapngReader( module, file );
		
		module.LogInfoFormat( "width=%i height=%i pixelFormat=%i frameCount=%i frameRate=%i",
			pReader->GetWidth(), pReader->GetHeight(), pReader->GetPixelFormat(),
			pReader->GetFrameCount(), pReader->GetFrameRate() );
		
	}catch( const deException &e ){
		pCleanUp();
		e.PrintError();
		throw;
	}
}

deapngDecoder::~deapngDecoder(){
	pCleanUp();
	pModule.LogInfoFormat( "Free decoder for %s", GetFile()->GetFilename() );
}



// Management
///////////////

int deapngDecoder::GetPosition(){
	return pReader->GetCurrentFrame();
}

void deapngDecoder::SetPosition( int position ){
	pReader->SeekFrame( position );
}

bool deapngDecoder::DecodeFrame( void *buffer1, int size1, void *buffer2, int size2 ){
	if( pReader->GetCurrentFrame() >= pReader->GetFrameCount() ){
		return false; // trying to read past end of stream
	}
	
	pReader->ReadImage();
	pReader->CopyAccumImage( buffer1, size1 );
	return true;
}



// Private Functions
//////////////////////

void deapngDecoder::pCleanUp(){
	if( pReader ){
		delete pReader;
	}
}
