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

#include "deoglDynamicTBOFloat8.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOFloat8
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOFloat8::deoglDynamicTBOFloat8(deoglRenderThread &renderThread, int componentCount) :
deoglDynamicTBO(renderThread, componentCount, 1){
}

deoglDynamicTBOFloat8::~deoglDynamicTBOFloat8(){
}



// Management
///////////////

static inline uint8_t deoglDynamicTBOFloat8_convert(float value){
	return (uint8_t)(decMath::clamp(value, 0.0f, 1.0f) * 255.0f);
}

void deoglDynamicTBOFloat8::AddBool(bool value){
	AddFloat(value ? 1.0f : 0.0f);
}

void deoglDynamicTBOFloat8::AddFloat(float value){
	pEnlarge(1);
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value);
}

void deoglDynamicTBOFloat8::AddVec2(float value1, float value2){
	pEnlarge(2);
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value1);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value2);
}

void deoglDynamicTBOFloat8::AddVec2(const decVector2 &value){
	pEnlarge(2);
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.x);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.y);
}

void deoglDynamicTBOFloat8::AddVec3(float value1, float value2, float value3){
	pEnlarge(3);
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value1);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value2);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value3);
}

void deoglDynamicTBOFloat8::AddVec3(const decVector &value){
	pEnlarge(3);
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.x);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.y);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.z);
}

void deoglDynamicTBOFloat8::AddVec3(const decColor &value){
	pEnlarge(3);
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.r);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.g);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.b);
}

void deoglDynamicTBOFloat8::AddVec4(float value1, float value2, float value3, float value4){
	pEnlarge(4);
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value1);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value2);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value3);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value4);
}

void deoglDynamicTBOFloat8::AddVec4(const decVector &value, float value4){
	pEnlarge(4);
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.x);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.y);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.z);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value4);
}

void deoglDynamicTBOFloat8::AddVec4(const decVector4 &value){
	pEnlarge(4);
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.x);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.y);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.z);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.w);
}

void deoglDynamicTBOFloat8::AddVec4(const decColor &value){
	pEnlarge(4);
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.r);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.g);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.b);
	dataFloat[pDataCount++] = deoglDynamicTBOFloat8_convert(value.a);
}

void deoglDynamicTBOFloat8::SetBoolAt(int offset, bool value){
	SetFloatAt(offset, value ? 1.0f : 0.0f);
}

