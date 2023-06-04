/* 
 * Drag[en]gine IGDE World Editor
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

#include "meCLClosestElement.h"
#include "../filter/meFilterObjects.h"
#include "../world/meWorld.h"
#include "../world/meColliderOwner.h"
#include "../world/decal/meDecal.h"
#include "../world/object/meObject.h"
#include "../world/object/meObjectSnapPoint.h"
#include "../world/terrain/meHeightTerrain.h"
#include "../world/terrain/meHeightTerrainSector.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>



// Class meCLClosestElement
/////////////////////////////

// Constructor, destructor
////////////////////////////

meCLClosestElement::meCLClosestElement( meWorld &world ) :
pWorld( world ),

pTestHeightTerrain( false ),
pTestObjects( false ),
pTestDecals( false ),
pTestSnapPoints( false ),

pHitDistance( 0.0f ),
pHitHTSector( nullptr ),
pHitObject( nullptr ),
pHitDecal( nullptr ),
pHitSnapPoint( nullptr ),
pHasHit( false ),

pFilterObjects( nullptr ),

pIgnoreDecal( nullptr ){
}

meCLClosestElement::~meCLClosestElement(){
}



// Management
///////////////

void meCLClosestElement::SetTestHeightTerrain( bool test ){
	pTestHeightTerrain = test;
}

void meCLClosestElement::SetTestObjects( bool test ){
	pTestObjects = test;
}

void meCLClosestElement::SetTestDecals( bool test ){
	pTestDecals = test;
}

void meCLClosestElement::SetTestSnapPoints( bool test ){
	pTestSnapPoints = test;
}



void meCLClosestElement::SetFilterObjects( const meFilterObjects *filter ){
	pFilterObjects = filter;
}



void meCLClosestElement::SetIgnoreDecal( meDecal *decal ){
	pIgnoreDecal = decal;
}



void meCLClosestElement::Reset(){
	pHitDistance = 0.0f;
	pHitNormal.SetZero();
	
	pHitHTSector = nullptr;
	pHitObject = nullptr;
	pHitDecal = nullptr;
	pHitSnapPoint = nullptr;
	
	pHasHit = false;
}



// Notifications
//////////////////

void meCLClosestElement::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( info->IsHTSector() ){
		if( ! pTestHeightTerrain ){
			return;
		}
		
		meHeightTerrainSector * const htsector = pWorld.GetHeightTerrain()->
			GetSectorWith( info->GetHTSector()->GetSector() );
		
		if( htsector->GetTextureCount() == 0 ){
			return;
		}
		
		if( pHasHit && info->GetDistance() >= pHitDistance ){
			return;
		}
		
		Reset();
		pHitHTSector = htsector;
		pHitDistance = info->GetDistance();
		pHitNormal = info->GetNormal();
		pHasHit = true;
		
	}else if( info->IsCollider() ){
		const meColliderOwner * const colliderOwner = meColliderOwner::GetColliderOwner(
			*pWorld.GetEnvironment(), info->GetCollider() );
		if( ! colliderOwner ){
			return;
		}
		
		if( colliderOwner->GetObject() ){
			if( ! pTestObjects ){
				return;
			}
			if( pIgnoreObjects.Has( colliderOwner->GetObject() ) ){
				return;
			}
			if( pFilterObjects && ! pFilterObjects->AcceptObject( colliderOwner->GetObject() ) ){
				return;
			}
			if( pHasHit && info->GetDistance() >= pHitDistance ){
				return;
			}
			
			Reset();
			pHitObject = colliderOwner->GetObject();
			pHitDistance = info->GetDistance();
			pHitNormal = info->GetNormal();
			pHasHit = true;
			
		}else if( colliderOwner->GetDecal() ){
			if( ! pTestDecals ){
				return;
			}
			if( colliderOwner->GetDecal() == pIgnoreDecal ){
				return;
			}
			if( pHasHit && info->GetDistance() >= pHitDistance ){
				return;
			}
			
			Reset();
			pHitDecal = colliderOwner->GetDecal();
			pHitDistance = info->GetDistance();
			pHitNormal = info->GetNormal();
			pHasHit = true;
			
		}else if( colliderOwner->GetSnapPoint() ){
			if( ! pTestSnapPoints ){
				return;
			}
			if( pHasHit && info->GetDistance() >= pHitDistance ){
				return;
			}
			
			Reset();
			pHitSnapPoint = colliderOwner->GetSnapPoint();
			pHitDistance = info->GetDistance();
			pHitNormal = info->GetNormal();
			pHasHit = true;
		}
	}
}

bool meCLClosestElement::CanHitCollider( deCollider *owner, deCollider *collider ){
	return true;
}

void meCLClosestElement::ColliderChanged( deCollider *owner ){
}
