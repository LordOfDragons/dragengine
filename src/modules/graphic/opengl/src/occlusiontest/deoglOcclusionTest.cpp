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

#include "deoglOcclusionTest.h"
#include "deoglOcclusionTestListener.h"
#include "../capabilities/deoglCapabilities.h"
#include "../configuration/deoglConfiguration.h"
#include "../extensions/deoglExtensions.h"
#include "../renderthread/deoglRenderThread.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../shaders/paramblock/deoglSPBMapBufferRead.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionTest::deoglOcclusionTest(deoglRenderThread &renderThread) :
pInputListeners(nullptr),
pInputData(nullptr),
pInputDataCount(0),
pInputDataSize(0)
{
	try{
		pSSBOInput.TakeOver(new deoglSPBlockSSBO(renderThread, deoglSPBlockSSBO::etStream));
		pSSBOInput->SetRowMajor(renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working());
		pSSBOInput->SetParameterCount(2);
		pSSBOInput->GetParameterAt(0).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1); // vec3 minExtend
		pSSBOInput->GetParameterAt(1).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1); // vec3 maxExtend
		pSSBOInput->MapToStd140();
		pSSBOInput->SetBindingPoint(0);
		
		pSSBOResult.TakeOver(new deoglSPBlockSSBO(renderThread, deoglSPBlockSSBO::etRead));
		pSSBOResult->SetRowMajor(renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working());
		pSSBOResult->SetParameterCount(1);
		pSSBOResult->GetParameterAt(0).SetAll(deoglSPBParameter::evtBool, 4, 1, 1); // bvec4 result
		pSSBOResult->MapToStd140();
		pSSBOResult->SetBindingPoint(1);
		pSSBOResult->EnsureBuffer();
		
		pResizeInputData(256);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglOcclusionTest::~deoglOcclusionTest(){
	pCleanUp();
}



// Management
///////////////

int deoglOcclusionTest::AddInputData(const decVector &minExtend, const decVector &maxExtend,
deoglOcclusionTestListener *listener){
	if(pInputDataCount == pInputDataSize){
		pResizeInputData(pInputDataSize + 128);
	}
	
	sInputData &inputData = pInputData[pInputDataCount];
	inputData.minExtend.x = (GLfloat)minExtend.x;
	inputData.minExtend.y = (GLfloat)minExtend.y;
	inputData.minExtend.z = (GLfloat)minExtend.z;
	inputData.maxExtend.x = (GLfloat)maxExtend.x;
	inputData.maxExtend.y = (GLfloat)maxExtend.y;
	inputData.maxExtend.z = (GLfloat)maxExtend.z;
	
	pInputListeners[pInputDataCount] = listener;
	
	return pInputDataCount++;
}

void deoglOcclusionTest::RemoveAllInputData(){
	pInputDataCount = 0;
}



void deoglOcclusionTest::UpdateSSBO(){
	const int elementCount = decMath::max(pInputDataCount, 1);
	if(elementCount > pSSBOInput->GetElementCount()){
		pSSBOInput->SetElementCount(elementCount);
		pSSBOInput->EnsureBuffer();
	}
	
	if(pInputDataCount > 0){
		deoglSPBlockSSBO &vbo = pSSBOInput;
		const deoglSPBMapBuffer mapped(vbo);
		int i;
		
		for(i=0; i<pInputDataCount; i++){
			const sInputData &inputData = pInputData[i];
			vbo.SetParameterDataVec3(0, i, inputData.minExtend);
			vbo.SetParameterDataVec3(1, i, inputData.maxExtend);
		}
	}
	
	const int resultCount = (elementCount - 1) / 4 + 1;
	if(resultCount > pSSBOResult->GetElementCount()){
		pSSBOResult->SetElementCount(resultCount);
		pSSBOResult->EnsureBuffer();
	}
}

void deoglOcclusionTest::UpdateResults(){
	if(pInputDataCount == 0 || !pSSBOResult){
		return;
	}
	
	const int resultCount = (pInputDataCount - 1) / 4 + 1;
	const deoglSPBMapBufferRead mapped(pSSBOResult, 0, resultCount);
	const uint32_t * const result = (const uint32_t *)pSSBOResult->GetMappedBuffer();
	int i;
	
	for(i=0; i<pInputDataCount; i++){
		if(!result[i] && pInputListeners[i]){
			pInputListeners[i]->OcclusionTestInvisible();
		}
	}
}



// Private Functions
//////////////////////

void deoglOcclusionTest::pCleanUp(){
	if(pInputListeners){
		delete [] pInputListeners;
	}
	if(pInputData){
		delete [] pInputData;
	}
}

void deoglOcclusionTest::pResizeInputData(int size){
	if(size <= pInputDataSize){
		return;
	}
	
	sInputData * const newInputData = new sInputData[size];
	if(pInputData){
		memcpy(newInputData, pInputData, sizeof(sInputData) * pInputDataCount);
		delete [] pInputData;
	}
	pInputDataSize = size;
	pInputData = newInputData;
	
	deoglOcclusionTestListener ** const newListeners = new deoglOcclusionTestListener*[size];
	if(pInputListeners){
		memcpy(newListeners, pInputListeners, sizeof(deoglOcclusionTestListener*) * pInputDataCount);
		delete [] pInputListeners;
	}
	pInputListeners = newListeners;
}
