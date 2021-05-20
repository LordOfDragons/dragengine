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

#include "deoglRPTFindContent.h"
#include "../deoglRenderPlan.h"
#include "../deoglPlanVisitorCullElements.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../collidelist/deoglCollideListLight.h"
#include "../../../collidelist/deoglCollideListHTSector.h"
#include "../../../collidelist/deoglCollideListHTSCluster.h"
#include "../../../envmap/deoglEnvironmentMap.h"
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
pElapsedTime( 0.0f ){
}

deoglRPTFindContent::~deoglRPTFindContent(){
}



// Management
///////////////

// #define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include <dragengine/common/utils/decTimer.h>
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
		
		deoglPlanVisitorCullElements &visitor = *pPlan.GetVisitorCullElements();
		deoglDCollisionFrustum * const frustum = pPlan.GetUseFrustum();
		deoglCollideList &collideList = pPlan.GetCollideList();
		deoglRWorld &world = *pPlan.GetWorld();
		int i;
		
		visitor.Init( frustum );
		visitor.SetCullPixelSize( 1.0f );
		visitor.SetCullDynamicComponents( pPlan.GetIgnoreDynamicComponents() );
		visitor.SetCullLayerMask( pPlan.GetUseLayerMask() );
		visitor.SetLayerMask( pPlan.GetLayerMask() );
		
		visitor.VisitWorldOctree( world.GetOctree() );
		SPECIAL_TIMER_PRINT("Octree")
		
		//collideList.SortLinear( world->GetSectorSize(), pCameraSector, pCameraPosition, pCameraInverseMatrix.TransformView() );
		collideList.SortComponentsByModels();
		SPECIAL_TIMER_PRINT("Sort Components")
		
		const int lightCount = collideList.GetLightCount();
		for( i=0; i<lightCount; i++ ){
			deoglCollideListLight &cllight = *collideList.GetLightAt( i );
			cllight.TestInside( pPlan );
			
			// NOTE this call is not thread safe because it potentially causes changes to the
			//      internal array. but this thread is the only one using this function call
			//      so it is safe to do it here
			pPlan.GetLightFor( cllight.GetLight() );
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
		pSemaphore.Wait();
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
