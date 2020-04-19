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
#include "deoalWOVRayBlocked.h"
#include "../../../component/deoalAComponent.h"
#include "../../../component/deoalAComponentTexture.h"
#include "../../../model/deoalAModel.h"
#include "../../../model/deoalModelFace.h"
#include "../../../model/octree/deoalModelOctree.h"
#include "../../../world/octree/deoalWorldOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoalWOVRayBlocked
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWOVRayBlocked::deoalWOVRayBlocked() :
pBlocked( false ){
}

deoalWOVRayBlocked::~deoalWOVRayBlocked(){
}



// Visiting
/////////////

void deoalWOVRayBlocked::SetBlocked( bool blocked ){
	pBlocked = blocked;
}



void deoalWOVRayBlocked::VisitNode( deoalWorldOctree &node ){
	const decLayerMask &layerMask = GetLayerMask();
	const int count = node.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoalAComponent &component = *node.GetComponentAt( i );
		// this is implicit. components are not added to the octree if not affecting sound
// 		if( ! component.GetAffectsSound() ){
// 			continue;
// 		}
		if( component.GetLayerMask().MatchesNot( layerMask ) ){
			continue;
		}
		
		if( pRayHitsBox( component.GetBoxCenter(), component.GetBoxHalfExtends() ) ){
			VisitComponent( &component );
			if( pBlocked ){
				return;
			}
		}
	}
	
	for( i=0; i<8; i++ ){
		deoalWorldOctree * const childNode = ( deoalWorldOctree* )node.GetNodeAt( i );
		if( childNode && pRayHitsBox( childNode->GetCenter(), childNode->GetHalfSize() ) ){
			VisitNode( *childNode );
			if( pBlocked ){
				return;
			}
		}
	}
}

void deoalWOVRayBlocked::VisitNode( deoalDOctree*, int ){
	DETHROW( deeInvalidAction );   // do not use this version
}

void deoalWOVRayBlocked::VisitComponent( deoalAComponent *component ){
	deoalAModel &model = *component->GetModel();
	if( ! model.GetRTBVH() && ! component->GetBVH() ){
		return;
	}
	
	const decDMatrix &invMatrix = component->GetInverseMatrix();
	const decVector rayOrigin( invMatrix * GetRayOrigin() );
	const decVector rayDirection( invMatrix.TransformNormal( GetRayDirection() ) );
	
	deoalMOVRayBlocked visitor( *component );
	visitor.SetRay( rayOrigin, rayDirection );
	
	if( component->GetBVH() ){
		visitor.VisitBVH( *component->GetBVH() );
		
	}else{
		visitor.VisitBVH( *model.GetRTBVH() );
	}
	
	pBlocked = visitor.GetBlocked();
}
