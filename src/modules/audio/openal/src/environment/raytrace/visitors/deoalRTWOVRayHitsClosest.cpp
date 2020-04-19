/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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
#include <stdlib.h>
#include <string.h>

#include "deoalMOVRayHitsClosest.h"
#include "deoalRTWOVRayHitsClosest.h"
#include "../../../component/deoalAComponent.h"
#include "../../../component/deoalAComponentTexture.h"
#include "../../../model/deoalAModel.h"
#include "../../../model/deoalModelFace.h"
#include "../../../model/octree/deoalModelOctree.h"
#include "../../../world/octree/deoalRTWorldBVH.h"

#include <dragengine/common/exceptions.h>



// Class deoalRTWOVRayHitsClosest
///////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRTWOVRayHitsClosest::deoalRTWOVRayHitsClosest() :
pHasResult( false ),
pFrontFacing( true ),
pLimitDistance( 1.0f ){
}

deoalRTWOVRayHitsClosest::~deoalRTWOVRayHitsClosest(){
}



// Visiting
/////////////

void deoalRTWOVRayHitsClosest::SetHasResult( bool hasResult ){
	pHasResult = hasResult;
}

void deoalRTWOVRayHitsClosest::SetFrontFacing( bool frontFacing ){
	pFrontFacing = frontFacing;
}

void deoalRTWOVRayHitsClosest::SetLimitDistance( float limitDistance ){
	pLimitDistance = limitDistance;
}

#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATLogger.h"
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
void deoalRTWOVRayHitsClosest::StartTiming(){
	timingAll = 0.0f;
	timingAllCount = 0;
	timingComponent = 0.0f;
	timingComponentCount = 0;
	timingComponentOctree = 0.0f;
	timingComponentOctreeCount = 0;
	timingFaces = 0.0f;
	timingFacesCount = 0;
	timerAll.Reset();
}

void deoalRTWOVRayHitsClosest::EndTiming(){
}
#endif



void deoalRTWOVRayHitsClosest::VisitBVH( const deoalRTWorldBVH & bvh ){
	#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
	timingAllCount++;
	timerAll.Reset();
	#endif
	
	pHasResult = false;
	
	if( bvh.GetVisitNodeCount() > 0 ){
		pVisitNode( bvh, bvh.GetVisitNodes()[ 0 ] );
	}
	
	#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
	timingAll += timerAll.GetElapsedTime();
	#endif
}

void deoalRTWOVRayHitsClosest::VisitComponent( const deoalRTWorldBVH::sVisitComponent &rtcomponent ){
	// WARNING everything in here has to be thread-safe
	#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
	timingComponentCount++;
	timerComponent.Reset();
	#endif
	
	const deoalAComponent &component = *rtcomponent.component;
	const deoalAModel &model = *component.GetModel();
	
	const decVector rayOrigin( rtcomponent.inverseMatrix * GetRayOrigin() );
	const decVector rayDirection( rtcomponent.inverseMatrix.TransformNormal( GetRayDirection() ) );
	
	deoalMOVRayHitsClosest visitor( *rtcomponent.component, model );
	visitor.SetRay( rayOrigin, rayDirection );
	visitor.SetFrontFacing( pFrontFacing );
	visitor.SetLimitDistance( pLimitDistance );
	
	#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
	timingComponent += timerComponent.GetElapsedTime();
	#endif
	
	if( component.GetBVH() ){
		visitor.VisitBVH( *component.GetBVH() );
		
	}else{
		visitor.VisitBVH( *model.GetRTBVH() );
	}
	
	if( ! visitor.GetHasResult() || visitor.GetResultDistance() >= pLimitDistance ){
		#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
		const float elapsed = timerComponent.GetElapsedTime();
		timingComponentOctree += elapsed;
		timingComponentOctreeCount++;
		#ifdef MOVRAYHITSFACES_DO_TIMIING
		timingFaces += visitor.timing;
		timingFacesCount += visitor.timingCount;
		#endif
		#endif
		return;
	}
	
	const decDMatrix &matrix = component.GetMatrix();
	decVector normal( matrix.TransformNormal( visitor.GetResultNormal() ) );
	if( component.GetHasScaling() ){
		normal.Normalize();
	}
	
	pResult.SetComponentFace( visitor.GetRayLength() * visitor.GetResultDistance(),
		component.GetMatrix() * visitor.GetResultPoint(), normal, rtcomponent.component,
		visitor.GetResultFace(), pFrontFacing );
	pHasResult = true;
	
	pLimitDistance = visitor.GetResultDistance();
	
	#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
	const float elapsed = timerComponent.GetElapsedTime();
	timingComponentOctree += elapsed;
	timingComponentOctreeCount++;
	#ifdef MOVRAYHITSFACES_DO_TIMIING
	timingFaces += visitor.timing;
	timingFacesCount += visitor.timingCount;
	#endif
	#endif
}



