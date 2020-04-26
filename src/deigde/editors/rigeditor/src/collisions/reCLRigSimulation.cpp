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

#include "reCLRigSimulation.h"
#include "../rig/reRig.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Rig Editor"




// Class reCLRigSimulation
////////////////////////////

// Constructor, destructor
////////////////////////////

reCLRigSimulation::reCLRigSimulation( reRig &rig ) :
pRig( rig ){
}

reCLRigSimulation::~reCLRigSimulation(){
}



// Notifications
//////////////////

void reCLRigSimulation::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( owner != pRig.GetEngineSimulationCollider() ){
		return;
	}
	
	deColliderComponent &collider = *pRig.GetEngineSimulationCollider();
	
	switch( collider.GetResponseType() ){
	case deCollider::ertDynamic:{
		const int ownerBoneIndex = info->GetOwnerBone();
		
		if( ownerBoneIndex == -1 ){
			/*
			const decVector &linvelo = collider.GetLinearVelocity();
			const float mass = collider.GetMass();
			const float impulse = linvelo.Length() * mass;
			pRig.GetLogger()->LogInfoFormat( LOGSOURCE, "CollisionResponse rig: velocity=%f mass=%f impulse=%f",
				linvelo.Length(), mass, impulse );
			*/
			
		}else{
			deColliderBone &ownerBone = collider.GetBoneAt( ownerBoneIndex );
			const decVector &linvelo = ownerBone.GetLinearVelocity();
			
			if( ownerBone.GetDynamic() ){
				/*
				const float mass = ownerBone.GetMass();
				const float impulse = linvelo.Length() * mass;
				pRig.GetLogger()->LogInfoFormat( LOGSOURCE, "CollisionResponse bone(%i): velocity=%f mass=%f impulse=%f",
					ownerBoneIndex, linvelo.Length(), mass, impulse );
				*/
				
			}else{
				const decVector &normal = info->GetNormal();
				if( linvelo * normal < 0.0f ){
					ownerBone.SetPosition( ownerBone.GetPosition() + decDVector( normal ) * 0.001 );
					ownerBone.SetLinearVelocity( linvelo - normal * ( normal * linvelo ) );
				}
			}
		}
		
		}break;
		
	case deCollider::ertKinematic:{
		const decVector &linvelo = collider.GetLinearVelocity();
		const decVector &normal = info->GetNormal();
		if( linvelo * normal < 0.0f ){
			collider.SetPosition( collider.GetPosition() + decDVector( normal ) * 0.001 );
			collider.SetLinearVelocity( linvelo - normal * ( normal * linvelo ) );
		}
		}break;
		
	case deCollider::ertStatic:
		break;;
	}
}

bool reCLRigSimulation::CanHitCollider( deCollider *owner, deCollider *collider ){
	// currently we allow no collisions at all. once we have pushers or other
	// objects this can change.
	return false;
	
	/*
	deColliderVisitorIdentify identify;
	
	collider->Visit( identify );
	if( identify.IsVolume() ){
		deColliderVolume *colliderVolume = identify.CastToVolume();
		
		if( pCanSelectBones && pGetBoneFromCollider( colliderVolume ) ){
			return true;
		}
		
		if( pCanSelectShapes && pGetShapeFromCollider( colliderVolume ) ){
			return true;
		}
	}
	
	return false;
	*/
}

void reCLRigSimulation::ColliderChanged( deCollider *owner ){
	pRig.UpdateFromSimulation();
}
