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

#include "deoglGIAreaTracker.h"
#include "../billboard/deoglRBillboard.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideListHTSector.h"
#include "../collidelist/deoglCollideListHTSCluster.h"
#include "../collidelist/deoglCollideListPropField.h"
#include "../collidelist/deoglCollideListPropFieldType.h"
#include "../collidelist/deoglCollideListPropFieldCluster.h"
#include "../component/deoglRComponent.h"
#include "../propfield/deoglRPropField.h"
#include "../propfield/deoglRPropFieldType.h"
#include "../propfield/deoglPropFieldCluster.h"
#include "../model/deoglRModel.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../terrain/heightmap/deoglHTSCluster.h"
#include "../terrain/heightmap/deoglHTView.h"
#include "../terrain/heightmap/deoglHTViewSector.h"
#include "../terrain/heightmap/deoglRHTSector.h"
#include "../utils/collision/deoglDCollisionDetection.h"
#include "../world/deoglWorldOctree.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>



// Class deoglGIAreaTracker
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIAreaTracker::deoglGIAreaTracker() :
pAllLeaving( false ),
pWorld( NULL ),
pUpdateThreshold( 1.0 ),
pValid( false ){
}



// Management
///////////////

void deoglGIAreaTracker::SetWorld( deoglRWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	pWorld = world;
	pValid = false;
}

void deoglGIAreaTracker::SetHalfExtends( const decDVector &halfExtends ){
	if( halfExtends.IsEqualTo( pHalfExtends ) ){
		return;
	}
	
	pHalfExtends = halfExtends;
	
	pBoxLastUpdate.minExtend = pLastUpdatePosition - pHalfExtends;
	pBoxLastUpdate.maxExtend = pLastUpdatePosition + pHalfExtends;
	
	pBox.minExtend = pPosition - pHalfExtends;
	pBox.maxExtend = pPosition + pHalfExtends;
	
	pValid = false;
}

void deoglGIAreaTracker::SetLayerMask( const decLayerMask &layerMask ){
	if( layerMask == pLayerMask ){
		return;
	}
	
	pLayerMask = layerMask;
	pValid = false;
}

void deoglGIAreaTracker::SetUpdateThreshold( double threshold ){
	pUpdateThreshold = threshold;
}

void deoglGIAreaTracker::SetPosition( const decDVector &position ){
	if( pValid && position.IsEqualTo( pPosition, pUpdateThreshold ) ){
		return;
	}
	
	pPosition = position;
	
	pBox.minExtend = pPosition - pHalfExtends;
	pBox.maxExtend = pPosition + pHalfExtends;
}

void deoglGIAreaTracker::Update(){
	// clear result
	pEntering.Clear();
	pLeaving.Clear();
	pAllLeaving = false;
	
	// if there is no world set all leaving flag.
	// in general though caller is responsible to handle this case
	if( ! pWorld ){
		pAllLeaving = true;
		return;
	}
	
	// if position did not change do nothing
	if( pValid && pPosition.IsEqualTo( pLastUpdatePosition ) ){
		return;
	}
	
	// calculate union box of old and new box. if the boxes are disjoint the max extend
	// of the union box will be equal to or less than the min extend. no extra work is
	// required since this invalid box will never match anything as it should
	pBoxKeep.minExtend = pBoxLastUpdate.minExtend.Largest( pBox.minExtend );
	pBoxKeep.maxExtend = pBoxLastUpdate.maxExtend.Smallest( pBox.maxExtend );
	
	// visit world
	if( pValid && pBoxKeep.minExtend < pBoxKeep.maxExtend ){
		// box to visit is box enclosing old and new area
		pBoxVisit.minExtend = pBoxLastUpdate.minExtend.Smallest( pBox.minExtend );
		pBoxVisit.maxExtend = pBoxLastUpdate.maxExtend.Largest( pBox.maxExtend );
		pVisitNodeColliding( pWorld->GetOctree() );
		
	}else{
		// areas are disjoint or tracking has been invalidated. visit new area and mark all
		// instances leaving. faster than visiting both areas and mark all old ones leaving
		pAllLeaving = true;
		pBoxVisit = pBox;
		pVisitNodeCollidingNewOnly( pWorld->GetOctree() );
	}
	
	// tracking is now valid
	pLastUpdatePosition = pPosition;
	pBoxLastUpdate = pBox;
	pValid = true;
}



bool deoglGIAreaTracker::HasChanged() const{
	return pEntering.GetComponentCount() > 0 || pLeaving.GetComponentCount() > 0 || pAllLeaving;
}

bool deoglGIAreaTracker::HasNotChanged() const{
	return pEntering.GetComponentCount() == 0 && pLeaving.GetComponentCount() == 0 && ! pAllLeaving;
}

void deoglGIAreaTracker::ClearChanges(){
	pEntering.Clear();
	pLeaving.Clear();
	pAllLeaving = false;
}

bool deoglGIAreaTracker::RejectComponent( const deoglRComponent &component ) const{
	if( component.GetLayerMask().IsNotEmpty() && pLayerMask.MatchesNot( component.GetLayerMask() ) ){
		return true;
	}
	
	if( ! component.GetModel() || component.GetLODCount() == 0 ){
		return true;
	}
	
	const deoglRModel::sExtends &extends = component.GetModel()->GetExtends();
	if( ( extends.maximum - extends.minimum ) < decVector( 0.5f, 0.5f, 0.5f ) ){
		return true; // skip small models to improve performance
	}
	
	return false;
}

