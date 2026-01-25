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
pInputData(256, {})
{
	pSSBOInput = deoglSPBlockSSBO::Ref::New(renderThread, deoglSPBlockSSBO::etStream);
	pSSBOInput->SetRowMajor(renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working());
	pSSBOInput->SetParameterCount(2);
	pSSBOInput->GetParameterAt(0).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1); // vec3 minExtend
	pSSBOInput->GetParameterAt(1).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1); // vec3 maxExtend
	pSSBOInput->MapToStd140();
	pSSBOInput->SetBindingPoint(0);
	
	pSSBOResult = deoglSPBlockSSBO::Ref::New(renderThread, deoglSPBlockSSBO::etRead);
	pSSBOResult->SetRowMajor(renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working());
	pSSBOResult->SetParameterCount(1);
	pSSBOResult->GetParameterAt(0).SetAll(deoglSPBParameter::evtBool, 4, 1, 1); // bvec4 result
	pSSBOResult->MapToStd140();
	pSSBOResult->SetBindingPoint(1);
	pSSBOResult->EnsureBuffer();
}

deoglOcclusionTest::~deoglOcclusionTest() = default;



// Management
///////////////

int deoglOcclusionTest::AddInputData(const decVector &minExtend, const decVector &maxExtend,
deoglOcclusionTestListener *listener){
	pInputData.Add({});
	sInputData &inputData = pInputData.Last();
	inputData.minExtend.x = (GLfloat)minExtend.x;
	inputData.minExtend.y = (GLfloat)minExtend.y;
	inputData.minExtend.z = (GLfloat)minExtend.z;
	inputData.maxExtend.x = (GLfloat)maxExtend.x;
	inputData.maxExtend.y = (GLfloat)maxExtend.y;
	inputData.maxExtend.z = (GLfloat)maxExtend.z;
	
	pInputListeners.Add(listener);
	
	return pInputData.GetCount() - 1;
}

void deoglOcclusionTest::RemoveAllInputData(){
	pInputData.RemoveAll();
	pInputListeners.RemoveAll();
}



void deoglOcclusionTest::UpdateSSBO(){
	const int elementCount = decMath::max(pInputData.GetCount(), 1);
	if(elementCount > pSSBOInput->GetElementCount()){
		pSSBOInput->SetElementCount(elementCount);
		pSSBOInput->EnsureBuffer();
	}
	
	if(pInputData.IsNotEmpty()){
		deoglSPBlockSSBO &vbo = pSSBOInput;
		const deoglSPBMapBuffer mapped(vbo);
		pInputData.VisitIndexed([&](int i, const sInputData &inputData){
			vbo.SetParameterDataVec3(0, i, inputData.minExtend);
			vbo.SetParameterDataVec3(1, i, inputData.maxExtend);
		});
	}
	
	const int resultCount = (elementCount - 1) / 4 + 1;
	if(resultCount > pSSBOResult->GetElementCount()){
		pSSBOResult->SetElementCount(resultCount);
		pSSBOResult->EnsureBuffer();
	}
}

void deoglOcclusionTest::UpdateResults(){
	if(pInputData.IsEmpty() || !pSSBOResult){
		return;
	}
	
	const int resultCount = (pInputData.GetCount() - 1) / 4 + 1;
	const deoglSPBMapBufferRead mapped(pSSBOResult, 0, resultCount);
	const uint32_t * const result = (const uint32_t *)pSSBOResult->GetMappedBuffer();
	int i;
	
	for(i=0; i<pInputData.GetCount(); i++){
		if(!result[i] && pInputListeners.GetCount() > i && pInputListeners[i]){
			pInputListeners[i]->OcclusionTestInvisible();
		}
	}
}
