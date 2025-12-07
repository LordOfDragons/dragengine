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

#include "deoglSPBParameter.h"
#include "deoglSPBlockMemory.h"
#include "../deoglShaderCompiled.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglSPBlockMemory
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSPBlockMemory::deoglSPBlockMemory(deoglRenderThread &renderThread) :
deoglShaderParameterBlock(renderThread),
pBuffer(NULL),
pBufferCapacity(0){
}

deoglSPBlockMemory::deoglSPBlockMemory(const deoglSPBlockMemory &paramBlock) :
deoglShaderParameterBlock(paramBlock),
pBuffer(NULL),
pBufferCapacity(0){
}

deoglSPBlockMemory::~deoglSPBlockMemory(){
	if(IsBufferMapped()){
		pClearMapped();
	}
	if(pBuffer){
		delete [] pBuffer;
	}
}



// Management
///////////////

void deoglSPBlockMemory::Activate() const{
	DETHROW(deeInvalidParam);
}

void deoglSPBlockMemory::Activate(int) const{
	DETHROW(deeInvalidParam);
}

void deoglSPBlockMemory::Deactivate() const{
	DETHROW(deeInvalidParam);
}

void deoglSPBlockMemory::Deactivate(int) const{
	DETHROW(deeInvalidParam);
}

void deoglSPBlockMemory::MapBuffer(){
	if(IsBufferMapped() || GetBufferSize() == 0){
		DETHROW(deeInvalidParam);
	}
	
	pGrowBuffer();
	pSetMapped(pBuffer);
}

void deoglSPBlockMemory::MapBuffer(int element){
	MapBuffer(element, 1);
}

void deoglSPBlockMemory::MapBuffer (int element, int count){
	if(IsBufferMapped() || GetBufferSize() == 0 || element < 0 || count < 1
	|| element + count > GetElementCount()){
		DETHROW(deeInvalidParam);
	}
	
	pGrowBuffer();
	pSetMapped(pBuffer, element, count);
}

void deoglSPBlockMemory::UnmapBuffer(){
	if(! IsBufferMapped()){
		DETHROW(deeInvalidParam);
	}
	
	pClearMapped();
}

deoglShaderParameterBlock *deoglSPBlockMemory::Copy() const{
	return new deoglSPBlockMemory(*this);
}



// Private Functions
//////////////////////

void deoglSPBlockMemory::pGrowBuffer(){
	const int bufferSize = GetBufferSize();
	
	if(pBuffer && bufferSize > pBufferCapacity){
		char * const newBuffer = new char[bufferSize];
		if(pBufferCapacity > 0){
			memcpy(newBuffer, pBuffer, pBufferCapacity);
		}
		delete [] pBuffer;
		pBuffer = newBuffer;
		pBufferCapacity = bufferSize;
	}
	
	if(! pBuffer){
		pBuffer = new char[bufferSize];
		pBufferCapacity = bufferSize;
	}
}
