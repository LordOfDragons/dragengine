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

#include "deoglDynamicSkin.h"
#include "deoglDynamicSkinListener.h"
#include "deoglRDynamicSkin.h"
#include "renderables/deoglDSRenderable.h"
#include "renderables/deoglDSRenderableCamera.h"
#include "renderables/deoglDSRenderableCanvas.h"
#include "renderables/deoglDSRenderableColor.h"
#include "renderables/deoglDSRenderableImage.h"
#include "renderables/deoglDSRenderableValue.h"
#include "renderables/deoglDSRenderableVideoFrame.h"
#include "renderables/render/deoglRDSRenderable.h"
#include "../../deGraphicOpenGl.h"
#include "../../billboard/deoglBillboard.h"
#include "../../component/deoglComponent.h"
#include "../../component/deoglComponentTexture.h"
#include "../../decal/deoglDecal.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderable.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableVisitorIdentify.h>



// Class deoglDynamicSkin
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicSkin::deoglDynamicSkin(deGraphicOpenGl &ogl, const deDynamicSkin &dynamicSkin) :
pOgl(ogl),
pDynamicSkin(dynamicSkin),
pRDynamicSkin(NULL),
pDirtyRenderables(true)
{
	const int renderableCount = dynamicSkin.GetRenderableCount();
	int i;
	
	try{
		pRDynamicSkin.TakeOverWith(ogl.GetRenderThread());
		
		for(i=0; i<renderableCount; i++){
			RenderableAdded(i, dynamicSkin.GetRenderableAt(i));
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglDynamicSkin::~deoglDynamicSkin(){
	pCleanUp();
}



// Management
///////////////

int deoglDynamicSkin::GetRenderableCount() const{
	return pRenderables.GetCount();
}

deoglDSRenderable *deoglDynamicSkin::GetRenderableAt(int index) const{
	return (deoglDSRenderable*)pRenderables.GetAt(index);
}



void deoglDynamicSkin::SyncToRender(){
	const int count = pRenderables.GetCount();
	int i;
	
	if(pDirtyRenderables){
		pRDynamicSkin->RemoveAllRenderables();
		
		for(i=0; i<count; i++){
			deoglDSRenderable &renderable = *((deoglDSRenderable*)pRenderables.GetAt(i));
			renderable.SyncToRender();
			pRDynamicSkin->AddRenderable(renderable.GetRRenderable());
		}
		
		pDirtyRenderables = false;
	}
	
	for(i=0; i<count; i++){
		((deoglDSRenderable*)pRenderables.GetAt(i))->SyncToRender();
	}
}



// Listeners
//////////////

void deoglDynamicSkin::AddListener(deoglDynamicSkinListener *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void deoglDynamicSkin::RemoveListener(deoglDynamicSkinListener *listener){
	pListeners.RemoveIfPresent(listener);
}

void deoglDynamicSkin::NotifyDestroyed(){
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglDynamicSkinListener*)pListeners.GetAt(i))->DynamicSkinDestroyed();
	}
}

void deoglDynamicSkin::NotifyRenderablesChanged(){
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglDynamicSkinListener*)pListeners.GetAt(i))->DynamicSkinRenderablesChanged();
	}
}

void deoglDynamicSkin::NotifyRenderableChanged(deoglDSRenderable &renderable){
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglDynamicSkinListener*)pListeners.GetAt(i))->DynamicSkinRenderableChanged(renderable);
	}
}

void deoglDynamicSkin::NotifyRenderableRequiresSync(deoglDSRenderable &renderable){
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglDynamicSkinListener*)pListeners.GetAt(i))->DynamicSkinRenderableRequiresSync(renderable);
	}
}



// Notifications
//////////////////

void deoglDynamicSkin::RenderableAdded(int, deDSRenderable *renderable){
	if(!renderable){
		DETHROW(deeInvalidParam);
	}
	
	deDSRenderableVisitorIdentify identify;
	renderable->Visit(identify);
	
	switch(identify.GetType()){
	case deDSRenderableVisitorIdentify::eptCamera:
		pRenderables.Add(new deoglDSRenderableCamera(*this, identify.CastToCamera()));
		break;
		
	case deDSRenderableVisitorIdentify::eptCanvas:
		pRenderables.Add(new deoglDSRenderableCanvas(*this, identify.CastToCanvas()));
		break;
		
	case deDSRenderableVisitorIdentify::eptColor:
		pRenderables.Add(new deoglDSRenderableColor(*this, identify.CastToColor()));
		break;
		
	case deDSRenderableVisitorIdentify::eptImage:
		pRenderables.Add(new deoglDSRenderableImage(*this, identify.CastToImage()));
		break;
		
	case deDSRenderableVisitorIdentify::eptValue:
		pRenderables.Add(new deoglDSRenderableValue(*this, identify.CastToValue()));
		break;
		
	case deDSRenderableVisitorIdentify::eptVideoFrame:
		pRenderables.Add(new deoglDSRenderableVideoFrame(*this, identify.CastToVideoFrame()));
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	pDirtyRenderables = true;
	NotifyRenderablesChanged();
}

void deoglDynamicSkin::RenderableChanged(int index, deDSRenderable *renderable){
	((deoglDSRenderable*)pRenderables.GetAt(index))->RenderableChanged();
}

void deoglDynamicSkin::RenderableRemoved(int index, deDSRenderable*){
	delete (deoglDSRenderable*)pRenderables.GetAt(index);
	pRenderables.RemoveFrom(index);
	
	pDirtyRenderables = true;
	NotifyRenderablesChanged();
}

void deoglDynamicSkin::AllRenderablesRemoved(){
	const int count = pRenderables.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delete (deoglDSRenderable*)pRenderables.GetAt(i);
	}
	pRenderables.RemoveAll();
	
	pDirtyRenderables = true;
	NotifyRenderablesChanged();
}



// Private Functions
//////////////////////

void deoglDynamicSkin::pCleanUp(){
	int i, count = pRenderables.GetCount();
	for(i=0; i<count; i++){
		delete (deoglDSRenderable*)pRenderables.GetAt(i);
	}
	pRenderables.RemoveAll();
	
	
	// notify owners we are about to be deleted. required since owners hold only a weak pointer
	// to the dynamic skin and are notified only after switching to a new dynamic skin. in this
	// case they can not use the old pointer to remove themselves from the dynamic skin
	NotifyDestroyed();
}
