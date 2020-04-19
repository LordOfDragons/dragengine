/* 
 * Drag[en]gine Synthesizer Module
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

#include "desynDecodeBuffer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deSoundDecoder.h>



// Class desynDecodeBuffer
////////////////////////////

// Constructor, destructor
////////////////////////////

desynDecodeBuffer::desynDecodeBuffer( int size ) :
pBuffer( NULL ),
pSize( 0 )
{
	SetSize( size );
}

desynDecodeBuffer::~desynDecodeBuffer(){
	if( pBuffer ){
		delete [] pBuffer;
	}
}



// Management
///////////////

void desynDecodeBuffer::SetSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( size == pSize ){
		return;
	}
	
	char *buffer = new char[ size ];
	
	if( pBuffer ){
		delete [] pBuffer;
	}
	pBuffer = buffer;
	pSize = size;
}

int desynDecodeBuffer::Decode( deSoundDecoder &decoder, int size ){
	int bytesRead, remaining;
	
	if( size > pSize ){
		SetSize( size );
	}
	
	bytesRead = decoder.ReadSamples( pBuffer, size );
	
	remaining = size - bytesRead;
	if( remaining > 0 ){
		memset( pBuffer + bytesRead, '\0', remaining );
	}
	
	return bytesRead;
}

int desynDecodeBuffer::DecodeLooping( deSoundDecoder &decoder, int size ){
	int totalBytesRead = 0;
	int remaining = size;
	int bytesRead = 0;
	int position = 0;
	
	if( size > pSize ){
		SetSize( size );
	}
	
	while( true ){
		bytesRead = decoder.ReadSamples( pBuffer + position, remaining );
		if( bytesRead == 0 ) break; // EOF
		totalBytesRead += bytesRead;
		
		remaining -= bytesRead;
		if( remaining == 0 ) break;
		
		position += bytesRead;
		decoder.SetPosition( 0 );
	}
	
	return totalBytesRead;
}
