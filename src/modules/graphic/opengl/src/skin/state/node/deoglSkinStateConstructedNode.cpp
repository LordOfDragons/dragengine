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

#include "deoglSkinStateConstructedNode.h"
#include "deoglSkinStateCNGroup.h"
#include "deoglSkinStateCNImage.h"
#include "deoglSkinStateCNShape.h"
#include "deoglSkinStateCNText.h"
#include "../deoglSkinState.h"
#include "../deoglSkinStateMapped.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitorIdentify.h>



// Class deoglSkinStateConstructedNode
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateConstructedNode::deoglSkinStateConstructedNode(deSkinPropertyNode &node, eType type) :
pType(type),
pPosition(node.GetPosition()),
pSize(node.GetSize()),
pRotation(node.GetRotation()),
pShear(node.GetShear()),
pBrightness(node.GetBrightness()),
pContrast(node.GetContrast()),
pGamma(node.GetGamma()),
pColorize(node.GetColorize()),
pTransparency(node.GetTransparency()),
pCombineMode(node.GetCombineMode()),
pDirtyTransform(true),
pDirtyColorTransform(true)
{
	int i;
	for(i=0; i<deSkinPropertyNode::MappedCount; i++){
		pMapped[i] = node.GetMappedFor((deSkinPropertyNode::eMapped)i);
	}
	
	if(node.GetMask()){
		pMask = CreateNode(*node.GetMask());
	}
}

deoglSkinStateConstructedNode::deoglSkinStateConstructedNode(const deoglSkinStateConstructedNode &node) :
pType(node.pType),
pPosition(node.pPosition),
pSize(node.pSize),
pRotation(node.pRotation),
pShear(node.pShear),
pBrightness(node.pBrightness),
pContrast(node.pContrast),
pGamma(node.pGamma),
pColorize(node.pColorize),
pTransparency(node.pTransparency),
pCombineMode(node.pCombineMode),
pDirtyTransform(true),
pDirtyColorTransform(true)
{
	int i;
	for(i=0; i<deSkinPropertyNode::MappedCount; i++){
		pMapped[i] = node.pMapped[i];
	}
	
	if(node.pMask){
		pMask = node.pMask->Copy();
	}
}

deoglSkinStateConstructedNode::~deoglSkinStateConstructedNode(){
}



// Management
///////////////

int deoglSkinStateConstructedNode::GetMappedFor(deSkinPropertyNode::eMapped type) const{
	return pMapped[type];
}

void deoglSkinStateConstructedNode::Update(deoglSkinState &state){
	if(pMapped[deSkinPropertyNode::emPositionX] != -1){
		pPosition.x = (int)(state.GetMappedAt(pMapped[deSkinPropertyNode::emPositionX]).GetValue() + 0.5f);
		pDirtyTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emPositionY] != -1){
		pPosition.y = (int)(state.GetMappedAt(pMapped[deSkinPropertyNode::emPositionY]).GetValue() + 0.5f);
		pDirtyTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emPositionZ] != -1){
		pPosition.z = (int)(state.GetMappedAt(pMapped[deSkinPropertyNode::emPositionZ]).GetValue() + 0.5f);
		pDirtyTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emSizeX] != -1){
		pSize.x = (int)(state.GetMappedAt(pMapped[deSkinPropertyNode::emSizeX]).GetValue() + 0.5f);
		pDirtyTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emSizeY] != -1){
		pSize.y = (int)(state.GetMappedAt(pMapped[deSkinPropertyNode::emSizeY]).GetValue() + 0.5f);
		pDirtyTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emSizeZ] != -1){
		pSize.z = (int)(state.GetMappedAt(pMapped[deSkinPropertyNode::emSizeZ]).GetValue() + 0.5f);
		pDirtyTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emRotation] != -1){
		pRotation = state.GetMappedAt(pMapped[deSkinPropertyNode::emRotation]).GetValue() * TWO_PI;
		pDirtyTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emShear] != -1){
		pShear = state.GetMappedAt(pMapped[deSkinPropertyNode::emShear]).GetValue() * TWO_PI;
		pDirtyTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emBrightness] != -1){
		pBrightness = state.GetMappedAt(pMapped[deSkinPropertyNode::emBrightness]).GetValue();
		pDirtyColorTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emContrast] != -1){
		pContrast = state.GetMappedAt(pMapped[deSkinPropertyNode::emContrast]).GetValue();
		pDirtyColorTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emGamma] != -1){
		pGamma = state.GetMappedAt(pMapped[deSkinPropertyNode::emGamma]).GetValue();
	}
	if(pMapped[deSkinPropertyNode::emColorizeRed] != -1){
		pColorize.r = state.GetMappedAt(pMapped[deSkinPropertyNode::emColorizeRed]).GetValue();
		pDirtyColorTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emColorizeGreen] != -1){
		pColorize.g = state.GetMappedAt(pMapped[deSkinPropertyNode::emColorizeGreen]).GetValue();
		pDirtyColorTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emColorizeBlue] != -1){
		pColorize.b = state.GetMappedAt(pMapped[deSkinPropertyNode::emColorizeBlue]).GetValue();
		pDirtyColorTransform = true;
	}
	if(pMapped[deSkinPropertyNode::emTransparency] != -1){
		pTransparency = state.GetMappedAt(pMapped[deSkinPropertyNode::emTransparency]).GetValue();
	}
	
	pUpdateTransform();
	pUpdateColorTransform();
}

