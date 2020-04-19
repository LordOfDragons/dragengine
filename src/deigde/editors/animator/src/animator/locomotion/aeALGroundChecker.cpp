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

#include "aeALGroundChecker.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/common/exceptions.h>


// Class aeALGroundChecker
////////////////////////////

// Constructor, destructor
////////////////////////////

aeALGroundChecker::aeALGroundChecker(){
	pColInfo = new deCollisionInfo;
	if( ! pColInfo ) DETHROW( deeOutOfMemory );
	
	pHasCollision = false;
}

aeALGroundChecker::~aeALGroundChecker(){
	if( pColInfo ) pColInfo->FreeReference();
}



// Management
///////////////

void aeALGroundChecker::Reset(){
	pColInfo->Clear();
	pHasCollision = false;
}



// Notifications
//////////////////

void aeALGroundChecker::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( ! pHasCollision || info->GetDistance() < pColInfo->GetDistance() ){
		if( info->IsHTSector() ){
			pColInfo->SetHTSector( info->GetHeightTerrain(), info->GetHTSector() );
			
		}else if( info->GetCollider() ){
			pColInfo->SetCollider( info->GetCollider(), info->GetBone(), info->GetShape(), info->GetFace() );
			
		}else{
			return;
		}
		
		pColInfo->SetDistance( info->GetDistance() );
		pColInfo->SetNormal( info->GetNormal() );
		
		pHasCollision = true;
	}
}

bool aeALGroundChecker::CanHitCollider( deCollider *owner, deCollider *collider ){
	return true;
}
