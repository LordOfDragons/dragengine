/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoalDecodeBuffer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deSoundDecoder.h>



// Class deoalDecodeBuffer
////////////////////////////

// Constructor, destructor
////////////////////////////

deoalDecodeBuffer::deoalDecodeBuffer( int size ) :
pBuffer( NULL ),
pSize( 0 )
{
	SetSize( size );
}

deoalDecodeBuffer::~deoalDecodeBuffer(){
	if( pBuffer ){
		delete [] pBuffer;
	}
}



// Management
///////////////

void deoalDecodeBuffer::SetSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( size == pSize ){
		return;
	}
	
	char * const buffer = new char[ size ];
	if( pBuffer ){
		delete [] pBuffer;
	}
	pBuffer = buffer;
	pSize = size;
}

int deoalDecodeBuffer::Decode( deSoundDecoder &decoder, int size ){
	if( size > pSize ){
		SetSize( size );
	}
	
	const int bytesRead = decoder.ReadSamples( pBuffer, size );
	const int remaining = size - bytesRead;
	if( remaining > 0 ){
		memset( pBuffer + bytesRead, '\0', remaining );
	}
	
	return bytesRead;
}

int deoalDecodeBuffer::DecodeLooping( deSoundDecoder &decoder, int size ){
	int totalBytesRead = 0;
	int remaining = size;
	int position = 0;
	
	if( size > pSize ){
		SetSize( size );
	}
	
	while( true ){
		const int bytesRead = decoder.ReadSamples( pBuffer + position, remaining );
		
		// bytesRead == 0 means EOF.
		
		totalBytesRead += bytesRead;
		
		remaining -= bytesRead;
		if( remaining == 0 ){
			break;
		}
		
		position += bytesRead;
		
		decoder.SetPosition( 0 ); // rewind
	}
	
	return totalBytesRead;
}
