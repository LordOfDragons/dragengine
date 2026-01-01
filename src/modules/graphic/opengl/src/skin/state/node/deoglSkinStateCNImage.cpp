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

#include "deoglSkinStateCNImage.h"
#include "../deoglSkinState.h"
#include "../../../texture/deoglImage.h"
#include "../../../rendering/deoglRenderCanvasContext.h"
#include "../../../rendering/deoglRenderConstructed.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTRenderers.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>


// Class deoglSkinStateCNImage
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateCNImage::deoglSkinStateCNImage(deSkinPropertyNodeImage &node) :
deoglSkinStateConstructedNode(node, etImage),
pSyncImage(node.GetImage()),
pImage(pSyncImage && pSyncImage->GetPeerGraphic()
	? ((deoglImage*)pSyncImage->GetPeerGraphic())->GetRImage() : nullptr),
pRepeat(node.GetRepeat()),
pDirtyTCTransform(true){
}

deoglSkinStateCNImage::deoglSkinStateCNImage(const deoglSkinStateCNImage &node) :
deoglSkinStateConstructedNode(node),
pSyncImage(node.pSyncImage),
pImage(node.pImage),
pRepeat(node.pRepeat),
pTCTransform(node.pTCTransform),
pDirtyTCTransform(true){
}

deoglSkinStateCNImage::~deoglSkinStateCNImage(){
}



// Management
///////////////

void deoglSkinStateCNImage::Update(deoglSkinState &state){
	if(!pImage){
		return;
	}
	
	deoglSkinStateConstructedNode::Update(state);
	
	pUpdateTCTransform();
	
	deoglImage * const peer = (deoglImage*)pSyncImage->GetPeerGraphic();
	if(peer){
		peer->SyncToRender();
	}
}

void deoglSkinStateCNImage::PrepareForRender(deoglSkinState &state){
	if(!pImage){
		return;
	}
	
	deoglSkinStateConstructedNode::PrepareForRender(state);
	
	pImage->PrepareForRender();
}

void deoglSkinStateCNImage::Render(deoglSkinState &state, const deoglRenderCanvasContext &context){
	if(!pImage){
		return;
	}
	
	deoglSkinStateConstructedNode::Render(state, context);
	
	deoglRenderCanvasContext imageContext(context, *this);
	imageContext.SetTCClampMinimum(pTCClampMin);
	imageContext.SetTCClampMaximum(pTCClampMax);
	state.GetRenderThread().GetRenderers().GetConstructed().DrawNodeImage(imageContext, *this);
}

deoglSkinStateConstructedNode::Ref deoglSkinStateCNImage::Copy() const{
	return deoglSkinStateCNImage::Ref::New(*this);
}



// Private Functions
//////////////////////

void deoglSkinStateCNImage::pUpdateTCTransform(){
	if(!pDirtyTCTransform){
		return;
	}
	
	pDirtyTCTransform = false;
	
	const decVector2 halfPixel(0.5f / (float)pImage->GetWidth(), 0.5f / (float)pImage->GetHeight());
	
	const float repeatScaleU = (float)pRepeat.x;
	const float repeatScaleV = (float)pRepeat.y;
	
	pTCTransform = decTexMatrix2::CreateScale(repeatScaleU, repeatScaleV);
	pTCClampMin = halfPixel;
	pTCClampMax = decVector2(repeatScaleU, repeatScaleV) - halfPixel;
}
