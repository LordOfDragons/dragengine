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

#include "deoglRPTFindContent.h"
#include "deoglRPVisitorFindContent.h"
#include "../deoglRenderPlan.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../collidelist/deoglCollideListLight.h"
#include "../../../collidelist/deoglCollideListHTSector.h"
#include "../../../collidelist/deoglCollideListHTSCluster.h"
#include "../../../envmap/deoglEnvironmentMap.h"
#include "../../../gi/deoglGIState.h"
#include "../../../gi/deoglGICascade.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTLogger.h"
#include "../../../terrain/heightmap/deoglHTViewSector.h"
#include "../../../terrain/heightmap/deoglRHTSector.h"
#include "../../../utils/collision/deoglDCollisionSphere.h"
#include "../../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>


// Class deoglRPTFindContent
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPTFindContent::deoglRPTFindContent( deoglRenderPlan &plan ) :
deParallelTask( &plan.GetRenderThread().GetOgl() ),
pPlan( plan ),
pElapsedTime( 0.0f )
{
	SetMarkFinishedAfterRun( true );
}

deoglRPTFindContent::~deoglRPTFindContent(){
}



// Management
///////////////

// #define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT(w) if(pPlan.GetDebugTiming()) pPlan.GetRenderThread().GetLogger().LogInfoFormat("RPTFindContent: " w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglRPTFindContent::Run(){
	if( IsCancelled() ){
		return;
	}
	
	decTimer timerFindContent;
	try{
		INIT_SPECIAL_TIMING
		
		deoglDCollisionFrustum * const frustum = pPlan.GetUseFrustum();
		deoglCollideList &collideList = pPlan.GetCollideList();
		deoglRWorld &world = *pPlan.GetWorld();
		int i;
		
		// visit for content
		deoglRPVisitorFindContent visitor( pPlan );
		visitor.SetCullPixelSize( 1.0f ); // has to come before Init() call
		visitor.SetCullDynamicComponents( pPlan.GetIgnoreDynamicComponents() );
		visitor.SetCullLayerMask( pPlan.GetUseLayerMask() );
		visitor.SetLayerMask( pPlan.GetLayerMask() );
		visitor.Init( frustum );
		
		const deoglGIState * const gistate = pPlan.GetUpdateGIState();
		if( gistate ){
			// correctly we would have to use here the cascade position but during this
			// parallel task run at an unknown time the cascade position is updated.
			// so instead we use the camera position which is the base for the cascade
			// position update. this can cause a potential error at the boundary of the
			// GI cascade but chances for this are slim. if really a problem half the
			// probe spacing can be added to the extends
			const deoglGICascade &cascade = gistate->GetActiveCascade();
			const decDVector &position = pPlan.GetCameraPosition();
			const decDVector halfExtend( cascade.GetDetectionBox() );
			visitor.EnableGIBox( position - halfExtend, position + halfExtend );
		}
		
		visitor.VisitWorldOctree( world.GetOctree() );
		SPECIAL_TIMER_PRINT("Octree")
		
		// NOTE there is no code anymore requiring components to be sorted. spare ourself the time
		//collideList.SortLinear( world->GetSectorSize(), pCameraSector, pCameraPosition, pCameraInverseMatrix.TransformView() );
		//collideList.SortComponentsByModels();
		//SPECIAL_TIMER_PRINT("Sort Components")
		
		const int lightCount = collideList.GetLightCount();
		for( i=0; i<lightCount; i++ ){
			deoglCollideListLight &cllight = *collideList.GetLightAt( i );
			cllight.TestInside( pPlan );
		}
		SPECIAL_TIMER_PRINT("Lights")
		
		if( pPlan.GetHeightTerrainView() ){
			collideList.AddHTSectorsColliding( pPlan.GetHeightTerrainView(), frustum );
				/*{
				const int sc = collideList.GetHTSectorCount();
				for(i=0; i<sc; i++){
					const deoglCollideListHTSector &s = *collideList.GetHTSectorAt(i);
					const decPoint &c = s.GetSector()->GetSector().GetCoordinates();
					const int cc = s.GetSector()->GetSector().GetClusterCount();
					pPlan.GetRenderThread().GetLogger().LogInfoFormat(
						"FindContent: HTSector (%d,%d) %d/%d\n", c.x, c.y, s.GetClusterCount(), cc * cc);
				}
				}*/
			SPECIAL_TIMER_PRINT("HTSector")
		}
		
		collideList.AddPropFieldsColliding( world, *frustum );
		SPECIAL_TIMER_PRINT("PropField")
		
		
		
		// HACK: add environment maps using a simple hack until we have something better
		const deoglEnvironmentMapList &envMapList = world.GetEnvMapList();
		deoglDCollisionSphere envMapSphere( decDVector(), 20.0 );
		const int envMapCount = envMapList.GetCount();
		deoglEnvironmentMap *envMap;
		int envMapIndex;
		
		for( envMapIndex=0; envMapIndex<envMapCount; envMapIndex++ ){
			envMap = envMapList.GetAt( envMapIndex );
			
			if( envMap->GetSkyOnly() ){
				//pCollideList.GetEnvironmentMapList().Add( envMap );
				
			}else{
				envMapSphere.SetCenter( envMap->GetPosition() );
				
				if( frustum->SphereHitsFrustum( &envMapSphere ) ){
					collideList.GetEnvironmentMapList().Add( envMap );
				}
			}
		}
		SPECIAL_TIMER_PRINT("EnvMap")
		
	}catch( const deException &e ){
		pPlan.GetRenderThread().GetLogger().LogException( e );
		pSemaphore.Signal();
		throw;
	}
	
	pElapsedTime = timerFindContent.GetElapsedTime();
	pSemaphore.Signal();
}

void deoglRPTFindContent::Finished(){
	pSemaphore.Signal(); // in case cancelled before run finished
}

decString deoglRPTFindContent::GetDebugName() const{
	return "RPTFindContent";
}
