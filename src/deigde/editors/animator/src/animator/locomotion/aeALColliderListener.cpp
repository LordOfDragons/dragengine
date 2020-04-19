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

#include "aeALColliderListener.h"
#include "aeAnimatorLocomotion.h"
#include "../aeAnimator.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"



// Class aeALColliderListener
///////////////////////////////

// Constructor, destructor
////////////////////////////

aeALColliderListener::aeALColliderListener(){
	pLocomotion = NULL;
}

aeALColliderListener::~aeALColliderListener(){
}



// Management
///////////////

void aeALColliderListener::SetAnimatorLocomotion( aeAnimatorLocomotion *locomotion ){
	pLocomotion = locomotion;
}



// Notifications
//////////////////

void aeALColliderListener::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	const decVector &linvelo = owner->GetLinearVelocity();
	const decVector &normal = info->GetNormal();
	
	if( linvelo * normal < 0.0f ){
		owner->SetPosition( owner->GetPosition() + decDVector( normal ) * 0.001 );
		owner->SetLinearVelocity( linvelo - normal * ( normal * linvelo ) );
	}
	
	if( pLocomotion->GetLegCount() > 1 ){
		float correctTurn = owner->GetAngularVelocity().y * info->GetDistance();
		if( correctTurn > 0.0f ){
			correctTurn += 0.1f;
		}else{
			correctTurn -= 0.01f;
		}
		pLocomotion->SetOrientation( pLocomotion->GetOrientation().GetValue() - correctTurn );
		pLocomotion->GetLookLeftRight().SetValue( pLocomotion->GetLookLeftRight().GetValue() + correctTurn );
		pLocomotion->GetLookLeftRight().SetGoal( pLocomotion->GetLookLeftRight().GetGoal() + correctTurn );
		
		owner->SetOrientation( pLocomotion->GetOrientationQuaternion() );
		owner->SetAngularVelocity( decVector() );
	}
}

bool aeALColliderListener::CanHitCollider( deCollider *owner, deCollider *collider ){
	return true;
}

void aeALColliderListener::ColliderChanged( deCollider *owner ){
	pLocomotion->OnColliderChanged();
}
