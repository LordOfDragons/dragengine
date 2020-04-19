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

#include "deoglCanvasView.h"
#include "deoglCanvasCanvasView.h"
#include "render/deoglRCanvasView.h"
#include "../deGraphicOpenGl.h"
#include "../skin/dynamic/renderables/deoglDSRenderableCanvas.h"
#include "../target/deoglRenderTarget.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class deoglCanvasView
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvasView::deoglCanvasView( deGraphicOpenGl &ogl, deCanvasView &canvas ) :
deoglCanvas( ogl, canvas ),
pCanvasView( canvas ),
pRCanvasView( NULL ),
pDirtyPaint( false ),
pResizeRenderTarget( false ),
pSyncRequestSend( false ){
}

deoglCanvasView::~deoglCanvasView(){
	// notify owners we are about to be deleted. required since owners hold only a weak pointer
	// to the dynamic skin and are notified only after switching to a new dynamic skin. in this
	// case they can not use the old pointer to remove themselves from the dynamic skin
	int i, count = pNotifyRenderables.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglDSRenderableCanvas* )pNotifyRenderables.GetAt( i ) )->DropCanvasView();
	}
	
	count = pNotifyCanvas.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglCanvasCanvasView* )pNotifyCanvas.GetAt( i ) )->DropCanvasView();
	}
}



// Management
///////////////

void deoglCanvasView::DropRCanvas(){
	pRCanvasView = NULL;
	deoglCanvas::DropRCanvas();
}

void deoglCanvasView::ChildOrderChanged(){
	ContentChanged(); // cause rebuild of child rcanvas
}

void deoglCanvasView::SetDirtyPaint(){
	if( pDirtyPaint ){
		return;
	}
	
	pDirtyPaint = true;
	SetDirtyParentPaint(); // goes up to parent
	pRequiresSync();
}

void deoglCanvasView::SyncToRender(){
	// prepare children first since deoglCanvas::PrepareForRender() calls
	// PrepareContentForRender() which would result in NULL RCanvas to be added
	deCanvas *child = pCanvasView.GetRootCanvas();
	
	while( child ){
		( ( deoglCanvas* )child->GetPeerGraphic() )->SyncToRender();
		child = child->GetLLViewNext();
	}
	
	// if paint dirty update the paint tracker in the render counterpart
	if( pDirtyPaint ){
		pDirtyPaint = false;
		
		if( pRCanvasView ){
			pRCanvasView->IncrementPaintTracker();
		}
	}
	
	// resize render target if existing and required
	if( pResizeRenderTarget ){
		pResizeRenderTarget = false;
		
		if( pRCanvasView ){
			pRCanvasView->SetResizeRenderTarget();
		}
	}
	
	// now it is safe to process the rest of the update
	pSyncRequestSend = false;
	deoglCanvas::SyncToRender();
}

void deoglCanvasView::SyncContentToRender(){
	deCanvas *child = pCanvasView.GetRootCanvas();
	
	pRCanvasView->RemoveAllChildren();
	
	while( child ){
		pRCanvasView->AddChild( ( ( deoglCanvas* )child->GetPeerGraphic() )->GetRCanvas() );
		child = child->GetLLViewNext();
	}
}



// Notifications
//////////////////

void deoglCanvasView::PositionChanged(){
	deoglCanvas::PositionChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::SizeChanged(){
	deoglCanvas::SizeChanged();
	pResizeRenderTarget = true;
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::TransformChanged(){
	deoglCanvas::TransformChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::ColorTransformChanged(){
	deoglCanvas::ColorTransformChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::VisibleChanged(){
	deoglCanvas::VisibleChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::OrderChanged(){
	deoglCanvas::OrderChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::TransparencyChanged(){
	deoglCanvas::TransparencyChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::BlendModeChanged(){
	deoglCanvas::BlendModeChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::ContentChanged(){
	deoglCanvas::ContentChanged();
	pDirtyPaint = true;
	pRequiresSync();
}


// Protected Functions
////////////////////////

deoglRCanvas *deoglCanvasView::CreateRCanvas(){
	pRCanvasView = new deoglRCanvasView( GetOgl().GetRenderThread() );
	return pRCanvasView;
}



// Private Functions
//////////////////////

void deoglCanvasView::pRequiresSync(){
	if( pSyncRequestSend ){
		return;
	}
	
	pSyncRequestSend = true;
	
	int i, count = pNotifyRenderables.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglDSRenderableCanvas* )pNotifyRenderables.GetAt( i ) )->CanvasViewRequiresSync();
	}
	
	count = pNotifyCanvas.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglCanvasCanvasView* )pNotifyCanvas.GetAt( i ) )->CanvasViewRequiresSync();
	}
}
