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
#include "decMemoryFileReader.h"
#include "../exceptions.h"



// Class decMemoryFileReader
//////////////////////////////

// Constructor, Destructor
////////////////////////////

decMemoryFileReader::decMemoryFileReader(decMemoryFile *memoryFile) :
pFile(NULL),
pPosition(0)
{
	if(!memoryFile){
		DETHROW(deeInvalidParam);
	}
	
	pFile = memoryFile;
	memoryFile->AddReference();
}

decMemoryFileReader::decMemoryFileReader(const decMemoryFileReader &reader) :
pFile(reader.pFile),
pPosition(reader.pPosition)
{
}

decMemoryFileReader::~decMemoryFileReader(){
}



// Management
///////////////

const char *decMemoryFileReader::GetFilename(){
	return pFile->GetFilename();
}

int decMemoryFileReader::GetLength(){
	return pFile->GetLength();
}

TIME_SYSTEM decMemoryFileReader::GetModificationTime(){
	return pFile->GetModificationTime();
}



// Seeking
////////////

int decMemoryFileReader::GetPosition(){
	return pPosition;
}

void decMemoryFileReader::SetPosition(int position){
	if(position < 0 || position > pFile->GetLength()){
		DETHROW(deeOutOfBoundary);
	}
	pPosition = position;
}

void decMemoryFileReader::MovePosition(int offset){
	const int newPos = pPosition + offset;
	if(newPos < 0 || newPos > pFile->GetLength()){
		DETHROW(deeOutOfBoundary);
	}
	pPosition = newPos;
}

void decMemoryFileReader::SetPositionEnd(int position){
	if(position < 0 || position > pFile->GetLength()){
		DETHROW(deeOutOfBoundary);
	}
	pPosition = pFile->GetLength() - position;
}



// Reading
////////////

void decMemoryFileReader::Read(void *buffer, int size){
	if(!buffer){
		DETHROW(deeInvalidParam);
	}
	if(pPosition + size > pFile->GetLength()){
		DETHROW(deeInvalidParam);
	}
	
	memcpy(buffer, pFile->GetPointer() + pPosition, size);
	pPosition += size;
}

decBaseFileReader::Ref decMemoryFileReader::Duplicate(){
	return decBaseFileReader::Ref::New(new decMemoryFileReader(*this));
}
