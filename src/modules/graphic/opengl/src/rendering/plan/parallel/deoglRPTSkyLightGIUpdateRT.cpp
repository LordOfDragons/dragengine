/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
pElapsedTime( 0.0f )
{
	SetMarkFinishedAfterRun( true );
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
		pSemaphore.Signal();
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
	addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etShadowOrthogonal );
	
	// components
	const int componentCount = collideList.GetComponentCount();
	for( i=0; i<componentCount; i++ ){
		deoglCollideListComponent &component = *collideList.GetComponentAt( i );
		if( pIsComponentStatic( *component.GetComponent() ) ){
			component.SetLODLevelMax();
			addToRenderTask.AddComponent( component );
		}
	}
	
	// height terrain
	addToRenderTask.AddHeightTerrains( collideList, true );
	addToRenderTask.AddHeightTerrainSectorClusters( collideList, true );
}

void deoglRPTSkyLightGIUpdateRT::pUpdateDynamicRT(){
	deoglAddToRenderTask &addToRenderTask = pPlan.GetGIRenderTaskAddDynamic();
	deoglCollideList &collideList = pPlan.GetGICollideList();
	int i;
	
	pPlan.GetGIRenderTaskDynamic().Clear();
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoShadowNone( true );
	addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etShadowOrthogonal );
	
	// components
	const int componentCount = collideList.GetComponentCount();
	for( i=0; i<componentCount; i++ ){
		deoglCollideListComponent &component = *collideList.GetComponentAt( i );
		if( ! pIsComponentStatic( *component.GetComponent() ) ){
			component.SetLODLevelMax();
			addToRenderTask.AddComponent( component );
		}
	}
	
	// prop fields
	addToRenderTask.AddPropFields( collideList, false );
	addToRenderTask.AddPropFieldClusters( collideList, false );
}

bool deoglRPTSkyLightGIUpdateRT::pIsComponentStatic( const deoglRComponent& component ) const{
	// GetRenderStatic() is not working well since this can change frequently also for
	// dynmic objects when resting for a couple of seconds. we want though to avoid
	// updating static sky shadow maps as little as possible. for this reason we use
	// here only the render mode
	
	//return component.GetRenderStatic();
	
	return component.GetMovementHint() == deComponent::emhStationary
		&& component.GetRenderMode() == deoglRComponent::ermStatic;
}
