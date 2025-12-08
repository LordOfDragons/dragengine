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

#include "deoglDSRenderableValue.h"
#include "render/deoglRDSRenderableValue.h"
#include "../deoglDynamicSkin.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableValue.h>



// Class deoglDSRenderableValue
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDSRenderableValue::deoglDSRenderableValue(deoglDynamicSkin &dynamicSkin, const deDSRenderableValue &renderable) :
deoglDSRenderable(dynamicSkin, renderable),
pRenderableValue(renderable),
pRRenderableValue(NULL),
pValue(renderable.GetValue()),
pDirty(true)
{
	try{
		pRRenderableValue.TakeOver(new deoglRDSRenderableValue(*dynamicSkin.GetRDynamicSkin()));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglDSRenderableValue::~deoglDSRenderableValue(){
	pCleanUp();
}



// Management
///////////////

deoglRDSRenderable *deoglDSRenderableValue::GetRRenderable() const{
	return pRRenderableValue;
}

void deoglDSRenderableValue::RenderableChanged(){
	const float value = pRenderableValue.GetValue();
	
	if(fabsf(value - pValue) >= FLOAT_SAFE_EPSILON){
		pValue = value;
		pDirty = true;
		
		pDynamicSkin.NotifyRenderableChanged(*this);
	}
	
	if(pRenderableValue.GetName() != pRRenderableValue->GetName()){
		pDynamicSkin.NotifyRenderablesChanged();
	}
}

void deoglDSRenderableValue::SyncToRender(){
	if(pDirty){
		pRRenderableValue->SetName(pRenderableValue.GetName());
		pRRenderableValue->SetValue(pRenderableValue.GetValue());
		pDirty = false;
	}
}



// Private Functions
//////////////////////

void deoglDSRenderableValue::pCleanUp(){
}