// Protected Functions
////////////////////////

struct sSortNode{
	const deoalRTWorldBVH::sVisitNode *node;
	float distance;
};

void deoalRTWOVRayHitsClosest::pVisitNode( const deoalRTWorldBVH &bvh, const deoalRTWorldBVH::sVisitNode &node ){
	// visit components
	if( node.componentCount > 0 ){
		const deoalRTWorldBVH::sVisitComponent * const components =
			bvh.GetVisitComponents() + node.firstComponent;
		float closestDistance;
		int i;
		
		for( i=0; i<node.componentCount; i++ ){
			const deoalRTWorldBVH::sVisitComponent &component = components[ i ];
			if( pRayHitsBox( component.center, component.halfSize, closestDistance )
			&& closestDistance < pLimitDistance ){
				VisitComponent( component );
			}
		}
		
	// visit child nodes if hit by ray
	}else{
		#if 1
		const deoalRTWorldBVH::sVisitNode &child1 = bvh.GetVisitNodes()[ node.node1 ];
		const deoalRTWorldBVH::sVisitNode &child2 = bvh.GetVisitNodes()[ node.node2 ];
		float closestDistance;
		
		if( ( child2.center - child1.center ) * GetRayDirection() > 0.0f ){
			const deoalRTWorldBVH::sVisitNode &child1 = bvh.GetVisitNodes()[ node.node1 ];
			if( pRayHitsBox( child1.center, child1.halfSize, closestDistance )
			&& closestDistance < pLimitDistance ){
				pVisitNode( bvh, child1 );
			}
			
			const deoalRTWorldBVH::sVisitNode &child2 = bvh.GetVisitNodes()[ node.node2 ];
			if( pRayHitsBox( child2.center, child2.halfSize, closestDistance )
			&& closestDistance < pLimitDistance ){
				pVisitNode( bvh, child2 );
			}
			
		}else{
			const deoalRTWorldBVH::sVisitNode &child2 = bvh.GetVisitNodes()[ node.node2 ];
			if( pRayHitsBox( child2.center, child2.halfSize, closestDistance )
			&& closestDistance < pLimitDistance ){
				pVisitNode( bvh, child2 );
			}
			
			const deoalRTWorldBVH::sVisitNode &child1 = bvh.GetVisitNodes()[ node.node1 ];
			if( pRayHitsBox( child1.center, child1.halfSize, closestDistance )
			&& closestDistance < pLimitDistance ){
				pVisitNode( bvh, child1 );
			}
		}
		
		#else
		float closestDistance;
		
		const deoalRTWorldBVH::sVisitNode &child1 = bvh.GetVisitNodes()[ node.node1 ];
		if( pRayHitsBox( child1.center, child1.halfSize, closestDistance )
		&& closestDistance < pLimitDistance ){
			pVisitNode( bvh, child1 );
		}
		
		const deoalRTWorldBVH::sVisitNode &child2 = bvh.GetVisitNodes()[ node.node2 ];
		if( pRayHitsBox( child2.center, child2.halfSize, closestDistance )
		&& closestDistance < pLimitDistance ){
			pVisitNode( bvh, child2 );
		}
		#endif
	}
}
