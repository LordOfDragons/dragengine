/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
	context.UpdateTransformMask();
	
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
