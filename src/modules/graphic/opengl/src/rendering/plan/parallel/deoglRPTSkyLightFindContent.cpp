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

#include "deoglRPTSkyLightFindContent.h"
#include "deoglRLSVisitorCollectElements.h"
#include "../deoglRenderPlan.h"
#include "../deoglRenderPlanSkyLight.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../collidelist/deoglCollideListLight.h"
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
pElapsedTime( 0.0f ){
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
	
	decTimer timerFindContent;
	try{
		INIT_SPECIAL_TIMING
		
		deoglCollideList &collideList = pPlan.GetCollideList();
		deoglRLSVisitorCollectElements visitor( collideList );
		
		visitor.InitFromFrustum( pPlan.GetPlan(), *pPlan.GetLayer(), 2000.0f );
		visitor.SetCullLayerMask( pPlan.GetPlan().GetUseLayerMask() );
		visitor.SetLayerMask( pPlan.GetPlan().GetLayerMask() );
		
		visitor.AddOcclusionTestInputData( pPlan.GetOcclusionTest(),
			pPlan.GetPlan().GetWorld()->GetReferencePosition() );
		
		const int shadowMapSize = pPlan.GetPlan().GetShadowSkySize();
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
		
		visitor.VisitWorldOctree( pPlan.GetPlan().GetWorld()->GetOctree() );
		
		pPlan.SetFrustumBoxExtend( visitor.GetFrustumBoxMinExtend(), visitor.GetFrustumBoxMaxExtend() );
		
		SPECIAL_TIMER_PRINT("Octree")
		
	}catch( const deException &e ){
		pPlan.GetPlan().GetRenderThread().GetLogger().LogException( e );
		pSemaphore.Wait();
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
