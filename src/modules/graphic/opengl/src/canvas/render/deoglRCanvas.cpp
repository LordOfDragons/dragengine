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

#include "deoglRCanvas.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../target/deoglRenderTarget.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvas
///////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvas::deoglRCanvas(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pOrder(0.0f),
pTransparency(1.0f),
pBlendMode(deCanvas::ebmBlend),
pMask(NULL),
pVisible(true),
pMaskRenderTarget(NULL){
}

deoglRCanvas::~deoglRCanvas(){
}



// Management
///////////////

void deoglRCanvas::SetPosition(const decVector2 &position){
	pPosition = position;
}

void deoglRCanvas::SetSize(const decVector2 &size){
	pSize = size;
}

void deoglRCanvas::SetTransform(const decTexMatrix2 &transform){
	pTransform = transform;
}

void deoglRCanvas::SetColorTransform(const decColorMatrix &transform){
	pColorTransform = transform;
}

void deoglRCanvas::SetOrder(float order){
	pOrder = order;
}

void deoglRCanvas::SetTransparency(float transparency){
	pTransparency = transparency;
}

void deoglRCanvas::SetBlendMode(deCanvas::eBlendModes mode){
	pBlendMode = mode;
}

void deoglRCanvas::SetMask(deoglRCanvas *mask){
	if(pMask == mask){
		return;
	}
	
	
	pMask = mask;
}

void deoglRCanvas::SetVisible(bool visible){
	pVisible = visible;
}

void deoglRCanvas::DirtyMaskRenderTarget(){
	if(pMaskRenderTarget){
		pMaskRenderTarget->SetTextureDirty(true);
	}
}



void deoglRCanvas::PrepareForRender(const deoglRenderPlanMasked *renderPlanMask){
	if(!pMask){
		return;
	}
	
	pMask->PrepareForRender(renderPlanMask);
	
	const int width = (int)(GetSize().x + 0.5f);
	const int height = (int)(GetSize().y + 0.5f);
	
	if(pMaskRenderTarget){
		pMaskRenderTarget->SetSize(decPoint(width, height));
		
	}else{
		pMaskRenderTarget.TakeOverWith(GetRenderThread(), decPoint(width, height), 1, 8);
	}
}

void deoglRCanvas::PrepareForRenderRender(const deoglRenderPlanMasked *renderPlanMask){
	if(!pMask || !pMaskRenderTarget->GetTextureDirty()){
		return;
	}
	
	pMaskRenderTarget->SetTextureDirty(false);
	
	pMaskRenderTarget->PrepareFramebuffer();
	GetRenderThread().GetFramebuffer().Activate(pMaskRenderTarget->GetFBO());
	
	deoglRenderCanvasContext context(*pMask, pMaskRenderTarget->GetFBO(),
		decPoint(), pMaskRenderTarget->GetSize(), false, renderPlanMask);
	
	// for rendering into the render target the canvas position and transform has
	// to be negated. this way rendering with the position and transform as used
	// for regular rendering cancels each other out resulting in an identity
	// transformation. this way no second code path is required.
	context.SetTransform(pMask->GetTransform().Invert().ToTexMatrix2() * context.GetTransform());
	//context.UpdateTransformMask();
	context.SetTCTransformMask(*pMaskRenderTarget);
	
	GetRenderThread().GetRenderers().GetCanvas().Prepare(context);
	pMask->Render(context);
	
	// TODO we need to merge this mask with a potentially previous mask if nested
	//      widgets have individual masks. this requires rendering the previous
	//      mask into this mask with the correct transformation. to do this we
	//      have to walk up the mask/parent chain to find the previous mask and
	//      render this render target texture. this requires a new method in
	//      deoglRenderCanvas to keep this there
	
	pMaskRenderTarget->ReleaseFramebuffer(); // temporary
}
