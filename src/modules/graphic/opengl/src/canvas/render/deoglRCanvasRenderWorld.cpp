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

#include "deoglRCanvasRenderWorld.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../rendering/defren/deoglDeferredRendering.h"
#include "../../rendering/plan/deoglRenderPlan.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../world/deoglRCamera.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvasRenderWorld
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvasRenderWorld::deoglRCanvasRenderWorld( deoglRenderThread &renderThread ) :
deoglRCanvas( renderThread ),
pCamera( NULL ),
pForceToneMapAdaption( false ){
	LEAK_CHECK_CREATE( renderThread, CanvasRenderWorld );
}

deoglRCanvasRenderWorld::~deoglRCanvasRenderWorld(){
	LEAK_CHECK_FREE( GetRenderThread(), CanvasRenderWorld );
	SetCamera( NULL );
}



// Management
///////////////

void deoglRCanvasRenderWorld::SetCamera( deoglRCamera *camera ){
	if( camera == pCamera ){
		return;
	}
	
	if( pCamera ){
		pCamera->FreeReference();
	}
	
	pCamera = camera;
	
	if( camera ){
		camera->AddReference();
	}
	
	pForceToneMapAdaption = true;
}



void deoglRCanvasRenderWorld::PrepareForRender( const deoglRenderPlanMasked *renderPlanMask ){
	if( ! pCamera || ! pCamera->GetParentWorld() ){
		return;
	}
	
	deoglRCanvas::PrepareForRender( renderPlanMask );
	
	if( pForceToneMapAdaption ){
		pCamera->SetForceToneMapAdaption( true );
		pForceToneMapAdaption = false;
	}
	
	// this is called during DrawCanvasRenderWorld
	//pCamera->GetParentWorld()->PrepareForRender( pCamera->GetPlan() );
}

void deoglRCanvasRenderWorld::Render( const deoglRenderCanvasContext &context ){
	const deoglRenderCanvasContext renderWorldContext( context, *this );
	GetRenderThread().GetRenderers().GetCanvas().DrawCanvasRenderWorld( renderWorldContext, *this );
}