void deoglDynamicTBOFloat8::SetFloatAt(int offset, float value){
	if(offset < 0 || offset >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[offset] = deoglDynamicTBOFloat8_convert(value);
}

void deoglDynamicTBOFloat8::SetVec2At(int offset, float value1, float value2){
	if(offset < 0 || offset + 1 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[offset] = deoglDynamicTBOFloat8_convert(value1);
	dataFloat[offset + 1] = deoglDynamicTBOFloat8_convert(value2);
}

void deoglDynamicTBOFloat8::SetVec2At(int offset, const decVector2 &value){
	if(offset < 0 || offset + 1 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[offset] = deoglDynamicTBOFloat8_convert(value.x);
	dataFloat[offset + 1] = deoglDynamicTBOFloat8_convert(value.y);
}

void deoglDynamicTBOFloat8::SetVec3At(int offset, float value1, float value2, float value3){
	if(offset < 0 || offset + 2 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[offset] = deoglDynamicTBOFloat8_convert(value1);
	dataFloat[offset + 1] = deoglDynamicTBOFloat8_convert(value2);
	dataFloat[offset + 2] = deoglDynamicTBOFloat8_convert(value3);
}

void deoglDynamicTBOFloat8::SetVec3At(int offset, const decVector &value){
	if(offset < 0 || offset + 2 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[offset] = deoglDynamicTBOFloat8_convert(value.x);
	dataFloat[offset + 1] = deoglDynamicTBOFloat8_convert(value.y);
	dataFloat[offset + 2] = deoglDynamicTBOFloat8_convert(value.z);
}

void deoglDynamicTBOFloat8::SetVec3At(int offset, const decColor &value){
	if(offset < 0 || offset + 2 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[offset] = deoglDynamicTBOFloat8_convert(value.r);
	dataFloat[offset + 1] = deoglDynamicTBOFloat8_convert(value.g);
	dataFloat[offset + 2] = deoglDynamicTBOFloat8_convert(value.b);
}

void deoglDynamicTBOFloat8::SetVec4At(int offset, float value1, float value2,
float value3, float value4){
	if(offset < 0 || offset + 3 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[offset] = deoglDynamicTBOFloat8_convert(value1);
	dataFloat[offset + 1] = deoglDynamicTBOFloat8_convert(value2);
	dataFloat[offset + 2] = deoglDynamicTBOFloat8_convert(value3);
	dataFloat[offset + 3] = deoglDynamicTBOFloat8_convert(value4);
}

void deoglDynamicTBOFloat8::SetVec4At(int offset, const decVector &value, float value4){
	if(offset < 0 || offset + 3 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[offset] = deoglDynamicTBOFloat8_convert(value.x);
	dataFloat[offset + 1] = deoglDynamicTBOFloat8_convert(value.y);
	dataFloat[offset + 2] = deoglDynamicTBOFloat8_convert(value.z);
	dataFloat[offset + 3] = deoglDynamicTBOFloat8_convert(value4);
}

void deoglDynamicTBOFloat8::SetVec4At(int offset, const decVector4 &value){
	if(offset < 0 || offset + 3 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[offset] = deoglDynamicTBOFloat8_convert(value.x);
	dataFloat[offset + 1] = deoglDynamicTBOFloat8_convert(value.y);
	dataFloat[offset + 2] = deoglDynamicTBOFloat8_convert(value.z);
	dataFloat[offset + 3] = deoglDynamicTBOFloat8_convert(value.w);
}

void deoglDynamicTBOFloat8::SetVec4At(int offset, const decColor &value){
	if(offset < 0 || offset + 3 >= pDataCount){
		DETHROW(deeInvalidParam);
	}
	
	uint8_t * const dataFloat = (uint8_t*)pData;
	dataFloat[offset] = deoglDynamicTBOFloat8_convert(value.r);
	dataFloat[offset + 1] = deoglDynamicTBOFloat8_convert(value.g);
	dataFloat[offset + 2] = deoglDynamicTBOFloat8_convert(value.b);
	dataFloat[offset + 3] = deoglDynamicTBOFloat8_convert(value.a);
}


static inline float deoglDynamicTBOFloat8_convert(uint8_t value){
	return (float)value * 255.0f;
}

void deoglDynamicTBOFloat8::DebugPrint(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	logger.LogInfoFormat("TBO %d-Float8:", pComponentCount);
	uint8_t *data = (uint8_t*)pData;
	int i, pixel = 0;
	
	pEnsurePadding();
	
	switch(pComponentCount){
	case 1:
		for(i=0; i<pDataCount; i++){
			const float v1 = deoglDynamicTBOFloat8_convert(*(data++));
			logger.LogInfoFormat("%5d: [%11.4f]", pixel++, v1);
		}
		break;
		
	case 2:
		for(i=0; i<pDataCount; i+=2){
			const float v1 = deoglDynamicTBOFloat8_convert(*(data++));
			const float v2 = deoglDynamicTBOFloat8_convert(*(data++));
			logger.LogInfoFormat("%5d: [%11.4f | %11.4f]", pixel++, v1, v2);
		}
		break;
		
	case 3:
		for(i=0; i<pDataCount; i+=3){
			const float v1 = deoglDynamicTBOFloat8_convert(*(data++));
			const float v2 = deoglDynamicTBOFloat8_convert(*(data++));
			const float v3 = deoglDynamicTBOFloat8_convert(*(data++));
			logger.LogInfoFormat("%5d: [%11.4f | %11.4f | %11.4f]", pixel++, v1, v2, v3);
		}
		break;
		
	case 4:
		for(i=0; i<pDataCount; i+=4){
			const float v1 = deoglDynamicTBOFloat8_convert(*(data++));
			const float v2 = deoglDynamicTBOFloat8_convert(*(data++));
			const float v3 = deoglDynamicTBOFloat8_convert(*(data++));
			const float v4 = deoglDynamicTBOFloat8_convert(*(data++));
			logger.LogInfoFormat("%5d: [%11.4f | %11.4f | %11.4f | %11.4f]", pixel++, v1, v2, v3, v4);
		}
		break;
	}
}

GLenum deoglDynamicTBOFloat8::GetTBOFormat(){
	switch(pComponentCount){
	case 1:
		return GL_R8;
		
	case 2:
		return GL_RG8;
		
	case 3:
		return GL_RGB8;
		
	case 4:
		return GL_RGBA8;
		
	default:
		DETHROW(deeInvalidParam);
	}
}
