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
#include "deoglRenderPlanTaskFindContent.h"
#include "deoglPlanVisitorCullElements.h"
#include "../../deGraphicOpenGl.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListLight.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../utils/collision/deoglDCollisionSphere.h"
#include "../../world/deoglRWorld.h"


// Class deoglRenderPlanTaskFindContent
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanTaskFindContent::deoglRenderPlanTaskFindContent( deoglRenderPlan &plan ) :
deParallelTask( &plan.GetRenderThread().GetOgl() ),
pPlan( plan ){
}

deoglRenderPlanTaskFindContent::~deoglRenderPlanTaskFindContent(){
}



// Management
///////////////

#define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include "../../renderthread/deoglRTLogger.h"
#include <dragengine/common/utils/decTimer.h>
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT(w) if(pPlan.GetDebugTiming()) pPlan.GetRenderThread().GetLogger().LogInfoFormat("RenderPlan.Prepare: " w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglRenderPlanTaskFindContent::Run(){
	INIT_SPECIAL_TIMING
	
	deoglPlanVisitorCullElements &visitor = *pPlan.GetVisitorCullElements();
	deoglDCollisionFrustum * const frustum = pPlan.GetUseFrustum();
	deoglCollideList &collideList = pPlan.GetCollideList();
	deoglRWorld &world = *pPlan.GetWorld();
	
	visitor.Init( frustum );
	visitor.SetCullPixelSize( 1.0f );
	visitor.SetCullDynamicComponents( pPlan.GetIgnoreDynamicComponents() );
	visitor.SetCullLayerMask( pPlan.GetUseLayerMask() );
	visitor.SetLayerMask( pPlan.GetLayerMask() );
	
	visitor.VisitWorldOctree( world.GetOctree() );
	SPECIAL_TIMER_PRINT("Octree")
	
	if( pPlan.GetHeightTerrainView() ){
		collideList.AddHTSectorsColliding( pPlan.GetHeightTerrainView(), frustum );
		SPECIAL_TIMER_PRINT("HTSector")
	}
	
	collideList.AddPropFieldsColliding( world, frustum );
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
}

void deoglRenderPlanTaskFindContent::Finished(){
}

decString deoglRenderPlanTaskFindContent::GetDebugName() const{
	return "RenderPlanTaskFindContent";
}
