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

#include "decMemoryFile.h"
#include "decMemoryFileWriter.h"
#include "../exceptions.h"



// Class decMemoryFileWriter
//////////////////////////////

// Constructor, Destructor
////////////////////////////

decMemoryFileWriter::decMemoryFileWriter(decMemoryFile *memoryFile, bool append) :

pPosition(0)
{
	if(!memoryFile){
		DETHROW(deeInvalidParam);
	}
	
	pFile = memoryFile;
	
	if(append){
		pPosition = memoryFile->GetLength();
		
	}else{
		memoryFile->Resize(0, false);
	}
	
	memoryFile->Touch();
}

decMemoryFileWriter::decMemoryFileWriter(const decMemoryFileWriter &writer) :
pFile(writer.pFile),
pPosition(writer.pPosition)
{
	pFile->Touch();
}

decMemoryFileWriter::~decMemoryFileWriter(){
	if(pFile){
		pFile->Touch();
	}
}



// Seeking
////////////

int decMemoryFileWriter::GetPosition(){
	return pPosition;
}

void decMemoryFileWriter::SetPosition(int position){
	if(position < 0 || position > pFile->GetLength()){
		DETHROW(deeOutOfBoundary);
	}
	pPosition = position;
}

void decMemoryFileWriter::MovePosition(int offset){
	const int newPos = pPosition + offset;
	if(newPos < 0 || newPos > pFile->GetLength()){
		DETHROW(deeOutOfBoundary);
	}
	pPosition = newPos;
}

void decMemoryFileWriter::SetPositionEnd(int position){
	if(position < 0 || position > pFile->GetLength()){
		DETHROW(deeOutOfBoundary);
	}
	pPosition = pFile->GetLength() - position;
}



// Management
///////////////

const char *decMemoryFileWriter::GetFilename(){
	return pFile->GetFilename();
}

void decMemoryFileWriter::Write(const void *buffer, int size){
	if(size == 0){
		return;
	}
	
	if(!buffer || size < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(pPosition + size > pFile->GetLength()){
		pFile->Resize(pPosition + size, false);
	}
	memcpy(pFile->GetPointer() + pPosition, buffer, size);
	pPosition += size;
}

decBaseFileWriter::Ref decMemoryFileWriter::Duplicate(){
	return decBaseFileWriter::Ref::New(new decMemoryFileWriter(*this));
}
