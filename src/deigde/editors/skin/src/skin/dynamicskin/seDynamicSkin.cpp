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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seDynamicSkin.h"
#include "seDynamicSkinRenderable.h"
#include "../seSkin.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinManager.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Skin Editor"



// Class seDynamicSkin
////////////////////////

// Constructor, destructor
////////////////////////////

seDynamicSkin::seDynamicSkin(seSkin *parentSkin){
	if(!parentSkin){
		DETHROW(deeInvalidParam);
	}
	
	pParentSkin = parentSkin;
	
	pEngDynamicSkin = nullptr;
	
	pActiveRenderable = nullptr;
	
	try{
		pEngDynamicSkin = parentSkin->GetEngine()->GetDynamicSkinManager()->CreateDynamicSkin();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

seDynamicSkin::~seDynamicSkin(){
	pCleanUp();
}



// Management
///////////////

void seDynamicSkin::Dispose(){
	RemoveAllRenderables();
}

void seDynamicSkin::Reset(){
}

void seDynamicSkin::Update(float elapsed){
	pRenderables.Visit([&](seDynamicSkinRenderable &r){
		r.Update(elapsed);
	});
}



// Renderables
////////////////

void seDynamicSkin::AddRenderable(seDynamicSkinRenderable *renderable){
	if(!renderable || pRenderables.HasMatching([&](const seDynamicSkinRenderable &r){
		return r.GetName() == renderable->GetName();
	})){
		DETHROW(deeInvalidParam);
	}
	
	pRenderables.Add(renderable);
	renderable->SetDynamicSkin(this);
	
	pParentSkin->NotifyDynamicSkinRenderableStructureChanged();
	
	if(!pActiveRenderable){
		SetActiveRenderable(renderable);
	}
}

void seDynamicSkin::RemoveRenderable(seDynamicSkinRenderable *renderable){
	if(!pRenderables.Has(renderable)){
		DETHROW(deeInvalidParam);
	}
	
	if(renderable->GetActive()){
		if(pRenderables.GetCount() > 1){
			seDynamicSkinRenderable *activeRenderable = pRenderables.GetAt(0);
			
			if(activeRenderable == renderable){
				activeRenderable = pRenderables.GetAt(1);
			}
			
			SetActiveRenderable(activeRenderable);
			
		}else{
			SetActiveRenderable(nullptr);
		}
	}
	
	renderable->SetDynamicSkin(nullptr);
	pRenderables.Remove(renderable);
	
	pParentSkin->NotifyDynamicSkinRenderableStructureChanged();
}

void seDynamicSkin::RemoveAllRenderables(){
	SetActiveRenderable(nullptr);
	
	pRenderables.Visit([&](seDynamicSkinRenderable &r){
		r.SetDynamicSkin(nullptr);
	});
	pRenderables.RemoveAll();
	
	pParentSkin->NotifyDynamicSkinRenderableStructureChanged();
}

bool seDynamicSkin::HasActiveRenderable() const{
	return pActiveRenderable != nullptr;
}

void seDynamicSkin::SetActiveRenderable(seDynamicSkinRenderable *renderable){
	if(renderable != pActiveRenderable){
		if(pActiveRenderable){
			pActiveRenderable->SetActive(false);
		}
		
		pActiveRenderable = renderable;
		
		if(renderable){
			renderable->SetActive(true);
		}
		
		pParentSkin->NotifyDynamicSkinActiveRenderableChanged();
	}
}



// Private Functions
//////////////////////

void seDynamicSkin::pCleanUp(){
	SetActiveRenderable(nullptr);
	RemoveAllRenderables();
}
