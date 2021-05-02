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

#include "deoglRPTSkyLightGIPrepare.h"
#include "../deoglRenderPlan.h"
#include "../deoglRenderPlanSkyLight.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../collidelist/deoglCollideListLight.h"
#include "../../../component/deoglRComponent.h"
#include "../../../envmap/deoglEnvironmentMap.h"
#include "../../../gi/deoglGIState.h"
#include "../../../rendering/light/deoglRLSVisitorCollectElements.h"
#include "../../../rendering/task/persistent/deoglPersistentRenderTask.h"
#include "../../../rendering/task/persistent/deoglPersistentRenderTaskOwner.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../utils/collision/deoglDCollisionSphere.h"
#include "../../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>


// Class deoglRPTSkyLightGIPrepare
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPTSkyLightGIPrepare::deoglRPTSkyLightGIPrepare( deoglRenderPlanSkyLight &plan ) :
deParallelTask( &plan.GetPlan().GetRenderThread().GetOgl() ),
pPlan( plan ),
pElapsedFindContent( 0.0f ),
pElapsedUpdateRenderTask( 0.0f ),
pCountAdded( 0 ),
pCountRemoved( 0 ),
pElapsedAdded( 0 ),
pElapsedRemoved( 0 ){
}

deoglRPTSkyLightGIPrepare::~deoglRPTSkyLightGIPrepare(){
}



// Management
///////////////

