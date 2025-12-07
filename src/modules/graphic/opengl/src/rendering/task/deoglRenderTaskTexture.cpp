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

#include "deoglRenderTask.h"
#include "deoglRenderTaskTexture.h"
#include "deoglRenderTaskVAO.h"
#include "shared/deoglRenderTaskSharedVAO.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskTexture
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskTexture::deoglRenderTaskTexture() :
pTexture(NULL),

pVAOCount(0),

pHasVAO(NULL),
pHasVAOCount(0),
pHasVAOSize(0){
}

deoglRenderTaskTexture::~deoglRenderTaskTexture(){
	if(pHasVAO){
		delete [] pHasVAO;
	}
	
	const int vaoCount = pVAOs.GetCount();
	int i;
	for(i=0; i<vaoCount; i++){
		delete (deoglRenderTaskVAO*)pVAOs.GetAt(i);
	}
	pVAOs.RemoveAll();
}



// Management
///////////////

void deoglRenderTaskTexture::Reset(){
	pTexture = NULL;
	pHasVAOCount = 0;
	pVAOCount = 0;
}



int deoglRenderTaskTexture::GetTotalPointCount() const{
	int i, pointCount = 0;
	for(i=0; i<pVAOCount; i++){
		pointCount += ((deoglRenderTaskVAO*)pVAOs.GetAt(i))->GetTotalPointCount();
	}
	return pointCount;
}

int deoglRenderTaskTexture::GetTotalInstanceCount() const{
	int i, instanceCount = 0;
	for(i=0; i<pVAOCount; i++){
		instanceCount += ((deoglRenderTaskVAO*)pVAOs.GetAt(i))->GetInstanceCount();
	}
	return instanceCount;
}

int deoglRenderTaskTexture::GetTotalSubInstanceCount() const{
	int i, subInstanceCount = 0;
	for(i=0; i<pVAOCount; i++){
		subInstanceCount += ((deoglRenderTaskVAO*)pVAOs.GetAt(i))->GetTotalSubInstanceCount();
	}
	return subInstanceCount;
}



void deoglRenderTaskTexture::SetTexture(const deoglRenderTaskSharedTexture *texture){
	pTexture = texture;
}



deoglRenderTaskVAO *deoglRenderTaskTexture::GetVAOAt(int index) const{
	return (deoglRenderTaskVAO*)pVAOs.GetAt(index);
}

deoglRenderTaskVAO *deoglRenderTaskTexture::AddVAO(const deoglRenderTaskSharedVAO *vao){
	if(!vao){
		DETHROW(deeInvalidParam);
	}
	
	const int index = vao->GetIndex();
	
	if(index >= pHasVAOCount){
		if(index >= pHasVAOSize){
			deoglRenderTaskVAO ** const newArray = new deoglRenderTaskVAO*[index + 1];
			
			if(pHasVAO){
				if(pHasVAOCount > 0){
					memcpy(newArray, pHasVAO, sizeof(deoglRenderTaskVAO*) * pHasVAOCount);
				}
				delete [] pHasVAO;
			}
			
			pHasVAO = newArray;
			pHasVAOSize = index + 1;
		}
		
		if(pHasVAOCount <= index){
			memset(pHasVAO + pHasVAOCount, 0, sizeof(deoglRenderTaskVAO*) * (index - pHasVAOCount + 1));
			pHasVAOCount = index + 1;
		}
	}
	
	deoglRenderTaskVAO *rtvao = pHasVAO[index];
	if(rtvao){
		return rtvao;
	}
	
	if(pVAOCount == pVAOs.GetCount()){
		rtvao = new deoglRenderTaskVAO;
		pVAOs.Add(rtvao);
		
	}else{
		rtvao = (deoglRenderTaskVAO*)pVAOs.GetAt(pVAOCount);
		rtvao->Reset();
	}
	pVAOCount++;
	
	rtvao->SetVAO(vao);
	pHasVAO[index] = rtvao;
	return rtvao;
}
