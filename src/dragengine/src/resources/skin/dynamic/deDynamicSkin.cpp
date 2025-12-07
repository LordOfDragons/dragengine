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

#include "deDynamicSkin.h"
#include "deDynamicSkinManager.h"
#include "renderables/deDSRenderable.h"

#include "../../../systems/modules/graphic/deBaseGraphicDynamicSkin.h"
#include "../../../common/exceptions.h"



// Class deDynamicSkin
////////////////////////

// Constructor, destructor
////////////////////////////

deDynamicSkin::deDynamicSkin(deDynamicSkinManager *manager) : deResource(manager){
	pRenderables = NULL;
	pRenderableCount = 0;
	pRenderableSize = 0;
	
	pPeerGraphic = NULL;
}

deDynamicSkin::~deDynamicSkin(){
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	RemoveAllRenderables();
	if(pRenderables) delete [] pRenderables;
}



// Management
///////////////

deDSRenderable *deDynamicSkin::GetRenderableAt(int index) const{
	if(index < 0 || index >= pRenderableCount) DETHROW(deeInvalidParam);
	
	return pRenderables[index];
}

deDSRenderable *deDynamicSkin::GetRenderableNamed(const char *name) const{
	int index = IndexOfRenderableNamed(name);
	
	if(index == -1){
		return NULL;
		
	}else{
		return pRenderables[index];
	}
}

bool deDynamicSkin::HasRenderable(deDSRenderable *renderable) const{
	return IndexOfRenderable(renderable) != -1;
}

bool deDynamicSkin::HasRenderableNamed(const char *name) const{
	return IndexOfRenderableNamed(name) != -1;
}

int deDynamicSkin::IndexOfRenderable(deDSRenderable *renderable) const{
	if(!renderable) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pRenderableCount; i++){
		if(renderable == pRenderables[i]){
			return i;
		}
	}
	
	return -1;
}

int deDynamicSkin::IndexOfRenderableNamed(const char *name) const{
	if(!name) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pRenderableCount; i++){
		if(strcmp(name, pRenderables[i]->GetName()) == 0){
			return i;
		}
	}
	
	return -1;
}

void deDynamicSkin::AddRenderable(deDSRenderable *renderable){
	if(!renderable || HasRenderableNamed(renderable->GetName())) DETHROW(deeInvalidParam);
	
	if(pRenderableCount == pRenderableSize){
		int newSize = pRenderableSize * 3 / 2 + 1;
		deDSRenderable **newArray = new deDSRenderable*[newSize];
		if(pRenderables){
			memcpy(newArray, pRenderables, sizeof(deDSRenderable*) * pRenderableSize);
			delete [] pRenderables;
		}
		pRenderables = newArray;
		pRenderableSize = newSize;
	}
	
	pRenderables[pRenderableCount] = renderable;
	pRenderableCount++;
	
	if(pPeerGraphic){
		pPeerGraphic->RenderableAdded(pRenderableCount - 1, renderable);
	}
}

void deDynamicSkin::RemoveRenderable(deDSRenderable *renderable){
	int i, index = IndexOfRenderable(renderable);
	if(index == -1) DETHROW(deeInvalidParam);
	
	if(pPeerGraphic){
		pPeerGraphic->RenderableRemoved(index, renderable);
	}
	
	for(i=index+1; i<pRenderableCount; i++){
		pRenderables[i - 1] = pRenderables[i];
	}
	pRenderableCount--;
	
	delete renderable;
}

void deDynamicSkin::RemoveRenderableNamed(const char *name){
	int i, index = IndexOfRenderableNamed(name);
	
	if(index != -1){
		if(pPeerGraphic){
			pPeerGraphic->RenderableRemoved(index, pRenderables[index]);
		}
		
		delete pRenderables[index];
		
		for(i=index+1; i<pRenderableCount; i++){
			pRenderables[i - 1] = pRenderables[i];
		}
		pRenderableCount--;
	}
}

void deDynamicSkin::RemoveAllRenderables(){
	if(pRenderableCount > 0){
		if(pPeerGraphic){
			pPeerGraphic->AllRenderablesRemoved();
		}
		
		while(pRenderableCount > 0){
			pRenderableCount--;
			delete pRenderables[pRenderableCount];
		}
	}
}



void deDynamicSkin::NotifyRenderableChanged(int index){
	if(index < 0 || index >= pRenderableCount) DETHROW(deeInvalidParam);
	
	if(pPeerGraphic){
		pPeerGraphic->RenderableChanged(index, pRenderables[index]);
	}
}




// System Peers
/////////////////

void deDynamicSkin::SetPeerGraphic(deBaseGraphicDynamicSkin *peer){
	if(pPeerGraphic) delete pPeerGraphic;
	pPeerGraphic = peer;
}
