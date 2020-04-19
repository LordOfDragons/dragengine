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

#include "deoglDSRenderableCanvas.h"
#include "render/deoglRDSRenderableCanvas.h"
#include "../deoglDynamicSkin.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../canvas/deoglCanvasView.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableCanvas.h>



// Class deoglDSRenderableCanvas
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDSRenderableCanvas::deoglDSRenderableCanvas(
deoglDynamicSkin &dynamicSkin, const deDSRenderableCanvas &renderable ) :
deoglDSRenderable( dynamicSkin, renderable ),
pRenderableCanvas( renderable ),
pRRenderableCanvas( NULL ),
pCanvasView( NULL ),
pDirty( true )
{
	try{
		pRRenderableCanvas = new deoglRDSRenderableCanvas( *dynamicSkin.GetRDynamicSkin() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDSRenderableCanvas::~deoglDSRenderableCanvas(){
	pCleanUp();
}



// Management
///////////////

deoglRDSRenderable *deoglDSRenderableCanvas::GetRRenderable() const{
	return pRRenderableCanvas;
}

void deoglDSRenderableCanvas::RenderableChanged(){
	if( pCanvasView ){
		pCanvasView->GetNotifyRenderables().Remove( this );
	}
	
	if( pRenderableCanvas.GetCanvas() ){
		pCanvasView = ( deoglCanvasView* )pRenderableCanvas.GetCanvas()->GetPeerGraphic();
		pCanvasView->GetNotifyRenderables().Add( this );
		
	}else{
		pCanvasView = NULL;
	}
	
	pDirty = true;
}

void deoglDSRenderableCanvas::SyncToRender(){
	if( pCanvasView ){
		pCanvasView->SyncToRender();
	}
	
	if( pDirty ){
		pRRenderableCanvas->SetName( pRenderableCanvas.GetName() );
		
		if( pCanvasView ){
			pRRenderableCanvas->SetCanvas( pCanvasView->GetRCanvasView() );
			
		}else{
			pRRenderableCanvas->SetCanvas( NULL );
		}
		
		pDirty = false;
	}
}

void deoglDSRenderableCanvas::CanvasViewRequiresSync(){
	pDynamicSkin.RenderableRequiresSync();
}

void deoglDSRenderableCanvas::DropCanvasView(){
	pCanvasView = NULL;
}



// Private Functions
//////////////////////

void deoglDSRenderableCanvas::pCleanUp(){
	if( pRRenderableCanvas ){
		pRRenderableCanvas->FreeReference();
	}
	
	if( pCanvasView ){
		pCanvasView->GetNotifyRenderables().Remove( this );
	}
}
