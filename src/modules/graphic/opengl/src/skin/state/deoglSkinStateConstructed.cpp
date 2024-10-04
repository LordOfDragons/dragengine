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

#include "deoglSkinStateConstructed.h"
#include "../deoglSkinConstructedProperty.h"
#include "../state/deoglSkinState.h"
#include "../../debug/deoglDebugTraceGroup.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../rendering/deoglRenderConstructed.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTFramebuffer.h"

#include <dragengine/common/exceptions.h>



// Class deoglSkinStateConstructed
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateConstructed::deoglSkinStateConstructed() :
pProperty( nullptr ),
pDirty( true ){
}

deoglSkinStateConstructed::~deoglSkinStateConstructed(){
}



// Management
///////////////

void deoglSkinStateConstructed::SetProperty( deoglSkinConstructedProperty *property ){
	if( property == pProperty ){
		return;
	}
	
	pProperty = property;
	pContent.TakeOver( new deoglSkinStateCNGroup( property->GetContent() ) );
}

void deoglSkinStateConstructed::Update( deoglSkinState &skinState ){
	if( ! pContent ){
		return;
	}
	
	pContent->Update( skinState );
	pDirty = true;
}

void deoglSkinStateConstructed::PrepareForRender( deoglSkinState &skinState ){
	if( ! pContent || ! pDirty ){
		return;
	}
	
	const deoglDebugTraceGroup debugTrace( skinState.GetRenderThread(), "SkinStateConstructed.PrepareForRender" );
	pDirty = false;
	pContent->PrepareForRender( skinState );
	
	pPrepareRenderTarget( skinState );
	pRenderTarget->SetTextureDirty( false );
	
	skinState.GetRenderThread().GetFramebuffer().Activate( pRenderTarget->GetFBO() );
	
	deoglRenderCanvasContext context( pContent, pRenderTarget->GetFBO(),
		decPoint(), pRenderTarget->GetSize(), false, nullptr );
	// for rendering into the render target the canvas position and transform has to be negated.
	// this way rendering with the position and transform as used for regular rendering cancels
	// each other out resulting in an identity transformation. this way no second code path is
	// required.
	context.SetTransform( pContent->GetTransform().Invert().ToTexMatrix2() * context.GetTransform() );
	//context.UpdateTransformMask();
	context.SetTCTransformMask( *pRenderTarget );
	
	skinState.GetRenderThread().GetRenderers().GetConstructed().Prepare( context );
	
	const GLfloat clearColor[ 4 ] = { pProperty->GetColor().r, pProperty->GetColor().g,
		pProperty->GetColor().b, pRenderTarget->GetComponentCount() == 4 ? 0.0f : 1.0f };
	OGL_CHECK( skinState.GetRenderThread(), pglClearBufferfv( GL_COLOR, 0, clearColor ) );
	
	pContent->Render( skinState, context );
}



// Private Functions
//////////////////////

void deoglSkinStateConstructed::pPrepareRenderTarget( deoglSkinState &skinState ){
	const decPoint size( pContent->GetSize().x, pContent->GetSize().y );
	
	if( pRenderTarget ){
		if( pRenderTarget->GetSize() != size ){
			pRenderTarget = nullptr;
		}
	}
	
	if( pRenderTarget ){
		return;
	}
	
	pRenderTarget.TakeOver( new deoglRenderTarget(
		skinState.GetRenderThread(), size, 3, pProperty->GetBitCount() ) );
	
	pRenderTarget->PrepareTexture();
	pRenderTarget->PrepareFramebuffer();
}
