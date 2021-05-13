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

#include "deoglRCanvasCanvasView.h"
#include "deoglRCanvasView.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvasRenderTarget
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvasCanvasView::deoglRCanvasCanvasView( deoglRenderThread &renderThread ) :
deoglRCanvas( renderThread ),
pCanvasView( NULL ),
pTCClampMin( 0.0f, 0.0f ),
pTCClampMax( 1.0f, 1.0f ){
	LEAK_CHECK_CREATE( renderThread, CanvasCanvasView );
}

deoglRCanvasCanvasView::~deoglRCanvasCanvasView(){
	LEAK_CHECK_FREE( GetRenderThread(), CanvasCanvasView );
	SetCanvasView( NULL );
}



// Management
///////////////

void deoglRCanvasCanvasView::SetCanvasView( deoglRCanvasView *canvasView ){
	if( canvasView == pCanvasView ){
		return;
	}
	
	if( pCanvasView ){
		pCanvasView->FreeReference();
	}
	pCanvasView = canvasView;
	if( canvasView ){
		canvasView->AddReference();
	}
}

void deoglRCanvasCanvasView::SetTCTransform( const decTexMatrix2 &transform ){
	pTCTransform = transform;
}

void deoglRCanvasCanvasView::SetTCClampMinimum( const decVector2 &clamp ){
	pTCClampMin = clamp;
}

void deoglRCanvasCanvasView::SetTCClampMaximum( const decVector2 &clamp ){
	pTCClampMax = clamp;
}



void deoglRCanvasCanvasView::PrepareForRender( const deoglRenderPlanMasked *renderPlanMask ){
	if( ! pCanvasView ){
		return;
	}
	
	deoglRCanvas::PrepareForRender( renderPlanMask );
	pCanvasView->PrepareRenderTarget( renderPlanMask );
}

void deoglRCanvasCanvasView::Render( const deoglRenderCanvasContext &context ){
	if( ! pCanvasView ){
		return;
	}
	
	deoglRenderCanvasContext viewContext( context, *this );
	viewContext.SetTCClampMinimum( pTCClampMin );
	viewContext.SetTCClampMaximum( pTCClampMax );
	GetRenderThread().GetRenderers().GetCanvas().DrawCanvasCanvasView( viewContext, *this );
}
