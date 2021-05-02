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
#include "../../../rendering/task/persistent/deoglPersistentRenderTask.h"
#include "../../../rendering/task/persistent/deoglPersistentRenderTaskOwner.h"
#include "../../../renderthread/deoglRenderThread.h"
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
pElapsedTime( 0.0f ),
pCountAdded( 0 ),
pCountRemoved( 0 ),
pElapsedAdded( 0 ),
pElapsedRemoved( 0 ){
}

deoglRPTSkyLightGIUpdateRT::~deoglRPTSkyLightGIUpdateRT(){
}



// Management
///////////////

#define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include "../../../renderthread/deoglRTLogger.h"
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
		decTimer timer, timer2;
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
				pElapsedAdded = ( int )( timer2.GetElapsedTime() * 1e6f );
		
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
				pElapsedRemoved = ( int )( timer2.GetElapsedTime() * 1e6f );
		
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
