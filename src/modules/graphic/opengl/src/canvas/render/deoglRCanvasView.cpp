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

#include "deoglRCanvasView.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../target/deoglRenderTarget.h"
#include "../../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvasView
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvasView::deoglRCanvasView(deoglRenderThread &renderThread) :
deoglRCanvas(renderThread),
pPaintTracker(0),
pResizeRenderTarget(false)
{
	LEAK_CHECK_CREATE(renderThread, CanvasView);
}

deoglRCanvasView::~deoglRCanvasView(){
	LEAK_CHECK_FREE(GetRenderThread(), CanvasView);
	RemoveAllChildren();
}



// Management
///////////////

void deoglRCanvasView::AddChild(deoglRCanvas *canvas){
	if(!canvas){
		DETHROW(deeInvalidParam);
	}
	
	const float order = canvas->GetOrder();
	const int count = pChildren.GetCount();
	int i;
	
	// NOTE we could use here a binary insertion algorithm since the list is sorted.
	//      for the beginning though it should not be required since the number of
	//      child canvas are low so a linear search is fine.
	for(i=0; i<count; i++){
		const deoglRCanvas &child = *((deoglRCanvas*)pChildren.GetAt(i));
		
		if(order < child.GetOrder()){
			pChildren.Insert(canvas, i);
			return;
		}
	}
	
	pChildren.Add(canvas);
}

void deoglRCanvasView::RemoveAllChildren(){
	pChildren.RemoveAll();
}

bool deoglRCanvasView::HasNoChildren() const{
	return pChildren.GetCount() == 0;
}



void deoglRCanvasView::IncrementPaintTracker(){
	pPaintTracker++;
	
	if(pRenderTarget){
		pRenderTarget->SetTextureDirty(true);
	}
}

void deoglRCanvasView::SetResizeRenderTarget(){
	pResizeRenderTarget = true;
}

void deoglRCanvasView::PrepareRenderTarget(const deoglRenderPlanMasked *renderPlanMask,
int componentCount, int bitCount){
	PrepareForRender(renderPlanMask);
	
	if(pRenderTarget && pRenderTarget->GetComponentCount() == componentCount
	&& pRenderTarget->GetBitCount() == bitCount){
		if(pResizeRenderTarget){
			pRenderTarget->SetSize(decVector2(GetSize()).Round());
			pRenderTarget->PrepareTexture();
			pResizeRenderTarget = false;
		}
		
	}else{
		if(pRenderTarget){
			pRenderTarget = nullptr;
		}
		
		pRenderTarget = deoglRenderTarget::Ref::New(GetRenderThread(),
			decVector2(GetSize()).Round(), componentCount, bitCount);
		pRenderTarget->PrepareTexture();
		pResizeRenderTarget = false;
	}
}

void deoglRCanvasView::RenderRenderTarget(const deoglRenderPlanMasked *renderPlanMask){
	PrepareForRenderRender(renderPlanMask);
	
	if(!pRenderTarget->GetTextureDirty()){
		return;
	}
	
	// mark texture no more dirty although not updated yet. this prevents re-entrant loops
	// due to the canvas being used in a dynamic skin in the same world it is rendering
	pRenderTarget->SetTextureDirty(false);
	
	// prepare and activate framebuffer
	pRenderTarget->PrepareFramebuffer();
	GetRenderThread().GetFramebuffer().Activate(pRenderTarget->GetFBO());
	
	// render content
	deoglRenderCanvasContext context(*this, pRenderTarget->GetFBO(),
		decPoint(), pRenderTarget->GetSize(), false, renderPlanMask);
	// for rendering into the render target the canvas position and transform has to be negated.
	// this way rendering with the position and transform as used for regular rendering cancels
	// each other out resulting in an identity transformation. this way no second code path is
	// required.
	context.SetTransform(GetTransform().Invert().ToTexMatrix2() * context.GetTransform());
	//context.UpdateTransformMask();
	context.SetTCTransformMask(*pRenderTarget);
	
	GetRenderThread().GetRenderers().GetCanvas().Prepare(context);
	
	// clear the render target. this is required for situations where transparent overlays
	// are rendered with children canvas not covering all pixels
	const GLfloat clearColor[4] = {0.0f, 0.0f, 0.0f,
		pRenderTarget->GetComponentCount() == 4 ? 0.0f : 1.0f};
	OGL_CHECK(GetRenderThread(), pglClearBufferfv(GL_COLOR, 0, clearColor));
	
	// render content
	Render(context);
	
	// release framebuffer
	pRenderTarget->ReleaseFramebuffer(); // temporary
}



void deoglRCanvasView::PrepareForRender(const deoglRenderPlanMasked *renderPlanMask){
	const int count = pChildren.GetCount();
	if(count == 0){
		return;
	}
	
	deoglRCanvas::PrepareForRender(renderPlanMask);
	
	int i;
	for(i=0; i<count; i++){
		deoglRCanvas &child = *((deoglRCanvas*)pChildren.GetAt(i));
		if(child.GetVisible()){
			child.PrepareForRender(renderPlanMask);
		}
	}
}

void deoglRCanvasView::PrepareForRenderRender(const deoglRenderPlanMasked *renderPlanMask){
	const int count = pChildren.GetCount();
	if(count == 0){
		return;
	}
	
	deoglRCanvas::PrepareForRenderRender(renderPlanMask);
	
	int i;
	for(i=0; i<count; i++){
		deoglRCanvas &child = *((deoglRCanvas*)pChildren.GetAt(i));
		if(child.GetVisible()){
			child.PrepareForRenderRender(renderPlanMask);
		}
	}
}

void deoglRCanvasView::Render(const deoglRenderCanvasContext &context){
	const deoglRenderCanvasContext childContext(context, *this);
	if(childContext.IsZeroClip()){
		return;
	}
	
	// render children
	const int count = pChildren.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglRCanvas &child = *((deoglRCanvas*)pChildren.GetAt(i));
		if(child.GetVisible()){
			child.Render(childContext);
		}
	}
}
