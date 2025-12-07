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

#include "deoglRDSRenderableCamera.h"
#include "../../deoglRDynamicSkin.h"
#include "../../../../renderthread/deoglRenderThread.h"
#include "../../../../rendering/plan/deoglRenderPlan.h"
#include "../../../../world/deoglRCamera.h"

#include <dragengine/common/exceptions.h>



// Class deoglRDSRenderableCamera
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRDSRenderableCamera::deoglRDSRenderableCamera(deoglRDynamicSkin &dynamicSkin) :
deoglRDSRenderable(etCamera, dynamicSkin),
pCamera(NULL)
{
	LEAK_CHECK_CREATE(dynamicSkin.GetRenderThread(), DSRenderableCamera);
}

deoglRDSRenderableCamera::~deoglRDSRenderableCamera(){
	LEAK_CHECK_FREE(GetDynamicSkin().GetRenderThread(), DSRenderableCamera);
	pCleanUp();
}



// Management
///////////////

void deoglRDSRenderableCamera::SetCamera(deoglRCamera *camera){
	if(camera == pCamera){
		return;
	}
	
	
	pCamera = camera;
	
	if(camera){
		camera->AddReference();
	}
}

void deoglRDSRenderableCamera::PrepareForRender(const deoglRenderPlanMasked *renderPlanMask){
	if(pCamera){
		pCamera->GetPlan().PrepareRender(renderPlanMask);
	}
}

deoglRenderPlan *deoglRDSRenderableCamera::GetRenderPlan() const{
	return pCamera ? &pCamera->GetPlan() : NULL;
}



// Private Functions
//////////////////////

void deoglRDSRenderableCamera::pCleanUp(){
	SetCamera(NULL);
}
