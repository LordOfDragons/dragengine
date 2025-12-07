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

#include "deoglRDynamicSkin.h"
#include "renderables/render/deoglRDSRenderable.h"
#include "../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglRDynamicSkin
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRDynamicSkin::deoglRDynamicSkin(deoglRenderThread &renderThread) :
pRenderThread(renderThread)
{
	LEAK_CHECK_CREATE(renderThread, DynamicSkin);
}

deoglRDynamicSkin::~deoglRDynamicSkin(){
	LEAK_CHECK_FREE(pRenderThread, DynamicSkin);
}



// Management
///////////////

int deoglRDynamicSkin::GetRenderableCount() const{
	return pRenderables.GetCount();
}

deoglRDSRenderable *deoglRDynamicSkin::GetRenderableAt(int index) const{
	return (deoglRDSRenderable*)pRenderables.GetAt(index);
}

deoglRDSRenderable *deoglRDynamicSkin::GetRenderableNamed(const char *name) const{
	const int count = pRenderables.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglRDSRenderable * const renderable = (deoglRDSRenderable*)pRenderables.GetAt(i);
		if(renderable->GetName() == name){
			return renderable;
		}
	}
	
	return NULL;
}

int deoglRDynamicSkin::IndexOfRenderableNamed(const char *name) const{
	const int count = pRenderables.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((deoglRDSRenderable*)pRenderables.GetAt(i))->GetName() == name){
			return i;
		}
	}
	
	return -1;
}

void deoglRDynamicSkin::AddRenderable(deoglRDSRenderable *renderable){
	pRenderables.Add(renderable);
}

void deoglRDynamicSkin::RemoveAllRenderables(){
	pRenderables.RemoveAll();
}
