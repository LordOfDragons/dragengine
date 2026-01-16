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
pTexture(nullptr),
pVAOCount(0){
}

deoglRenderTaskTexture::~deoglRenderTaskTexture(){
}



// Management
///////////////

void deoglRenderTaskTexture::Reset(){
	pTexture = nullptr;
	pHasVAO.RemoveAll();
	pVAOCount = 0;
}



int deoglRenderTaskTexture::GetTotalPointCount() const{
	return pVAOs.Inject(0, [](int sum, const deoglRenderTaskVAO &vao){
		return sum + vao.GetTotalPointCount();
	}, 0, pVAOCount);
}

int deoglRenderTaskTexture::GetTotalInstanceCount() const{
	return pVAOs.Inject(0, [](int sum, const deoglRenderTaskVAO &vao){
		return sum + vao.GetInstanceCount();
	}, 0, pVAOCount);
}

int deoglRenderTaskTexture::GetTotalSubInstanceCount() const{
	return pVAOs.Inject(0, [](int sum, const deoglRenderTaskVAO &vao){
		return sum + vao.GetTotalSubInstanceCount();
	}, 0, pVAOCount);
}



void deoglRenderTaskTexture::SetTexture(const deoglRenderTaskSharedTexture *texture){
	pTexture = texture;
}



deoglRenderTaskVAO *deoglRenderTaskTexture::GetVAOAt(int index) const{
	return pVAOs.GetAt(index);
}

deoglRenderTaskVAO *deoglRenderTaskTexture::AddVAO(const deoglRenderTaskSharedVAO *vao){
	DEASSERT_NOTNULL(vao)
	
	const int index = vao->GetIndex();
	
	while(index >= pHasVAO.GetCount()){
		pHasVAO.Add(nullptr);
	}
	
	deoglRenderTaskVAO *rtvao = pHasVAO.GetAt(index);
	if(rtvao){
		return rtvao;
	}
	
	if(pVAOCount == pVAOs.GetCount()){
		pVAOs.Add(deoglRenderTaskVAO::Ref::New());
		rtvao = pVAOs.Last();
		
	}else{
		rtvao = pVAOs.GetAt(pVAOCount);
		rtvao->Reset();
	}
	pVAOCount++;
	
	rtvao->SetVAO(vao);
	pHasVAO.SetAt(index, rtvao);
	return rtvao;
}
