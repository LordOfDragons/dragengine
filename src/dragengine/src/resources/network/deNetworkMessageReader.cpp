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

#include "deNetworkMessage.h"
#include "deNetworkMessageReader.h"
#include "../../common/exceptions.h"



// Class deNetworkMessageReader
/////////////////////////////////

// Constructor, Destructor
////////////////////////////

deNetworkMessageReader::deNetworkMessageReader(deNetworkMessage *message) :
pMessage(message),
pPosition(0)
{
	if(! message){
		DETHROW(deeInvalidParam);
	}
}

deNetworkMessageReader::deNetworkMessageReader(const deNetworkMessageReader &reader) :
pMessage(reader.pMessage),
pPosition(reader.pPosition){
}

deNetworkMessageReader::~deNetworkMessageReader(){
}



// Management
///////////////

const char *deNetworkMessageReader::GetFilename(){
	return "<NetworkMessage>";
}

int deNetworkMessageReader::GetLength(){
	return pMessage->GetDataLength();
}

TIME_SYSTEM deNetworkMessageReader::GetModificationTime(){
	return pMessage->GetTimeStamp();
}



// Seeking
////////////

int deNetworkMessageReader::GetPosition(){
	return pPosition;
}

void deNetworkMessageReader::SetPosition(int position){
	if(position < 0 || position > pMessage->GetDataLength()){
		DETHROW(deeOutOfBoundary);
	}
	pPosition = position;
}

void deNetworkMessageReader::MovePosition(int offset){
	const int newPos = pPosition + offset;
	if(newPos < 0 || newPos > pMessage->GetDataLength()){
		DETHROW(deeOutOfBoundary);
	}
	pPosition = newPos;
}

void deNetworkMessageReader::SetPositionEnd(int position){
	if(position < 0 || position > pMessage->GetDataLength()){
		DETHROW(deeOutOfBoundary);
	}
	pPosition = pMessage->GetDataLength() - position;
}



// Reading
////////////

void deNetworkMessageReader::Read(void *buffer, int size){
	if(! buffer){
		DETHROW(deeInvalidParam);
	}
	
	if(pPosition + size > pMessage->GetDataLength()){
		DETHROW(deeInvalidParam);
	}
	
	memcpy(buffer, pMessage->GetBuffer() + pPosition, size);
	pPosition += size;
}

decBaseFileReader::Ref deNetworkMessageReader::Duplicate(){
	return decBaseFileReader::Ref::New(new deNetworkMessageReader(*this));
}
