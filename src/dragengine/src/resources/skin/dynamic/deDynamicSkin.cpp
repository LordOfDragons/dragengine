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

deDynamicSkin::deDynamicSkin(deDynamicSkinManager *manager) : deResource(manager),
pPeerGraphic(nullptr){
}

deDynamicSkin::~deDynamicSkin(){
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = nullptr;
	}
}



// Management
///////////////

deDSRenderable *deDynamicSkin::GetRenderableNamed(const char *name) const{
	for(auto &renderable : pRenderables){
		if(renderable->GetName() == name){
			return renderable;
		}
	}
	return nullptr;
}

bool deDynamicSkin::HasRenderable(deDSRenderable *renderable) const{
	return pRenderables.Has(renderable);
}

bool deDynamicSkin::HasRenderableNamed(const char *name) const{
	return pRenderables.HasMatching([&](const deDSRenderable &renderable){
		return renderable.GetName() == name;
	});
}

int deDynamicSkin::IndexOfRenderable(deDSRenderable *renderable) const{
	return pRenderables.IndexOf(renderable);
}

int deDynamicSkin::IndexOfRenderableNamed(const char *name) const{
	if(!name) DETHROW(deeInvalidParam);
	return pRenderables.IndexOfMatching([&](const deDSRenderable &renderable){
		return renderable.GetName() == name;
	});
}

void deDynamicSkin::AddRenderable(deTUniqueReference<deDSRenderable> &&renderable){
	DEASSERT_NOTNULL(renderable)
	if(HasRenderableNamed(renderable->GetName())) DETHROW(deeInvalidParam);
	
	pRenderables.Add(std::move(renderable));
	
	if(pPeerGraphic){
		pPeerGraphic->RenderableAdded(pRenderables.GetCount() - 1, pRenderables.Last());
	}
}

void deDynamicSkin::RemoveRenderable(deDSRenderable *renderable){
	int index = pRenderables.IndexOf(renderable);
	if(index == -1) DETHROW(deeInvalidParam);
	
	if(pPeerGraphic){
		pPeerGraphic->RenderableRemoved(index, renderable);
	}
	
	pRenderables.RemoveFrom(index);
}

void deDynamicSkin::RemoveRenderableNamed(const char *name){
	int index = IndexOfRenderableNamed(name);
	
	if(index != -1){
		if(pPeerGraphic){
			pPeerGraphic->RenderableRemoved(index, pRenderables.GetAt(index));
		}
		
		pRenderables.RemoveFrom(index);
	}
}

void deDynamicSkin::RemoveAllRenderables(){
	if(pRenderables.IsNotEmpty()){
		if(pPeerGraphic){
			pPeerGraphic->AllRenderablesRemoved();
		}
		
		pRenderables.RemoveAll();
	}
}



void deDynamicSkin::NotifyRenderableChanged(int index){
	if(index < 0 || index >= pRenderables.GetCount()) DETHROW(deeInvalidParam);
	
	if(pPeerGraphic){
		pPeerGraphic->RenderableChanged(index, pRenderables.GetAt(index));
	}
}




// System Peers
/////////////////

void deDynamicSkin::SetPeerGraphic(deBaseGraphicDynamicSkin *peer){
	if(pPeerGraphic) delete pPeerGraphic;
	pPeerGraphic = peer;
}
