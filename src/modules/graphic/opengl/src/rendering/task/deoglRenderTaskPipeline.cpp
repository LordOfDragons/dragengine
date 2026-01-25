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

pTextureCount(0){
}

deoglRenderTaskPipeline::~deoglRenderTaskPipeline(){
}



// Management
///////////////

void deoglRenderTaskPipeline::Reset(){
	pPipeline = nullptr;
	pHasTexture.RemoveAll();
	pTextureCount = 0;
}



int deoglRenderTaskPipeline::GetTotalPointCount() const{
	return pTextures.Inject(0, 0, pTextureCount, [](int sum, const deoglRenderTaskTexture &t){
		return sum + t.GetTotalPointCount();
	});
}

int deoglRenderTaskPipeline::GetTotalVAOCount() const{
	return pTextures.Inject(0, 0, pTextureCount, [](int sum, const deoglRenderTaskTexture &t){
		return sum + t.GetVAOCount();
	});
}

int deoglRenderTaskPipeline::GetTotalInstanceCount() const{
	return pTextures.Inject(0, 0, pTextureCount, [](int sum, const deoglRenderTaskTexture &t){
		return sum + t.GetTotalInstanceCount();
	});
}

int deoglRenderTaskPipeline::GetTotalSubInstanceCount() const{
	return pTextures.Inject(0, 0, pTextureCount, [](int sum, const deoglRenderTaskTexture &t){
		return sum + t.GetTotalSubInstanceCount();
	});
}



void deoglRenderTaskPipeline::SetPipeline(const deoglPipeline *pipeline){
	pPipeline = pipeline;
}



deoglRenderTaskTexture *deoglRenderTaskPipeline::GetTextureAt(int index) const{
	return pTextures.GetAt(index);
}

deoglRenderTaskTexture *deoglRenderTaskPipeline::AddTexture(const deoglRenderTaskSharedTexture *texture){
	DEASSERT_NOTNULL(texture)
	
	const int index = texture->GetIndex();
	
	while(index >= pHasTexture.GetCount()){
		pHasTexture.Add(nullptr);
	}
	
	deoglRenderTaskTexture *rttexture = pHasTexture.GetAt(index);
	if(rttexture){
		return rttexture;
	}
	
	if(pTextureCount == pTextures.GetCount()){
		pTextures.Add(deoglRenderTaskTexture::Ref::New());
		rttexture = pTextures.Last();
		
	}else{
		rttexture = pTextures.GetAt(pTextureCount);
		rttexture->Reset();
	}
	pTextureCount++;
	
	rttexture->SetTexture(texture);
	pHasTexture.SetAt(index, rttexture);
	return rttexture;
}