#define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include "../../../renderthread/deoglRTLogger.h"
#include <dragengine/common/utils/decTimer.h>
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT(w) if(pPlan.GetPlan().GetDebugTiming()) pPlan.GetPlan().GetRenderThread().GetLogger().LogInfoFormat("RPTSkyLightGIPrepare: " w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglRPTSkyLightGIPrepare::Run(){
	if( IsCancelled() ){
		return;
	}
	
	try{
		INIT_SPECIAL_TIMING
		decTimer timer;
		
		pFindContent();
		SPECIAL_TIMER_PRINT("FindContent")
		
		decTimer timer2;
		pUpdateRenderTask();
		pElapsedUpdateRenderTask = timer2.GetElapsedTime();
		SPECIAL_TIMER_PRINT("UpdateRenderTask")
		
		pElapsedFindContent = timer.GetElapsedTime();
		
	}catch( const deException &e ){
		pPlan.GetPlan().GetRenderThread().GetLogger().LogException( e );
		pSemaphore.Wait();
		throw;
	}
	
	pSemaphore.Signal();
}

void deoglRPTSkyLightGIPrepare::Finished(){
	pSemaphore.Signal(); // in case cancelled before run finished
}

decString deoglRPTSkyLightGIPrepare::GetDebugName() const{
	return "RPTSkyLightGIPrepare";
}



// Private Functions
//////////////////////

void deoglRPTSkyLightGIPrepare::pFindContent(){
	// collect elements. we have to calculate the gi position ourself since the real
	// position update happens in parallel. the detection box though is static so
	// we do not have to calculate it on our own
	const deoglGIState &giState = *pPlan.GetPlan().GetRenderGIState();
	
	const decDVector position( giState.WorldClosestGrid( pPlan.GetPlan().GetCameraPosition() ) );
	
	deoglRLSVisitorCollectElements collectElements( pPlan.GetGICollideList() );
	collectElements.InitFromGIBox( position, giState.GetDetectionBox(), *pPlan.GetLayer(), 2000.0f );
	collectElements.SetCullLayerMask( pPlan.GetPlan().GetUseLayerMask() );
	collectElements.SetLayerMask( pPlan.GetPlan().GetLayerMask() );
	
	const decVector &boxMinExtend = collectElements.GetFrustumBoxMinExtend();
	const decVector &boxMaxExtend = collectElements.GetFrustumBoxMaxExtend();
	
	// splits are not used for GI shadows but the minimum size restriction is.
	// to avoid creating another method the AddSplit method is used for this
	const float splitSizeLimitPixels = 1.0f;
	const int shadowMapSize = 1024;
	
	const decVector boxSize( boxMaxExtend - boxMinExtend );
	const float sizeThresholdX = ( boxSize.x / ( float )shadowMapSize ) * splitSizeLimitPixels;
	const float sizeThresholdY = ( boxSize.y / ( float )shadowMapSize ) * splitSizeLimitPixels;
	
	collectElements.AddSplit( boxMinExtend, boxMaxExtend, decVector2( sizeThresholdX, sizeThresholdY ) );
	
	collectElements.VisitWorldOctree( pPlan.GetPlan().GetWorld()->GetOctree() );
}

void deoglRPTSkyLightGIPrepare::pUpdateRenderTask(){
			decTimer timer;
	int i;
	
	// flip update marker. allows to track changes with the least amount of work
	const bool updateMarker = ! pPlan.GetGIRenderTaskUpdateMarker();
	pPlan.SetGIRenderTaskUpdateMarker( updateMarker );
	
	// add components not in task yet
	deoglComponentListener * const componentChangeListener = pPlan.GetGIComponentChangeListener();
	deoglAddToPersistentRenderTask &addToRenderTask = pPlan.GetGIRenderTaskAdd();
	deoglPersistentRenderTask &renderTask = pPlan.GetGIRenderTask();
	deoglCollideList &collideList = pPlan.GetGICollideList();
	
	const int countComponents = collideList.GetComponentCount();
	for( i=0; i<countComponents; i++ ){
		deoglRComponent &component = *collideList.GetComponentAt( i )->GetComponent();
		deoglPersistentRenderTaskOwner *owner = renderTask.GetOwnerWith( &component, component.GetUniqueKey() );
		if( owner ){
			owner->SetUpdateMarker( updateMarker );
			//owner->SetExtends( component.GetMinimumExtend(), component.GetMaximumExtend() );
			continue;
		}
		
		owner = renderTask.AddOwner( &component, component.GetUniqueKey() );
		owner->SetUpdateMarker( updateMarker );
		owner->SetComponent( &component );
		//owner->SetExtends( component.GetMinimumExtend(), component.GetMaximumExtend() );
		
		addToRenderTask.AddComponent( *owner, component, -1 );
		
		component.AddListener( componentChangeListener );
		pCountAdded++;
		
// 		{
// 			const decDVector p(component.GetMatrix().GetPosition());
// 			pRenderThread.GetLogger().LogInfoFormat("GIUpdateRenderTask: Added (%g,%g,%g) %s",
// 				p.x, p.y, p.z, component.GetModel()->GetFilename().GetString());
// 		}
	}
			pElapsedAdded = ( int )( timer.GetElapsedTime() * 1e6f );
	
	// remove components no more in task
	const int allCollideCounts = countComponents; // + countBillboards...
	if( allCollideCounts < renderTask.GetOwnerCount() ){
		decPointerLinkedList::cListEntry *iterOwner = renderTask.GetRootOwner();
		while( iterOwner ){
			deoglPersistentRenderTaskOwner * const owner = ( deoglPersistentRenderTaskOwner* )iterOwner->GetOwner();
			iterOwner = iterOwner->GetNext();
			
			if( owner->GetUpdateMarker() == updateMarker ){
				continue;
			}
			
	// 		{
	// 			const deoglRComponent &component = *( ( deoglRComponent* )owner->GetOwner() );
	// 			const decDVector p(component.GetMatrix().GetPosition());
	// 			pRenderThread.GetLogger().LogInfoFormat("GIUpdateRenderTask: Remove (%g,%g,%g) %s",
	// 				p.x, p.y, p.z, component.GetModel()->GetFilename().GetString());
	// 		}
			
			if( owner->GetComponent() ){
				owner->GetComponent()->RemoveListener( componentChangeListener );
			}
			pCountRemoved++;
			
			renderTask.RemoveOwnedBy( *owner );
			renderTask.RemoveOwner( owner );
		}
	}
			pElapsedRemoved = ( int )( timer.GetElapsedTime() * 1e6f );
	
	// prepare task has to be done in the render thread
}
