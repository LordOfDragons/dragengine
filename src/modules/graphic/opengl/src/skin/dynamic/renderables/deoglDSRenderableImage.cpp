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

#include "deoglDSRenderableImage.h"
#include "render/deoglRDSRenderableImage.h"
#include "../deoglDynamicSkin.h"
#include "../../../texture/deoglImage.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableImage.h>



// Class deoglDSRenderableImage
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDSRenderableImage::deoglDSRenderableImage(deoglDynamicSkin &dynamicSkin, const deDSRenderableImage &renderable) :
deoglDSRenderable(dynamicSkin, renderable),
pRenderableImage(renderable),
pRRenderableImage(NULL),
pImage(NULL),
pDirty(true)
{
	try{
		pRRenderableImage.TakeOver(new deoglRDSRenderableImage(*dynamicSkin.GetRDynamicSkin()));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglDSRenderableImage::~deoglDSRenderableImage(){
	pCleanUp();
}



// Management
///////////////

deoglRDSRenderable *deoglDSRenderableImage::GetRRenderable() const{
	return pRRenderableImage;
}

void deoglDSRenderableImage::RenderableChanged(){
	deoglImage * const image = pRenderableImage.GetImage()
		? (deoglImage*)pRenderableImage.GetImage()->GetPeerGraphic() : NULL;
	
	if(image != pImage){
		pImage = image;
		pDirty = true;
		
		pDynamicSkin.NotifyRenderableChanged(*this);
	}
	
	if(pRenderableImage.GetName() != pRRenderableImage->GetName()){
		pDynamicSkin.NotifyRenderablesChanged();
	}
}

void deoglDSRenderableImage::SyncToRender(){
	if(pDirty){
		pRRenderableImage->SetName(pRenderableImage.GetName());
		
		if(pRenderableImage.GetImage()){
			deoglImage &image = *((deoglImage*)pRenderableImage.GetImage()->GetPeerGraphic());
			image.SyncToRender();
			pRRenderableImage->SetImage(image.GetRImage());
			
		}else{
			pRRenderableImage->SetImage(NULL);
		}
		
		pDirty = false;
	}
}



// Private Functions
//////////////////////

void deoglDSRenderableImage::pCleanUp(){
}
