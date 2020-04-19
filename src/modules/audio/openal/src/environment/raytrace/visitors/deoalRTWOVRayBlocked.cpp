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

#include "deoalMOVRayBlocked.h"
#include "deoalRTWOVRayBlocked.h"
#include "../../../component/deoalAComponent.h"
#include "../../../component/deoalAComponentTexture.h"
#include "../../../model/deoalAModel.h"
#include "../../../model/deoalModelFace.h"
#include "../../../model/octree/deoalModelOctree.h"
#include "../../../world/octree/deoalWorldOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoalRTWOVRayBlocked
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRTWOVRayBlocked::deoalRTWOVRayBlocked() :
pBlocked( false ){
}

deoalRTWOVRayBlocked::~deoalRTWOVRayBlocked(){
}



// Visiting
/////////////

void deoalRTWOVRayBlocked::SetBlocked( bool blocked ){
	pBlocked = blocked;
}

#ifdef RTWOVRAYBLOCKED_DO_TIMING
#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATLogger.h"
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
void deoalRTWOVRayBlocked::StartTiming(){
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

void deoalRTWOVRayBlocked::EndTiming(){
}
#endif



void deoalRTWOVRayBlocked::VisitBVH( const deoalRTWorldBVH &bvh ){
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	timingAllCount++;
	timerAll.Reset();
	#endif
	
	if( bvh.GetVisitNodeCount() > 0 ){
		VisitNode( bvh, bvh.GetVisitNodes()[ 0 ] );
	}
	
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	timingAll += timerAll.GetElapsedTime();
	#endif
}

void deoalRTWOVRayBlocked::VisitNode( const deoalRTWorldBVH &bvh,
const deoalRTWorldBVH::sVisitNode &node ){
	// visit components
	if( node.componentCount > 0 ){
		const deoalRTWorldBVH::sVisitComponent * const components =
			bvh.GetVisitComponents() + node.firstComponent;
		int i;
		
		for( i=0; i<node.componentCount; i++ ){
			const deoalRTWorldBVH::sVisitComponent &component = components[ i ];
			if( /*pRayHitsSphere( component.sphereCenter, component.sphereRadiusSquared )
			&& */pRayHitsBox( component.center, component.halfSize ) ){
				VisitComponent( component );
				if( pBlocked ){
					return;
				}
			}
		}
		
	// visit child nodes if hit by ray
	}else{
		const deoalRTWorldBVH::sVisitNode &child1 = bvh.GetVisitNodes()[ node.node1 ];
		if( pRayHitsBox( child1.center, child1.halfSize ) ){
			VisitNode( bvh, child1 );
			if( pBlocked ){
				return;
			}
		}
		
		const deoalRTWorldBVH::sVisitNode &child2 = bvh.GetVisitNodes()[ node.node2 ];
		if( pRayHitsBox( child2.center, child2.halfSize ) ){
			VisitNode( bvh, child2 );
			if( pBlocked ){
				return;
			}
		}
	}
}

void deoalRTWOVRayBlocked::VisitComponent( const deoalRTWorldBVH::sVisitComponent &rtcomponent ){
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	timingComponentCount++;
	timerComponent.Reset();
	#endif
	
	const decVector rayOrigin( rtcomponent.inverseMatrix * GetRayOrigin() );
	const decVector rayDirection( rtcomponent.inverseMatrix.TransformNormal( GetRayDirection() ) );
	
	deoalAComponent &component = *rtcomponent.component;
	
	deoalMOVRayBlocked visitor( component );
	visitor.SetRay( rayOrigin, rayDirection );
	
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	timingComponent += timerComponent.GetElapsedTime();
	#endif
	
	if( component.GetBVH() ){
		visitor.VisitBVH( *component.GetBVH() );
		
	}else{
		visitor.VisitBVH( *component.GetModel()->GetRTBVH() );
	}
	
	pBlocked = visitor.GetBlocked();
	
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	const float elapsed = timerComponent.GetElapsedTime();
	timingComponentOctree += elapsed;
	timingComponentOctreeCount++;
	#endif
}
