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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deSmokeEmitter.h"
#include "deSmokeEmitterManager.h"
#include "../component/deComponent.h"
#include "../skin/deSkin.h"

#include "../../deEngine.h"
#include "../../systems/modules/graphic/deBaseGraphicSmokeEmitter.h"
#include "../../systems/modules/physics/deBasePhysicsSmokeEmitter.h"
#include "../../common/exceptions.h"



// Class deSmokeEmitter
///////////////////////////

// Constructor, destructor
////////////////////////////

deSmokeEmitter::deSmokeEmitter(deSmokeEmitterManager *manager) : deResource(manager){
	pCastVelocityMin = 1.0f;
	pCastVelocityMax = 1.0f;
	pCastDensityMin = 0.1f;
	pCastDensityMax = 0.1f;
	pEnableCasting = false;
	
	pComponent = NULL;
	pCastTexture = 0;
	pSkin = NULL;
	
	pUseLocalGravity = false;
	
	pPoints = NULL;
	pPointCount = 0;
	
	pPeerGraphic = NULL;
	pPeerPhysics = NULL;
	
	pParentWorld = NULL;
	pLLWorldNext = NULL;
	pLLWorldPrev = NULL;
}

deSmokeEmitter::~deSmokeEmitter(){
	if(pPeerPhysics){
		delete pPeerPhysics;
		pPeerPhysics = NULL;
	}
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	if(pSkin) pSkin->FreeReference();
	if(pComponent) pComponent->FreeReference();
}



// Management
///////////////

void deSmokeEmitter::SetEmitterPosition(const decDVector &position){
	if(! position.IsEqualTo(pEmitterPosition)){
		pEmitterPosition = position;
		
		if(pPeerPhysics) pPeerPhysics->EmitterPositionChanged();
	}
}

void deSmokeEmitter::SetEmitterOrientation(const decQuaternion &orientation){
	if(! orientation.IsEqualTo(pEmitterOrientation)){
		pEmitterOrientation = orientation;
		
		if(pPeerPhysics) pPeerPhysics->EmitterOrientationChanged();
	}
}



void deSmokeEmitter::SetVolumePosition(const decDVector &position){
	if(! position.IsEqualTo(pVolumePosition)){
		pVolumePosition = position;
		
		if(pPeerGraphic) pPeerGraphic->VolumePositionChanged();
	}
}



void deSmokeEmitter::SetMinCastDirection(const decVector &deviation){
	if(! deviation.IsEqualTo(pCastDirectionMin)){
		pCastDirectionMin = deviation;
		
		if(pPeerPhysics) pPeerPhysics->CastPropertyChanged();
	}
}

void deSmokeEmitter::SetMaxCastDirection(const decVector &deviation){
	if(! deviation.IsEqualTo(pCastDirectionMax)){
		pCastDirectionMax = deviation;
		
		if(pPeerPhysics) pPeerPhysics->CastPropertyChanged();
	}
}

void deSmokeEmitter::SetMinCastVelocity(float velocity){
	if(fabsf(velocity - pCastVelocityMin) > 1e-5f){
		pCastVelocityMin = velocity;
		
		if(pPeerPhysics) pPeerPhysics->CastPropertyChanged();
	}
}

void deSmokeEmitter::SetMaxCastVelocity(float velocity){
	if(fabsf(velocity - pCastVelocityMax) > 1e-5f){
		pCastVelocityMax = velocity;
		
		if(pPeerPhysics) pPeerPhysics->CastPropertyChanged();
	}
}

void deSmokeEmitter::SetMinCastDensity(float density){
	if(fabsf(density - pCastDensityMin) > 1e-5f){
		pCastDensityMin = density;
		
		if(pPeerPhysics) pPeerPhysics->CastPropertyChanged();
	}
}

void deSmokeEmitter::SetMaxCastDensity(float density){
	if(fabsf(density - pCastDensityMax) > 1e-5f){
		pCastDensityMax = density;
		
		if(pPeerPhysics) pPeerPhysics->CastPropertyChanged();
	}
}

void deSmokeEmitter::SetEnableCasting(bool enable){
	if(enable != pEnableCasting){
		pEnableCasting = enable;
		
		if(pPeerPhysics) pPeerPhysics->EnableCastingChanged();
	}
}



void deSmokeEmitter::SetComponent(deComponent *component){
	if(component != pComponent){
		if(pComponent) pComponent->FreeReference();
		
		pComponent = component;
		
		if(component) component->AddReference();
		
		if(pPeerPhysics) pPeerPhysics->ComponentChanged();
	}
}

void deSmokeEmitter::SetCastTexture(int texture){
	if(texture != pCastTexture){
		pCastTexture = texture;
		
		if(pPeerPhysics) pPeerPhysics->CastTextureChanged();
	}
}

void deSmokeEmitter::SetSkin(deSkin *skin){
	if(skin != pSkin){
		if(pSkin) pSkin->FreeReference();
		
		pSkin = skin;
		
		if(skin) skin->AddReference();
		
		if(pPeerGraphic) pPeerGraphic->SkinChanged();
		if(pPeerPhysics) pPeerPhysics->SkinChanged();
	}
}

void deSmokeEmitter::SetCollisionFilter(const decCollisionFilter &collisionFilter){
	if(collisionFilter == pCollisionFilter){
		return;
	}
	
	pCollisionFilter = collisionFilter;
	
	if(pPeerPhysics){
		pPeerPhysics->CollisionFilterChanged();
	}
}

void deSmokeEmitter::SetGravity(const decVector &gravity){
	if(! gravity.IsEqualTo(pGravity)){
		pGravity = gravity;
		
		if(pPeerPhysics) pPeerPhysics->GravityChanged();
	}
}

void deSmokeEmitter::SetUseLocalGravity(bool useLocalGravity){
	if(pUseLocalGravity != useLocalGravity){
		pUseLocalGravity = useLocalGravity;
		
		if(pPeerPhysics) pPeerPhysics->GravityChanged();
	}
}



void deSmokeEmitter::SetPoints(deSmokeDensityPoint *points, int count){
	pPoints = points;
	pPointCount = count;
	
	if(pPeerGraphic) pPeerGraphic->PointsChanged();
}

void deSmokeEmitter::NotifyPointsChanged(){
	if(pPeerGraphic) pPeerGraphic->PointsChanged();
}



// System Peers
/////////////////

void deSmokeEmitter::SetPeerGraphic(deBaseGraphicSmokeEmitter *peer){
	if(pPeerGraphic) delete pPeerGraphic;
	pPeerGraphic = peer;
}

void deSmokeEmitter::SetPeerPhysics(deBasePhysicsSmokeEmitter *peer){
	if(pPeerPhysics) delete pPeerPhysics;
	pPeerPhysics = peer;
}



// Linked List
////////////////

void deSmokeEmitter::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deSmokeEmitter::SetLLWorldPrev(deSmokeEmitter *smokeEmitter){
	pLLWorldPrev = smokeEmitter;
}

void deSmokeEmitter::SetLLWorldNext(deSmokeEmitter *smokeEmitter){
	pLLWorldNext = smokeEmitter;
}
