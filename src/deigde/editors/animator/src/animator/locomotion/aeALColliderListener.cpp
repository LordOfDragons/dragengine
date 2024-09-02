/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
