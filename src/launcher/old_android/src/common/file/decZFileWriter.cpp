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

decZFileWriter::decZFileWriter(decBaseFileWriter *writer) :
pWriter(NULL),

pZStream(NULL),

pBufferIn(NULL),
pBufferInSize(0),
pBufferInPosition(0),

pBufferOut(NULL),
pBufferOutSize(0)
{
	if(! writer){
		DETHROW(deeInvalidParam);
	}
	
	writer->WriteByte(0); // options in case we want to expand on functionality internally
	
	pBufferIn = NULL;
	pBufferInPosition = 0;
	pBufferInSize = 0;
	pBufferOut = NULL;
	pBufferOutSize = 0;
	
	z_stream * const zstream = new z_stream;
	memset(zstream, 0, sizeof(z_stream));
	zstream->zalloc = NULL;
	zstream->zfree = NULL;
	zstream->opaque = NULL;
	//if( deflateInit( zstream, Z_BEST_SPEED ) != Z_OK ){
	//if( deflateInit( zstream, Z_BEST_COMPRESSION ) != Z_OK ){
	if(deflateInit(zstream, Z_DEFAULT_COMPRESSION) != Z_OK){
		delete zstream;
		DETHROW(deeOutOfMemory);
	}
	
	pBufferIn = new Bytef[BUFFER_SIZE];
	pBufferInSize = BUFFER_SIZE;
	pBufferOut = new Bytef[BUFFER_SIZE];
	pBufferOutSize = BUFFER_SIZE;
	
	zstream->next_in = (Bytef*)pBufferIn;
	zstream->avail_in = 0;
	zstream->next_out = (Bytef*)pBufferOut;
	zstream->avail_out = pBufferOutSize;
	pZStream = zstream;
	
	pWriter = writer;
	pWriter->AddReference();
}

decZFileWriter::~decZFileWriter(){
	EndZWriting();
	
	if(pZStream){
		delete (z_stream*)pZStream;
	}
	
	if(pWriter){
		pWriter->FreeReference();
	}
	if(pBufferOut){
		delete [] (Bytef*)pBufferOut;
	}
	if(pBufferIn){
		delete [] (Bytef*)pBufferIn;
	}
}



// Seeking
////////////

int decZFileWriter::GetPosition(){
	return pWriter->GetPosition();
}

void decZFileWriter::SetPosition(int position){
	DETHROW(deeInvalidAction);
}

void decZFileWriter::MovePosition(int offset){
	DETHROW(deeInvalidAction);
}

void decZFileWriter::SetPositionEnd(int position){
	DETHROW(deeInvalidAction);
}



// Management
///////////////

const char *decZFileWriter::GetFilename(){
	return pWriter->GetFilename();
}

void decZFileWriter::Write(const void *buffer, int size){
	z_stream * const zstream = (z_stream*)pZStream;
	const char *cbuffer = (const char*)buffer;
	
	while(size > 0){
		if(pBufferInPosition + size > pBufferInSize){
			const int writeSize = pBufferInSize - pBufferInPosition;
			memcpy((Bytef*)pBufferIn + pBufferInPosition, cbuffer, writeSize);
			
			zstream->next_in = (Bytef*)pBufferIn;
			zstream->avail_in = pBufferInSize;
			
			while(zstream->avail_in > 0){
				if(deflate(zstream, Z_NO_FLUSH) != Z_OK){
					DETHROW(deeInvalidParam);
				}
				
				if(zstream->avail_out == 0){
					pWriter->Write(pBufferOut, pBufferOutSize);
					zstream->next_out = (Bytef*)pBufferOut;
					zstream->avail_out = pBufferOutSize;
				}
			}
			
			pBufferInPosition = 0;
			cbuffer += writeSize;
			size -= writeSize;
			
		}else{
			memcpy((Bytef*)pBufferIn + pBufferInPosition, cbuffer, size);
			pBufferInPosition += size;
			size = 0;
		}
	}
}



void decZFileWriter::EndZWriting(){
	z_stream * const zstream = (z_stream*)pZStream;
	zstream->next_in = (Bytef*)pBufferIn;
	zstream->avail_in = pBufferInPosition;
	
	while(true){
		const int result = deflate(zstream, Z_FINISH);
		
		if(result == Z_OK){
			pWriter->Write(pBufferOut, pBufferOutSize);
			zstream->next_out = (Bytef*)pBufferOut;
			zstream->avail_out = pBufferOutSize;
			
		}else if(result == Z_STREAM_END){
			break;
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
	
	pBufferInPosition = 0;
	
	const int remainingSize = pBufferOutSize - (int)zstream->avail_out;
	if(remainingSize > 0){
		pWriter->Write(pBufferOut, remainingSize);
		zstream->next_out = (Bytef*)pBufferOut;
		zstream->avail_out = pBufferOutSize;
	}
	
	if(deflateEnd(zstream) != Z_OK){
		DETHROW(deeInvalidParam);
	}
}
