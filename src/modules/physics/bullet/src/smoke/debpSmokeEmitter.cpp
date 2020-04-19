/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpSmokeEmitter.h"

#include "../debpCommon.h"
#include "../dePhysicsBullet.h"
#include "../coldet/debpCollisionDetection.h"
#include "../component/debpComponent.h"
#include "../forcefield/debpForceField.h"
#include "../world/debpWorld.h"

#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/smoke/deSmokeEmitter.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/common/exceptions.h>



// Class debpSmokeEmitter
///////////////////////////

// Constructor, destructor
////////////////////////////

debpSmokeEmitter::debpSmokeEmitter( dePhysicsBullet *bullet, deSmokeEmitter *smokeEmitter ){
	if( ! bullet || ! smokeEmitter ) DETHROW( deeInvalidParam );
	
	pBullet = bullet;
	pSmokeEmitter = smokeEmitter;
	
	pParentWorld = NULL;
	
	pPoints = NULL;
	pPointCount = 0;
	pPointSize = 0;
	
	pDirtyEmitterMatrix = true;
	
	pFluctTimer = 0.0f;
	pFluctDir = 0.0f;
	pFluctStr = 0.0f;
	
	GravityChanged();
}

debpSmokeEmitter::~debpSmokeEmitter(){
	pCleanUp();
}



// Management
///////////////

void debpSmokeEmitter::SetParentWorld( debpWorld *parentWorld ){
	pParentWorld = parentWorld;
	GravityChanged();
}

void debpSmokeEmitter::PreparePoints( float elapsed ){
}

void debpSmokeEmitter::StepPoints( float elapsed ){
}

const decMatrix &debpSmokeEmitter::GetEmitterMatrix(){
	if( pDirtyEmitterMatrix ){
		pEmitterMatrix.SetFromQuaternion( pSmokeEmitter->GetEmitterOrientation() );
		pDirtyEmitterMatrix = false;
	}
	
	return pEmitterMatrix;
}

void debpSmokeEmitter::CastPoint(){
}

void debpSmokeEmitter::KillPoint( int index ){
}

void debpSmokeEmitter::ApplyForceField( debpForceField *forceField, float elapsed ){
}



// Notifications
//////////////////

void debpSmokeEmitter::EmitterPositionChanged(){
}

void debpSmokeEmitter::EmitterOrientationChanged(){
	pDirtyEmitterMatrix = true;
}

void debpSmokeEmitter::CastPropertyChanged(){
}

void debpSmokeEmitter::EnableCastingChanged(){
}

void debpSmokeEmitter::ComponentChanged(){
}

void debpSmokeEmitter::CastTextureChanged(){
}

void debpSmokeEmitter::SkinChanged(){
}

void debpSmokeEmitter::CollisionFilterChanged(){
}

void debpSmokeEmitter::GravityChanged(){
	if( pSmokeEmitter->GetUseLocalGravity() ){
		pGravity = pSmokeEmitter->GetGravity();
		
	}else if( pParentWorld ){
		pGravity = pParentWorld->GetWorld().GetGravity();
		
	}else{
		pGravity.SetZero();
	}
}



// Private Functions
//////////////////////

void debpSmokeEmitter::pCleanUp(){
	if( pPoints ) delete [] pPoints;
}
