/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <string.h>

#include "deoglRenderCanvasContext.h"
#include "../canvas/render/deoglRCanvas.h"
#include "../canvas/render/deoglRCanvasView.h"
#include "../framebuffer/deoglFramebuffer.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderCanvasContext
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderCanvasContext::deoglRenderCanvasContext( const deoglRenderCanvasContext &copy ) :
pFBO( copy.pFBO ),
pViewportOffset( copy.pViewportOffset ),
pViewportSize( copy.pViewportSize ),

pClipFactor( copy.pClipFactor ),
pClipMin( copy.pClipMin ),
pClipMax( copy.pClipMax ),

pTransform( copy.pTransform ),
pTCClampMin( 0.0f, 0.0f ),
pTCClampMax( 1.0f, 1.0f ),
pColorTransform( copy.pColorTransform ),
pTransparency( copy.pTransparency ){
}

deoglRenderCanvasContext::deoglRenderCanvasContext( const deoglRCanvasView &canvas,
deoglFramebuffer *fbo, const decPoint &viewportOffset, const decPoint &viewportSize,
bool upsideDown ) :
pFBO( fbo ),
pViewportOffset( viewportOffset ),
pViewportSize( viewportSize ),

pClipMin( -1.0f, -1.0f ),
pClipMax( 1.0f, 1.0f ),

//pTransform( canvas.GetTransform() ), // initial render to fbo context needs identity transform
pTCClampMin( 0.0f, 0.0f ),
pTCClampMax( 1.0f, 1.0f ),
pColorTransform( canvas.GetColorTransform() ),
pTransparency( canvas.GetTransparency() )
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
		pTransform *= decTexMatrix2::CreateST( biasScaleU, -biasScaleV, biasOffsetU, -biasOffsetV );
		
	}else{
		pTransform *= decTexMatrix2::CreateST( biasScaleU, biasScaleV, biasOffsetU, biasOffsetV );
	}
}

deoglRenderCanvasContext::deoglRenderCanvasContext( const deoglRenderCanvasContext &parentContext,
const deoglRCanvas &childCanvas ) :
pFBO( parentContext.pFBO ),
pViewportOffset( parentContext.pViewportOffset ),
pViewportSize( parentContext.pViewportSize ),

pClipFactor( parentContext.GetClipFactor() ),

pTransform( childCanvas.GetTransform() * parentContext.pTransform ),
pTCClampMin( 0.0f, 0.0f ),
pTCClampMax( 1.0f, 1.0f ),
pColorTransform( childCanvas.GetColorTransform() * parentContext.pColorTransform ),
pTransparency( childCanvas.GetTransparency() * parentContext.pTransparency )
{
	pCalculateClipping( childCanvas.GetSize() );
	pClipMin.SetLargest( parentContext.pClipMin );
	pClipMax.SetSmallest( parentContext.pClipMax );
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
