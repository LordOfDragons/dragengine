/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeCLClosestHit.h"
#include "aeElementVisitable.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/common/exceptions.h>



// Class aeCLClosestHit
/////////////////////////

// Constructor, destructor
////////////////////////////

aeCLClosestHit::aeCLClosestHit() :
pHitDistance( 0.0f ),
pHasHit( false ),
pHitCollider( nullptr ),
pHitShape( -1 ),
pHitGizmo( nullptr ){
}

aeCLClosestHit::~aeCLClosestHit(){
}



// Management
///////////////

void aeCLClosestHit::Reset(){
	pHitDistance = 0.0f;
	pHasHit = false;
	pHitCollider = nullptr;
	pHitShape = -1;
	pHitGizmo = nullptr;
}

void aeCLClosestHit::IdentifyHitElement( igdeEnvironment &environment ){
	if( ! pHitCollider ){
		return;
	}
	
	aeElementVisitable * const visitable = ( aeElementVisitable* )environment.GetColliderUserPointer( pHitCollider );
	if( visitable ){
		visitable->visitElement( *this );
	}
}



// Notifications
//////////////////

void aeCLClosestHit::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	float distance = info->GetDistance();
	
	if( pHasHit && distance >= pHitDistance ){
		return;
	}
	
	pHitDistance = distance;
	pHitNormal = info->GetNormal();
	pHitCollider = info->GetCollider();
	pHitShape = info->GetShape();
	pHasHit = true;
}

bool aeCLClosestHit::CanHitCollider( deCollider *owner, deCollider *collider ){
	return true;
}

void aeCLClosestHit::ColliderChanged( deCollider *owner ){
}



// Visiting
/////////////

void aeCLClosestHit::visitGizmo( igdeGizmo *gizmo ){
	pHitGizmo = gizmo;
}
