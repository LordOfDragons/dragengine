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
#include "deoglRenderTaskPipeline.h"
#include "deoglRenderTaskTexture.h"
#include "shared/deoglRenderTaskSharedTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskPipeline
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskPipeline::deoglRenderTaskPipeline() :
pPipeline(nullptr),

pTextureCount(0),

pHasTexture(nullptr),
pHasTextureCount(0),
pHasTextureSize(0){
}

deoglRenderTaskPipeline::~deoglRenderTaskPipeline(){
	if(pHasTexture){
		delete [] pHasTexture;
	}
	
	const int textureCount = pTextures.GetCount();
	int i;
	for(i=0; i<textureCount; i++){
		delete (deoglRenderTaskTexture*)pTextures.GetAt(i);
	}
	pTextures.RemoveAll();
}



// Management
///////////////

void deoglRenderTaskPipeline::Reset(){
	pPipeline = nullptr;
	pHasTextureCount = 0;
	pTextureCount = 0;
}



int deoglRenderTaskPipeline::GetTotalPointCount() const{
	int i, pointCount = 0;
	for(i=0; i<pTextureCount; i++){
		pointCount += ((deoglRenderTaskTexture*)pTextures.GetAt(i))->GetTotalPointCount();
	}
	return pointCount;
}

int deoglRenderTaskPipeline::GetTotalVAOCount() const{
	int i, vaoCount = 0;
	for(i=0; i<pTextureCount; i++){
		vaoCount += ((deoglRenderTaskTexture*)pTextures.GetAt(i))->GetVAOCount();
	}
	return vaoCount;
}

int deoglRenderTaskPipeline::GetTotalInstanceCount() const{
	int i, instanceCount = 0;
	for(i=0; i<pTextureCount; i++){
		instanceCount += ((deoglRenderTaskTexture*)pTextures.GetAt(i))->GetTotalInstanceCount();
	}
	return instanceCount;
}

int deoglRenderTaskPipeline::GetTotalSubInstanceCount() const{
	int i, subInstanceCount = 0;
	for(i=0; i<pTextureCount; i++){
		subInstanceCount += ((deoglRenderTaskTexture*)pTextures.GetAt(i))->GetTotalSubInstanceCount();
	}
	return subInstanceCount;
}



void deoglRenderTaskPipeline::SetPipeline(const deoglPipeline *pipeline){
	pPipeline = pipeline;
}



deoglRenderTaskTexture *deoglRenderTaskPipeline::GetTextureAt(int index) const{
	return (deoglRenderTaskTexture*)pTextures.GetAt(index);
}

deoglRenderTaskTexture *deoglRenderTaskPipeline::AddTexture(const deoglRenderTaskSharedTexture *texture){
	DEASSERT_NOTNULL(texture)
	
	const int index = texture->GetIndex();
	
	if(index >= pHasTextureCount){
		if(index >= pHasTextureSize){
			deoglRenderTaskTexture ** const newArray = new deoglRenderTaskTexture*[index + 1];
			
			if(pHasTexture){
				if(pHasTextureCount > 0){
					memcpy(newArray, pHasTexture, sizeof(deoglRenderTaskTexture*) * pHasTextureCount);
				}
				delete [] pHasTexture;
			}
			
			pHasTexture = newArray;
			pHasTextureSize = index + 1;
		}
		
		if(pHasTextureCount <= index){
			memset(pHasTexture + pHasTextureCount, 0, sizeof(deoglRenderTaskTexture*) * (index - pHasTextureCount + 1));
			pHasTextureCount = index + 1;
		}
	}
	
	deoglRenderTaskTexture *rttexture = pHasTexture[index];
	if(rttexture){
		return rttexture;
	}
	
	if(pTextureCount == pTextures.GetCount()){
		rttexture = new deoglRenderTaskTexture;
		pTextures.Add(rttexture);
		
	}else{
		rttexture = (deoglRenderTaskTexture*)pTextures.GetAt(pTextureCount);
		rttexture->Reset();
	}
	pTextureCount++;
	
	rttexture->SetTexture(texture);
	pHasTexture[index] = rttexture;
	return rttexture;
}
