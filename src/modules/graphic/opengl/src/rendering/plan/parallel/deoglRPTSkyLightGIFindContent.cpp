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

#include "deoglRPTSkyLightGIFindContent.h"
#include "deoglRLSVisitorCollectElements.h"
#include "../deoglRenderPlan.h"
#include "../deoglRenderPlanSkyLight.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../collidelist/deoglCollideListLight.h"
#include "../../../component/deoglRComponent.h"
#include "../../../envmap/deoglEnvironmentMap.h"
#include "../../../gi/deoglGIState.h"
#include "../../../gi/deoglGICascade.h"
#include "../../../rendering/task/persistent/deoglPersistentRenderTask.h"
#include "../../../rendering/task/persistent/deoglPersistentRenderTaskOwner.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTLogger.h"
#include "../../../sky/deoglRSkyInstanceLayer.h"
#include "../../../sky/deoglSkyLayerGICascade.h"
#include "../../../utils/collision/deoglDCollisionSphere.h"
#include "../../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>


// Class deoglRPTSkyLightGIFindContent
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPTSkyLightGIFindContent::deoglRPTSkyLightGIFindContent(deoglRenderPlanSkyLight &plan) :
deParallelTask(&plan.GetPlan().GetRenderThread().GetOgl()),
pPlan(plan),
pElapsedTime(0.0f)
{
	SetMarkFinishedAfterRun(true);
}

deoglRPTSkyLightGIFindContent::~deoglRPTSkyLightGIFindContent(){
}



// Management
///////////////

// #define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include <dragengine/common/utils/decTimer.h>
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT(w) if(plan.GetDebugTiming()) plan.GetRenderThread().GetLogger().LogInfoFormat("RPTSkyLightGIFindContent: " w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglRPTSkyLightGIFindContent::Run(){
	if(IsCancelled()){
		return;
	}
	
	deoglRenderPlan &plan = pPlan.GetPlan();
	
	try{
		INIT_SPECIAL_TIMING
		decTimer timer;
		
		// collect elements. we have to calculate the gi position ourself since the real
		// position update happens in parallel. the detection box though is static so
		// we do not have to calculate it on our own
		const deoglGIState * const giState = plan.GetUpdateGIState();
		if(! giState){
			DETHROW(deeInvalidParam);
		}
		
		const deoglGICascade &cascade = giState->GetSkyShadowCascade();
		const deoglSkyLayerGICascade * const slgc = pPlan.GetLayer()->GetGICascade(cascade);
		if(! slgc){
			DETHROW(deeInvalidParam);
		}
		
// 		deoglDCollisionFrustum * const frustum = plan.GetUseFrustum();
		deoglCollideList &collideList = pPlan.GetGICollideList();
		deoglRWorld &world = *plan.GetWorld();
		
		deoglRLSVisitorCollectElements collectElements(collideList);
		collectElements.InitFromGIBox(slgc->GetPosition(), cascade.GetDetectionBox(), *pPlan.GetLayer(), 2000.0f);
		collectElements.SetCullLayerMask(plan.GetUseLayerMask());
		collectElements.SetLayerMask(plan.GetLayerMask());
		
		const decVector &boxMinExtend = collectElements.GetFrustumBoxMinExtend();
		const decVector &boxMaxExtend = collectElements.GetFrustumBoxMaxExtend();
		
		// splits are not used for GI shadows but the minimum size restriction is.
		// to avoid creating another method the AddSplit method is used for this
		const float splitSizeLimitPixels = 0.5f; //1.0f; // smaller to avoid problems
		const int shadowMapSize = pPlan.GetGIShadowSize();
		
		const decVector boxSize(boxMaxExtend - boxMinExtend);
		const float sizeThresholdX = (boxSize.x / (float)shadowMapSize) * splitSizeLimitPixels;
		const float sizeThresholdY = (boxSize.y / (float)shadowMapSize) * splitSizeLimitPixels;
		
		collectElements.AddSplit(boxMinExtend, boxMaxExtend, decVector2(sizeThresholdX, sizeThresholdY));
		
		collectElements.VisitWorldOctree(world.GetOctree());
		
// 		if( plan.GetHeightTerrainView() ){
// 			collideList.AddHTSectorsColliding( plan.GetHeightTerrainView(), frustum );
// 		}
// 		
// 		collideList.AddPropFieldsColliding( world, frustum );
		
		SPECIAL_TIMER_PRINT("FindContent")
		pElapsedTime = timer.GetElapsedTime();
		
	}catch(const deException &e){
		plan.GetRenderThread().GetLogger().LogException(e);
		pSemaphore.Signal();
		throw;
	}
	
	pSemaphore.Signal();
}

void deoglRPTSkyLightGIFindContent::Finished(){
	pSemaphore.Signal(); // in case cancelled before run finished
}

decString deoglRPTSkyLightGIFindContent::GetDebugName() const{
	return "RPTSkyLightGIFindContent";
}
