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

#include "deoglRCanvasRenderWorld.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../rendering/defren/deoglDeferredRendering.h"
#include "../../rendering/plan/deoglRenderPlan.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../world/deoglRCamera.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvasRenderWorld
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvasRenderWorld::deoglRCanvasRenderWorld(deoglRenderThread &renderThread) :
deoglRCanvas(renderThread),
pCamera(NULL),
pForceToneMapAdaption(false){
	LEAK_CHECK_CREATE(renderThread, CanvasRenderWorld);
}

deoglRCanvasRenderWorld::~deoglRCanvasRenderWorld(){
	LEAK_CHECK_FREE(GetRenderThread(), CanvasRenderWorld);
	SetCamera(NULL);
}



// Management
///////////////

void deoglRCanvasRenderWorld::SetCamera(deoglRCamera *camera){
	if(camera == pCamera){
		return;
	}
	
	
	pCamera = camera;
	
	pForceToneMapAdaption = true;
}



void deoglRCanvasRenderWorld::PrepareForRender(const deoglRenderPlanMasked *renderPlanMask){
	if(!pCamera || !pCamera->GetParentWorld()){
		return;
	}
	
	deoglRCanvas::PrepareForRender(renderPlanMask);
	
	if(pForceToneMapAdaption){
		pCamera->SetForceToneMapAdaption(true);
		pForceToneMapAdaption = false;
	}
	
	// this is called during DrawCanvasRenderWorld
	//pCamera->GetParentWorld()->PrepareForRender( pCamera->GetPlan() );
}

void deoglRCanvasRenderWorld::Render(const deoglRenderCanvasContext &context){
	const deoglRenderCanvasContext renderWorldContext(context, *this);
	GetRenderThread().GetRenderers().GetCanvas().DrawCanvasRenderWorld(renderWorldContext, *this);
}