void deoglSkinStateConstructedNode::PrepareForRender(deoglSkinState &state){
	if(!pMask){
		return;
	}
	
	pMask->PrepareForRender(state);
}

void deoglSkinStateConstructedNode::Render(deoglSkinState &state, const deoglRenderCanvasContext &){
	if(!pMask){
		return;
	}
	
	const decPoint size(pSize.x, pSize.y);
	
	if(pMaskRenderTarget){
		pMaskRenderTarget->SetSize(size);
		
	}else{
		pMaskRenderTarget = new deoglRenderTarget(state.GetRenderThread(), size, 1, 8);
	}
}



deoglSkinStateConstructedNode::Ref deoglSkinStateConstructedNode::CreateNode(deSkinPropertyNode &node){
	deSkinPropertyNodeVisitorIdentify identify;
	node.Visit(identify);
	
	switch(identify.GetType()){
	case deSkinPropertyNodeVisitorIdentify::entGroup:
		return Ref::New(new deoglSkinStateCNGroup(identify.CastToGroup()));
		
	case deSkinPropertyNodeVisitorIdentify::entImage:
		return Ref::New(new deoglSkinStateCNImage(identify.CastToImage()));
		
	case deSkinPropertyNodeVisitorIdentify::entShape:
		return Ref::New(new deoglSkinStateCNShape(identify.CastToShape()));
		
	case deSkinPropertyNodeVisitorIdentify::entText:
		return Ref::New(new deoglSkinStateCNText(identify.CastToText()));
		
	default:
		DETHROW(deeInvalidParam);
	}
}



// Protected Functions
////////////////////////

void deoglSkinStateConstructedNode::pUpdateTransform(){
	if(!pDirtyTransform){
		return;
	}
	
	pDirtyTransform = false;
	
	const decPoint sizeAbs(abs(pSize.x), abs(pSize.y));
	// clamp.x = decMath::max( sizeAbs.x - 1, 0 );
	// clamp.y = decMath::max( sizeAbs.y - 1, 0 );
	
	const decVector2 offset((float)sizeAbs.x * 0.5f, (float)sizeAbs.y * 0.5f);
	const decVector2 position(decPoint(pPosition.x, pPosition.y));
	
	pTransform = decTexMatrix2::CreateScale(pSize.x < 0.0f ? -1.0f : 1.0f, pSize.y < 0.0f ? -1.0f : 1.0f)
		* decTexMatrix2::CreateTranslation( -offset )
		* decTexMatrix2::CreateShear( tanf( pShear ), 0.0f )
		* decTexMatrix2::CreateRotation( pRotation )
		* decTexMatrix2::CreateTranslation( offset + position );
}

void deoglSkinStateConstructedNode::pUpdateColorTransform(){
	if(!pDirtyColorTransform){
		return;
	}
	
	pDirtyColorTransform = false;
	
	pColorTransform = decColorMatrix::CreateContrast(pContrast)
		* decColorMatrix::CreateBrightness( pBrightness )
		* decColorMatrix::CreateScaling( pColorize );
}
