/* 
 * Drag[en]gine JPEG Image Module
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

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>

#include "deJpegImageInfos.h"



// Class deJpegImageInfo
//////////////////////////

// Constructor, destructor
////////////////////////////

deJpegImageInfo::deJpegImageInfo( deJpegModule *module, const char *filename ) :
pModule( module ),

pFilename( filename ),
pFileSize( 0 ),
pFilePosition( 0 ),

pDataBuffer( NULL ),
pDataBufferPosition( 0 ),
pReader( NULL )
{
	if( ! module || ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	memset( &pErrorMgr, '\0', sizeof( pErrorMgr ) );
	memset( &pSourceMgr, '\0', sizeof( pSourceMgr ) );
	memset( &pDecompress, '\0', sizeof( pDecompress ) );
	pDecompress.client_data = this;
	pDecompress.err = &pErrorMgr;
}

deJpegImageInfo::~deJpegImageInfo(){
	if( pDataBuffer ){
		delete [] pDataBuffer;
	}
}



// Management
///////////////



// Data Buffer
////////////////

void deJpegImageInfo::InitRead( decBaseFileReader *reader ){
	if( ! reader ){
		DETHROW( deeInvalidParam );
	}
	
	pReader = reader;
	
	pFileSize = reader->GetLength();
	pFilePosition = 0;
	
	if( ! pDataBuffer ){
		pDataBuffer = new JOCTET[ 1024 ];
	}
	pDataBufferPosition = 0;
	
	pSourceMgr.bytes_in_buffer = 0;
	pDecompress.src = &pSourceMgr;
}

void deJpegImageInfo::ReadNext(){
	if( ! pReader || ! pDataBuffer ){
		DETHROW( deeInvalidParam );
	}
	
	const int bytes = decMath::min( pFileSize - pFilePosition, 1024 );
	
	if( bytes > 0 ){
		pReader->Read( pDataBuffer, bytes );
		pFilePosition += bytes;
	}
	
	pDataBufferPosition = 0;
	
	pSourceMgr.next_input_byte = pDataBuffer;
	pSourceMgr.bytes_in_buffer = ( size_t )bytes;
}

void deJpegImageInfo::SkipNext( int bytes ){
	if( ! pReader || ! pDataBuffer ){
		DETHROW( deeInvalidParam );
	}
	
// 	printf( "Skip bytes=%i dbp=%i\n", bytes, pDataBufferPosition );
	
	/*
	pDataBufferPosition += bytes;
	
	if( pDataBufferPosition > 1024 ){
		pReader->MovePosition( pDataBufferPosition - 1024 );
		pDataBufferPosition = 1024;
	}
	
	pSourceMgr.next_input_byte = pDataBuffer + pDataBufferPosition;
	pSourceMgr.bytes_in_buffer = ( size_t )( 1024 - pDataBufferPosition );
	*/
	
	if( bytes <= 0 ){
		return;
	}
	
	while( bytes > ( int )pSourceMgr.bytes_in_buffer ){
		bytes -= pSourceMgr.bytes_in_buffer;
		ReadNext();
	}
	pSourceMgr.next_input_byte += bytes;
	pSourceMgr.bytes_in_buffer -= bytes;
}

void deJpegImageInfo::CloseReader(){
	if( pDataBuffer ){
		delete [] pDataBuffer;
		pDataBuffer = NULL;
	}
	pReader = NULL;
}



// Notifications
//////////////////

int deJpegImageInfo::GetWidth(){
	return ( int )pDecompress.image_width;
}

int deJpegImageInfo::GetHeight(){
	return ( int )pDecompress.image_height;
}

int deJpegImageInfo::GetDepth(){
	return 1;
}

int deJpegImageInfo::GetComponentCount(){
	return ( int )pDecompress.num_components;
}

int deJpegImageInfo::GetBitCount(){
	return 8;
}
