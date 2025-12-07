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
	
	pEngDynamicSkin = NULL;
	
	pActiveRenderable = NULL;
	
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
	const int count = pRenderableList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pRenderableList.GetAt(i)->Update(elapsed);
	}
}



// Renderables
////////////////

void seDynamicSkin::AddRenderable(seDynamicSkinRenderable *renderable){
	if(!renderable || pRenderableList.HasNamed(renderable->GetName().GetString())){
		DETHROW(deeInvalidParam);
	}
	
	pRenderableList.Add(renderable);
	renderable->SetDynamicSkin(this);
	
	pParentSkin->NotifyDynamicSkinRenderableStructureChanged();
	
	if(!pActiveRenderable){
		SetActiveRenderable(renderable);
	}
}

void seDynamicSkin::RemoveRenderable(seDynamicSkinRenderable *renderable){
	if(!pRenderableList.Has(renderable)){
		DETHROW(deeInvalidParam);
	}
	
	if(renderable->GetActive()){
		if(pRenderableList.GetCount() > 1){
			seDynamicSkinRenderable *activeRenderable = pRenderableList.GetAt(0);
			
			if(activeRenderable == renderable){
				activeRenderable = pRenderableList.GetAt(1);
			}
			
			SetActiveRenderable(activeRenderable);
			
		}else{
			SetActiveRenderable(NULL);
		}
	}
	
	renderable->SetDynamicSkin(NULL);
	pRenderableList.Remove(renderable);
	
	pParentSkin->NotifyDynamicSkinRenderableStructureChanged();
}

void seDynamicSkin::RemoveAllRenderables(){
	const int count = pRenderableList.GetCount();
	int t;
	
	SetActiveRenderable(NULL);
	
	for(t=0; t<count; t++){
		pRenderableList.GetAt(t)->SetDynamicSkin(NULL);
	}
	pRenderableList.RemoveAll();
	
	pParentSkin->NotifyDynamicSkinRenderableStructureChanged();
}

bool seDynamicSkin::HasActiveRenderable() const{
	return pActiveRenderable != NULL;
}

void seDynamicSkin::SetActiveRenderable(seDynamicSkinRenderable *renderable){
	if(renderable != pActiveRenderable){
		if(pActiveRenderable){
			pActiveRenderable->SetActive(false);
			pActiveRenderable->FreeReference();
		}
		
		pActiveRenderable = renderable;
		
		if(renderable){
			renderable->AddReference();
			renderable->SetActive(true);
		}
		
		pParentSkin->NotifyDynamicSkinActiveRenderableChanged();
	}
}



// Private Functions
//////////////////////

void seDynamicSkin::pCleanUp(){
	SetActiveRenderable(NULL);
	RemoveAllRenderables();
	
	if(pEngDynamicSkin){
		pEngDynamicSkin->FreeReference();
	}
}
