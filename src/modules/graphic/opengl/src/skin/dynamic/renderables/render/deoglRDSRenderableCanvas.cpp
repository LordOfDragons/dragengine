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

#include "deoglRDSRenderableCanvas.h"
#include "../../deoglRDynamicSkin.h"
#include "../../../../renderthread/deoglRenderThread.h"
#include "../../../../canvas/render/deoglRCanvasView.h"
#include "../../../../target/deoglRenderTarget.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableCanvas.h>



// Class deoglRDSRenderableCanvas
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRDSRenderableCanvas::deoglRDSRenderableCanvas(
	deoglRDynamicSkin &dynamicSkin, const deDSRenderableCanvas &renderable) :
deoglRDSRenderable(etCanvas, dynamicSkin),
pCanvas(NULL),
pComponentCount(renderable.GetComponentCount()),
pBitCount(renderable.GetBitCount())
{
	LEAK_CHECK_CREATE(dynamicSkin.GetRenderThread(), DSRenderableCanvas);
}

deoglRDSRenderableCanvas::~deoglRDSRenderableCanvas(){
	LEAK_CHECK_FREE(GetDynamicSkin().GetRenderThread(), DSRenderableCanvas);
	if(pCanvas){
		pCanvas->FreeReference();
	}
}



// Management
///////////////

void deoglRDSRenderableCanvas::SetCanvas(deoglRCanvasView *canvas){
	if(canvas == pCanvas){
		return;
	}
	
	if(pCanvas){
		pCanvas->FreeReference();
	}
	
	pCanvas = canvas;
	
	if(canvas){
		canvas->AddReference();
	}
}

void deoglRDSRenderableCanvas::SetComponentCount(int componentCount){
	pComponentCount = componentCount;
}

void deoglRDSRenderableCanvas::SetBitCount(int bitCount){
	pBitCount = bitCount;
}

void deoglRDSRenderableCanvas::PrepareForRender(const deoglRenderPlanMasked *renderPlanMask){
	if(pCanvas){
		pCanvas->PrepareRenderTarget(renderPlanMask, pComponentCount, pBitCount);
	}
}

void deoglRDSRenderableCanvas::Render(const deoglRenderPlanMasked *renderPlanMask){
	if(pCanvas){
		pCanvas->RenderRenderTarget(renderPlanMask);
	}
}

deoglTexture *deoglRDSRenderableCanvas::GetRenderTexture(){
	if(! pCanvas){
		return NULL;
	}
	
	deoglRenderTarget * const renderTarget = pCanvas->GetRenderTarget();
	if(! renderTarget){
		return NULL;
	}
	
	return renderTarget->GetTexture();
}
