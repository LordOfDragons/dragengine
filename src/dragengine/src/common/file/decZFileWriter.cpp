/* 
 * Drag[en]gine Game Engine
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
#include <zlib.h>

#include "decZFileWriter.h"
#include "../exceptions.h"



// Definitions
////////////////

// Size in bytes of the in and out buffer to use
#define BUFFER_SIZE 4196



// Class decZFileWriter
/////////////////////////

// Constructor, Destructor
////////////////////////////

decZFileWriter::decZFileWriter( decBaseFileWriter *writer ) :
pPureMode( false ),

pZStream( NULL ),

pBufferIn( NULL ),
pBufferInSize( 0 ),
pBufferInPosition( 0 ),

pBufferOut( NULL ),
pBufferOutSize( 0 )
{
	if( ! writer ){
		DETHROW( deeInvalidParam );
	}
	pInit( writer, false );
}

decZFileWriter::decZFileWriter( decBaseFileWriter *writer, bool pureMode ) :
pPureMode( pureMode ),

pZStream( NULL ),

pBufferIn( NULL ),
pBufferInSize( 0 ),
pBufferInPosition( 0 ),

pBufferOut( NULL ),
pBufferOutSize( 0 )
{
	if( ! writer ){
		DETHROW( deeInvalidParam );
	}
	pInit( writer, pureMode );
}

decZFileWriter::~decZFileWriter(){
	EndZWriting();
	
	if( pZStream ){
		delete ( z_stream* )pZStream;
	}
	
	pWriter = nullptr;
	if( pBufferOut ){
		delete [] ( Bytef* )pBufferOut;
	}
	if( pBufferIn ){
		delete [] ( Bytef* )pBufferIn;
	}
}



// Seeking
////////////

int decZFileWriter::GetPosition(){
	return pWriter->GetPosition();
}

void decZFileWriter::SetPosition( int position ){
	DETHROW( deeInvalidAction );
}

void decZFileWriter::MovePosition( int offset ){
	DETHROW( deeInvalidAction );
}

void decZFileWriter::SetPositionEnd( int position ){
	DETHROW( deeInvalidAction );
}



// Management
///////////////

const char *decZFileWriter::GetFilename(){
	return pWriter->GetFilename();
}

void decZFileWriter::Write( const void *buffer, int size ){
	z_stream * const zstream = ( z_stream* )pZStream;
	const char *cbuffer = ( const char* )buffer;
	
	while( size > 0 ){
		if( pBufferInPosition + size > pBufferInSize ){
			const int writeSize = pBufferInSize - pBufferInPosition;
			memcpy( ( Bytef* )pBufferIn + pBufferInPosition, cbuffer, writeSize );
			
			zstream->next_in = ( Bytef* )pBufferIn;
			zstream->avail_in = pBufferInSize;
			
			while( zstream->avail_in > 0 ){
				if( deflate( zstream, Z_NO_FLUSH ) != Z_OK ){
					DETHROW( deeInvalidParam );
				}
				
				if( zstream->avail_out == 0 ){
					pWriter->Write( pBufferOut, pBufferOutSize );
					zstream->next_out = ( Bytef* )pBufferOut;
					zstream->avail_out = pBufferOutSize;
				}
			}
			
			pBufferInPosition = 0;
			cbuffer += writeSize;
			size -= writeSize;
			
		}else{
			memcpy( ( Bytef* )pBufferIn + pBufferInPosition, cbuffer, size );
			pBufferInPosition += size;
			size = 0;
		}
	}
}

decBaseFileWriter::Ref decZFileWriter::Duplicate(){
	return decBaseFileWriter::Ref::New( new decZFileWriter( pWriter, pPureMode ) );
}



void decZFileWriter::EndZWriting(){
	z_stream * const zstream = ( z_stream* )pZStream;
	zstream->next_in = ( Bytef* )pBufferIn;
	zstream->avail_in = pBufferInPosition;
	
	while( true ){
		const int result = deflate( zstream, Z_FINISH );
		
		if( result == Z_OK ){
			pWriter->Write( pBufferOut, pBufferOutSize );
			zstream->next_out = ( Bytef* )pBufferOut;
			zstream->avail_out = pBufferOutSize;
			
		}else if( result == Z_STREAM_END ){
			break;
			
		}else{
			DETHROW( deeInvalidParam );
		}
	}
	
	pBufferInPosition = 0;
	
	const int remainingSize = pBufferOutSize - ( int )zstream->avail_out;
	if( remainingSize > 0 ){
		pWriter->Write( pBufferOut, remainingSize );
		zstream->next_out = ( Bytef* )pBufferOut;
		zstream->avail_out = pBufferOutSize;
	}
	
	if( deflateEnd( zstream ) != Z_OK ){
		DETHROW( deeInvalidParam );
	}
}



// Private Functions
//////////////////////

void decZFileWriter::pInit( decBaseFileWriter *writer, bool pureMode ){
	if( ! pureMode ){
		writer->WriteByte( 0 ); // options in case we want to expand on functionality internally
	}
	
	pBufferIn = NULL;
	pBufferInPosition = 0;
	pBufferInSize = 0;
	pBufferOut = NULL;
	pBufferOutSize = 0;
	
	z_stream * const zstream = new z_stream;
	memset( zstream, 0, sizeof( z_stream ) );
	zstream->zalloc = NULL;
	zstream->zfree = NULL;
	zstream->opaque = NULL;
	//if( deflateInit( zstream, Z_BEST_SPEED ) != Z_OK ){
	//if( deflateInit( zstream, Z_BEST_COMPRESSION ) != Z_OK ){
	if( deflateInit( zstream, Z_DEFAULT_COMPRESSION ) != Z_OK ){
		delete zstream;
		DETHROW( deeOutOfMemory );
	}
	
	pBufferIn = new Bytef[ BUFFER_SIZE ];
	pBufferInSize = BUFFER_SIZE;
	pBufferOut = new Bytef[ BUFFER_SIZE ];
	pBufferOutSize = BUFFER_SIZE;
	
	zstream->next_in = ( Bytef* )pBufferIn;
	zstream->avail_in = 0;
	zstream->next_out = ( Bytef* )pBufferOut;
	zstream->avail_out = pBufferOutSize;
	pZStream = zstream;
	
	pWriter = writer;
}