bool deoglGIAreaTracker::ComponentTouches( const deoglRComponent &component ) const{
	return pValid && component.GetMaximumExtend() > pBox.minExtend
		&& component.GetMinimumExtend() < pBox.maxExtend;
}



// Private Functions
//////////////////////

void deoglGIAreaTracker::pVisitNodeColliding( const deoglWorldOctree &node ){
	// nodes fully contained inside the "keep box" are ignored since content in them
	// can not enter nor leave. then content is tested against both the "new box" and
	// the "old box". depending on the results content is added to the appropriate list:
	// - fully inside keep: do nothing
	// - touches inside new and old: do nothing
	// - touches inside new: add to entering list
	// - touches inside old: add to leaving list
	
	const decDVector &nodeMinExtend = node.GetMinimumExtend();
	const decDVector &nodeMaxExtend = node.GetMaximumExtend();
	
	if( nodeMinExtend >= pBoxKeep.minExtend && nodeMaxExtend <= pBoxKeep.maxExtend ){
		return;
	}
	
	const int result = deoglDCollisionDetection::AABoxIntersectsAABox(
		nodeMinExtend, nodeMaxExtend, pBoxVisit.minExtend, pBoxVisit.maxExtend );
	
	if( result == deoglDCollisionDetection::eirInside ){
		pVisitNode( node );
		
	}else if( result == deoglDCollisionDetection::eirPartial ){
		pVisitComponents( node );
		
		int i;
		for( i=0; i<8; i++ ){
			const deoglWorldOctree * const child = ( deoglWorldOctree* )node.GetNodeAt( i );
			if( child ){
				pVisitNodeColliding( *child );
			}
		}
	}
}

void deoglGIAreaTracker::pVisitNode( const deoglWorldOctree &node ){
	if( node.GetMinimumExtend() >= pBoxKeep.minExtend && node.GetMaximumExtend() <= pBoxKeep.maxExtend ){
		return;
	}
	
	pVisitComponents( node );
	
	int i;
	for( i=0; i<8; i++ ){
		const deoglWorldOctree * const child = ( deoglWorldOctree* )node.GetNodeAt( i );
		if( child ){
			pVisitNode( *child );
		}
	}
}

void deoglGIAreaTracker::pVisitComponents( const deoglWorldOctree &node ){
	const int count = node.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRComponent * const addComponent = node.GetComponentAt( i );
		const deoglRComponent &component = *addComponent;
		
		const decDVector &cmin = component.GetMinimumExtend();
		const decDVector &cmax = component.GetMaximumExtend();
		
		if( cmin >= pBoxKeep.minExtend && cmax <= pBoxKeep.maxExtend ){
			continue;
		}
		if( RejectComponent( component ) ){
			continue;
		}
		
		const bool touchNew = cmax > pBox.minExtend && cmin < pBox.maxExtend;
		const bool touchOld = cmax > pBoxLastUpdate.minExtend && cmin < pBoxLastUpdate.maxExtend;
		
		if( touchNew && ! touchOld ){
			pEntering.AddComponent( addComponent );
			
		}else if( touchOld && ! touchNew ){
			pLeaving.AddComponent( addComponent );
		}
	}
}



void deoglGIAreaTracker::pVisitNodeCollidingNewOnly( const deoglWorldOctree &node ){
	const int result = deoglDCollisionDetection::AABoxIntersectsAABox(
		node.GetMinimumExtend(), node.GetMaximumExtend(), pBox.minExtend, pBox.maxExtend );
	
	if( result == deoglDCollisionDetection::eirInside ){
		pVisitNodeNewOnly( node );
		
	}else if( result == deoglDCollisionDetection::eirPartial ){
		pVisitComponentsNewOnly( node );
		
		int i;
		for( i=0; i<8; i++ ){
			const deoglWorldOctree * const child = ( deoglWorldOctree* )node.GetNodeAt( i );
			if( child ){
				pVisitNodeCollidingNewOnly( *child );
			}
		}
	}
}

void deoglGIAreaTracker::pVisitNodeNewOnly( const deoglWorldOctree &node ){
	pVisitComponentsNewOnly( node );
	
	int i;
	for( i=0; i<8; i++ ){
		const deoglWorldOctree * const child = ( deoglWorldOctree* )node.GetNodeAt( i );
		if( child ){
			pVisitNodeNewOnly( *child );
		}
	}
}

void deoglGIAreaTracker::pVisitComponentsNewOnly( const deoglWorldOctree &node ){
	const int count = node.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRComponent * const addComponent = node.GetComponentAt( i );
		const deoglRComponent &component = *addComponent;
		
		const decDVector &cmin = component.GetMinimumExtend();
		const decDVector &cmax = component.GetMaximumExtend();
		
		if( ! ( cmax > pBox.minExtend && cmin < pBox.maxExtend ) ){
			continue;
		}
		if( RejectComponent( component ) ){
			continue;
		}
		
		pEntering.AddComponent( addComponent );
	}
}
