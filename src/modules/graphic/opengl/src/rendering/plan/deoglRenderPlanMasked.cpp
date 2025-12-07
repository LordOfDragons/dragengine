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
#include <string.h>

#include "deoglRenderPlan.h"
#include "deoglRenderPlanMasked.h"
#include "../../component/deoglRComponent.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderPlanMasked
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanMasked::deoglRenderPlanMasked() :
pPlan(NULL),
pComponent(NULL),
pComponentTexture(0),
pUseClipPlane(false),
pClipNormal(0.0f, 1.0f, 0.0f),
pClipDistance(0.0f),
pClipNormalStereo(0.0f, 1.0f, 0.0f),
pClipDistanceStereo(0.0f),
pStencilMask(0),
pParentStencilMask(0){
}

deoglRenderPlanMasked::~deoglRenderPlanMasked(){
}



// Management
///////////////

void deoglRenderPlanMasked::SetPlan(deoglRenderPlan *plan){
	if(plan == pPlan){
		return;
	}
	
	if(pPlan){
		// important or strange segfaults can happen. for example lights are stored as
		// pointers without refcounting. if this clean up is missing the light can be
		// destroyed while the plan is not used. then the next time the plan is used
		// during prepare phase it tries to do a clean up to be safe and acesses the
		// now destroyed light
		pPlan->CleanUp();
	}
	
	pPlan = plan;
	
	pComponent = NULL;
	pComponentTexture = 0;
	
	pUseClipPlane = false;
	pClipNormal.Set(0.0f, 1.0f, 0.0f);
	pClipDistance = 0.0f;
	pClipNormalStereo.Set(0.0f, 1.0f, 0.0f);
	pClipDistanceStereo = 0.0f;
	
	pStencilMask = 0;
	pParentStencilMask = 0;
}

void deoglRenderPlanMasked::SetComponent(deoglRComponent *component, int texture){
	pComponent = component;
	pComponentTexture = texture;
}

void deoglRenderPlanMasked::SetUseClipPlane(bool useClipPlane){
	pUseClipPlane = useClipPlane;
}

void deoglRenderPlanMasked::SetClipNormal(const decVector &normal){
	pClipNormal = normal;
}

void deoglRenderPlanMasked::SetClipDistance(float distance){
	pClipDistance = distance;
}

void deoglRenderPlanMasked::SetClipNormalStereo(const decVector &normal){
	pClipNormalStereo = normal;
}

void deoglRenderPlanMasked::SetClipDistanceStereo(float distance){
	pClipDistanceStereo = distance;
}

void deoglRenderPlanMasked::SetStencilMask(int mask){
	pStencilMask = mask;
}

void deoglRenderPlanMasked::SetParentStencilMask(int mask){
	pParentStencilMask = mask;
}
