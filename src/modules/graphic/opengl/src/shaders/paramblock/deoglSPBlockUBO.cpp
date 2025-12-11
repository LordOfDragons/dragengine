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
#include "deoglSPBlockUBO.h"
#include "../deoglShaderCompiled.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglSPBlockUBO
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglSPBlockUBO::deoglSPBlockUBO(deoglRenderThread &renderThread) :
deoglShaderParameterBlock(renderThread),
pUBO(0),
pBindingPoint(0),
pCompact(true),
pAllocateBuffer(true),
pWriteBuffer(NULL),
pWriteBufferCapacity(0),
pWriteBufferUsed(false){
}

deoglSPBlockUBO::deoglSPBlockUBO(const deoglSPBlockUBO &paramBlock) :
deoglShaderParameterBlock(paramBlock),
pUBO(0),
pBindingPoint(paramBlock.pBindingPoint),
pCompact(paramBlock.pCompact),
pAllocateBuffer(true),
pWriteBuffer(NULL),
pWriteBufferCapacity(0),
pWriteBufferUsed(false){
}

deoglSPBlockUBO::~deoglSPBlockUBO(){
	if(IsBufferMapped()){
		pClearMapped(); // done by delete
	}
	GetRenderThread().GetDelayedOperations().DeleteOpenGLBuffer(pUBO);
	if(pWriteBuffer){
		delete [] pWriteBuffer;
	}
}



// Management
///////////////

void deoglSPBlockUBO::SetBindingPoint(int bindingPoint){
	pBindingPoint = bindingPoint;
}

void deoglSPBlockUBO::SetCompact(bool compact){
	DEASSERT_FALSE(IsBufferMapped())
	
	pCompact = compact;
}

void deoglSPBlockUBO::Activate() const{
	DEASSERT_NOTNULL(pUBO)
	DEASSERT_FALSE(IsBufferMapped())
	
	OGL_CHECK(GetRenderThread(), pglBindBufferBase(GL_UNIFORM_BUFFER, pBindingPoint, pUBO));
}

void deoglSPBlockUBO::Activate(int bindingPoint) const{
	DEASSERT_NOTNULL(pUBO)
	DEASSERT_FALSE(IsBufferMapped())
	
	OGL_CHECK(GetRenderThread(), pglBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, pUBO));
}

void deoglSPBlockUBO::Deactivate() const{
	OGL_CHECK(GetRenderThread(), pglBindBufferBase(GL_UNIFORM_BUFFER, pBindingPoint, 0));
}

void deoglSPBlockUBO::Deactivate(int bindingPoint) const{
	OGL_CHECK(GetRenderThread(), pglBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, 0));
}

void deoglSPBlockUBO::MapBuffer(){
	DEASSERT_FALSE(IsBufferMapped())
	DEASSERT_TRUE(GetBufferSize() > 0)
	
	if(false){ // use mapped
		if(!pUBO){
			OGL_CHECK(GetRenderThread(), pglGenBuffers(1, &pUBO));
			if(!pUBO){
				DETHROW(deeOutOfMemory);
			}
		}
		
		OGL_CHECK(GetRenderThread(), pglBindBuffer(GL_UNIFORM_BUFFER, pUBO));
		OGL_CHECK(GetRenderThread(), pglBufferData(GL_UNIFORM_BUFFER,
			GetBufferSize(), NULL, GL_DYNAMIC_DRAW));
				// clear using old clear method
		pAllocateBuffer = false;
		
		// NOTE glMapBuffer() does not exist on EGL. glMapBufferRange() though fails to run if
		//      used from here. no idea how this is supposed to work with glMapBuffer() not
		//      working. furthermore glBufferData() seems faster than glMapBuffer*()
		try{
			char *data;
			
			if(pglMapBuffer){
				OGL_CHECK(GetRenderThread(), data = reinterpret_cast<char*>(pglMapBuffer(
					GL_UNIFORM_BUFFER, GL_WRITE_ONLY)));
				
			}else{
				OGL_CHECK(GetRenderThread(), data = reinterpret_cast<char*>(pglMapBufferRange(
					GL_UNIFORM_BUFFER, 0, GetBufferSize(), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)));
			}
			
			if(!data){
				DETHROW(deeInvalidParam);
			}
			pSetMapped(data);
			
		}catch(const deException &){
			OGL_CHECK(GetRenderThread(), pglBindBuffer(GL_UNIFORM_BUFFER, 0));
			throw;
		}
		
	}else{
		pGrowWriteBuffer(GetBufferSize());
		pWriteBufferUsed = true;
		pSetMapped(pWriteBuffer);
	}
}

void deoglSPBlockUBO::MapBuffer(int element){
	MapBuffer(element, 1);
}

