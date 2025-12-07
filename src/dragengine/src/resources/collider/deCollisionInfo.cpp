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

#include "deCollisionInfo.h"
#include "../terrain/heightmap/deHeightTerrain.h"
#include "../terrain/heightmap/deHeightTerrainSector.h"
#include "../collider/deCollider.h"
#include "../collider/deColliderVolume.h"
#include "../collider/deColliderRig.h"
#include "../collider/deColliderComponent.h"
#include "../component/deComponent.h"
#include "../../common/exceptions.h"



// Class deCollisionInfo
//////////////////////////

// Constructor, destructor
////////////////////////////

deCollisionInfo::deCollisionInfo() :
pOwnerBone(-1),
pOwnerShape(-1),
pOwnerFace(-1),

pHeightTerrain(NULL),
pHTSector(NULL),
pCollider(NULL),
pBone(-1),
pShape(-1),
pFace(-1),

pParticleLifetime(0.0f),
pParticleMass(0.0f),
pParticleResponse(deParticleEmitterType::ecrDestroy),

pDistance(0.0f),
pImpulse(0.0f),

pStopTesting(false){
}

deCollisionInfo::deCollisionInfo(const deCollisionInfo &info) :
pOwnerBone(info.pOwnerBone),
pOwnerShape(info.pOwnerShape),
pOwnerFace(info.pOwnerFace),

pHeightTerrain(NULL),
pHTSector(info.pHTSector),
pCollider(NULL),
pBone(info.pBone),
pShape(info.pShape),
pFace(info.pFace),

pParticleLifetime(info.pParticleLifetime),
pParticleMass(info.pParticleMass),
pParticleResponse(info.pParticleResponse),

pDistance(info.pDistance),
pImpulse(info.pImpulse),

pStopTesting(info.pStopTesting)
{
	if(info.pHeightTerrain){
		pHeightTerrain = info.pHeightTerrain;
		pHeightTerrain->AddReference();
	}
	if(info.pCollider){
		pCollider = info.pCollider;
		pCollider->AddReference();
	}
}

deCollisionInfo::~deCollisionInfo(){
	Clear();
}



// Management
///////////////

void deCollisionInfo::SetOwnerBone(int bone){
	if(bone < -1){
		DETHROW(deeInvalidParam);
	}
	pOwnerBone = bone;
}

void deCollisionInfo::SetOwnerShape(int shape){
	if(shape < -1){
		DETHROW(deeInvalidParam);
	}
	pOwnerShape = shape;
}

void deCollisionInfo::SetOwnerFace(int face){
	if(face < -1){
		DETHROW(deeInvalidParam);
	}
	pOwnerFace = face;
}



bool deCollisionInfo::IsHTSector() const{
	return pHTSector != NULL;
}

bool deCollisionInfo::IsCollider() const{
	return pCollider != NULL;
}

bool deCollisionInfo::HasCollision() const{
	return pHTSector != NULL || pCollider != NULL;
}

void deCollisionInfo::SetHTSector(deHeightTerrain *heightTerrain, deHeightTerrainSector *sector){
	if(!heightTerrain || !sector){
		DETHROW(deeInvalidParam);
	}
	
	Clear();
	
	pHeightTerrain = heightTerrain;
	pHeightTerrain->AddReference();
	pHTSector = sector;
}

void deCollisionInfo::SetCollider(deCollider *collider, int bone, int shape, int face){
	if(!collider || bone < -1 || shape < -1 || face < -1){
		DETHROW(deeInvalidParam);
	}
	
	Clear();
	
	pCollider = collider;
	pCollider->AddReference();
	pBone = bone;
	pShape = shape;
	pFace = face;
}

void deCollisionInfo::Clear(){
	if(pHeightTerrain){
		pHeightTerrain->FreeReference();
		pHeightTerrain = NULL;
	}
	pHTSector = NULL;
	
	if(pCollider){
		pCollider->FreeReference();
		pCollider = NULL;
	}
	pBone = -1;
	pShape = -1;
	pFace = -1;
	
	//pParticleLifetime = 0.0f;
	//pParticleMass = 0.0f;
	//pDistance = 0.0f;
	//pNormal.SetZero();
	pStopTesting = false;
	
	pOwnerBone = -1;
	pOwnerShape = -1;
	pOwnerFace = -1;
}



void deCollisionInfo::SetParticleLifetime(float lifetime){
	pParticleLifetime = lifetime;
}

void deCollisionInfo::SetParticleMass(float mass){
	pParticleMass = mass;
}

void deCollisionInfo::SetParticlePosition(const decDVector &position){
	pParticlePosition = position;
}

void deCollisionInfo::SetParticleVelocity(const decVector &velocity){
	pParticleVelocity = velocity;
}

void deCollisionInfo::SetParticleResponse(deParticleEmitterType::eCollisionResponses response){
	pParticleResponse = response;
}



void deCollisionInfo::SetDistance(float distance){
	pDistance = distance;
}

void deCollisionInfo::SetNormal(const decVector &normal){
	pNormal = normal;
}

void deCollisionInfo::SetPosition(const decDVector &position){
	pPosition = position;
}

void deCollisionInfo::SetImpulse(float impulse){
	pImpulse = impulse;
}



void deCollisionInfo::SetStopTesting(bool stopTesting){
	pStopTesting = stopTesting;
}



// Operators
//////////////

deCollisionInfo &deCollisionInfo::operator=(const deCollisionInfo &info){
	Clear();
	
	pOwnerBone = info.pOwnerBone;
	pOwnerShape = info.pOwnerShape;
	pOwnerFace = info.pOwnerFace;
	
	pHeightTerrain = info.pHeightTerrain;
	if(pHeightTerrain){
		pHeightTerrain->AddReference();
	}
	pHTSector = info.pHTSector;
	pCollider = info.pCollider;
	if(pCollider){
		pCollider->AddReference();
	}
	pBone = info.pBone;
	pShape = info.pShape;
	pFace = info.pFace;
	
	pParticleLifetime = info.pParticleLifetime;
	pParticleMass = info.pParticleMass;
	
	pDistance = info.pDistance;
	pImpulse = info.pImpulse;
	
	pStopTesting = info.pStopTesting;
	
	return *this;
}
