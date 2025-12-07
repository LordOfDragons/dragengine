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

#include "deoalSharedBuffer.h"
#include "deoalSharedBufferList.h"

#include <dragengine/common/exceptions.h>



// Class deoalSharedBufferList
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalSharedBufferList::deoalSharedBufferList(){
}

deoalSharedBufferList::~deoalSharedBufferList(){
	const int count = pBuffers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delete (deoalSharedBuffer*)pBuffers.GetAt(i);
	}
	pBuffers.RemoveAll();
}



// Management
///////////////

deoalSharedBuffer *deoalSharedBufferList::ClaimBuffer(int size){
	if(size < 0){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pBuffers.GetCount();
	deoalSharedBuffer *buffer = NULL;
	int i;
	
	for(i=0; i<count; i++){
		buffer = (deoalSharedBuffer*)pBuffers.GetAt(i);
		if(! buffer->GetInUse()){
			if(size > buffer->GetSize()){
				buffer->SetSize(size);
			}
			buffer->SetInUse(true);
			return buffer;
		}
	}
	
	buffer = NULL;
	try{
		buffer = new deoalSharedBuffer;
		buffer->SetSize(size);
		buffer->SetInUse(true);
		pBuffers.Add(buffer);
		return buffer;
		
	}catch(const deException &){
		if(buffer){
			delete buffer;
		}
		throw;
	}
}

void deoalSharedBufferList::ReleaseBuffer(deoalSharedBuffer *buffer){
	if(! buffer){
		DETHROW(deeInvalidParam);
	}
	
	buffer->SetInUse(false);
}
