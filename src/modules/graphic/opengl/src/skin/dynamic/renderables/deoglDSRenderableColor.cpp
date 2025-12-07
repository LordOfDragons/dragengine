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

#include "deoglDSRenderableColor.h"
#include "render/deoglRDSRenderableColor.h"
#include "../deoglDynamicSkin.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>



// Class deoglDSRenderableColor
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDSRenderableColor::deoglDSRenderableColor(deoglDynamicSkin &dynamicSkin, const deDSRenderableColor &renderable) :
deoglDSRenderable(dynamicSkin, renderable),
pRenderableColor(renderable),
pRRenderableColor(NULL),
pColor(renderable.GetColor()),
pDirty(true)
{
	try{
		pRRenderableColor = new deoglRDSRenderableColor(*dynamicSkin.GetRDynamicSkin());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglDSRenderableColor::~deoglDSRenderableColor(){
	pCleanUp();
}



// Management
///////////////

deoglRDSRenderable *deoglDSRenderableColor::GetRRenderable() const{
	return pRRenderableColor;
}

void deoglDSRenderableColor::RenderableChanged(){
	const decColor &color = pRenderableColor.GetColor();
	
	if(!color.IsEqualTo(pColor)){
		pColor = color;
		pDirty = true;
		
		pDynamicSkin.NotifyRenderableChanged(*this);
	}
	
	if(pRenderableColor.GetName() != pRRenderableColor->GetName()){
		pDynamicSkin.NotifyRenderablesChanged();
	}
}

void deoglDSRenderableColor::SyncToRender(){
	if(pDirty){
		pRRenderableColor->SetName(pRenderableColor.GetName());
		pRRenderableColor->SetColor(pRenderableColor.GetColor());
		pDirty = false;
	}
}



// Private Functions
//////////////////////

void deoglDSRenderableColor::pCleanUp(){
}
