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

#include "deoglRenderTaskInstance.h"
#include "deoglComputeRenderTask.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/paramblock/deoglSPBlockMemory.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBElement.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskInstance
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskInstance::deoglRenderTaskInstance() :
pInstance(nullptr),
pSIIndexInstanceSPB(nullptr),
pSIIndexInstanceFirst(0),
pDrawIndirectIndex(0),
pDrawIndirectCount(0){
}

deoglRenderTaskInstance::~deoglRenderTaskInstance() = default;



// Management
///////////////

void deoglRenderTaskInstance::SetInstance(const deoglRenderTaskSharedInstance *instance){
	pInstance = instance;
}


void deoglRenderTaskInstance::SetSIIndexInstanceParam(deoglShaderParameterBlock *paramBlock, int firstIndex){
	pSIIndexInstanceSPB = paramBlock;
	pSIIndexInstanceFirst = firstIndex;
}

void deoglRenderTaskInstance::WriteSIIndexInstanceInt(bool useFlags){
	DEASSERT_NOTNULL(pSIIndexInstanceSPB)
	
	if(useFlags){
		struct sIndexFlags{
			GLuint index;
			GLuint flags;
		};
		sIndexFlags * const data = (sIndexFlags*)pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		pSubInstances.VisitIndexed([&](int i, const sSubInstance &subInstance){
			data[i].index = (GLuint)subInstance.instance;
			data[i].flags = (GLuint)subInstance.flags;
		});
		
	}else{
		struct sIndex{
			GLuint index;
		};
		sIndex * const data = (sIndex*)pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		pSubInstances.VisitIndexed([&](int i, const sSubInstance &subInstance){
			data[i].index = (GLuint)subInstance.instance;
		});
	}
}

void deoglRenderTaskInstance::WriteSIIndexInstanceCompute(){
	DEASSERT_NOTNULL(pSIIndexInstanceSPB)
	
	deoglComputeRenderTask::sStep * const data = (deoglComputeRenderTask::sStep*)
		pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
	
	pSubInstances.VisitIndexed([&](int i, const sSubInstance &subInstance){
		data[i].spbInstance = (uint32_t)(subInstance.instance + 1);
		data[i].specialFlags = (uint32_t)subInstance.flags;
	});
}

void deoglRenderTaskInstance::SetDrawIndirectIndex(int index){
	pDrawIndirectIndex = index;
}

void deoglRenderTaskInstance::SetDrawIndirectCount(int count){
	pDrawIndirectCount = count;
}



void deoglRenderTaskInstance::Reset(){
	pInstance = nullptr;
	pSubInstances.RemoveAll();
	pSIIndexInstanceSPB = nullptr;
	pSIIndexInstanceFirst = 0;
	pDrawIndirectIndex = 0;
	pDrawIndirectCount = 0;
}
