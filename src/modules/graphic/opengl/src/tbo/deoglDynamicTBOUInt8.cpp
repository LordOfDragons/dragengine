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

#include "deoglDynamicTBOUInt8.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOUInt8
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOUInt8::deoglDynamicTBOUInt8(deoglRenderThread &renderThread, int componentCount) :
deoglDynamicTBO(renderThread, componentCount, sizeof(uint8_t)){
}

deoglDynamicTBOUInt8::~deoglDynamicTBOUInt8(){
}



// Management
///////////////

void deoglDynamicTBOUInt8::AddBool(bool value){
	if(value){
		AddInt((uint8_t)~1);
		
	}else{
		AddInt(0);
	}
}

void deoglDynamicTBOUInt8::AddInt(uint8_t value){
	pEnlarge(1);
	
	uint8_t * const dataUInt = (uint8_t*)pData;
	dataUInt[pDataCount++] = value;
}

void deoglDynamicTBOUInt8::AddVec2(uint8_t value1, uint8_t value2){
	pEnlarge(2);
	
	uint8_t * const dataUInt = (uint8_t*)pData;
	dataUInt[pDataCount++] = value1;
	dataUInt[pDataCount++] = value2;
}

void deoglDynamicTBOUInt8::AddVec2(const decPoint &value){
	pEnlarge(2);
	
	uint8_t * const dataUInt = (uint8_t*)pData;
	dataUInt[pDataCount++] = (uint8_t)value.x;
	dataUInt[pDataCount++] = (uint8_t)value.y;
}

void deoglDynamicTBOUInt8::AddVec3(uint8_t value1, uint8_t value2, uint8_t value3){
	pEnlarge(3);
	
	uint8_t * const dataUInt = (uint8_t*)pData;
	dataUInt[pDataCount++] = value1;
	dataUInt[pDataCount++] = value2;
	dataUInt[pDataCount++] = value3;
}

void deoglDynamicTBOUInt8::AddVec3(const decPoint3 &value){
	pEnlarge(3);
	
	uint8_t * const dataUInt = (uint8_t*)pData;
	dataUInt[pDataCount++] = (uint8_t)value.x;
	dataUInt[pDataCount++] = (uint8_t)value.y;
	dataUInt[pDataCount++] = (uint8_t)value.z;
}

void deoglDynamicTBOUInt8::AddVec4(uint8_t value1, uint8_t value2, uint8_t value3, uint8_t value4){
	pEnlarge(4);
	
	uint8_t * const dataUInt = (uint8_t*)pData;
	dataUInt[pDataCount++] = value1;
	dataUInt[pDataCount++] = value2;
	dataUInt[pDataCount++] = value3;
	dataUInt[pDataCount++] = value4;
}

void deoglDynamicTBOUInt8::SetBoolAt(int offset, bool value){
	SetIntAt(offset, value ? (uint8_t)~1 : 0);
}

void deoglDynamicTBOUInt8::SetIntAt(int offset, uint8_t value){
	if(offset < 0 || offset >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataUInt = (uint8_t*)pData;
	dataUInt[offset] = value;
}

void deoglDynamicTBOUInt8::SetVec2At(int offset, uint8_t value1, uint8_t value2){
	if(offset < 0 || offset + 1 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataUInt = (uint8_t*)pData;
	dataUInt[offset] = value1;
	dataUInt[offset + 1] = value2;
}

void deoglDynamicTBOUInt8::SetVec2At(int offset, const decPoint &value){
	if(offset < 0 || offset + 1 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataUInt = (uint8_t*)pData;
	dataUInt[offset] = (uint8_t)value.x;
	dataUInt[offset + 1] = (uint8_t)value.y;
}

void deoglDynamicTBOUInt8::SetVec3At(int offset, uint8_t value1, uint8_t value2, uint8_t value3){
	if(offset < 0 || offset + 2 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataUInt = (uint8_t*)pData;
	dataUInt[offset] = value1;
	dataUInt[offset + 1] = value2;
	dataUInt[offset + 2] = value3;
}

void deoglDynamicTBOUInt8::SetVec3At(int offset, const decPoint3 &value){
	if(offset < 0 || offset + 2 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataUInt = (uint8_t*)pData;
	dataUInt[offset] = (uint8_t)value.x;
	dataUInt[offset + 1] = (uint8_t)value.y;
	dataUInt[offset + 2] = (uint8_t)value.z;
}

void deoglDynamicTBOUInt8::SetVec4At(int offset, uint8_t value1, uint8_t value2,
uint8_t value3, uint8_t value4){
	if(offset < 0 || offset + 3 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataUInt = (uint8_t*)pData;
	dataUInt[offset] = value1;
	dataUInt[offset + 1] = value2;
	dataUInt[offset + 2] = value3;
	dataUInt[offset + 3] = value4;
}

void deoglDynamicTBOUInt8::DebugPrint(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	logger.LogInfoFormat("TBO %d-UInt8:", pComponentCount);
	uint8_t *data = (uint8_t*)pData;
	int i, pixel = 0;
	
	pEnsurePadding();
	
	switch(pComponentCount){
	case 1:
		for(i=0; i<pDataCount; i++){
			const uint8_t v1 = *(data++);
			logger.LogInfoFormat("%5d: [%3d]", pixel++, v1);
		}
		break;
		
	case 2:
		for(i=0; i<pDataCount; i+=2){
			const uint8_t v1 = *(data++);
			const uint8_t v2 = *(data++);
			logger.LogInfoFormat("%5d: [%3d | %3d]", pixel++, v1, v2);
		}
		break;
		
	case 3:
		for(i=0; i<pDataCount; i+=3){
			const uint8_t v1 = *(data++);
			const uint8_t v2 = *(data++);
			const uint8_t v3 = *(data++);
			logger.LogInfoFormat("%5d: [%3d | %3d | %3d]", pixel++, v1, v2, v3);
		}
		break;
		
	case 4:
		for(i=0; i<pDataCount; i+=4){
			const uint8_t v1 = *(data++);
			const uint8_t v2 = *(data++);
			const uint8_t v3 = *(data++);
			const uint8_t v4 = *(data++);
			logger.LogInfoFormat("%5d: [%3d | %3d | %3d | %3d]", pixel++, v1, v2, v3, v4);
		}
		break;
	}
}

GLenum deoglDynamicTBOUInt8::GetTBOFormat(){
	switch(pComponentCount){
	case 1:
		return GL_R8UI;
		
	case 2:
		return GL_RG8UI;
		
	case 3:
		return GL_RGB8UI;
		
	case 4:
		return GL_RGBA8UI;
		
	default:
		DETHROW(deeInvalidParam);
	}
}
