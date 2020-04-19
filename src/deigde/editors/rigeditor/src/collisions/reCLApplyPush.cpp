/* 
 * Drag[en]gine IGDE Rig Editor
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

#include "reCLApplyPush.h"
#include "../rig/reRig.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/physics/deBasePhysicsWorld.h>



// Class reCLApplyPush
////////////////////////

// Constructor, destructor
////////////////////////////

reCLApplyPush::reCLApplyPush( reRig *rig ){
	if( ! rig ) DETHROW( deeInvalidParam );
	
	pRig = rig;
	
	pPush.Set( 0.0f, 0.0f, 1.0f );
	pCollider = NULL;
}

reCLApplyPush::~reCLApplyPush(){
}



// Management
///////////////

void reCLApplyPush::SetRay( const decDVector &position, const decVector &direction ){
	pRayPosition = position;
	pRayDirection = direction;
}

void reCLApplyPush::SetPush( const decVector &push ){
	pPush = push;
}

void reCLApplyPush::SetCollider( deCollider *collider ){
	pCollider = collider;
}



void reCLApplyPush::Reset(){
	pClosestCollider = NULL;
	pClosestBone = 0;
	pClosestDistance = 0.0f;
}

void reCLApplyPush::ApplyPush(){
	if( pClosestCollider ){
		const decDVector hitPoint = pRayPosition + pRayDirection * ( double )pClosestDistance;
		const decDMatrix matrix = decDMatrix::CreateWorld( pClosestCollider->GetPosition(),
			pClosestCollider->GetOrientation() ).Invert();
		deColliderVisitorIdentify identify;
		
		pClosestCollider->Visit( identify );
		
		if( identify.IsVolume() ){
			deColliderVolume &colliderVolume = identify.CastToVolume();
			colliderVolume.ApplyImpulsAt( pPush, ( matrix * hitPoint ).ToVector() );
			
		}else if( identify.IsComponent() ){
			deColliderComponent &colliderComponent = identify.CastToComponent();
			
			if( pClosestBone == -1 ){
				colliderComponent.ApplyImpulsAt( pPush, ( matrix * hitPoint ).ToVector() );
				
			}else{
				const decDMatrix boneInvMatrix = colliderComponent.GetBoneAt( pClosestBone ).GetInverseMatrix();
				colliderComponent.ApplyBoneImpulsAt( pClosestBone, pPush, ( boneInvMatrix * hitPoint ).ToVector() );
			}
		}
	}
}



void reCLApplyPush::ApplyPushIn( deBasePhysicsWorld &phyWorld, const decLayerMask &layerMask ){
	Reset();
	phyWorld.RayHits( pRayPosition, pRayDirection.ToVector(), this, layerMask );
	ApplyPush();
}



// Notifications
//////////////////

void reCLApplyPush::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( info->IsCollider() ){
		if( ! pClosestCollider || info->GetDistance() < pClosestDistance ){
			pClosestCollider = info->GetCollider();
			pClosestBone = info->GetBone();
			pClosestDistance = info->GetDistance();
		}
	}
}

bool reCLApplyPush::CanHitCollider( deCollider *owner, deCollider *collider ){
	return collider == pCollider;
}

void reCLApplyPush::ColliderChanged( deCollider *owner ){
}
