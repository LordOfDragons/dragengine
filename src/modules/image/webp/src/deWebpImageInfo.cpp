/* 
 * Drag[en]gine Webp Image Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <webp/decode.h>

#include "deWebpImageInfo.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>


// Class deWebpImageInfo
//////////////////////////

// Constructor, destructor
////////////////////////////

deWebpImageInfo::deWebpImageInfo( decBaseFileReader &reader ) :
pFilename( reader.GetFilename() ),
pWidth( 0 ),
pHeight( 0 ),
pHasAlpha( false )
{
	const int size = reader.GetLength();
	pData.TakeOver( new decMemoryFile( "data" ) );
	pData->Resize( size );
	reader.Read( pData->GetPointer(), size );
	
	WebPBitstreamFeatures features;
	Assert( WebPGetFeatures( ( uint8_t* )pData->GetPointer(), size, &features ) );
	
	pWidth = features.width;
	pHeight = features.height;
	pHasAlpha = features.has_alpha;
}

deWebpImageInfo::~deWebpImageInfo(){
}



// Management
///////////////

int deWebpImageInfo::GetWidth(){
	return pWidth;
}

int deWebpImageInfo::GetHeight(){
	return pHeight;
}

int deWebpImageInfo::GetDepth(){
	return 1;
}

int deWebpImageInfo::GetComponentCount(){
	return pHasAlpha ? 4 : 3;
}

int deWebpImageInfo::GetBitCount(){
	return 8;
}

void deWebpImageInfo::Assert( VP8StatusCode statusCode ) const{
	switch( statusCode ){
	case VP8_STATUS_OK:
		return;
		
	case VP8_STATUS_OUT_OF_MEMORY:
		DETHROW( deeOutOfMemory );
		
	case VP8_STATUS_INVALID_PARAM:
		DETHROW( deeInvalidParam );
		
	case VP8_STATUS_BITSTREAM_ERROR:
		DETHROW_INFO( deeInvalidFileFormat, "Bit stream error" );
		
	case VP8_STATUS_UNSUPPORTED_FEATURE:
		DETHROW_INFO( deeInvalidFileFormat, "Unsupported feature" );
		
	case VP8_STATUS_SUSPENDED:
		DETHROW_INFO( deeInvalidFileFormat, "Suspended" );
		
	case VP8_STATUS_USER_ABORT:
		DETHROW_INFO( deeInvalidFileFormat, "User abort" );
		
	case VP8_STATUS_NOT_ENOUGH_DATA:
		DETHROW_INFO( deeInvalidFileFormat, "Not enough data" );
		
	default:
		DETHROW( deeInvalidFileFormat );
	}
}
