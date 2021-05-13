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
#include <stdlib.h>
#include <string.h>

#include "deoglRCanvas.h"
#include "../../delayedoperation/deoglDelayedDeletion.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../target/deoglRenderTarget.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvas
///////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvas::deoglRCanvas( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pOrder( 0.0f ),
pTransparency( 1.0f ),
pBlendSrc( GL_SRC_ALPHA ),
pBlendDest( GL_ONE_MINUS_SRC_ALPHA ),
pMask( NULL ),
pVisible( true ),
pMaskRenderTarget( NULL ){
}

class deoglRCanvasDeletion : public deoglDelayedDeletion{
public:
	deoglRenderTarget *maskRenderTarget;
	
	deoglRCanvasDeletion() :
	maskRenderTarget( NULL ){
	}
	
	virtual ~deoglRCanvasDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread& ){
		if( maskRenderTarget ){
			maskRenderTarget->FreeReference();
		}
	}
};

deoglRCanvas::~deoglRCanvas(){
	if( pMask ){
		pMask->FreeReference();
	}
	
	// delayed deletion of opengl containing objects
	deoglRCanvasDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglRCanvasDeletion;
		delayedDeletion->maskRenderTarget = pMaskRenderTarget;
		GetRenderThread().GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		GetRenderThread().GetLogger().LogException( e );
		// throw; -> otherwise terminate
	}
}



// Management
///////////////

void deoglRCanvas::SetPosition( const decVector2 &position ){
	pPosition = position;
}

void deoglRCanvas::SetSize( const decVector2 &size ){
	pSize = size;
}

void deoglRCanvas::SetTransform( const decTexMatrix2 &transform ){
	pTransform = transform;
}

void deoglRCanvas::SetColorTransform( const decColorMatrix &transform ){
	pColorTransform = transform;
}

void deoglRCanvas::SetOrder( float order ){
	pOrder = order;
}

void deoglRCanvas::SetTransparency( float transparency ){
	pTransparency = transparency;
}

void deoglRCanvas::SetBlendSrc( GLenum blendSrc ){
	pBlendSrc = blendSrc;
}

void deoglRCanvas::SetBlendDest( GLenum blendDest ){
	pBlendDest = blendDest;
}

void deoglRCanvas::SetMask( deoglRCanvas *mask ){
	if( mask == pMask ){
		return;
	}
	
	if( pMask ){
		pMask->FreeReference();
	}
	
	pMask = mask;
	
	if( mask ){
		mask->AddReference();
	}
}

void deoglRCanvas::SetVisible( bool visible ){
	pVisible = visible;
}

void deoglRCanvas::DirtyMaskRenderTarget(){
	if( pMaskRenderTarget ){
		pMaskRenderTarget->SetTextureDirty( true );
	}
}



void deoglRCanvas::PrepareForRender( const deoglRenderPlanMasked *renderPlanMask ){
	if( pMask ){
		pMask->PrepareForRender( renderPlanMask );
		
		const int width = ( int )( GetSize().x + 0.5f );
		const int height = ( int )( GetSize().y + 0.5f );
		
		if( pMaskRenderTarget ){
			pMaskRenderTarget->SetSize( width, height );
			
		}else{
			pMaskRenderTarget = new deoglRenderTarget( GetRenderThread(), width, height, 1, 8 );
		}
		
		if( pMaskRenderTarget->GetTextureDirty() ){
			pMaskRenderTarget->SetTextureDirty( false );
			
			pMaskRenderTarget->PrepareFramebuffer();
			
			const decPoint viewportSize( pMaskRenderTarget->GetWidth(), pMaskRenderTarget->GetHeight() );
			
			deoglRenderCanvasContext context( *pMask, pMaskRenderTarget->GetFBO(),
				decPoint(), viewportSize, false, renderPlanMask );
			
			// for rendering into the render target the canvas position and transform has
			// to be negated. this way rendering with the position and transform as used
			// for regular rendering cancels each other out resulting in an identity
			// transformation. this way no second code path is required.
			context.SetTransform( pMask->GetTransform().Invert().ToTexMatrix2() * context.GetTransform() );
			context.UpdateTransformMask();
			
			GetRenderThread().GetRenderers().GetCanvas().Prepare( context );
			pMask->Render( context );
			
			// TODO we need to merge this mask with a potentially previous mask if nested
			//      widgets have individual masks. this requires rendering the previous
			//      mask into this mask with the correct transformation. to do this we
			//      have to walk up the mask/parent chain to find the previous mask and
			//      render this render target texture. this requires a new method in
			//      deoglRenderCanvas to keep this there
			
			pMaskRenderTarget->ReleaseFramebuffer(); // temporary
		}
	}
}
