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
#include <limits.h>
#include <sys/stat.h>
#include <zlib.h>

#include "decZFileReader.h"
#include "../exceptions.h"



// Definitions
////////////////

// Size in bytes of the in and out buffer to use
#define BUFFER_SIZE 4196



// Class decZFileReader
/////////////////////////

// Constructor, Destructor
////////////////////////////

decZFileReader::decZFileReader(decBaseFileReader *reader) :
pReader(NULL),
pFilePosition(0),
pFileLength(0),

pZStream(NULL),

pBufferIn(NULL),
pBufferInSize(0),
pBufferInPosition(0),

pContent(NULL),
pContentSize(0),
pContentCapacity(0),
pContentPosition(0)
{
	if(! reader){
		DETHROW(deeInvalidParam);
	}
	
	const int options = reader->ReadByte();
	(void)options;
	
	pBufferIn = NULL;
	pBufferInPosition = 0;
	pBufferInSize = 0;
	pContent = NULL;
	pContentSize = 0;
	pContentCapacity = 0;
	pContentPosition = 0;
	
	z_stream * const zstream = new z_stream;
	zstream->zalloc = NULL;
	zstream->zfree = NULL;
	zstream->opaque = NULL;
	if(inflateInit(zstream) != Z_OK){
		delete zstream;
		DETHROW(deeOutOfMemory);
	}
	
	pBufferIn = new Bytef[BUFFER_SIZE];
	pBufferInSize = BUFFER_SIZE;
	pContent = malloc(BUFFER_SIZE);
	pContentCapacity = BUFFER_SIZE;
	
	zstream->next_in = (Bytef*)pBufferIn;
	zstream->avail_in = 0;
	zstream->next_out = (Bytef*)pContent;
	zstream->avail_out = pContentCapacity;
	pZStream = zstream;
	
	pReader = reader;
	pReader->AddReference();
	pFileLength = reader->GetLength();
	pFilePosition = 1; // one option byte read
}

decZFileReader::~decZFileReader(){
	if(pZStream){
		inflateEnd((z_stream*)pZStream);
		delete (z_stream*)pZStream;
	}
	
	if(pReader){
		pReader->FreeReference();
	}
	if(pContent){
		free(pContent);
	}
	if(pBufferIn){
		delete [] (Bytef*)pBufferIn;
	}
}



// Management
///////////////

const char *decZFileReader::GetFilename(){
	return pReader->GetFilename();
}

int decZFileReader::GetLength(){
	pDecompressAll();
	return pContentSize;
}

TIME_SYSTEM decZFileReader::GetModificationTime(){
	return pReader->GetModificationTime();
}



// Seeking
////////////

int decZFileReader::GetPosition(){
	return pContentPosition;
}

void decZFileReader::SetPosition(int position){
	pSetContentPosition(position);
}

void decZFileReader::MovePosition(int offset){
	pSetContentPosition(pContentPosition + offset);
}

void decZFileReader::SetPositionEnd(int position){
	pDecompressAll();
	pSetContentPosition(pContentSize - position);
}



// Reading
////////////

void decZFileReader::Read(void *buffer, int size){
	if(! buffer || size < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(size > 0){
		const int contentPosition = pContentPosition;
		pSetContentPosition(pContentPosition + size); // make sure the content is present
		
		if(contentPosition + size > pContentSize){
			DETHROW(deeInvalidParam);
		}
		
		memcpy(buffer, (Bytef*)pContent + contentPosition, size);
	}
}



void decZFileReader::pSetContentPosition(int position){
	if(position < 0){
		pContentPosition = 0;
		return;
	}
	
	z_stream * const zstream = (z_stream*)pZStream;
	
	while(position > pContentSize){
		if(zstream->avail_in == 0){
			int readSize = pBufferInSize;
			
			if(pFilePosition + readSize > pFileLength){
				readSize = pFileLength - pFilePosition;
			}
			
			if(readSize > 0){
				pReader->Read((Bytef*)pBufferIn, readSize);
				pFilePosition += readSize;
			}
			
			zstream->next_in = (Bytef*)pBufferIn;
			zstream->avail_in = readSize;
		}
		
		const int result = inflate(zstream, Z_NO_FLUSH);
		
		if(result == Z_OK){
			pContentSize = pContentCapacity - (int)zstream->avail_out;
			
			if(zstream->avail_out == 0){
				int newCapacity = pContentCapacity + (pContentCapacity >> 1); // + 50%
				if(newCapacity - pContentCapacity < BUFFER_SIZE){
					newCapacity = pContentCapacity + BUFFER_SIZE;
				}
				
				void * const content = realloc(pContent, newCapacity);
				if(! content){
					DETHROW(deeOutOfMemory);
				}
				pContent = content;
				pContentCapacity = newCapacity;
				
				zstream->next_out = (Bytef*)pContent + pContentSize;
				zstream->avail_out = newCapacity - pContentSize;
			}
			
		}else if(result == Z_STREAM_END){
			const int newCapacity = pContentCapacity - (int)zstream->avail_out;
			
			void * const content = realloc(pContent, newCapacity); // reduce to used size
			if(! content){
				DETHROW(deeOutOfMemory);
			}
			
			pContent = content;
			pContentSize = newCapacity;
			pContentCapacity = newCapacity;
			
			zstream->next_out = (Bytef*)pContent + pContentSize;
			zstream->avail_out = 0;
			
			break;
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
	
	if(position < pContentSize){
		pContentPosition = position;
		
	}else{
		pContentPosition = pContentSize;
	}
}

void decZFileReader::pDecompressAll(){
	pSetContentPosition(INT_MAX);
}
