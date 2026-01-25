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

#include "debpParticleEmitter.h"
#include "debpParticleEmitterInstance.h"
#include "debpParticleEmitterInstanceType.h"

#include "../debpCommon.h"
#include "../dePhysicsBullet.h"
#include "../coldet/debpCollisionDetection.h"
#include "../component/debpComponent.h"
#include "../forcefield/debpForceField.h"
#include "../visitors/debpClosestRayResultCallback.h"
#include "../world/debpWorld.h"

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"

#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterController.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceType.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/resources/world/deWorld.h>



#ifndef random
#define random rand
#endif



//#define DO_TIMING
#ifdef DO_TIMING
#include "dragengine/common/utils/decTimer.h"
static decTimer timer;
#endif

// Class debpParticleEmitterInstance
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

debpParticleEmitterInstance::debpParticleEmitterInstance(dePhysicsBullet *bullet, deParticleEmitterInstance *instance){
	if(!bullet || !instance){
		DETHROW(deeInvalidParam);
	}
	
	pBullet = bullet;
	pInstance = instance;
	
	pParentWorld = NULL;
	
	pBurstTimer = 0.0f;
	
	pCanCollide = false;
	
	pDirtyEmitterMatrix = true;
	pCheckForLastParticle = false;
	pEnsureCastOnce = false;
	
	pLastPosition = instance->GetPosition();
	
	EmitterChanged();
	CollisionFilterChanged();
}

debpParticleEmitterInstance::~debpParticleEmitterInstance(){
	pCleanUp();
}



// Management
///////////////

void debpParticleEmitterInstance::SetParentWorld(debpWorld *parentWorld){
	pParentWorld = parentWorld;
}



debpParticleEmitter *debpParticleEmitterInstance::GetParticleEmitter() const{
	if(pInstance->GetEmitter()){
		return (debpParticleEmitter*)pInstance->GetEmitter()->GetPeerPhysics();
		
	}else{
		return NULL;
	}
}

const decDMatrix &debpParticleEmitterInstance::GetEmitterMatrix(){
	if(pDirtyEmitterMatrix){
		pEmitterMatrix.SetWorld(pInstance->GetPosition(), pInstance->GetOrientation());
		pDirtyEmitterMatrix = false;
	}
	
	return pEmitterMatrix;
}

decDMatrix debpParticleEmitterInstance::GetEmitterMatrixByDistance(float distance) const{
	return decDMatrix::CreateWorld(pLastPosition * (1.0f - distance) + pInstance->GetPosition() * distance, pInstance->GetOrientation());
}



void debpParticleEmitterInstance::PrepareParticles(float elapsed){
	debpParticleEmitter * const emitter = GetParticleEmitter();
	const float travelledDistance = (float)(pInstance->GetPosition() - pLastPosition).Length();
	
	const bool casting = pInstance->GetEnableCasting() || pEnsureCastOnce;
	pEnsureCastOnce = false;
	
	if(emitter){
		emitter->UpdateParameters();
		
		pForceFieldSimulation.Update(elapsed);
		
		// update the particles
		pBurstTimer += elapsed;
		
		pTypes.Visit([&](debpParticleEmitterInstanceType &type){
//timer.Reset();
			type.PrepareParticles(casting, elapsed, travelledDistance);
//pBullet->LogInfoFormat( "PrepareParticle (%i) = %iys", pTypes[ i ].GetParticlesCount(), ( int )( timer.GetElapsedTime() * 1000000.0f ) );
		});
		
		if(pInstance->GetEmitter()->GetEmitBurst()){
			if(pInstance->GetEmitter()->GetBurstLifetime() > FLOAT_SAFE_EPSILON){
				pInstance->SetBurstTime(pBurstTimer / pInstance->GetEmitter()->GetBurstLifetime());
				
			}else{
				pInstance->SetBurstTime(0.0f);
			}
			
			if(casting && pBurstTimer >= pInstance->GetEmitter()->GetBurstLifetime()){
				pInstance->SetEnableCasting(false);
				RequestCheckForLastParticle();
			}
			
		}else{
			pInstance->SetBurstTime(0.0f);
		}
		
		/*{
		int T = pTypes.Inject(0, [&](int accum, debpParticleEmitterInstanceType &type){
			return accum += type.GetParticlesCount();
		});
		pBullet->LogInfoFormat("PrepareParticles %p: ec=%i eb=%i bt=%f blt=%f cflp=%i pc=%i ralpd=%i", this,
			pInstance->GetEnableCasting()?1:0, pInstance->GetEmitter()->GetEmitBurst()?1:0,
			pBurstTimer, pInstance->GetEmitter()->GetBurstLifetime(), pCheckForLastParticle?1:0, T,
			pInstance->GetRemoveAfterLastParticleDied()?1:0);
		}*/
	}
	
	pLastPosition = pInstance->GetPosition();
	
	ApplyForceFields(elapsed);
	
	//CheckForLastParticle(); // too early. StepParticles can also trigger a check which we would cancel the next time PrepareParticle is called
}

