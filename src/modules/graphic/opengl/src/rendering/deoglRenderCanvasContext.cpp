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

#include "deoglRenderCanvasContext.h"
#include "../canvas/render/deoglRCanvas.h"
#include "../canvas/render/deoglRCanvasView.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../target/deoglRenderTarget.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../skin/state/node/deoglSkinStateConstructedNode.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderCanvasContext
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderCanvasContext::deoglRenderCanvasContext( const deoglRenderCanvasContext &copy ) :
pFBO( copy.pFBO ),
pViewportOffset( copy.pViewportOffset ),
pViewportSize( copy.pViewportSize ),
pRenderPlanMask( copy.pRenderPlanMask ),

pClipFactor( copy.pClipFactor ),
pClipMin( copy.pClipMin ),
pClipMax( copy.pClipMax ),

pTransform( copy.pTransform ),
pTCClampMin( 0.0f, 0.0f ),
pTCClampMax( 1.0f, 1.0f ),
pColorTransform( copy.pColorTransform ),
pTransparency( copy.pTransparency ),
pMask( copy.pMask ),
pTransformMask( copy.pTransformMask ){
}

deoglRenderCanvasContext::deoglRenderCanvasContext( const deoglRCanvas &canvas,
deoglFramebuffer *fbo, const decPoint &viewportOffset, const decPoint &viewportSize,
bool upsideDown, const deoglRenderPlanMasked *renderPlanMask ) :
pFBO( fbo ),
pViewportOffset( viewportOffset ),
pViewportSize( viewportSize ),
pRenderPlanMask( renderPlanMask ),

pClipMin( -1.0f, -1.0f ),
pClipMax( 1.0f, 1.0f ),

//pTransform( canvas.GetTransform() ), // initial render to fbo context needs identity transform
pTCClampMin( 0.0f, 0.0f ),
pTCClampMax( 1.0f, 1.0f ),
pColorTransform( canvas.GetColorTransform() ),
pTransparency( canvas.GetTransparency() ),
pMask( NULL )
{
	// set clip factor to obtain correct clipping coordinates for the shaders
	pClipFactor.x = ( float )viewportSize.x * 0.5f;
	pClipFactor.y = ( float )viewportSize.y * 0.5f;
	
	// account for upside down and shifting to pixel center to not hit pixel edges
	const float biasScaleU = 2.0f / ( float )viewportSize.x;
	const float biasScaleV = 2.0f / ( float )viewportSize.y;
	const float biasOffsetU = -1.0f;
	const float biasOffsetV = -1.0f;
	
	if( upsideDown ){
		pTransform = decTexMatrix2::CreateST( biasScaleU, -biasScaleV, biasOffsetU, -biasOffsetV );
		
	}else{
		pTransform = decTexMatrix2::CreateST( biasScaleU, biasScaleV, biasOffsetU, biasOffsetV );
	}
	
	// mask
	if( canvas.GetMaskRenderTarget() ){
		pMask = canvas.GetMaskRenderTarget()->GetTexture();
		UpdateTransformMask();
	}
}

deoglRenderCanvasContext::deoglRenderCanvasContext( const deoglRenderCanvasContext &parentContext,
const deoglRCanvas &childCanvas ) :
pFBO( parentContext.pFBO ),
pViewportOffset( parentContext.pViewportOffset ),
pViewportSize( parentContext.pViewportSize ),
pRenderPlanMask( parentContext.pRenderPlanMask ),

pClipFactor( parentContext.pClipFactor ),

pTransform( childCanvas.GetTransform() * parentContext.pTransform ),
pTCClampMin( 0.0f, 0.0f ),
pTCClampMax( 1.0f, 1.0f ),
pColorTransform( childCanvas.GetColorTransform() * parentContext.pColorTransform ),
pTransparency( childCanvas.GetTransparency() * parentContext.pTransparency ),
pMask( parentContext.pMask ),
pTransformMask( parentContext.pTransformMask )
{
	pCalculateClipping( childCanvas.GetSize() );
	pClipMin.SetLargest( parentContext.pClipMin );
	pClipMax.SetSmallest( parentContext.pClipMax );
	
	// mask. child mask replaces parent mask. this is not correct but right now the only solution
	if( childCanvas.GetMaskRenderTarget() ){
		pMask = childCanvas.GetMaskRenderTarget()->GetTexture();
		UpdateTransformMask();
	}
}

deoglRenderCanvasContext::deoglRenderCanvasContext( const deoglSkinStateConstructedNode &node,
deoglFramebuffer *fbo, const decPoint &viewportOffset, const decPoint &viewportSize,
bool upsideDown, const deoglRenderPlanMasked *renderPlanMask ) :
pFBO( fbo ),
pViewportOffset( viewportOffset ),
pViewportSize( viewportSize ),
pRenderPlanMask( renderPlanMask ),

pClipMin( -1.0f, -1.0f ),
pClipMax( 1.0f, 1.0f ),

