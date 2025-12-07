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

#include "desynDecodeBuffer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deSoundDecoder.h>



// Class desynDecodeBuffer
////////////////////////////

// Constructor, destructor
////////////////////////////

desynDecodeBuffer::desynDecodeBuffer(int size) :
pBuffer(NULL),
pSize(0)
{
	SetSize(size);
}

desynDecodeBuffer::~desynDecodeBuffer(){
	if(pBuffer){
		delete [] pBuffer;
	}
}



// Management
///////////////

void desynDecodeBuffer::SetSize(int size){
	if(size < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(size == pSize){
		return;
	}
	
	char *buffer = new char[size];
	
	if(pBuffer){
		delete [] pBuffer;
	}
	pBuffer = buffer;
	pSize = size;
}

int desynDecodeBuffer::Decode(deSoundDecoder &decoder, int size){
	int bytesRead, remaining;
	
	if(size > pSize){
		SetSize(size);
	}
	
	bytesRead = decoder.ReadSamples(pBuffer, size);
	
	remaining = size - bytesRead;
	if(remaining > 0){
		memset(pBuffer + bytesRead, '\0', remaining);
	}
	
	return bytesRead;
}

int desynDecodeBuffer::DecodeLooping(deSoundDecoder &decoder, int size){
	int totalBytesRead = 0;
	int remaining = size;
	int bytesRead = 0;
	int position = 0;
	
	if(size > pSize){
		SetSize(size);
	}
	
	while(true){
		bytesRead = decoder.ReadSamples(pBuffer + position, remaining);
		if(bytesRead == 0) break; // EOF
		totalBytesRead += bytesRead;
		
		remaining -= bytesRead;
		if(remaining == 0) break;
		
		position += bytesRead;
		decoder.SetPosition(0);
	}
	
	return totalBytesRead;
}
