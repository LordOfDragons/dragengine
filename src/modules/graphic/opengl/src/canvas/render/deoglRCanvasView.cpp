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

#include "deoglRCanvasView.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../target/deoglRenderTarget.h"
#include "../../delayedoperation/deoglDelayedDeletion.h"
#include "../../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvasView
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvasView::deoglRCanvasView( deoglRenderThread &renderThread ) :
deoglRCanvas( renderThread ),
pPaintTracker( 0 ),
pRenderTarget( NULL ),
pResizeRenderTarget( false )
{
	LEAK_CHECK_CREATE( renderThread, CanvasView );
}

class deoglRCanvasViewDeletion : public deoglDelayedDeletion{
public:
	deoglRenderTarget *renderTarget;
	
	deoglRCanvasViewDeletion() :
	renderTarget( NULL ){
	}
	
	virtual ~deoglRCanvasViewDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread& ){
		if( renderTarget ){
			renderTarget->FreeReference();
		}
	}
};

deoglRCanvasView::~deoglRCanvasView(){
	LEAK_CHECK_FREE( GetRenderThread(), CanvasView );
	RemoveAllChildren();
	
	// delayed deletion of opengl containing objects
	deoglRCanvasViewDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglRCanvasViewDeletion;
		delayedDeletion->renderTarget = pRenderTarget;
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

void deoglRCanvasView::AddChild( deoglRCanvas *canvas ){
	if( ! canvas ){
		DETHROW( deeInvalidParam );
	}
	
	const float order = canvas->GetOrder();
	const int count = pChildren.GetCount();
	int i;
	
	// NOTE we could use here a binary insertion algorithm since the list is sorted.
	//      for the beginning though it should not be required since the number of
	//      child canvas are low so a linear search is fine.
	for( i=0; i<count; i++ ){
		const deoglRCanvas &child = *( ( deoglRCanvas* )pChildren.GetAt( i ) );
		
		if( order < child.GetOrder() ){
			pChildren.Insert( canvas, i );
			return;
		}
	}
	
	pChildren.Add( canvas );
}

void deoglRCanvasView::RemoveAllChildren(){
	pChildren.RemoveAll();
}

bool deoglRCanvasView::HasNoChildren() const{
	return pChildren.GetCount() == 0;
}



void deoglRCanvasView::IncrementPaintTracker(){
	pPaintTracker++;
	
	if( pRenderTarget ){
		pRenderTarget->SetTextureDirty( true );
	}
}

void deoglRCanvasView::SetResizeRenderTarget(){
	pResizeRenderTarget = true;
}

void deoglRCanvasView::PrepareRenderTarget(){
	PrepareForRender();
	
	if( pRenderTarget ){
		if( pResizeRenderTarget ){
			const int width = ( int )( GetSize().x + 0.5f );
			const int height = ( int )( GetSize().y + 0.5f );
			pRenderTarget->SetSize( width, height );
			pResizeRenderTarget = false;
		}
		
	}else{
		const int width = ( int )( GetSize().x + 0.5f );
		const int height = ( int )( GetSize().y + 0.5f );
		const int componentCount = 4; // if transparency=1 componentCount of 3 would work too
		const int bitCount = 8;
		
		pRenderTarget = new deoglRenderTarget( GetRenderThread(), width, height, componentCount, bitCount );
		pResizeRenderTarget = false;
	}
	
	if( pRenderTarget->GetTextureDirty() ){
		// mark texture no more dirty although not updated yet. this prevents re-entrant loops
		// due to the canvas being used in a dynamic skin in the same world it is rendering
		pRenderTarget->SetTextureDirty( false );
		
		// prepare and activate framebuffer
		pRenderTarget->PrepareFramebuffer();
		
		// render content
		const decPoint viewportSize( pRenderTarget->GetWidth(), pRenderTarget->GetHeight() );
		
		deoglRenderCanvasContext context( *this, pRenderTarget->GetFBO(), decPoint(), viewportSize, false );
		// for rendering into the render target the canvas position and transform has to be negated.
		// this way rendering with the position and transform as used for regular rendering cancels
		// each other out resulting in an identity transformation. this way no second code path is
		// required.
		context.SetTransform( GetTransform().Invert().ToTexMatrix2() * context.GetTransform() );
		
		GetRenderThread().GetRenderers().GetCanvas().Prepare( context );
		Render( context );
		
		// release framebuffer
		pRenderTarget->ReleaseFramebuffer(); // temporary
	}
}



void deoglRCanvasView::PrepareForRender(){
	const int count = pChildren.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRCanvas &child = *( ( deoglRCanvas* )pChildren.GetAt( i ) );
		if( child.GetVisible() ){
			child.PrepareForRender();
		}
	}
}

void deoglRCanvasView::Render( const deoglRenderCanvasContext &context ){
	const deoglRenderCanvasContext childContext( context, *this );
	if( childContext.IsZeroClip() ){
		return;
	}
	
	// render children
	const int count = pChildren.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRCanvas &child = *( ( deoglRCanvas* )pChildren.GetAt( i ) );
		if( child.GetVisible() ){
			child.Render( childContext );
		}
	}
}
