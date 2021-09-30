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
#include "../../delayedoperation/deoglDelayedDeletion.h"
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

pTaskDepth( NULL ),
pTaskGeometry( NULL )
{
	pSolidDepthTask = new deoglRenderTask( plan.GetRenderThread() );
	pSolidDepthOutlineTask = new deoglRenderTask( plan.GetRenderThread() );
	pSolidGeometryTask = new deoglRenderTask( plan.GetRenderThread() );
	pSolidGeometryHeight1Task = new deoglRenderTask( plan.GetRenderThread() );
	pSolidGeometryHeight2Task = new deoglRenderTask( plan.GetRenderThread() );
	pSolidGeometryOutlineTask = new deoglRenderTask( plan.GetRenderThread() );
	pSolidDecalsTask = new deoglRenderTask( plan.GetRenderThread() );
}

class deoglRenderPlanTasksDeletion : public deoglDelayedDeletion{
public:
	deoglRenderTask *solidDepthTask;
	deoglRenderTask *solidDepthOutlineTask;
	deoglRenderTask *solidGeometryTask;
	deoglRenderTask *solidGeometryHeight1Task;
	deoglRenderTask *solidGeometryHeight2Task;
	deoglRenderTask *solidGeometryOutlineTask;
	deoglRenderTask *solidDecalsTask;
	
	deoglRenderPlanTasksDeletion() :
	solidDepthTask( nullptr ),
	solidDepthOutlineTask( nullptr ),
	solidGeometryTask( nullptr ),
	solidGeometryHeight1Task( nullptr ),
	solidGeometryHeight2Task( nullptr ),
	solidGeometryOutlineTask( nullptr ),
	solidDecalsTask( nullptr ){
	}
	
	virtual ~deoglRenderPlanTasksDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread& ){
		if( solidDepthTask ){
			delete solidDepthTask;
		}
		if( solidDepthOutlineTask ){
			delete solidDepthOutlineTask;
		}
		if( solidGeometryTask ){
			delete solidGeometryTask;
		}
		if( solidGeometryHeight1Task ){
			delete solidGeometryHeight1Task;
		}
		if( solidGeometryHeight2Task ){
			delete solidGeometryHeight2Task;
		}
		if( solidGeometryOutlineTask ){
			delete solidGeometryOutlineTask;
		}
		if( solidDecalsTask ){
			delete solidDecalsTask;
		}
	}
};

deoglRenderPlanTasks::~deoglRenderPlanTasks(){
	CleanUp();
	
	// delayed deletion of opengl containing objects
	deoglRenderPlanTasksDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglRenderPlanTasksDeletion;
		delayedDeletion->solidDepthTask = pSolidDepthTask;
		delayedDeletion->solidDepthOutlineTask = pSolidDepthOutlineTask;
		delayedDeletion->solidGeometryTask = pSolidGeometryTask;
		delayedDeletion->solidGeometryHeight1Task = pSolidGeometryHeight1Task;
		delayedDeletion->solidGeometryHeight2Task = pSolidGeometryHeight2Task;
		delayedDeletion->solidGeometryOutlineTask = pSolidGeometryOutlineTask;
		delayedDeletion->solidDecalsTask = pSolidDecalsTask;
		pPlan.GetRenderThread().GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pPlan.GetRenderThread().GetLogger().LogException( e );
		//throw; -> otherwise terminate
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
}
