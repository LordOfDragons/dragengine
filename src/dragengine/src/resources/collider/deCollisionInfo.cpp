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
pHTSector(nullptr),
pBone(-1),
pShape(-1),
pFace(-1),

pParticleLifetime(0.0f),
pParticleMass(0.0f),
pParticleResponse(deParticleEmitterType::ecrDestroy),

pDistance(0.0f),
pImpulse(0.0f),

pStopTesting(false),
pStuck(false){
}

deCollisionInfo::deCollisionInfo(const deCollisionInfo &info) :
pOwnerBone(info.pOwnerBone),
pOwnerShape(info.pOwnerShape),
pOwnerFace(info.pOwnerFace),
pHeightTerrain(info.pHeightTerrain),
pHTSector(info.pHTSector),
pCollider(info.pCollider),
pBone(info.pBone),
pShape(info.pShape),
pFace(info.pFace),

pParticleLifetime(info.pParticleLifetime),
pParticleMass(info.pParticleMass),
pParticleResponse(info.pParticleResponse),

pDistance(info.pDistance),
pImpulse(info.pImpulse),

pOrgPosition(info.pOrgPosition),
pOrgDisplacement(info.pOrgDisplacement),
pOrgRotation(info.pOrgRotation),
pOrgOrientation(info.pOrgOrientation),

pBlockerPosition(info.pBlockerPosition),
pBlockerDisplacement(info.pBlockerDisplacement),
pBlockerRotation(info.pBlockerRotation),
pBlockerOrientation(info.pBlockerOrientation),

pStopTesting(info.pStopTesting),
pStuck(info.pStuck)
{
	info.pHistory.Visit([&](const deCollisionInfo &each){
		pHistory.Add(Ref::New(each));
	});
}

deCollisionInfo::~deCollisionInfo(){
	Clear();
}


// Management
///////////////

void deCollisionInfo::SetOwnerBone(int bone){
	DEASSERT_TRUE(bone >= -1)
	pOwnerBone = bone;
}

void deCollisionInfo::SetOwnerShape(int shape){
	DEASSERT_TRUE(shape >= -1)
	pOwnerShape = shape;
}

void deCollisionInfo::SetOwnerFace(int face){
	DEASSERT_TRUE(face >= -1)
	pOwnerFace = face;
}


bool deCollisionInfo::IsHTSector() const{
	return pHTSector != nullptr;
}

bool deCollisionInfo::IsCollider() const{
	return pCollider.IsNotNull();
}

bool deCollisionInfo::HasCollision() const{
	return pHTSector || pCollider;
}

void deCollisionInfo::SetHTSector(deHeightTerrain *heightTerrain, deHeightTerrainSector *sector){
	DEASSERT_NOTNULL(heightTerrain)
	DEASSERT_NOTNULL(sector)
	
	Clear();
	
	pHeightTerrain = heightTerrain;
	pHTSector = sector;
}

void deCollisionInfo::SetCollider(deCollider *collider, int bone, int shape, int face){
	DEASSERT_NOTNULL(collider)
	DEASSERT_TRUE(bone >= -1)
	DEASSERT_TRUE(shape >= -1)
	DEASSERT_TRUE(face >= -1)
	
	Clear();
	
	pCollider = collider;
	pBone = bone;
	pShape = shape;
	pFace = face;
}

void deCollisionInfo::Clear(){
	pHeightTerrain.Clear();
	pHTSector = nullptr;
	pCollider.Clear();
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
	
	pOrgPosition.SetZero();
	pOrgDisplacement.SetZero();
	pOrgRotation.SetZero();
	pOrgOrientation.SetZero();
	
	pBlockerPosition.SetZero();
	pBlockerDisplacement.SetZero();
	pBlockerRotation.SetZero();
	pBlockerOrientation.SetZero();
	
	pHistory.RemoveAll();
	pStuck = false;
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


void deCollisionInfo::SetOrgPosition(const decDVector &position){
	pOrgPosition = position;
}

void deCollisionInfo::SetOrgOrientation(const decQuaternion &orientation){
	pOrgOrientation = orientation;
}

void deCollisionInfo::SetOrgDisplacement(const decDVector &displacement){
	pOrgDisplacement = displacement;
}

void deCollisionInfo::SetOrgRotation(const decDVector &rotation){
	pOrgRotation = rotation;
}


void deCollisionInfo::SetBlockerPosition(const decDVector &position){
	pBlockerPosition = position;
}

void deCollisionInfo::SetBlockerOrientation(const decQuaternion &orientation){
	pBlockerOrientation = orientation;
}

void deCollisionInfo::SetBlockerDisplacement(const decDVector &displacement){
	pBlockerDisplacement = displacement;
}

void deCollisionInfo::SetBlockerRotation(const decDVector &rotation){
	pBlockerRotation = rotation;
}


void deCollisionInfo::SetStopTesting(bool stopTesting){
	pStopTesting = stopTesting;
}

void deCollisionInfo::SetStuck(bool stuck){
	pStuck = stuck;
}


// Operators
//////////////

deCollisionInfo &deCollisionInfo::operator=(const deCollisionInfo &info){
	Clear();
	
	pOwnerBone = info.pOwnerBone;
	pOwnerShape = info.pOwnerShape;
	pOwnerFace = info.pOwnerFace;
	
	pHeightTerrain = info.pHeightTerrain;
	pHTSector = info.pHTSector;
	pCollider = info.pCollider;
	pBone = info.pBone;
	pShape = info.pShape;
	pFace = info.pFace;
	
	pParticleLifetime = info.pParticleLifetime;
	pParticleMass = info.pParticleMass;
	
	pDistance = info.pDistance;
	pImpulse = info.pImpulse;
	
	pOrgPosition = info.pOrgPosition;
	pOrgOrientation = info.pOrgOrientation;
	pOrgDisplacement = info.pOrgDisplacement;
	pOrgRotation = info.pOrgRotation;
	
	pBlockerPosition = info.pBlockerPosition;
	pBlockerOrientation = info.pBlockerOrientation;
	pBlockerDisplacement = info.pBlockerDisplacement;
	pBlockerRotation = info.pBlockerRotation;
	
	pStopTesting = info.pStopTesting;
	
	info.pHistory.Visit([&](const deCollisionInfo &each){
		pHistory.Add(Ref::New(each));
	});
	pStuck = info.pStuck;
	
	return *this;
}
