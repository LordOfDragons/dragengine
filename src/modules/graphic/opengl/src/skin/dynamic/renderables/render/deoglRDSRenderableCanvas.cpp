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

#include "deoglRDSRenderableCanvas.h"
#include "../../deoglRDynamicSkin.h"
#include "../../../../renderthread/deoglRenderThread.h"
#include "../../../../canvas/render/deoglRCanvasView.h"
#include "../../../../target/deoglRenderTarget.h"

#include <dragengine/common/exceptions.h>



// Class deoglRDSRenderableCanvas
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRDSRenderableCanvas::deoglRDSRenderableCanvas( deoglRDynamicSkin &dynamicSkin ) :
deoglRDSRenderable( dynamicSkin ),
pCanvas( NULL )
{
	LEAK_CHECK_CREATE( dynamicSkin.GetRenderThread(), DSRenderableCanvas );
}

deoglRDSRenderableCanvas::~deoglRDSRenderableCanvas(){
	LEAK_CHECK_FREE( GetDynamicSkin().GetRenderThread(), DSRenderableCanvas );
	if( pCanvas ){
		pCanvas->FreeReference();
	}
}



// Management
///////////////

void deoglRDSRenderableCanvas::SetCanvas( deoglRCanvasView *canvas ){
	if( canvas == pCanvas ){
		return;
	}
	
	if( pCanvas ){
		pCanvas->FreeReference();
	}
	
	pCanvas = canvas;
	
	if( canvas ){
		canvas->AddReference();
	}
}

void deoglRDSRenderableCanvas::PrepareForRender( const deoglRenderPlanMasked *renderPlanMask ){
	if( pCanvas ){
		pCanvas->PrepareRenderTarget( renderPlanMask );
	}
}

deoglTexture *deoglRDSRenderableCanvas::GetRenderTexture(){
	if( ! pCanvas ){
		return NULL;
	}
	
	deoglRenderTarget * const renderTarget = pCanvas->GetRenderTarget();
	if( ! renderTarget ){
		return NULL;
	}
	
	return renderTarget->GetTexture();
}
