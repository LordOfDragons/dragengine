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

#include "deoglRenderPlan.h"
#include "deoglRenderPlanTasks.h"
#include "parallel/deoglRPTBuildRTsDepth.h"
#include "parallel/deoglRPTBuildRTsGeometry.h"
#include "../../deGraphicOpenGl.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>


// Class deoglRenderPlanTasks
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanTasks::deoglRenderPlanTasks( deoglRenderPlan &plan ) :
pPlan( plan ),

pSolidDepthTask( nullptr ),
pSolidDepthOutlineTask( nullptr ),
pSolidGeometryTask( nullptr ),
pSolidGeometryHeight1Task( nullptr ),
pSolidGeometryHeight2Task( nullptr ),
pSolidGeometryOutlineTask( nullptr ),
pSolidDecalsTask( nullptr ),

pSolidDepthXRayTask( nullptr ),
pSolidDepthOutlineXRayTask( nullptr ),
pSolidGeometryXRayTask( nullptr ),
pSolidGeometryHeight1XRayTask( nullptr ),
pSolidGeometryHeight2XRayTask( nullptr ),
pSolidGeometryOutlineXRayTask( nullptr ),
pSolidDecalsXRayTask( nullptr ),

pTaskDepth( NULL ),
pTaskGeometry( NULL )
{
	deoglRenderThread &renderthread = plan.GetRenderThread();
	
	pSolidDepthTask = new deoglRenderTask( renderthread );
	pSolidDepthOutlineTask = new deoglRenderTask( renderthread );
	pSolidGeometryTask = new deoglRenderTask( renderthread );
	pSolidGeometryHeight1Task = new deoglRenderTask( renderthread );
	pSolidGeometryHeight2Task = new deoglRenderTask( renderthread );
	pSolidGeometryOutlineTask = new deoglRenderTask( renderthread );
	pSolidDecalsTask = new deoglRenderTask( renderthread );
	
	pSolidDepthXRayTask = new deoglRenderTask( renderthread );
	pSolidDepthOutlineXRayTask = new deoglRenderTask( renderthread );
	pSolidGeometryXRayTask = new deoglRenderTask( renderthread );
	pSolidGeometryHeight1XRayTask = new deoglRenderTask( renderthread );
	pSolidGeometryHeight2XRayTask = new deoglRenderTask( renderthread );
	pSolidGeometryOutlineXRayTask = new deoglRenderTask( renderthread );
	pSolidDecalsXRayTask = new deoglRenderTask( renderthread );
}

deoglRenderPlanTasks::~deoglRenderPlanTasks(){
	CleanUp();
	
	if( pSolidDepthTask ){
		delete pSolidDepthTask;
	}
	if( pSolidDepthOutlineTask ){
		delete pSolidDepthOutlineTask;
	}
	if( pSolidGeometryTask ){
		delete pSolidGeometryTask;
	}
	if( pSolidGeometryHeight1Task ){
		delete pSolidGeometryHeight1Task;
	}
	if( pSolidGeometryHeight2Task ){
		delete pSolidGeometryHeight2Task;
	}
	if( pSolidGeometryOutlineTask ){
		delete pSolidGeometryOutlineTask;
	}
	if( pSolidDecalsTask ){
		delete pSolidDecalsTask;
	}
	
	if( pSolidDepthXRayTask ){
		delete pSolidDepthXRayTask;
	}
	if( pSolidDepthOutlineXRayTask ){
		delete pSolidDepthOutlineXRayTask;
	}
	if( pSolidGeometryXRayTask ){
		delete pSolidGeometryXRayTask;
	}
	if( pSolidGeometryHeight1XRayTask ){
		delete pSolidGeometryHeight1XRayTask;
	}
	if( pSolidGeometryHeight2XRayTask ){
		delete pSolidGeometryHeight2XRayTask;
	}
	if( pSolidGeometryOutlineXRayTask ){
		delete pSolidGeometryOutlineXRayTask;
	}
	if( pSolidDecalsXRayTask ){
		delete pSolidDecalsXRayTask;
	}
}



// Management
///////////////

void deoglRenderPlanTasks::StartBuildTasks( const deoglRenderPlanMasked *mask ){
	if( pTaskDepth || pTaskGeometry ){
		DETHROW( deeInvalidParam );
	}
	
	deParallelProcessing &pp = pPlan.GetRenderThread().GetOgl().GetGameEngine()->GetParallelProcessing();
	
	pTaskDepth = new deoglRPTBuildRTsDepth( *this, mask );
	pp.AddTaskAsync( pTaskDepth );
	
	pTaskGeometry = new deoglRPTBuildRTsGeometry( *this, mask );
	pp.AddTaskAsync( pTaskGeometry );
}

void deoglRenderPlanTasks::WaitFinishBuildingTasksDepth(){
	if( ! pTaskDepth ){
		return;
	}
	
	pTaskDepth->GetSemaphore().Wait();
	
	deoglRenderCanvas &rc = pPlan.GetRenderThread().GetRenderers().GetCanvas();
	rc.SampleDebugInfoPlanPrepareBuildRTs( pPlan, pTaskDepth->GetElapsedTime() );
	
	pTaskDepth->FreeReference();
	pTaskDepth = NULL;
	
	// this call does modify a shader parameter block and can thus not be parallel
	pSolidDepthTask->PrepareForRender();
	pSolidDepthOutlineTask->PrepareForRender();
	
	pSolidDepthXRayTask->PrepareForRender();
	pSolidDepthOutlineXRayTask->PrepareForRender();
}

void deoglRenderPlanTasks::WaitFinishBuildingTasksGeometry(){
	if( ! pTaskGeometry ){
		return;
	}
	
	pTaskGeometry->GetSemaphore().Wait();
	
	deoglRenderCanvas &rc = pPlan.GetRenderThread().GetRenderers().GetCanvas();
	rc.SampleDebugInfoPlanPrepareBuildRTs( pPlan, pTaskGeometry->GetElapsedTime() );
	
	pTaskGeometry->FreeReference();
	pTaskGeometry = NULL;
	
	// this call does modify a shader parameter block and can thus not be parallel
	pSolidGeometryTask->PrepareForRender();
	pSolidGeometryHeight1Task->PrepareForRender();
	pSolidGeometryHeight2Task->PrepareForRender();
	pSolidGeometryOutlineTask->PrepareForRender();
	pSolidDecalsTask->PrepareForRender();
	
	pSolidGeometryXRayTask->PrepareForRender();
	pSolidGeometryHeight1XRayTask->PrepareForRender();
	pSolidGeometryHeight2XRayTask->PrepareForRender();
	pSolidGeometryOutlineXRayTask->PrepareForRender();
	pSolidDecalsXRayTask->PrepareForRender();
}

void deoglRenderPlanTasks::CleanUp(){
	WaitFinishBuildingTasksDepth();
	WaitFinishBuildingTasksGeometry();
	
	pSolidDepthTask->Clear();
	pSolidDepthOutlineTask->Clear();
	
	pSolidGeometryTask->Clear();
	pSolidGeometryHeight1Task->Clear();
	pSolidGeometryHeight2Task->Clear();
	pSolidGeometryOutlineTask->Clear();
	pSolidDecalsTask->Clear();
	
	pSolidDepthXRayTask->Clear();
	pSolidDepthOutlineXRayTask->Clear();
	
	pSolidGeometryXRayTask->Clear();
	pSolidGeometryHeight1XRayTask->Clear();
	pSolidGeometryHeight2XRayTask->Clear();
	pSolidGeometryOutlineXRayTask->Clear();
	pSolidDecalsXRayTask->Clear();
}
