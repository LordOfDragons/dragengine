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

#include "deoglRPTSkyLightFindContent.h"
#include "deoglRLSVisitorCollectElements.h"
#include "../deoglRenderPlan.h"
#include "../deoglRenderPlanSkyLight.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../collidelist/deoglCollideListLight.h"
#include "../../../collidelist/deoglCollideListHTSCluster.h"
#include "../../../collidelist/deoglCollideListHTSector.h"
#include "../../../envmap/deoglEnvironmentMap.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTLogger.h"
#include "../../../utils/collision/deoglDCollisionSphere.h"
#include "../../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>


// Class deoglRPTSkyLightFindContent
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPTSkyLightFindContent::deoglRPTSkyLightFindContent( deoglRenderPlanSkyLight &plan ) :
deParallelTask( &plan.GetPlan().GetRenderThread().GetOgl() ),
pPlan( plan ),
pElapsedTime( 0.0f )
{
	SetMarkFinishedAfterRun( true );
}

deoglRPTSkyLightFindContent::~deoglRPTSkyLightFindContent(){
}



// Management
///////////////

// #define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include <dragengine/common/utils/decTimer.h>
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT(w) if(pPlan.GetPlan().GetDebugTiming()) pPlan.GetPlan().GetRenderThread().GetLogger().LogInfoFormat("RPTSkyLightFindContent: " w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglRPTSkyLightFindContent::Run(){
	if( IsCancelled() ){
		return;
	}
	
	deoglRenderPlan &plan = pPlan.GetPlan();
	
	decTimer timerFindContent;
	try{
		INIT_SPECIAL_TIMING
		
		deoglCollideList &collideList = pPlan.GetCollideList();
		deoglRLSVisitorCollectElements visitor( collideList );
		
		visitor.InitFromFrustum( plan, *pPlan.GetLayer(), 2000.0f );
		visitor.SetCullLayerMask( plan.GetUseLayerMask() );
		visitor.SetLayerMask( plan.GetLayerMask() );
		
		visitor.AddOcclusionTestInputData( pPlan.GetOcclusionTest(), plan.GetWorld()->GetReferencePosition() );
		
		const int shadowMapSize = plan.GetShadowSkySize();
		const int shadowLayerCount = pPlan.GetShadowLayerCount();
		const float splitSizeLimitPixels = 1.0f;
		const float splitFactor = ( 1.0f / ( float )shadowMapSize ) * splitSizeLimitPixels;
		int i;
		
		for( i=0; i<shadowLayerCount; i++ ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = pPlan.GetShadowLayerAt( i );
			const decVector splitMinExtend( sl.minExtend.x, sl.minExtend.y, sl.minExtend.z - 2000.0f );
			const decVector &splitMaxExtend = sl.maxExtend;
			const float sizeThresholdX = ( sl.maxExtend.x - sl.minExtend.x ) * splitFactor;
			const float sizeThresholdY = ( sl.maxExtend.y - sl.minExtend.y ) * splitFactor;
			
			visitor.AddSplit( splitMinExtend, splitMaxExtend, decVector2( sizeThresholdX, sizeThresholdY ) );
		}
		
		visitor.VisitWorldOctree( plan.GetWorld()->GetOctree() );
		
		pPlan.SetFrustumBoxExtend( visitor.GetFrustumBoxMinExtend(), visitor.GetFrustumBoxMaxExtend() );
		SPECIAL_TIMER_PRINT("Octree")
		
		if( plan.GetHeightTerrainView() ){
			visitor.VisitHTView( *plan.GetHeightTerrainView() );
			SPECIAL_TIMER_PRINT("HTSector")
		}
		
		visitor.VisitPropFields( *plan.GetWorld() );
		SPECIAL_TIMER_PRINT("PropField")
		
	}catch( const deException &e ){
		plan.GetRenderThread().GetLogger().LogException( e );
		pSemaphore.Signal();
		throw;
	}
	
	pElapsedTime = timerFindContent.GetElapsedTime();
	pSemaphore.Signal();
}

void deoglRPTSkyLightFindContent::Finished(){
	pSemaphore.Signal(); // in case cancelled before run finished
}

decString deoglRPTSkyLightFindContent::GetDebugName() const{
	return "RPTSkyLightFindContent";
}
