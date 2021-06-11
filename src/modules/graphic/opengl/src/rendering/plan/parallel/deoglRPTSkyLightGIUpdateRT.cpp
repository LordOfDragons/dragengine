/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
#include <string.h>

#include "deoglRPTSkyLightGIUpdateRT.h"
#include "deoglRLSVisitorCollectElements.h"
#include "../deoglRenderPlan.h"
#include "../deoglRenderPlanSkyLight.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../collidelist/deoglCollideListLight.h"
#include "../../../component/deoglRComponent.h"
#include "../../../envmap/deoglEnvironmentMap.h"
#include "../../../gi/deoglGIState.h"
#include "../../../rendering/task/deoglRenderTask.h"
#include "../../../rendering/lod/deoglLODCalculator.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTLogger.h"
#include "../../../utils/collision/deoglDCollisionSphere.h"
#include "../../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>


// Class deoglRPTSkyLightGIUpdateRT
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPTSkyLightGIUpdateRT::deoglRPTSkyLightGIUpdateRT( deoglRenderPlanSkyLight &plan ) :
deParallelTask( &plan.GetPlan().GetRenderThread().GetOgl() ),
pPlan( plan ),
pElapsedTime( 0.0f ){
}

deoglRPTSkyLightGIUpdateRT::~deoglRPTSkyLightGIUpdateRT(){
}



// Management
///////////////

// #define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include <dragengine/common/utils/decTimer.h>
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT(w) if(pPlan.GetPlan().GetDebugTiming()) pPlan.GetPlan().GetRenderThread().GetLogger().LogInfoFormat("RPTSkyLightGIUpdateRT: " w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglRPTSkyLightGIUpdateRT::Run(){
	if( IsCancelled() ){
		return;
	}
	
	try{
		INIT_SPECIAL_TIMING
		decTimer timer;
		
		// components. we can use here the sky light way of choosing LOD levels. but GI shadows
		// are typically broader than sky shadows and thus are more expensive to render. for
		// this reason it is better to use the highest LOD level available. this is acceptable
		// since GI shadow maps are not high resolution and GI rays do not need high resolution
		// shadow maps to work good enough
// 		deoglLODCalculator lodCalculator;
// 		lodCalculator.SetComponentLODMax( pPlan.GetGICollideList() );
		// ^== done inside update RT calls
		
		// update required render tasks
		if( pPlan.GetGIShadowUpdateStatic() ){
			pUpdateStaticRT();
		}
		
		pUpdateDynamicRT();
		
		pElapsedTime = timer.GetElapsedTime();
		SPECIAL_TIMER_PRINT("UpdateRenderTask")
		
	}catch( const deException &e ){
		pPlan.GetPlan().GetRenderThread().GetLogger().LogException( e );
		pSemaphore.Wait();
		throw;
	}
	
	pSemaphore.Signal();
}

void deoglRPTSkyLightGIUpdateRT::Finished(){
	pSemaphore.Signal(); // in case cancelled before run finished
}

decString deoglRPTSkyLightGIUpdateRT::GetDebugName() const{
	return "RPTSkyLightGIUpdateRT";
}



// Private Functions
//////////////////////

void deoglRPTSkyLightGIUpdateRT::pUpdateStaticRT(){
	deoglAddToRenderTask &addToRenderTask = pPlan.GetGIRenderTaskAddStatic();
	deoglCollideList &collideList = pPlan.GetGICollideList();
	int i;
	
	pPlan.GetGIRenderTaskStatic().Clear();
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoShadowNone( true );
	
	// components
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentShadowOrthogonal );
	const int componentCount = collideList.GetComponentCount();
	for( i=0; i<componentCount; i++ ){
		deoglCollideListComponent &component = *collideList.GetComponentAt( i );
		if( component.GetComponent()->GetRenderStatic() ){
			component.SetLODLevelMax();
			addToRenderTask.AddComponent( component );
		}
	}
	
	// height terrain
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estHeightMapShadowOrthogonal );
	addToRenderTask.AddHeightTerrains( collideList, true );
}

void deoglRPTSkyLightGIUpdateRT::pUpdateDynamicRT(){
	deoglAddToRenderTask &addToRenderTask = pPlan.GetGIRenderTaskAddDynamic();
	deoglCollideList &collideList = pPlan.GetGICollideList();
	int i;
	
	pPlan.GetGIRenderTaskDynamic().Clear();
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoShadowNone( true );
	
	// components
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentShadowOrthogonal );
	const int componentCount = collideList.GetComponentCount();
	for( i=0; i<componentCount; i++ ){
		deoglCollideListComponent &component = *collideList.GetComponentAt( i );
		if( ! component.GetComponent()->GetRenderStatic() ){
			component.SetLODLevelMax();
			addToRenderTask.AddComponent( component );
		}
	}
	
	// prop fields
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldShadowOrthogonal );
	addToRenderTask.AddPropFields( collideList, false );
}
