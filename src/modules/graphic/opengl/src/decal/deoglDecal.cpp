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

#include "deoglDecal.h"
#include "deoglRDecal.h"
#include "../deGraphicOpenGl.h"
#include "../component/deoglComponent.h"
#include "../skin/deoglSkin.h"
#include "../skin/deoglRSkin.h"
#include "../skin/dynamic/deoglDynamicSkin.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>



// Class deoglDecal
/////////////////////

// Constructor, destructor
////////////////////////////

deoglDecal::deoglDecal(deGraphicOpenGl &ogl, const deDecal &decal) :
pOgl(ogl),
pDecal(decal),
pRDecal(NULL),

pDynamicSkin(NULL),

pDirtyVBO(true),
pDirtyGeometry(true),
pDirtyTransform(true),
pDirtySkin(true),
pDirtyDynamicSkin(true),
pDirtyVisibility(true),
pDirtyParamBlocks(false),
pDirtyRenderableMapping(true),
pDirtyStaticTexture(true),

pDynamicSkinRequiresSync(true),

pNotifyTextureChanged(false),
pNotifyTUCChanged(false),

pParentComponent(NULL)
{
	try{
		pRDecal = new deoglRDecal(ogl.GetRenderThread());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglDecal::~deoglDecal(){
	pCleanUp();
}



// Management
///////////////

void deoglDecal::SyncToRender(){
	if(pDirtyGeometry){
		pRDecal->SetPosition(pDecal.GetPosition());
		pRDecal->SetOrientation(pDecal.GetOrientation());
		pRDecal->SetSize(pDecal.GetSize());
		pDirtyGeometry = false;
	}
	
	if(pDirtyTransform){
		pRDecal->SetTransform(pDecal.GetTransform());
		pDirtyTransform = false;
	}
	
	if(pDirtyVisibility){
		pRDecal->SetVisible(pDecal.GetVisible());
		pDirtyVisibility = false;
	}
	
	pSyncSkin();
	pSyncDynamicSkin();
	
	if(pDirtyRenderableMapping){
		pDirtyRenderableMapping = false;
		pRDecal->UpdateRenderableMapping();
		
		// we have to do this here and not in DirtyRenderableMapping() because
		// DirtyRenderableMapping() can be called between the UpdateRenderableMapping()
		// call above and the NotifyTexturesChanged() call below. if this happens the
		// pNotifyTexturesChanged flag will be cleared below while the
		// pDirtyRenderableMapping is true. this causes pNotifyTexturesChanged to be
		// not called the next time UpdateRenderableMapping() above is called. this in
		// turn causes listeners to miss an update and working with old data
		pNotifyTextureChanged = true;
	}
	
	if(pDirtyVBO){
		pDirtyVBO = false;
		pRDecal->SetDirtyVBO();
		pRDecal->SetDirtyGIBVH();
		pRDecal->NotifyGeometryChanged();
	}
	
	if(pDirtyParamBlocks){
		pDirtyParamBlocks = false;
		pRDecal->MarkParamBlocksDirty();
	}
	
	if(pDirtyStaticTexture){
		pDirtyStaticTexture = false;
		pRDecal->UpdateStaticTexture();
	}
	if(pNotifyTextureChanged){
		pNotifyTextureChanged = false;
		pRDecal->NotifyTextureChanged();
	}
	if(pNotifyTUCChanged){
		pNotifyTUCChanged = false;
		pRDecal->NotifyTUCChanged();
	}
}



void deoglDecal::SetParentComponent(deoglComponent *component){
	pParentComponent = component;
}



// Dynamic skin listener
//////////////////////////

void deoglDecal::DynamicSkinDestroyed(){
	pDynamicSkin = NULL;
}

void deoglDecal::DynamicSkinRenderablesChanged(){
	pDynamicSkinRequiresSync = true;
	pDirtyRenderableMapping = true;
	pDirtyStaticTexture = true;
	pNotifyTextureChanged = true;
	pNotifyTUCChanged = true;
	pRequiresSync();
}

void deoglDecal::DynamicSkinRenderableChanged(deoglDSRenderable&){
	pDynamicSkinRequiresSync = true;
	pDirtyRenderableMapping = true;
	pDirtyStaticTexture = true;
	pNotifyTextureChanged = true;
	pNotifyTUCChanged = true;
	pRequiresSync();
}

void deoglDecal::DynamicSkinRenderableRequiresSync(deoglDSRenderable&){
	pDynamicSkinRequiresSync = true;
	pDirtyStaticTexture = true;
	pNotifyTextureChanged = true;
	pRequiresSync();
}



// Notifications
//////////////////

void deoglDecal::GeometryChanged(){
	pDirtyGeometry = true;
	pDirtyVBO = true;
	
	pRequiresSync();
}

void deoglDecal::TransformChanged(){
	pDirtyTransform = true;
	pDirtyParamBlocks = true;
	
	pRequiresSync();
}

void deoglDecal::SkinChanged(){
	pDirtySkin  = true;
	pDirtyRenderableMapping = true;
	pDirtyStaticTexture = true;
	pNotifyTextureChanged = true;
	pNotifyTUCChanged = true;
	
	pRequiresSync();
}

void deoglDecal::DynamicSkinChanged(){
	if(pDynamicSkin){
		pDynamicSkin->RemoveListener(this);
	}
	
	if(pDecal.GetDynamicSkin()){
		pDynamicSkin = (deoglDynamicSkin*)pDecal.GetDynamicSkin()->GetPeerGraphic();
		pDynamicSkin->AddListener(this);
		
	}else{
		pDynamicSkin = NULL;
	}
	
	pDirtyDynamicSkin = true;
	pDirtyRenderableMapping = true;
	pDynamicSkinRequiresSync = true;
	pDirtyStaticTexture = true;
	pNotifyTextureChanged = true;
	pNotifyTUCChanged = true;
	
	pRequiresSync();
}

void deoglDecal::VisibleChanged(){
	pDirtyVisibility = true;
	
	pRequiresSync();
}



// Private Functions
//////////////////////

void deoglDecal::pCleanUp(){
	if(pRDecal){
		pRDecal->FreeReference();
	}
	
	if(pDynamicSkin){
		pDynamicSkin->RemoveListener(this);
	}
}



void deoglDecal::pSyncSkin(){
	if(!pDirtySkin){
		return;
	}
	
	if(pDecal.GetSkin()){
		pRDecal->SetSkin(((deoglSkin*)pDecal.GetSkin()->GetPeerGraphic())->GetRSkin());
		
	}else{
		pRDecal->SetSkin(NULL);
	}
	
	pDirtySkin = false;
	
	pRequiresSync();
}

void deoglDecal::pSyncDynamicSkin(){
	if(pDirtyDynamicSkin){
		if(pDynamicSkin){
			pRDecal->SetDynamicSkin(pDynamicSkin->GetRDynamicSkin());
			
		}else{
			pRDecal->SetDynamicSkin(NULL);
		}
		
		pDirtyDynamicSkin = false;
	}
	
	if(pDynamicSkinRequiresSync){
		pDynamicSkinRequiresSync = false;
		if(pDynamicSkin){
			pDynamicSkin->SyncToRender();
		}
	}
}



void deoglDecal::pRequiresSync(){
	if(pParentComponent){
		pParentComponent->DecalRequiresSync();
	}
}
