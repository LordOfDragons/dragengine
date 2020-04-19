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



// #define DO_TIMING 1

#ifdef DO_TIMING
#include "dragengine/common/utils/decTimer.h"
static decTimer timerTotal;
static decTimer timer;

#define DEBUG_RESET_TIMERS					timer.Reset(); timerTotal.Reset()
#define DEBUG_PRINT_TIMER(ogl,what)			(ogl).LogInfoFormat( "RCanvasRenderWorld Timer: %s = %iys", what, ( int )( timer.GetElapsedTime() * 1000000.0 ) )
#define DEBUG_PRINT_TIMER_TOTAL(ogl,what)	(ogl).LogInfoFormat( "RCanvasRenderWorld Timer-Total %s = %iys", what, ( int )( timerTotal.GetElapsedTime() * 1000000.0 ) )
#else
#define DEBUG_RESET_TIMERS
#define DEBUG_PRINT_TIMER(ogl,what)
#define DEBUG_PRINT_TIMER_TOTAL(ogl,what)
#endif



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



void deoglRCanvasRenderWorld::PrepareForRender(){
	if( ! pCamera || ! pCamera->GetParentWorld() ){
		return;
	}
	
	if( pForceToneMapAdaption ){
		pCamera->SetForceToneMapAdaption( true );
		pForceToneMapAdaption = false;
	}
	
	pCamera->GetParentWorld()->PrepareForRender();
}

void deoglRCanvasRenderWorld::Render( const deoglRenderCanvasContext &context ){
	const deoglRenderCanvasContext renderWorldContext( context, *this );
	GetRenderThread().GetRenderers().GetCanvas().DrawCanvasRenderWorld( renderWorldContext, *this );
}
