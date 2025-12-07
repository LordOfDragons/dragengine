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
#include "deNetworkMessageWriter.h"
#include "../../common/exceptions.h"



// Class deNetworkMessageWriter
/////////////////////////////////

// Constructor, Destructor
////////////////////////////

deNetworkMessageWriter::deNetworkMessageWriter(deNetworkMessage *message, bool append) :
pMessage(message),
pPosition(0)
{
	if(! message){
		DETHROW(deeInvalidParam);
	}
	if(! append){
		message->Clear();
	}
}

deNetworkMessageWriter::deNetworkMessageWriter(const deNetworkMessageWriter &writer) :
pMessage(writer.pMessage),
pPosition(writer.pPosition){
}

deNetworkMessageWriter::~deNetworkMessageWriter(){
}



// Seeking
////////////

int deNetworkMessageWriter::GetPosition(){
	return pPosition;
}

void deNetworkMessageWriter::SetPosition(int position){
	if(position < 0 || position > pMessage->GetDataLength()){
		DETHROW(deeOutOfBoundary);
	}
	pPosition = position;
}

void deNetworkMessageWriter::MovePosition(int offset){
	const int newPos = pPosition + offset;
	if(newPos < 0 || newPos > pMessage->GetDataLength()){
		DETHROW(deeOutOfBoundary);
	}
	pPosition = newPos;
}

void deNetworkMessageWriter::SetPositionEnd(int position){
	if(position < 0 || position > pMessage->GetDataLength()){
		DETHROW(deeOutOfBoundary);
	}
	pPosition = pMessage->GetDataLength() - position;
}



// Management
///////////////

const char *deNetworkMessageWriter::GetFilename(){
	return "<NetworkMessage>";
}

void deNetworkMessageWriter::Write(const void *buffer, int size){
	if(size == 0){
		return;
	}
	
	if(! buffer || size < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(pPosition + size > pMessage->GetDataLength()){
		pMessage->SetDataLength(pPosition + size);
	}
	memcpy(pMessage->GetBuffer() + pPosition, buffer, size);
	pPosition += size;
}

decBaseFileWriter::Ref deNetworkMessageWriter::Duplicate(){
	return decBaseFileWriter::Ref::New(new deNetworkMessageWriter(*this));
}
