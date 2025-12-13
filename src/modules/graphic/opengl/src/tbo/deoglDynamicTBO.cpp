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

#include "deoglDynamicTBO.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../memory/deoglMemoryManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBO
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBO::deoglDynamicTBO(deoglRenderThread &renderThread, int componentCount, int dataTypeSize) :
pRenderThread(renderThread),
pComponentCount(componentCount),
pDataTypeSize(dataTypeSize),
pVBO(0),
pTBO(0),
pData(NULL),
pDataSize(0),
pDataCount(0),
pMemUse(renderThread.GetMemoryManager().GetConsumption().bufferObject.tbo)
{
	if(componentCount < 1 || componentCount > 4 || dataTypeSize < 1 || dataTypeSize > 4){
		DETHROW(deeInvalidParam);
	}
}

deoglDynamicTBO::~deoglDynamicTBO(){
	pCleanUp();
}



// Management
///////////////

void deoglDynamicTBO::IncreaseDataCount(int byAmount){
	if(byAmount < 0){
		DETHROW(deeInvalidParam);
	}
	if(byAmount == 0){
		return;
	}
	
	pEnlarge(byAmount);
	pDataCount += byAmount;
}

void deoglDynamicTBO::SetDataCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	if(count > pDataCount){
		pEnlarge(count - pDataCount);
	}
	pDataCount = count;
}

int deoglDynamicTBO::GetPixelCount() const{
	int count = pDataCount / pComponentCount;
	if(pDataCount % pComponentCount != 0){
		count++;
	}
	return count;
}

void deoglDynamicTBO::IncreasePixelCount(int byAmount){
	IncreaseDataCount(byAmount * pComponentCount);
}

void deoglDynamicTBO::SetPixelCount(int count){
	SetDataCount(count * pComponentCount);
}

int deoglDynamicTBO::GetPixelOffset(int pixel) const{
	if(pixel < 0){
		DETHROW(deeInvalidParam);
	}
	return pixel * pComponentCount;
}

void deoglDynamicTBO::Clear(){
	pDataCount = 0;
}

void deoglDynamicTBO::AddTBO(const deoglDynamicTBO &tbo){
	if(tbo.pDataTypeSize != pDataTypeSize){
		DETHROW(deeInvalidParam);
	}
	if(tbo.pDataCount == 0){
		return;
	}
	
	pEnlarge(tbo.pDataCount);
	memcpy(pData + pDataCount * pDataTypeSize, tbo.pData, tbo.pDataCount * tbo.pDataTypeSize);
	pDataCount += tbo.pDataCount;
}

void deoglDynamicTBO::SetTBO(int offset, const deoglDynamicTBO &tbo){
	if(tbo.pDataTypeSize != pDataTypeSize){
		DETHROW(deeInvalidParam);
	}
	if(tbo.pDataCount == 0){
		return;
	}
	if(offset < 0 || offset + tbo.pDataCount - 1 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	memcpy(pData + offset * pDataTypeSize, tbo.pData, tbo.pDataCount * tbo.pDataTypeSize);
}

void deoglDynamicTBO::Update(){
	if(pDataCount == 0){
		return;
	}
	
	pEnsurePadding();
	pEnsureVBO();
	
	OGL_CHECK(pRenderThread, pglBindBuffer(GL_TEXTURE_BUFFER, pVBO));
	
	const int size = pDataCount * pDataTypeSize;
	OGL_CHECK(pRenderThread, pglBufferData(GL_TEXTURE_BUFFER, size, NULL, GL_STREAM_DRAW));
	OGL_CHECK(pRenderThread, pglBufferData(GL_TEXTURE_BUFFER, size, pData, GL_STREAM_DRAW));
	
	pEnsureTBO();
	
	OGL_CHECK(pRenderThread, pglBindBuffer(GL_TEXTURE_BUFFER, 0));
}

void deoglDynamicTBO::Update(int offset, int count){
	if(count == 0){
		return;
	}
	
	pEnsurePadding();
	
	if(offset < 0 || count < 0 || offset + count > GetPixelCount()){
		DETHROW(deeInvalidParam);
	}
	
	pEnsureVBO();
	
	OGL_CHECK(pRenderThread, pglBindBuffer(GL_TEXTURE_BUFFER, pVBO));
	
	const int byteOffset = offset * pComponentCount * pDataTypeSize;
	OGL_CHECK(pRenderThread, pglBufferSubData(GL_TEXTURE_BUFFER, byteOffset,
		count * pComponentCount * pDataTypeSize, pData + byteOffset));
	
	pEnsureTBO();
	
	OGL_CHECK(pRenderThread, pglBindBuffer(GL_TEXTURE_BUFFER, 0));
}



// Private Functions
//////////////////////

void deoglDynamicTBO::pCleanUp(){
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	dops.DeleteOpenGLTexture(pTBO);
	dops.DeleteOpenGLBuffer(pVBO);
	
	pMemUse = 0;
	if(pData){
		delete [] pData;
	}
}

void deoglDynamicTBO::pEnlarge(int count){
	if(pDataCount + count <= pDataSize){
		return;
	}
	
	const int newSize = (pDataCount + count) * 3 / 2 + 1;
	uint8_t * const newArray = new uint8_t[newSize * pDataTypeSize];
	
	if(pData){
		memcpy(newArray, pData, pDataCount * pDataTypeSize);
		delete [] pData;
	}
	
	pData = newArray;
	pDataSize = newSize;
}

void deoglDynamicTBO::pEnsureVBO(){
	if(pVBO){
		return;
	}
	
	OGL_CHECK(pRenderThread, pglGenBuffers(1, &pVBO));
	
	pMemUse = pDataCount * pDataTypeSize;
}

void deoglDynamicTBO::pEnsureTBO(){
	if(pTBO){
		return;
	}
	
	OGL_CHECK(pRenderThread, glGenTextures(1, &pTBO));
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	tsmgr.EnableBareTBO(0, pTBO);
	OGL_CHECK(pRenderThread, pglTexBuffer(GL_TEXTURE_BUFFER, GetTBOFormat(), pVBO));
	tsmgr.DisableStage(0);
}

void deoglDynamicTBO::pEnsurePadding(){
	const int remainder = pDataCount % pComponentCount;
	if(remainder > 0){
		pEnlarge(pComponentCount - remainder);
	}
}