void deoglSPBlockUBO::MapBuffer(int element, int count){
	DEASSERT_FALSE(IsBufferMapped())
	DEASSERT_TRUE(GetBufferSize() > 0)
	DEASSERT_TRUE(element >= 0)
	DEASSERT_TRUE(count > 0)
	DEASSERT_TRUE(element + count <= GetElementCount())
	
	if(false){ // use mapped
		if(!pUBO){
			OGL_CHECK(GetRenderThread(), pglGenBuffers(1, &pUBO));
			if(!pUBO){
				DETHROW(deeOutOfMemory);
			}
			pAllocateBuffer = true;
		}
		
		OGL_CHECK(GetRenderThread(), pglBindBuffer(GL_UNIFORM_BUFFER, pUBO));
		
		if(pAllocateBuffer){
			OGL_CHECK(GetRenderThread(), pglBufferData(GL_UNIFORM_BUFFER,
				GetBufferSize(), NULL, GL_DYNAMIC_DRAW));
			pAllocateBuffer = false;
		}
		
		try{
			char *data;
			
			OGL_CHECK(GetRenderThread(), data = reinterpret_cast<char*>(pglMapBufferRange(
				GL_UNIFORM_BUFFER, GetElementStride() * element, GetElementStride() * count,
				GL_WRITE_ONLY | GL_MAP_INVALIDATE_RANGE_BIT)));
			
			if(!data){
				DETHROW(deeInvalidParam);
			}
			pSetMapped(data, element, count);
			
		}catch(const deException &){
			OGL_CHECK(GetRenderThread(), pglBindBuffer(GL_UNIFORM_BUFFER, 0));
			throw;
		}
		
	}else{
		pGrowWriteBuffer(GetElementStride() * count);
		pWriteBufferUsed = true;
		pSetMapped(pWriteBuffer, element, count);
	}
}

void deoglSPBlockUBO::UnmapBuffer(){
	DEASSERT_TRUE(IsBufferMapped())
	
	if(pWriteBufferUsed){
		OGL_IF_CHECK(deoglRenderThread &renderThread = GetRenderThread();)
		
		if(!pUBO){
			OGL_CHECK(renderThread, pglGenBuffers(1, &pUBO));
			if(!pUBO){
				DETHROW(deeOutOfMemory);
			}
			pAllocateBuffer = true;
		}
		
		const int stride = GetElementStride();
		const int lower = pGetElementLower();
		const int upper = pGetElementUpper();
		
		const int offset = stride * lower;
		const int size = stride * (upper - lower + 1);
		
		if(pAllocateBuffer){
			OGL_CHECK(renderThread, pglBindBuffer(GL_UNIFORM_BUFFER, pUBO));
			OGL_CHECK(renderThread, pglBufferData(GL_UNIFORM_BUFFER,
				GetBufferSize(), NULL, GL_DYNAMIC_DRAW));
			pAllocateBuffer = false;
			
		}else{
			if(pglBindBufferRange){
				OGL_CHECK(renderThread, pglBindBufferRange(GL_UNIFORM_BUFFER, 0, pUBO, offset, size));
				
			}else{
				OGL_CHECK(renderThread, pglBindBuffer(GL_UNIFORM_BUFFER, pUBO));
			}
		}
		
		OGL_CHECK(renderThread, pglBufferSubData(GL_UNIFORM_BUFFER, offset, size, pWriteBuffer));
		pWriteBufferUsed = false;
		
	}else{
		OGL_CHECK(GetRenderThread(), pglBindBuffer(GL_UNIFORM_BUFFER, pUBO));
		OGL_CHECK(GetRenderThread(), pglUnmapBuffer(GL_UNIFORM_BUFFER));
	}
	
	OGL_CHECK(GetRenderThread(), pglBindBuffer(GL_UNIFORM_BUFFER, 0));
	pClearMapped();
}

int deoglSPBlockUBO::GetAlignmentRequirements() const{
	return pCompact ? 0 : GetRenderThread().GetCapabilities().GetUBOOffsetAlignment();
}

deoglShaderParameterBlock *deoglSPBlockUBO::Copy() const{
	return new deoglSPBlockUBO(*this);
}



void deoglSPBlockUBO::DebugPrintConfig(const char *name){
	const char * const valueTypeNames[3] = {"float", "int", "bool"};
	int i;
	
	GetRenderThread().GetLogger().LogInfoFormat("Shader Parameter Block '%s' (%p):"
		" parameters=%d ubo=%u binding=%u elementCount=%d elementStride=%d"
		" offsetPadding=%d size=%d %s", name, this, GetParameterCount(), pUBO, pBindingPoint,
		GetElementCount(), GetElementStride(), GetOffsetPadding(), GetBufferSize(),
		GetRowMajor() ? "rowMajor" : "colMajor");
	
	for(i=0; i<GetParameterCount(); i++){
		const deoglSPBParameter &parameter = GetParameterAt(i);
		GetRenderThread().GetLogger().LogInfoFormat("- Parameter %i: %s%ix%i[%i]"
			" offset=%u stride=%u arraystride=%i size=%u", i,
				valueTypeNames[parameter.GetValueType()],
				parameter.GetComponentCount(), parameter.GetVectorCount(),
				parameter.GetArrayCount(), parameter.GetOffset(),
				parameter.GetStride(), parameter.GetArrayStride(),
				parameter.GetDataSize());
	}
}



// Protected Functions
////////////////////////

void deoglSPBlockUBO::pUpdateBufferSize(){
	deoglShaderParameterBlock::pUpdateBufferSize();
	pAllocateBuffer = true;
}



// Private Functions
//////////////////////

void deoglSPBlockUBO::pGrowWriteBuffer(int size){
	if(size <= pWriteBufferCapacity){
		return;
	}
	
	char * const newBuffer = new char[size];
	
	if(pWriteBuffer){
		if(pWriteBufferCapacity > 0){
			memcpy(newBuffer, pWriteBuffer, pWriteBufferCapacity);
		}
		
		delete [] pWriteBuffer;
		pWriteBuffer = NULL;
		pWriteBufferCapacity = 0;
	}
	
	pWriteBuffer = newBuffer;
	pWriteBufferCapacity = size;
}