//pTransform( canvas.GetTransform() ), // initial render to fbo context needs identity transform
pTCClampMin( 0.0f, 0.0f ),
pTCClampMax( 1.0f, 1.0f ),
pColorTransform( node.GetColorTransform() ),
pTransparency( node.GetTransparency() ),
pMask( nullptr )
{
	// set clip factor to obtain correct clipping coordinates for the shaders
	pClipFactor.x = ( float )viewportSize.x * 0.5f;
	pClipFactor.y = ( float )viewportSize.y * 0.5f;
	
	// account for upside down and shifting to pixel center to not hit pixel edges
	const float biasScaleU = 2.0f / ( float )viewportSize.x;
	const float biasScaleV = 2.0f / ( float )viewportSize.y;
	const float biasOffsetU = -1.0f;
	const float biasOffsetV = -1.0f;
	
	if( upsideDown ){
		pTransform = decTexMatrix2::CreateST( biasScaleU, -biasScaleV, biasOffsetU, -biasOffsetV );
		
	}else{
		pTransform = decTexMatrix2::CreateST( biasScaleU, biasScaleV, biasOffsetU, biasOffsetV );
	}
	
	// mask
	if( node.GetMaskRenderTarget() ){
		pMask = node.GetMaskRenderTarget()->GetTexture();
		UpdateTransformMask();
	}
}

deoglRenderCanvasContext::deoglRenderCanvasContext( const deoglRenderCanvasContext &parentContext,
const deoglSkinStateConstructedNode &childNode ) :
pFBO( parentContext.pFBO ),
pViewportOffset( parentContext.pViewportOffset ),
pViewportSize( parentContext.pViewportSize ),
pRenderPlanMask( parentContext.pRenderPlanMask ),

pClipFactor( parentContext.pClipFactor ),

pTransform( childNode.GetTransform() * parentContext.pTransform ),
pTCClampMin( 0.0f, 0.0f ),
pTCClampMax( 1.0f, 1.0f ),
pColorTransform( childNode.GetColorTransform() * parentContext.pColorTransform ),
pTransparency( childNode.GetTransparency() * parentContext.pTransparency ),
pMask( parentContext.pMask ),
pTransformMask( parentContext.pTransformMask )
{
	pCalculateClipping( decPoint( childNode.GetSize().x, childNode.GetSize().y ) );
	pClipMin.SetLargest( parentContext.pClipMin );
	pClipMax.SetSmallest( parentContext.pClipMax );
	
	// mask. child mask replaces parent mask. this is not correct but right now the only solution
	if( childNode.GetMaskRenderTarget() ){
		pMask = childNode.GetMaskRenderTarget()->GetTexture();
		UpdateTransformMask();
	}
}

deoglRenderCanvasContext::~deoglRenderCanvasContext(){
}



// Management
///////////////

void deoglRenderCanvasContext::SetClipMin( const decVector2 &clipMin ){
	pClipMin = clipMin;
}

void deoglRenderCanvasContext::SetClipMax( const decVector2 &clipMax ){
	pClipMax = clipMax;
}

bool deoglRenderCanvasContext::IsZeroClip() const{
	return pClipMax.x <= pClipMin.x || pClipMax.y <= pClipMin.y;
}



void deoglRenderCanvasContext::SetTransform( const decTexMatrix2 &transform ){
	pTransform = transform;
}

void deoglRenderCanvasContext::SetTCClampMinimum( const decVector2 &clamp ){
	pTCClampMin = clamp;
}

void deoglRenderCanvasContext::SetTCClampMaximum( const decVector2 &clamp ){
	pTCClampMax = clamp;
}

void deoglRenderCanvasContext::SetColorTransform( const decColorMatrix &transform ){
	pColorTransform = transform;
}

void deoglRenderCanvasContext::SetTransparency( float transparency ){
	pTransparency = transparency;
}

void deoglRenderCanvasContext::SetMask( deoglTexture *mask ){
	pMask = mask;
}

void deoglRenderCanvasContext::SetTransformMask( const decTexMatrix2 &transform ){
	pTransformMask = transform;
}

void deoglRenderCanvasContext::UpdateTransformMask(){
	pTransformMask = ( decTexMatrix2::CreateST(
		( float )pViewportSize.x, ( float )pViewportSize.y, 0.5f, 0.5f )
			* pTransform ).Invert().ToTexMatrix2();
}



// Private Functions
//////////////////////

void deoglRenderCanvasContext::pCalculateClipping( const decVector2 &canvasSize ){
	// transform each corner to determine the extends
	decVector2 corner = pTransform * decVector2();
	pClipMin = corner;
	pClipMax = corner;
	
	corner = pTransform * decVector2( canvasSize.x, 0.0f );
	pClipMin.SetSmallest( corner );
	pClipMax.SetLargest( corner );
	
	corner = pTransform * canvasSize;
	pClipMin.SetSmallest( corner );
	pClipMax.SetLargest( corner );
	
	corner = pTransform * decVector2( 0.0f, canvasSize.y );
	pClipMin.SetSmallest( corner );
	pClipMax.SetLargest( corner );
}
