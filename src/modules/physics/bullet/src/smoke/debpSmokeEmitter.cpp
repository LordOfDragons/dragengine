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