void debpParticleEmitterInstance::ApplyForceFields(float elapsed){
	// TODO track force fields to update only if they are nearby
	if(!pParentWorld){
		return;
	}
	
	const deWorld &world = pParentWorld->GetWorld();
	
	world.GetForceFields().Visit([&](deForceField *forceField){
		if(!forceField->GetCollisionFilter().Collides(pInstance->GetCollisionFilter())){
			return;
		}
		
		const debpForceField &bpForceField = *((debpForceField*)forceField->GetPeerPhysics());
		
		// TODO check if the force field overlaps the component
		
		pTypes.Visit([&](debpParticleEmitterInstanceType &type){
			type.ApplyForceField(bpForceField, elapsed);
		});
	});
}

void debpParticleEmitterInstance::StepParticles(float elapsed){
	pTypes.Visit([&](debpParticleEmitterInstanceType &type){
//timer.Reset();
		type.StepParticles(elapsed);
//pBullet->LogInfoFormat( "StepParticle (%i) = %iys", type.GetParticlesCount(), (int)(timer.GetElapsedTime() * 1000000.0f));
	});
	
	CheckForLastParticle(); // StepParticles is called last so the check has to be done here not in PrepareParticles
}

void debpParticleEmitterInstance::FinishStepping(){
	pInstance->SetReferencePosition(pInstance->GetPosition()); // particles will be relative to this positon for rendering
	
	pTypes.Visit([&](debpParticleEmitterInstanceType &type){
		type.FinishStepping();
	});
}

void debpParticleEmitterInstance::RequestCheckForLastParticle(){
	pCheckForLastParticle = true;
}

void debpParticleEmitterInstance::CheckForLastParticle(){
	if(!pCheckForLastParticle || pInstance->GetEnableCasting()){
		return;
	}
	if(pInstance->GetEmitter()->GetEmitBurst() && pBurstTimer < pInstance->GetEmitter()->GetBurstLifetime()){
		return;
	}
	
	if(pTypes.HasMatching([](const debpParticleEmitterInstanceType &type){
		return type.GetParticlesCount() > 0;
	})){
		return;
	}
	
	pCheckForLastParticle = false;
	
	deParticleEmitterInstance::Ref guard(pInstance);
	pInstance->NotifyLastParticleDied();
	if(pInstance->GetRemoveAfterLastParticleDied()){
		pInstance->GetParentWorld()->RemoveParticleEmitter(pInstance);
	}
}



// Notifications
//////////////////

void debpParticleEmitterInstance::EmitterChanged(){
	pLastPosition = pInstance->GetPosition();
	pUpdateTypes();
	CollisionFilterChanged(); // setting emitter potentially changes the layer mask
}

void debpParticleEmitterInstance::PositionChanged(){
	pDirtyEmitterMatrix = true;
}

void debpParticleEmitterInstance::OrientationChanged(){
	pDirtyEmitterMatrix = true;
}

void debpParticleEmitterInstance::EnableCastingChanged(){
	pLastPosition = pInstance->GetPosition();
	
	if(pInstance->GetEnableCasting()){
		if(pInstance->GetWarmUpTime() > FLOAT_SAFE_EPSILON){
			pSimulateWarpUp();
		}
		pCheckForLastParticle = false;
		pEnsureCastOnce = true;
	}
}

void debpParticleEmitterInstance::WarmUpTimeChanged(){
}

void debpParticleEmitterInstance::CollisionFilterChanged(){
	pCanCollide = pInstance->GetCollisionFilter().CanCollide();
}

void debpParticleEmitterInstance::ControllerChanged(int controller){
	const deParticleEmitter * const emitter = pInstance->GetEmitter();
	
	if(emitter){
		pTypes.Visit([&](debpParticleEmitterInstanceType &type){
			const deParticleEmitterParameter &parameter = emitter->GetTypes().
				GetAt(type.GetType())->GetParameter(deParticleEmitterType::epInterval);
			
			if(parameter.GetControllerValue() > -1 || parameter.GetControllerSpread() > -1){
				type.UpdateCastIntervals();
			}
		});
	}
}

void debpParticleEmitterInstance::TypeChanged(int type){
	pTypes[type].OnTypeChanged();
}



void debpParticleEmitterInstance::ResetBurst(){
	pBurstTimer = 0.0f;
	pTypes.Visit([&](debpParticleEmitterInstanceType &type){
		type.ResetBurst();
	});
}

void debpParticleEmitterInstance::KillAllParticles(){
	pTypes.Visit([&](debpParticleEmitterInstanceType &type){
		type.KillAllParticles();
	});
}



// Private Functions
//////////////////////

void debpParticleEmitterInstance::pCleanUp(){
	pTypes.RemoveAll();
}

void debpParticleEmitterInstance::pUpdateTypes(){
	pTypes.RemoveAll();
	
	const int typeCount = pInstance->GetTypes().GetCount();
	if(typeCount == 0){
		return;
	}
	
	pTypes.AddRange(typeCount, {});
	int i;
	for(i=0; i<typeCount; i++){
		pTypes[i].SetInstance(this);
		pTypes[i].SetType(i);
		pTypes[i].OnTypeChanged();
	}
}

void debpParticleEmitterInstance::pSimulateWarpUp(){
	float remainingTime = pInstance->GetWarmUpTime();
	const float timeStep = 1.0f / 30.0f;
	float simulationStep;
	
	while(remainingTime > 0.0f){
		if(remainingTime < timeStep){
			simulationStep = remainingTime;
			
		}else{
			simulationStep = timeStep;
		}
		
		PrepareParticles(simulationStep);
		StepParticles(simulationStep);
		
		remainingTime -= timeStep;
	}
}
