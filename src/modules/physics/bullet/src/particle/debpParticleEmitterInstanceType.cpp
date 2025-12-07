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

#include "debpParticleEmitterInstance.h"
#include "debpParticleEmitter.h"
#include "debpParticleEmitterType.h"
#include "debpParticleEmitterInstanceType.h"
#include "../debpCommon.h"
#include "../debpCollisionObject.h"
#include "../dePhysicsBullet.h"
#include "../collider/debpCollider.h"
#include "../coldet/debpConvexResultCallback.h"
#include "../component/debpModel.h"
#include "../component/debpComponent.h"
#include "../forcefield/debpForceField.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../terrain/heightmap/debpHeightTerrain.h"
#include "../visitors/debpClosestRayResultCallback.h"
#include "../world/debpCollisionWorld.h"
#include "../world/debpWorld.h"

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/particle/deParticleEmitterController.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceManager.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/model/deModelLOD.h>



#ifndef random
#define random rand
#endif

const btScalar vRandomFactor = 1.0f / (btScalar)RAND_MAX;

// Class debpParticleEmitterInstanceType
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

debpParticleEmitterInstanceType::debpParticleEmitterInstanceType() :
pInstance(NULL),
pType(0),

pComponent(NULL),

pParticles(NULL),
pParticleCount(0),
pParticleSize(0),

pCastIntervalMin(0.0f),
pCastIntervalGap(1.0f),
pNextCastTimer(1.0f),

pBurstTimer(0.0f),
pBurstLastCurvePoint(-1),

pGraParticles(NULL),
pGraParticleSize(0){
}

debpParticleEmitterInstanceType::~debpParticleEmitterInstanceType(){
	pCleanUp();
}



// Management
///////////////

void debpParticleEmitterInstanceType::SetInstance(debpParticleEmitterInstance *instance){
	pInstance = instance;
}

void debpParticleEmitterInstanceType::SetType(int type){
	pType = type;
}



void debpParticleEmitterInstanceType::PrepareParticles(bool casting, float elapsed, float travelledDistance){
	debpParticleEmitter * const emitter = pInstance->GetParticleEmitter();
	
	if(emitter){
		const deParticleEmitterType &engType = pInstance->GetParticleEmitter()->GetEmitter()->GetTypeAt(pType);
		const debpParticleEmitterType &type = emitter->GetTypeAt(pType);
		//const btScalar btElapsed = ( btScalar )elapsed;
		//const btScalar btElapsedSquared = btElapsed * btElapsed;
		int p;
		
		// update the particles
		const bool updateProgress = (engType.GetSimulationType() != deParticleEmitterType::estBeam);
		
		for(p=0; p<pParticleCount; p++){
			sParticle &particle = pParticles[p];
			
			particle.timeToLive -= elapsed;
			if(particle.timeToLive > 0.0f){
				if(updateProgress){
					particle.lifetime = 1.0f - particle.lifetimeFactor * particle.timeToLive;
					ParticleSetProgressParams(particle);
				}
				
			}else{
				KillParticle(p);
				p--;
			}
		}
		
		// cast new particles if enabled until the cast timer runs out
		if(casting){
			// for beams and ribbons emit just one particle. for particles as many as requested
			const bool castMultipleParticles = (engType.GetSimulationType() == deParticleEmitterType::estParticle);
			deParticleEmitterInstance &instance = *pInstance->GetInstance();
			bool castAnotherParticle = true;
			
			if(emitter->GetEmitter()->GetEmitBurst()){
				const float baseCount = type.EvaluateCastParameter(instance, debpParticleEmitterType::escParticleCount, deParticleEmitterType::epParticleCount);
				const decCurveBezier &curve = engType.GetParameter(deParticleEmitterType::epParticleCount).GetCurveProgress();
				float lifetime = emitter->GetEmitter()->GetBurstLifetime();
				const int curvePointCount = curve.GetPointCount();
				int particleCount = 1;
				int i;
				
				if(lifetime > FLOAT_SAFE_EPSILON){
					if(curvePointCount > 0){
						while(pBurstLastCurvePoint < curvePointCount - 1){
							/*if( pBurstLastCurvePoint >= curvePointCount - 1 ){
								pBurstTimer += elapsed;
								
								if(pBurstTimer < lifetime){
									break;
									
								}else{
									elapsed = pBurstTimer - lifetime;
									pBurstTimer = 0.0f;
									pBurstLastCurvePoint = -1;
								}
								
							}else{*/
								const decVector2 &point = curve.GetPointAt(pBurstLastCurvePoint + 1).GetPoint();
								
								if(pBurstTimer / lifetime < point.x){
									pBurstTimer += elapsed;
									break;
									
								}else{
									if(castMultipleParticles){
										particleCount = (int)(baseCount * point.y + 0.5f);
									}
									
									for(i=0; i<particleCount; i++){
										if(castAnotherParticle){
											CastParticle(1.0f, 0.0f);
											castAnotherParticle = castMultipleParticles;
										}
									}
									
									elapsed -= point.x * lifetime - pBurstTimer;
									pBurstTimer = point.x * lifetime;
									pBurstLastCurvePoint++;
								}
							//}
						}
						
					}else{
						if(pBurstLastCurvePoint == -1){
							if(castMultipleParticles){
								particleCount = (int)(baseCount + 0.5f);
							}
							
							for(i=0; i<particleCount; i++){
								if(castAnotherParticle){
									CastParticle(1.0f, 0.0f);
									castAnotherParticle = castMultipleParticles;
								}
							}
							
							pBurstLastCurvePoint = 0;
						}
						/*
						while(true){
							if(pBurstLastCurvePoint == -1){
								particleCount = (int)(baseCount + 0.5f);
								
								for(i=0; i<particleCount; i++){
									if(castAnotherParticle){
										CastParticle();
										castAnotherParticle = castMultipleParticles;
									}
								}
								
								pBurstLastCurvePoint = 0;
							}
							
							pBurstTimer += elapsed;
							
							if(pBurstTimer < lifetime){
								break;
								
							}else{
								elapsed = pBurstTimer - lifetime;
								pBurstTimer = 0.0f;
								pBurstLastCurvePoint = -1;
							}
						}
						*/
					}
				}
				
			}else{
				const bool intervalAsDistance = engType.GetIntervalAsDistance();
				int particleCount = 1;
				int i;
				
				if(intervalAsDistance){
					float castDistance = 0.0f;
					float nextDistance;
					float factor;
					
					nextDistance = pNextCastTimer;
					pNextCastTimer -= travelledDistance;
					
					while(pNextCastTimer <= 0.0f){
						castDistance += nextDistance;
						factor = castDistance / travelledDistance;
						
						if(castMultipleParticles){
							particleCount = (int)(type.EvaluateCastParameter(instance,
								debpParticleEmitterType::escParticleCount,
								deParticleEmitterType::epParticleCount) + 0.5f);
						}
						
						for(i=0; i<particleCount; i++){
							if(castAnotherParticle){
								CastParticle(factor, elapsed * factor);
								castAnotherParticle = castMultipleParticles;
							}
						}
						
						nextDistance = pCastIntervalMin + pCastIntervalGap * ((float)random() * vRandomFactor);
						pNextCastTimer += nextDistance;
					}
					
				}else{
					pNextCastTimer -= elapsed;
					
					while(pNextCastTimer <= 0.0f){
						if(castMultipleParticles){
							particleCount = (int)(type.EvaluateCastParameter(instance,
								debpParticleEmitterType::escParticleCount,
								deParticleEmitterType::epParticleCount) + 0.5f);
						}
						
						for(i=0; i<particleCount; i++){
							if(castAnotherParticle){
								CastParticle(1.0f, 0.0f);
								castAnotherParticle = castMultipleParticles;
							}
						}
						
						pNextCastTimer += pCastIntervalMin + pCastIntervalGap * ((float)random() * vRandomFactor);
					}
				}
			}
		}
	}
}

void debpParticleEmitterInstanceType::ApplyForceField(const debpForceField &forceField, float elapsed){
	const deParticleEmitterType &engType = pInstance->GetParticleEmitter()->GetEmitter()->GetTypeAt(pType);
	if(engType.GetSimulationType() == deParticleEmitterType::estBeam){
		return;
	}
	
	const deForceField &engForceField = forceField.GetForceField();
	const float fluctStrength = pInstance->GetForceFieldFluctuation().GetStrength();
	const float fluctDirection = pInstance->GetForceFieldFluctuation().GetDirection();
	const float ffFluctuationDirection = engForceField.GetFluctuationDirection();
	const float ffFluctuationForce = engForceField.GetFluctuationForce();
	const float ffExponent = engForceField.GetExponent();
	const decDVector &ffpos = engForceField.GetPosition();
	const decVector &ffDir = forceField.GetDirection();
	const float ffRadius = engForceField.GetRadius();
	const float ffRadiusSquared = ffRadius * ffRadius;
	//const float elapsedSquared = elapsed * elapsed;
	const float ffForce = engForceField.GetForce();
	//const float ffVelocity = engForceField.GetForce() * 4.5f / 15.0f; // 15N => 4.5 m/s
	const deForceField::eFieldTypes ffFieldType = engForceField.GetFieldType();
	const deForceField::eApplicationTypes ffApplyType = engForceField.GetApplicationType();
	const float flucAngle = DEG2RAD * 180.0f;
	float distance, distanceSquared;
	float /*velocity,*/ addForce;
	//const float volumeFactor = 4.0f * PI / 3.0f;
	decVector direction;
	float forceFactor;
	decMatrix flucmat;
	int p;
	
	for(p=0; p<pParticleCount; p++){
		sParticle &particle = pParticles[p];
		
		direction.x = (float)(particle.position.x() - ffpos.x);
		direction.y = (float)(particle.position.y() - ffpos.y);
		direction.z = (float)(particle.position.z() - ffpos.z);
		
		distanceSquared = direction * direction;
		
		if(distanceSquared >= ffRadiusSquared) continue;
		
		if(ffFieldType == deForceField::eftLinear){
			distance = sqrtf(distanceSquared);
			direction = ffDir;
			
		}else{
			if(distanceSquared < 1e-6f) continue;
			
			distance = sqrtf(distanceSquared);
			direction *= 1.0f - distance / ffRadius;
		}
		
		switch(ffApplyType){
		case deForceField::eatDirect:
			//forceFactor = 1.0f;
			forceFactor = particle.forceFieldDirect;
			break;
			
		case deForceField::eatSurface:
			//forceFactor = particle.size * particle.size * PI;
			forceFactor = particle.forceFieldSurface;
			break;
			
		case deForceField::eatMass:
			//forceFactor = particle.size * particle.size * particle.size * volumeFactor;
			forceFactor = particle.forceFieldMass * particle.mass;
			break;
			
		case deForceField::eatSpeed:
			forceFactor = particle.forceFieldSpeed * (float)particle.linearVelocity.length();
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		
		//printf( "particle %i: velocity=%g dist=%g radius=%g exp=%g elapsed=%g\n", p, velocity, distance, ffRadius, ffExponent, elapsed );
		
		addForce = (ffForce + ffFluctuationForce * fluctStrength)
			* powf( 1.0f - distance / ffRadius, ffExponent ) * forceFactor;
		
		/*if( particle.gravity.y() < 1e-5f ){
			addvelo = force * 0.02f;
			//addtorque = 0.0f; //force * ( DEG2RAD * 45.0f );
			
		}else{
			addvelo = force * 0.2f;
			//addtorque = 0.0f;
		}*/
		//addvelo = force * 0.2f; // need a way to influende this parameter
		
		flucmat.SetRotationY(ffFluctuationDirection * fluctDirection * flucAngle);
		
		direction = flucmat.TransformNormal(direction) * addForce;
		particle.force.setX(particle.force.x() + direction.x);
		particle.force.setY(particle.force.y() + direction.y);
		particle.force.setZ(particle.force.z() + direction.z);
		//particle.angularVelocity += addtorque; // += direction * addtorque;
	}
}

void debpParticleEmitterInstanceType::StepParticles(float elapsed){
	const deParticleEmitterType &engType = pInstance->GetParticleEmitter()->GetEmitter()->GetTypeAt(pType);
	int i;
	
	if(engType.GetSimulationType() != deParticleEmitterType::estBeam){
		//pInstance->GetBullet()->LogInfoFormat( "StepParticles: pParticleCount=%i pParticleSize=%i elapsed=%f\n", pParticleCount, pParticleSize, elapsed );
		
		for(i=0; i<pParticleCount; i++){
			sParticle &particle = pParticles[i];
			
			if(! ParticleSimulate(particle, elapsed)){
				KillParticle(i);
				i--;
			}
		}
	}
}

void debpParticleEmitterInstanceType::FinishStepping(){
	int i;
	
	for(i=0; i<pParticleCount; i++){
		ParticleUpdateTrailEmitter(pParticles[i]);
	}
	
	UpdateGraphicParticles();
}

void debpParticleEmitterInstanceType::UpdateGraphicParticles(){
	const debpParticleEmitter * const emitter = pInstance->GetParticleEmitter();
	const decDVector &position = pInstance->GetInstance()->GetReferencePosition();
	const float rotationFactor = 255.0f / TWO_PI;
	int p;
	
	if(pParticleCount > pGraParticleSize){
		deParticleEmitterInstanceType::sParticle *newArray = new deParticleEmitterInstanceType::sParticle[pParticleCount];
		
		if(pGraParticles){
			delete [] pGraParticles;
		}
		pGraParticles = newArray;
		pGraParticleSize = pParticleCount;
	}
	
	if(emitter){
		const debpParticleEmitterType &type = emitter->GetTypeAt(pType);
		const btScalar factorAngVelo = (btScalar)type.GetParamFactorAngVelo();
		const btScalar factorLinVelo = (btScalar)type.GetParamFactorLinVelo();
		
		for(p=0; p<pParticleCount; p++){
			deParticleEmitterInstanceType::sParticle &destParticle = pGraParticles[p];
			const sParticle &srcParticle = pParticles[p];
			
			destParticle.lifetime = srcParticle.lifetime;
			destParticle.positionX = (float)(srcParticle.position.x() - position.x);
			destParticle.positionY = (float)(srcParticle.position.y() - position.y);
			destParticle.positionZ = (float)(srcParticle.position.z() - position.z);
			
			const btScalar velocity = srcParticle.linearVelocity.length();
			if(velocity > 1e-5){
				destParticle.linearDirectionX = (unsigned char)decMath::clamp(
					(int)(decMath::linearStep(srcParticle.linearVelocity.x(),
						-1.0f, 1.0f, 0.0f, 255.0f)), 0, 255);
				destParticle.linearDirectionY = (unsigned char)decMath::clamp(
					(int)(decMath::linearStep(srcParticle.linearVelocity.y(),
						-1.0f, 1.0f, 0.0f, 255.0f)), 0, 255);
				destParticle.linearDirectionZ = (unsigned char)decMath::clamp(
					(int)(decMath::linearStep(srcParticle.linearVelocity.z(),
						-1.0f, 1.0f, 0.0f, 255.0f)), 0, 255);
				
			}else{ // dummy direction along z axis
				destParticle.linearDirectionX = 127;
				destParticle.linearDirectionY = 127;
				destParticle.linearDirectionZ = 255;
			}
			destParticle.linearVelocity = velocity * factorLinVelo;
			destParticle.angularVelocity = srcParticle.angularVelocity * factorAngVelo;
			destParticle.rotation = (unsigned char)decMath::clamp((unsigned int)(
				decMath::normalize(srcParticle.rotation, 0.0f, TWO_PI) * rotationFactor), 0, 255);
			
			destParticle.castSize = srcParticle.castSize;
			destParticle.castEmissivity = srcParticle.castEmissivity;
			destParticle.castRed = srcParticle.castRed;
			destParticle.castGreen = srcParticle.castGreen;
			destParticle.castBlue = srcParticle.castBlue;
			destParticle.castTransparency = srcParticle.castTransparency;
		}
		
		pInstance->GetInstance()->GetTypeAt(pType).SetParticleArray(pGraParticles, pParticleCount);
		
	}else{
		pInstance->GetInstance()->GetTypeAt(pType).SetParticleArray(pGraParticles, 0);
	}
	
	pInstance->GetInstance()->NotifyTypeParticlesChangedAt(pType);
}

void debpParticleEmitterInstanceType::UpdateCastIntervals(){
	debpParticleEmitter * const emitter = pInstance->GetParticleEmitter();
	
	if(emitter){
		const debpParticleEmitterType &type = emitter->GetTypeAt(pType);
		deParticleEmitterInstance &instance = *pInstance->GetInstance();
		float value, spread;
		
		emitter->UpdateParameters();
		value = type.EvaluateValueParameter(instance, debpParticleEmitterType::escInterval, deParticleEmitterType::epInterval);
		spread = type.EvaluateSpreadParameter(instance, debpParticleEmitterType::escInterval, deParticleEmitterType::epInterval);
		if(spread < 0.0f){
			spread = 0.0f;
		}
		
		pCastIntervalMin = value - spread * 0.5f;
		if(pCastIntervalMin < 1e-5f){
			pCastIntervalMin = 1e-5f;
		}
		pCastIntervalGap = spread;
		
	}else{
		pCastIntervalMin = 0.0f;
		pCastIntervalGap = 1.0f;
	}
	
	if(pNextCastTimer > pCastIntervalMin + pCastIntervalGap){
		pNextCastTimer = pCastIntervalMin + pCastIntervalGap;
	}
}



void debpParticleEmitterInstanceType::ResetBurst(){
	pBurstTimer = 0.0f;
	pBurstLastCurvePoint = -1;
}

void debpParticleEmitterInstanceType::OnTypeChanged(){
	UpdateCastIntervals();
	
	pComponent = NULL;
	
	if(! pInstance){
		return;
	}
	
	const deParticleEmitterInstanceType &engInstType = pInstance->GetInstance()->GetTypeAt(pType);
	if(engInstType.GetComponent()){
		pComponent = (debpComponent*)engInstType.GetComponent()->GetPeerPhysics();
	}
	
	const deParticleEmitterType &engType = pInstance->GetParticleEmitter()->GetEmitter()->GetTypeAt(pType);
	const debpParticleEmitterType &type = pInstance->GetParticleEmitter()->GetTypeAt(pType);
	if(! type.GetModel()){
		return;
	}
	
	switch(engType.GetCastFrom()){
	case deParticleEmitterType::ecfVertex:
		type.GetModel()->PrepareNormals();
		break;
		
	case deParticleEmitterType::ecfFace:
		type.GetModel()->PrepareNormals();
		type.GetModel()->PrepareFaceProbabilities();
		break;
		
	default:
		break;
	}
}



void debpParticleEmitterInstanceType::CastParticle(float distance, float timeOffset){
	const debpParticleEmitter * const emitter = pInstance->GetParticleEmitter();
	
	if(emitter && pParticleCount < 10000){ // currently hard-coded... not more than 10k active particles
		const deParticleEmitterType &engType = emitter->GetEmitter()->GetTypeAt(pType);
		
		if(engType.GetSimulationType() == deParticleEmitterType::estBeam){
			CastBeamParticle(distance);
			
		}else{
			CastSingleParticle(distance, timeOffset);
		}
	}
}

void debpParticleEmitterInstanceType::KillParticle(int index){
	if(index < 0 || index >= pParticleCount){
		DETHROW(deeInvalidParam);
	}
	
	deParticleEmitterInstance * const trailEmitter = pParticles[index].trailEmitter;
	const int simtype = pInstance->GetParticleEmitter()->GetEmitter()->GetTypeAt(pType).GetSimulationType();
	
	//debpParticleInstance &P = pParticles[ index ];
	//pBullet->LogInfoFormat( "kill particle: i=%i p(%.3f,%.3f,%.3f) v=(%.3f,%.3f,%.3f) s=%.1f ttl=%.1f", index, P.position.x, P.position.y, P.position.z, P.velocity.x, P.velocity.y, P.velocity.z, P.size, P.timeToLive );
	
	if(trailEmitter){
		trailEmitter->SetEnableCasting(false);
		trailEmitter->SetRemoveAfterLastParticleDied(true);
		trailEmitter->FreeReference();
		pParticles[index].trailEmitter = NULL;
	}
	
	if(simtype == deParticleEmitterType::estParticle){
		if(index < pParticleCount - 1){
			memcpy(pParticles + index, pParticles + (pParticleCount - 1), sizeof(sParticle));
		}
		
	}else{ // ribbon or beam
		if(index < pParticleCount - 1){
			memmove(pParticles + index, pParticles + (index + 1), sizeof(sParticle) * (pParticleCount - 1 - index));
		}
	}
	
	pParticleCount--;
	
	if(pParticleCount == 0){
		pInstance->RequestCheckForLastParticle();
	}
}

void debpParticleEmitterInstanceType::KillAllParticles(){
	if(pParticleCount == 0){
		return;
	}
	
	while(pParticleCount > 0){
		pParticleCount--;
		
		deParticleEmitterInstance * const trailEmitter = pParticles[pParticleCount].trailEmitter;
		if(trailEmitter){
			trailEmitter->SetEnableCasting(false);
			trailEmitter->SetRemoveAfterLastParticleDied(true);
			trailEmitter->FreeReference();
			pParticles[pParticleCount].trailEmitter = NULL;
		}
	}
	
	pInstance->RequestCheckForLastParticle();
}



void debpParticleEmitterInstanceType::CastSingleParticle(float distance, float timeOffset){
	// enlarge the particles array if required 
	if(pParticleCount == pParticleSize){
		const int newSize = pParticleSize + 10;
		sParticle * const newArray = new sParticle[newSize];
		
		if(pParticles){
			memcpy(newArray, pParticles, sizeof(sParticle) * pParticleSize);
			delete [] pParticles;
		}
		pParticles = newArray;
		pParticleSize = newSize;
	}
	
	// set up new particle
	sParticle &particle = pParticles[pParticleCount++];
	
	ParticleSetCastParams(particle, distance, timeOffset);
	ParticleSetProgressParams(particle);
	ParticleCreateTrailEmitter(particle);
	
	//printf( "cast: size=%g emi=%g color=(%i,%i,%i,%i)\n", particle.castSize, particle.castEmissivity, particle.castRed, particle.castGreen, particle.castBlue, particle.castTransparency );
	//pBullet->LogInfoFormat( "cast particle: i=%i p(%.3g,%.3g,%.3g) v=(%.3g,%.3g,%.3g) s=%.1f ttl=%.1f", pParticleCount-1, P.position.x, P.position.y, P.position.z, P.velocity.x, P.velocity.y, P.velocity.z, P.size, P.timeToLive );
	/*pBullet->LogInfoFormat( "cast particle: size=%i clr(%i,%i,%i,%i) emi=%i", particle.castSize, particle.castRed,
		particle.castGreen, particle.castBlue, particle.castTransparency, particle.castEmissivity);*/
	//pBullet->LogInfo( "cast particle" );
}

void debpParticleEmitterInstanceType::CastBeamParticle(float distance){
	const debpParticleEmitter * const emitter = pInstance->GetParticleEmitter();
	const debpParticleEmitterType &type = emitter->GetTypeAt(pType);
	const int particleCount = (int)(type.EvaluateCastParameter(*pInstance->GetInstance(),
		debpParticleEmitterType::escParticleCount, deParticleEmitterType::epParticleCount) + 0.5f);
	
	// kill the previous beam if existing
	KillAllParticles();
	
	// at least one particle has to be cast otherwise forget about the beam
	if(particleCount > 0){
		int i;
		
		// enlarge the particles array if required. we enlarge by the maximum particle count even
		// if we should use less later on
		if(pParticleCount + particleCount > pParticleSize){
			const int newSize = pParticleCount + particleCount + 10;
			sParticle * const newArray = new sParticle[newSize];
			
			if(pParticles){
				memcpy(newArray, pParticles, sizeof(sParticle) * pParticleSize);
				delete [] pParticles;
			}
			pParticles = newArray;
			pParticleSize = newSize;
		}
		
		// set up new particle
		sParticle &particleCast = pParticles[pParticleCount++];
		
		ParticleSetCastParams(particleCast, distance, 0.0f);
		ParticleSetProgressParams(particleCast);
		
		// simulate the particle all the way to the end if there is more than one particle. if a kill particle
		// is found the end of the beam is assumed
		if(particleCount > 1){
			const float lifetimeStep = 1.0f / (float)(particleCount - 1);
			const float simTimeStep = particleCast.timeToLive * lifetimeStep;
			
			for(i=1; i<particleCount; i++){
				memcpy(pParticles + pParticleCount, pParticles + (pParticleCount - 1), sizeof(sParticle));
				
				sParticle &particleProgress = pParticles[pParticleCount++];
				particleProgress.lifetime += lifetimeStep;
				
				ParticleSetProgressParams(particleProgress);
				//ParticleCreateTrailEmitter( particleProgress ); // does this make sense? it would be possible
				if(! ParticleSimulate(particleProgress, simTimeStep)){
					break;
				}
			}
		}
		
		// adjust the particle lifetime to match the actual used particle count to obtain location along beam
		if(pParticleCount > 1){
			const float lifetimeStep = 1.0f / (float)(pParticleCount - 1);
			
			for(i=1; i<pParticleCount; i++){
				pParticles[i].lifetime = lifetimeStep * (float)i;
			}
		}
		
		// cast trail only for the first particle in the beam. this has to be done after all particles have
		// been cast since the first particle is used as blue print and copied over to all other particles
		// then modified. if the trail emitter exists already it is multiplied across the beam which is not
		// only looking wrong it trashes the reference count of the trail emitter.
		ParticleCreateTrailEmitter(particleCast);
	}
}



void debpParticleEmitterInstanceType::ParticleSetCastParams(sParticle &particle, float distance, float timeOffset){
	// important to avoid problems later on in bad cases
	particle.trailEmitter = NULL;
	
	// calculate cast matrix for the particle
	decDMatrix castMatrix;
	
	ParticleCastMatrix(castMatrix);
	
	if(distance >= 0.9999f){
		castMatrix *= pInstance->GetEmitterMatrix();
		
	}else{
		castMatrix *= pInstance->GetEmitterMatrixByDistance(distance);
	}
	
	// determine cast values
	const debpParticleEmitter &emitter = *pInstance->GetParticleEmitter();
	const debpParticleEmitterType &type = emitter.GetTypeAt(pType);
	const deParticleEmitterInstance &instance = *pInstance->GetInstance();
	const deParticleEmitterType &engType = emitter.GetEmitter()->GetTypeAt(pType);
	decDVector view;
	
	particle.castSize = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escSize, deParticleEmitterType::epSize);
	particle.castMass = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escMass, deParticleEmitterType::epMass);
	particle.castRotation = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escRotation, deParticleEmitterType::epRotation) * (PI * 2.0f);
	particle.castLinearVelocity = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escLinearVelocity, deParticleEmitterType::epLinearVelocity);
	particle.castAngularVelocity = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escAngularVelocity, deParticleEmitterType::epAngularVelocity);
	particle.castBrown = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escBrown, deParticleEmitterType::epBrownMotion);
	particle.castDamp = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escDamp, deParticleEmitterType::epDamping);
	particle.castDrag = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escDrag, deParticleEmitterType::epDrag);
	particle.castGravity.setX((btScalar)type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escGravityX, deParticleEmitterType::epGravityX));
	particle.castGravity.setY((btScalar)type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escGravityY, deParticleEmitterType::epGravityY));
	particle.castGravity.setZ((btScalar)type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escGravityZ, deParticleEmitterType::epGravityZ));
	particle.castLocalGravity = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escLocalGravity, deParticleEmitterType::epLocalGravity);
	particle.castForceFieldDirect = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escForceFieldDirect, deParticleEmitterType::epForceFieldDirect);
	particle.castForceFieldSurface = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escForceFieldSurface, deParticleEmitterType::epForceFieldSurface);
	particle.castForceFieldMass = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escForceFieldVolume, deParticleEmitterType::epForceFieldMass);
	particle.castForceFieldSpeed = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escForceFieldSpeed, deParticleEmitterType::epForceFieldSpeed);
	particle.castElasticity = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escElasticity, deParticleEmitterType::epElasticity);
	particle.castRoughness = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escRoughness, deParticleEmitterType::epRoughness);
	
	if(engType.GetCollisionEmitter()){
		particle.castEmitDirection = type.EvaluateCastParameter(instance,
			debpParticleEmitterType::escEmitDirection, deParticleEmitterType::epEmitDirection);
		
	}else{
		particle.castEmitDirection = 0.0f;
	}
	
	particle.castEmissivity = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escEmissivity, deParticleEmitterType::epEmissivity);
	particle.castRed = (unsigned char)(type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escRed, deParticleEmitterType::epRed) * 255.0f);
	particle.castGreen = (unsigned char)(type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escGreen, deParticleEmitterType::epGreen) * 255.0f);
	particle.castBlue = (unsigned char)(type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escBlue, deParticleEmitterType::epBlue) * 255.0f);
	particle.castTransparency = (unsigned char)(type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escTransparency, deParticleEmitterType::epTransparency) * 255.0f);
	
	particle.timeToLive = type.EvaluateCastParameter(instance,
		debpParticleEmitterType::escTimeToLive, deParticleEmitterType::epTimeToLive);
	particle.lifetimeFactor = 1.0f / particle.timeToLive;
	particle.lifetime = timeOffset;
	
	particle.position.setValue((btScalar)castMatrix.a14, (btScalar)castMatrix.a24, (btScalar)castMatrix.a34);
	particle.rotation = particle.castRotation;
	
	view = castMatrix.TransformView() * particle.castLinearVelocity;
	particle.linearVelocity.setValue((btScalar)view.x, (btScalar)view.y, (btScalar)view.z);
	
	particle.angularVelocity = particle.castAngularVelocity
		* type.EvaluateProgressParameter( debpParticleEmitterType::escAngularVelocity, 0.0f );
}

void debpParticleEmitterInstanceType::ParticleCastMatrix(decDMatrix &matrix){
	const debpParticleEmitter &emitter = *pInstance->GetParticleEmitter();
	const deParticleEmitterType &engType = emitter.GetEmitter()->GetTypeAt(pType);
	const debpParticleEmitterType &type = emitter.GetTypeAt(pType);
	const deParticleEmitterInstance &instance = *pInstance->GetInstance();
	const debpModel *model = type.GetModel();
	bool deformedModel = false;
	float rval, rval2;
	decVector vector;
	
	// determine which model to use
	if(pComponent && pComponent->GetModel()){
		model = pComponent->GetModel();
		if(model){
			pComponent->PrepareWeights();
			deformedModel = true;
			
			if(engType.GetCastFrom() == deParticleEmitterType::ecfFace){
				// the model in the particle emitter type is guaranteed to be prepared but the
				// one in the component is not. furthermore it can change at any time so make
				// sure it is prepared. if it is already prepared nothing happens
				pComponent->GetModel()->PrepareNormals();
				pComponent->GetModel()->PrepareFaceProbabilities();
			}
		}
	}
	
	if(model){
		switch(engType.GetCastFrom()){
		case deParticleEmitterType::ecfVertex:{
			const decVector * const normals = model->GetNormals();
			const deModel &engModel = model->GetModel();
			const deModelLOD &engModelLOD = *engModel.GetLODAt(0);
			const int vertexCount = engModelLOD.GetVertexCount();
			
			int vertex = (int)(((float)random() / (float)RAND_MAX) * (float)vertexCount);
			if(vertex < 0){
				vertex = 0;
				
			}else if(vertex > vertexCount - 1){
				vertex = vertexCount - 1;
			}
			
			const deModelVertex &engVertex = engModelLOD.GetVertexAt(vertex);
			decVector normal(normals[vertex]);
			
			if(deformedModel && engVertex.GetWeightSet() != -1){
				// this is not correct here. transforming the normal by the matrix is usually not
				// producing the right solution due to skewing. for casting a particle though this
				// should not be a problem. once the data is calculated in the physics module
				// itself this can be rectified if needed
				
				// DEPRECATED TODO Weights have to be removed from deComponent !!!
				// NOTE this calculation is done for one vertex or for a face (three vertices).
				//      instead of calculating all values it would be possible to calculate here
				//      only the required vertices (1 or 3). for this the weights can be directly
				//      calculated not requring the full matrix. for the time being the deModelLOD
				//      has to be accessed directly. if data is retained this requires retaining
				//      the model data. this retaining can be held for the lifetime of the model
				//      since it is to be expected the model is used for particle casting the
				//      entire time.
				normal = pComponent->GetWeights(engVertex.GetWeightSet()).TransformNormal(normal);
			}
			
			if(normal.y > 1.0f - VECTOR_THRESHOLD){
				matrix.SetWorld(decDVector(engVertex.GetPosition()), decQuaternion(-0.707107f, 0.0f, 0.0f, 0.707107f));
				
			}else if(normal.y < VECTOR_THRESHOLD - 1.0f){
				matrix.SetWorld(decDVector(engVertex.GetPosition()), decQuaternion(0.707107f, 0.0f, 0.0f, 0.707107f));
				
			}else{
				matrix.SetWorld(decDVector(engVertex.GetPosition()), decDVector(normal), decDVector(0.0, 1.0, 0.0));
			}
			}break;
			
		case deParticleEmitterType::ecfFace:{
			// for faces we can do it the following way. first we determine the face to use using
			// a probability distribution based on the face areas. this has been precalculated
			// in the model object already. for the chosen face a random position is then calculated.
			// faces are half a skewed rectangle. the result is the same the non-skewed rectangle is
			// used instead. since the values are evenly distributed in the rectangle the half outside
			// the face can be simply mirrored to the inside. this does not change the distribution
			// since both parts have the same distribution
			const decVector * const normals = model->GetNormals();
			const deModel &engModel = model->GetModel();
			const deModelLOD &engModelLOD = *engModel.GetLODAt(0);
			
			const int face = model->IndexOfFaceWithProbability((float)random() / (float)RAND_MAX);
			
			const deModelFace &engFace = engModelLOD.GetFaceAt(face);
			const deModelVertex &engV1 = engModelLOD.GetVertexAt(engFace.GetVertex1());
			const deModelVertex &engV2 = engModelLOD.GetVertexAt(engFace.GetVertex2());
			const deModelVertex &engV3 = engModelLOD.GetVertexAt(engFace.GetVertex3());
			
			decVector v1(engV1.GetPosition());
			decVector v2(engV2.GetPosition());
			decVector v3(engV3.GetPosition());
			
			if(deformedModel){
				if(engV1.GetWeightSet() != -1){
					v1 = pComponent->GetWeights(engV1.GetWeightSet()) * v1;
				}
				if(engV2.GetWeightSet() != -1){
					v2 = pComponent->GetWeights(engV2.GetWeightSet()) * v2;
				}
				if(engV3.GetWeightSet() != -1){
					v3 = pComponent->GetWeights(engV3.GetWeightSet()) * v3;
				}
			}
			
			rval = (float)random() / (float)RAND_MAX;
			rval2 = (float)random() / (float)RAND_MAX;
			
			if(rval + rval2 > 1.0f){
				const float temp = rval;
				rval = 1.0f - rval2;
				rval2 = 1.0f - temp;
			}
			
			decVector pos(v1 * (1.0f - rval - rval2) + v2 * rval2 + v3 * rval);
			
			const float l1 = sqrtf(rval * rval + rval2 * rval2); // sqrt((rval,rval2) - (0,0))
			const float l2 = sqrtf(rval * rval + (rval2 - 1.0f) * (rval2 - 1.0f)); // sqrt((rval,rval2) - (0,1))
			const float l3 = sqrtf((rval - 1.0f) * (rval - 1.0f) + rval2 * rval2); // sqrt((rval,rval2) - (1,0))
			const float lt = l1 + l2 + l3;
			
			decVector normal;
			
			if(lt > FLOAT_SAFE_EPSILON){
				decVector n1(normals[engFace.GetVertex1()]);
				decVector n2(normals[engFace.GetVertex2()]);
				decVector n3(normals[engFace.GetVertex3()]);
				
				if(deformedModel){
					// this is not correct here. transforming the normal by the matrix is usually not
					// producing the right solution due to skewing. for casting a particle though this
					// should not be a problem. once the data is calculated in the physics module
					// itself this can be rectified if needed
					if(engV1.GetWeightSet() != -1){
						n1 = pComponent->GetWeights(engV1.GetWeightSet()).TransformNormal(n1);
					}
					if(engV2.GetWeightSet() != -1){
						n2 = pComponent->GetWeights(engV2.GetWeightSet()).TransformNormal(n2);
					}
					if(engV3.GetWeightSet() != -1){
						n3 = pComponent->GetWeights(engV3.GetWeightSet()).TransformNormal(n3);
					}
				}
				
				normal = n1 * (l1 / lt) + n2 * (l2 / lt) + n3 * (l3 / lt);
				
			}else{
				normal = engFace.GetFaceNormal();
			}
			
			if(normal.y > 1.0f - VECTOR_THRESHOLD){
				matrix.SetWorld(decDVector(pos), decQuaternion(-0.707107f, 0.0f, 0.0f, 0.707107f));
				
			}else if(normal.y < VECTOR_THRESHOLD - 1.0f){
				matrix.SetWorld(decDVector(pos), decQuaternion(0.707107f, 0.0f, 0.0f, 0.707107f));
				
			}else{
				matrix.SetWorld(decDVector(pos), decDVector(normal), decDVector(0.0, 1.0, 0.0));
			}
			}break;
			
		case deParticleEmitterType::ecfVolume:
			return;
		}
	}
	
#if 0
	rval = type.EvaluateCastParameter(instance, debpParticleEmitterType::escCastAngleX, deParticleEmitterType::epCastAngleX) * 2.0 * PI;
	vector.y = rval; //castDirectionMin.x * (1.0f - rval) + castDirectionMax.x * rval;
	rval = type.EvaluateCastParameter(instance, debpParticleEmitterType::escCastAngleY, deParticleEmitterType::epCastAngleY) * 2.0 * PI;
	vector.x = rval; //castDirectionMin.y * (1.0f - rval) + castDirectionMax.y * rval;
	//rval = ( float )random() * vRandomFactor;
	vector.z = 0.0f; //castDirectionMin.z * (1.0f - rval) + castDirectionMax.z * rval;
#endif
	// http://stackoverflow.com/questions/5529148/algorithm-calculate-pseudo-random-point-inside-an-ellipse
	// phi = random() * 2 * pi
	// rho = random()
	// x = sqrt( rho ) * cos( phi )
	// y = sqrt( rho ) * sin( phi )
	// rot.y = x * ( ( 2 * pi ) / 2
	// rot.x = y * ( ( 2 * pi ) / 2
	rval = ((float)random() / (float)RAND_MAX) * 2.0f * PI;
	rval2 = sqrtf((float)random() / (float)RAND_MAX);
	
	vector.y = PI * (type.EvaluateValueParameter(instance, debpParticleEmitterType::escCastAngleX, deParticleEmitterType::epCastAngleX)
		+ type.EvaluateSpreadParameter(instance, debpParticleEmitterType::escCastAngleX, deParticleEmitterType::epCastAngleX) * rval2 * cosf(rval));
	vector.x = PI * (type.EvaluateValueParameter(instance, debpParticleEmitterType::escCastAngleY, deParticleEmitterType::epCastAngleY)
		+ type.EvaluateSpreadParameter(instance, debpParticleEmitterType::escCastAngleY, deParticleEmitterType::epCastAngleY) * rval2 * sinf(rval));
	vector.z = 0.0f;
	
	if(model){
		matrix = decDMatrix::CreateRotation(decDVector(vector)) * matrix;
		
	}else{
		matrix.SetRotation(decDVector(vector));
	}
}

void debpParticleEmitterInstanceType::ParticleCreateTrailEmitter(sParticle &particle){
	const deParticleEmitterType &engType = pInstance->GetParticleEmitter()->GetEmitter()->GetTypeAt(pType);
	if(! engType.GetTrailEmitter() || ! pInstance->GetParentWorld()){
		return;
	}
	
	deWorld &engWorld = pInstance->GetParentWorld()->GetWorld();
	const deEngine &engine = *pInstance->GetBullet()->GetGameEngine();
	const btVector3 direction = -particle.linearVelocity;
	
	try{
		particle.trailEmitter = engine.GetParticleEmitterInstanceManager()->CreateInstance();
		particle.trailEmitter->SetEmitter(engType.GetTrailEmitter());
		particle.trailEmitter->SetCollisionFilter(pInstance->GetInstance()->GetCollisionFilter());
		particle.trailEmitter->SetRemoveAfterLastParticleDied(false);
		particle.trailEmitter->SetTimeScale(1.0f);
		particle.trailEmitter->SetEnableCasting(true);
		
		particle.trailEmitter->SetPosition(decDVector(particle.position.getX(), particle.position.getY(), particle.position.getZ()));
		particle.trailEmitter->SetReferencePosition(particle.trailEmitter->GetPosition());
		
		if(direction.getY() > 1.0 - DVECTOR_THRESHOLD){
			particle.trailEmitter->SetOrientation(decQuaternion(-0.707107f, 0.0f, 0.0f, 0.707107f));
			
		}else if(direction.getY() < DVECTOR_THRESHOLD - 1.0){
			particle.trailEmitter->SetOrientation(decQuaternion(0.707107f, 0.0f, 0.0f, 0.707107f));
			
		}else{
			particle.trailEmitter->SetOrientation(decMatrix::CreateVU(decVector((float)direction.getX(),
				(float)direction.getY(), (float)direction.getZ()), decVector(0.0f, 1.0f, 0.0f)).ToQuaternion());
		}
		
		engWorld.AddParticleEmitter(particle.trailEmitter);
		
	}catch(const deException &){
		if(particle.trailEmitter){
			particle.trailEmitter->FreeReference();
			particle.trailEmitter = NULL;
		}
		throw;
	}
}

void debpParticleEmitterInstanceType::ParticleSetProgressParams(sParticle &particle){
	const debpParticleEmitter &emitter = *pInstance->GetParticleEmitter();
	const debpParticleEmitterType &type = emitter.GetTypeAt(pType);
	const debpWorld * const world = pInstance->GetParentWorld();
	
	particle.size = particle.castSize *
		type.EvaluateProgressParameter(debpParticleEmitterType::escSize, particle.lifetime);
	particle.mass = decMath::max(1e-5f, particle.castMass *
		type.EvaluateProgressParameter(debpParticleEmitterType::escMass, particle.lifetime));
	
	particle.brown = particle.castBrown *
		type.EvaluateProgressParameter(debpParticleEmitterType::escBrown, particle.lifetime);
	particle.damp = particle.castDamp *
		type.EvaluateProgressParameter(debpParticleEmitterType::escDamp, particle.lifetime);
	particle.drag = particle.castDrag *
		type.EvaluateProgressParameter(debpParticleEmitterType::escDrag, particle.lifetime);
	particle.elasticity = particle.castElasticity *
		type.EvaluateProgressParameter(debpParticleEmitterType::escElasticity, particle.lifetime);
	particle.roughness = particle.castRoughness *
		type.EvaluateProgressParameter(debpParticleEmitterType::escRoughness, particle.lifetime);
	
	particle.forceFieldDirect = particle.castForceFieldDirect *
		type.EvaluateProgressParameter(debpParticleEmitterType::escForceFieldDirect, particle.lifetime);
	particle.forceFieldSurface = particle.castForceFieldSurface *
		type.EvaluateProgressParameter(debpParticleEmitterType::escForceFieldSurface, particle.lifetime);
	particle.forceFieldMass = particle.castForceFieldMass *
		type.EvaluateProgressParameter(debpParticleEmitterType::escForceFieldVolume, particle.lifetime);
	particle.forceFieldSpeed = particle.castForceFieldSpeed *
		type.EvaluateProgressParameter(debpParticleEmitterType::escForceFieldSpeed, particle.lifetime);
	
	// calculate gravity. it is a bit convoluted to avoid calculating not used stuff
	const btScalar localGravity = (btScalar)(particle.castLocalGravity
		* type.EvaluateProgressParameter( debpParticleEmitterType::escLocalGravity, particle.lifetime ) );
	
	if(localGravity < 1e-5f){
		if(world){
			const decVector &worldGravity = world->GetWorld().GetGravity();
			
			particle.gravity.setX((btScalar)worldGravity.x);
			particle.gravity.setY((btScalar)worldGravity.y);
			particle.gravity.setZ((btScalar)worldGravity.z);
			
		}else{
			particle.gravity.setZero();
		}
		
	}else{
		particle.gravity.setX(particle.castGravity.getX() *
			(btScalar)type.EvaluateProgressParameter(debpParticleEmitterType::escGravityX, particle.lifetime));
		particle.gravity.setY(particle.castGravity.getY() *
			(btScalar)type.EvaluateProgressParameter(debpParticleEmitterType::escGravityY, particle.lifetime));
		particle.gravity.setZ(particle.castGravity.getZ() *
			(btScalar)type.EvaluateProgressParameter(debpParticleEmitterType::escGravityZ, particle.lifetime));
		
		if(world){
			if(localGravity < 0.99999){
				const decVector &worldGravity = world->GetWorld().GetGravity();
				const btScalar blend2 = 1.0f - localGravity;
				
				particle.gravity.setX(particle.gravity.getX() * localGravity + (btScalar)worldGravity.x * blend2);
				particle.gravity.setY(particle.gravity.getY() * localGravity + (btScalar)worldGravity.y * blend2);
				particle.gravity.setZ(particle.gravity.getZ() * localGravity + (btScalar)worldGravity.z * blend2);
			}
			
		}else{
			if(localGravity < 0.99999){
				particle.gravity *= localGravity;
			}
		}
	}
	
	// apply gravity
	particle.force = particle.gravity * particle.mass;
	
	// apply brown motion
	if(particle.brown > 1e-5f){
		btVector3 brownMotion;
		
		brownMotion.setX((btScalar)random() * vRandomFactor * (btScalar)2 - (btScalar)1);
		brownMotion.setY((btScalar)random() * vRandomFactor * (btScalar)2 - (btScalar)1);
		brownMotion.setZ((btScalar)random() * vRandomFactor * (btScalar)2 - (btScalar)1);
		
		particle.force += brownMotion * (btScalar)(particle.brown * particle.mass);
	}
}

bool debpParticleEmitterInstanceType::ParticleSimulate(sParticle &particle, float elapsed){
	// apply force
	particle.linearVelocity += particle.force * (btScalar)(elapsed / particle.mass);
	
	// apply air drag
	if(particle.drag > 1e-10f){
		const btScalar factor1 = (btScalar)1 - (btScalar)(particle.drag * 
			particle.linearVelocity.dot(particle.linearVelocity) * elapsed / particle.mass);
		
		if(factor1 > (btScalar)0){
			particle.linearVelocity *= factor1;
			
		}else{
			particle.linearVelocity.setZero();
		}
	}
	
	// damp velocities
	if(particle.damp > 1e-5f){
		const btScalar factor1 = (btScalar)1 - (btScalar)particle.damp;// * btElapsed;
		
		if(factor1 > (btScalar)0){
			particle.linearVelocity *= factor1;
			particle.angularVelocity *= factor1;
			
		}else{
			particle.linearVelocity.setZero();
			particle.angularVelocity = 0.0f;
		}
	}
	
	// apply angular rotation
	particle.rotation = fmodf(particle.rotation + particle.angularVelocity * elapsed, PI * 2.0f);
	
	// step linear motion
	if(pInstance->GetCanCollide()){
		return ParticleTestCollision(particle, elapsed);
		
	}else{
		particle.position += particle.linearVelocity * (btScalar)elapsed;
	}
	
	return true;
}

class cClosestParticleCallback : public btCollisionWorld::ClosestConvexResultCallback{
private:
	const deParticleEmitterInstance &pInstance;
	
public:
	cClosestParticleCallback(const btVector3 &rayFromWorld, const btVector3 &rayToWorld,
	const deParticleEmitterInstance &instance) :
	ClosestConvexResultCallback(rayFromWorld, rayToWorld), pInstance(instance){
	}
	
	virtual bool needsCollision(btBroadphaseProxy *proxy0) const {
		if(! ClosestConvexResultCallback::needsCollision(proxy0)){
			return false;
		}
		
		const btCollisionObject &collisionObject = *((btCollisionObject*)proxy0->m_clientObject);
		const debpCollisionObject &colObj = *((debpCollisionObject*)collisionObject.getUserPointer());
		
		if(colObj.IsOwnerCollider()){
			deCollider &collider = colObj.GetOwnerCollider()->GetCollider();
			return pInstance.GetCollisionFilter().Collides(collider.GetCollisionFilter())
				&& ! pInstance.HasIgnoreCollider(&collider);
			
		}else if(colObj.IsOwnerHTSector()){
			return pInstance.GetCollisionFilter().Collides(colObj.GetOwnerHTSector()
				->GetHeightTerrain()->GetHeightTerrain()->GetCollisionFilter());
		}
		
		return false;
	}
};

bool debpParticleEmitterInstanceType::ParticleTestCollision(sParticle &particle, float elapsed){
	if(! pInstance->GetParentWorld()){
		return true; // happens during loading while warmstarting
	}
	
	// pBullet->LogInfoFormat( "step particle %i: elapsed=%g displacement=(%g,%g,%g)", p, elapsed, displacement.getX(), displacement.getY(), displacement.getZ() );
	const deParticleEmitterType &engType = pInstance->GetParticleEmitter()->GetEmitter()->GetTypeAt(pType);
	debpCollisionWorld &dynamicsWorld = *pInstance->GetParentWorld()->GetDynamicsWorld();
	btVector3 displacement = particle.linearVelocity * elapsed;
	int loop;
	
	// the bullet shpere-box test is quite error-prone. particles keep on falling through
	// no matter what you do. what is the problem here?
	//btSphereShape sphereShape( ( btScalar )decMath::max( engType.GetPhysicsSize(), 0.001f ) );
	btSphereShape sphereShape((btScalar)decMath::max(engType.GetPhysicsSize(), 0.01f));
	
	for(loop=0; loop<5; loop++){
		if(displacement.length2() < 1e-8){
			particle.linearVelocity.setZero();
			break;
		}
		
		const btVector3 rayToWorld = particle.position + displacement;
		
		// TODO use size of particle to do a sphere collision test instead of a ray test
		
		// WARNING bullet has a broken ray-box test implementation using Gjk which has a tendency
		// to miss collisions half of the time. as a quick fix a sweep test is done with
		// a tiny sphere which yields a comparable result but is not prone to the problem
		//debpClosestRayResultCallback rayResult( particle.position, rayToWorld, &collisionFilter );
		//dynamicsWorld.rayTest( particle.position, rayToWorld, rayResult );
		
		//pBullet->LogInfoFormat( "rayTest: pos=(%g,%g,%g) to(%g,%g,%g) time=%g", particle.position.getX(), particle.position.getY(),
		//	particle.position.getZ(), rayToWorld.getX(), rayToWorld.getY(), rayToWorld.getZ(), elapsed );
		
		cClosestParticleCallback rayResult(particle.position, rayToWorld, *pInstance->GetInstance());
		{
		//sphereShape.setUnscaledRadius( ... );
		const btQuaternion btQuaterion((btScalar)0.0, (btScalar)0.0, (btScalar)0.0, (btScalar)1.0);
		const btTransform btTransformFrom(btQuaterion, particle.position);
		const btTransform btTransformTo(btQuaterion, rayToWorld);
		
		dynamicsWorld.convexSweepTest(&sphereShape, btTransformFrom, btTransformTo, rayResult, BT_ZERO);
		}
		
		if(! rayResult.hasHit()){
			particle.position += displacement;
			//pBullet->LogInfoFormat( "no hit: pos=(%g,%g,%g)", particle.position.getX(), particle.position.getY(), particle.position.getZ() );
			break;
		}
		
		//const debpCollisionObject &colObj = *( ( debpCollisionObject* )rayResult.m_collisionObject->getUserPointer() );
		// determine if particles have to be emitted
		float particleLinearVelocity = 0.0f;
		bool doEmitParticles = false;
		
		if(engType.GetCollisionEmitter()){
			particleLinearVelocity = (float)particle.linearVelocity.length();
			particleLinearVelocity *= particle.elasticity;
			
			if(particleLinearVelocity * particle.mass > engType.GetEmitMinImpulse()){
				// sanity check to avoid dead-loops due to an emitter without emit-burst set as these would live forever
				if(engType.GetCollisionEmitter()->GetEmitBurst()){
					doEmitParticles = true;
				}
			}
		}
		
		// determine collision response
		deParticleEmitterType::eCollisionResponses collisionResponse = engType.GetCollisionResponse();
		
		if(collisionResponse != deParticleEmitterType::ecrDestroy || doEmitParticles){
			particle.position += displacement * rayResult.m_closestHitFraction;
			particle.position += rayResult.m_hitNormalWorld * (btScalar)0.0001; // prevent falling through
			displacement *= (btScalar)1 - rayResult.m_closestHitFraction;
		}
		
		if(collisionResponse == deParticleEmitterType::ecrCustom){
			//const debpCollisionObject &colObj = *( ( debpCollisionObject* )rayResult.m_collisionObject->getUserPointer() );
			const debpCollisionObject &colObj = *((debpCollisionObject*)rayResult.m_hitCollisionObject->getUserPointer());
			deCollisionInfo &cinfo = *pInstance->GetParentWorld()->GetCollisionInfo();
			
			if(colObj.IsOwnerCollider()){
				cinfo.SetCollider(&colObj.GetOwnerCollider()->GetCollider(), -1, -1, -1);
				/*
				if(collider->IsVolume()){
					
				}else if(collider->IsComponent()){
					
				}else if(collider->IsRigged()){
				}
				*/
				
			}else if(colObj.IsOwnerHTSector()){ // height terrain sector
				deHeightTerrain * const heightTerrain = colObj.GetOwnerHTSector()->GetHeightTerrain()->GetHeightTerrain();
				
				cinfo.SetHTSector(heightTerrain, NULL);
			}
			
			const decVector cinormal(
				(float)rayResult.m_hitNormalWorld.getX(),
				(float)rayResult.m_hitNormalWorld.getY(),
				(float)rayResult.m_hitNormalWorld.getZ());
			const decDVector ciposition(
				(double)particle.position.getX(),
				(double)particle.position.getY(),
				(double)particle.position.getZ());
			const decVector civelocity(
				(float)particle.linearVelocity.getX(),
				(float)particle.linearVelocity.getY(),
				(float)particle.linearVelocity.getZ());
			
			cinfo.SetNormal(cinormal);
			cinfo.SetDistance((float)(elapsed * ((btScalar)1.0 - rayResult.m_closestHitFraction)));
			cinfo.SetParticleLifetime(particle.lifetime);
			cinfo.SetParticleMass(particle.mass);
			cinfo.SetParticlePosition(ciposition);
			cinfo.SetParticleVelocity(civelocity);
			cinfo.SetParticleResponse(deParticleEmitterType::ecrDestroy);
			
			pInstance->GetInstance()->CollisionResponse(&cinfo);
			
			collisionResponse = cinfo.GetParticleResponse();
		}
		
		if(collisionResponse == deParticleEmitterType::ecrPhysical || doEmitParticles){
			displacement -= rayResult.m_hitNormalWorld * (rayResult.m_hitNormalWorld.dot(displacement) * (btScalar)2);
			// TODO roughness
			displacement *= particle.elasticity;
		}
		
		// create an emitter instance if one is set for this particle type and the preconditions are met
		if(doEmitParticles){
			deParticleEmitter * const emitEmitter = engType.GetCollisionEmitter();
			deEngine &engine = *pInstance->GetBullet()->GetGameEngine();
			deParticleEmitterInstance *emitInstance = NULL;
			
			try{
				// create instance
				emitInstance = engine.GetParticleEmitterInstanceManager()->CreateInstance();
				emitInstance->SetEmitter(emitEmitter);
				emitInstance->SetCollisionFilter(pInstance->GetInstance()->GetCollisionFilter());
				emitInstance->SetTimeScale(1.0f);
				emitInstance->SetRemoveAfterLastParticleDied(true);
				emitInstance->SetEnableCasting(true);
				
				// set position and orientation
				const btVector3 emitPosition = rayResult.m_hitPointWorld + rayResult.m_hitNormalWorld * (btScalar)0.001;
				emitInstance->SetPosition(decDVector(emitPosition.getX(), emitPosition.getY(), emitPosition.getZ()));
				emitInstance->SetReferencePosition(emitInstance->GetPosition());
				btVector3 emitNormal;
				
				const debpParticleEmitterType &type = pInstance->GetParticleEmitter()->GetTypeAt(pType);
				const float emitDirection = particle.castEmitDirection * type.EvaluateProgressParameter(
					debpParticleEmitterType::escEmitDirection, particle.lifetime);
				
				if(emitDirection < FLOAT_SAFE_EPSILON){
					emitNormal = rayResult.m_hitNormalWorld;
					
				}else if(emitDirection > 1.0f - FLOAT_SAFE_EPSILON){
					emitNormal = displacement.normalized();
					
				}else{
					emitNormal = displacement.normalized() * (btScalar)emitDirection
						+ rayResult.m_hitNormalWorld * (btScalar)(1.0f - emitDirection);
				}
				
				if(emitNormal.getY() > 1.0 - DVECTOR_THRESHOLD){
					//emitInstance->SetOrientation( decMatrix::CreateRotationX( HALF_PI ).ToQuaternion() );
					emitInstance->SetOrientation(decQuaternion(-0.707107f, 0.0f, 0.0f, 0.707107f));
					
				}else if(emitNormal.getY() < DVECTOR_THRESHOLD - 1.0){
					//emitInstance->SetOrientation( decMatrix::CreateRotationX( -HALF_PI ).ToQuaternion() );
					emitInstance->SetOrientation(decQuaternion(0.707107f, 0.0f, 0.0f, 0.707107f));
					
				}else{
					emitInstance->SetOrientation(decMatrix::CreateVU(decVector((float)emitNormal.getX(),
						(float)emitNormal.getY(), (float)emitNormal.getZ()), decVector(0.0f, 1.0f, 0.0f)).ToQuaternion());
				}
				
				// set controller values
				ParticleSetEmitterControllers(particle, *emitInstance, particleLinearVelocity);
				
				// add to the world. this has to come before casting just to be safe
				pInstance->GetParentWorld()->GetWorld().AddParticleEmitter(emitInstance);
				
				// cast a burst of particles
				emitInstance->FreeReference();
				
			}catch(const deException &){
				if(emitInstance){
					emitInstance->FreeReference();
				}
				throw;
			}
		}
		
		// apply collision response
		switch(collisionResponse){
		case deParticleEmitterType::ecrPhysical:
			particle.linearVelocity = displacement / elapsed;
			elapsed *= 1.0f - (float)rayResult.m_closestHitFraction;
			break;
			
		case deParticleEmitterType::ecrCustom:{
			deCollisionInfo &cinfo = *pInstance->GetParentWorld()->GetCollisionInfo();
			const decDVector &ciposition = cinfo.GetParticlePosition();
			const decVector &civelocity = cinfo.GetParticleVelocity();
			
			particle.position.setValue((btScalar)ciposition.x, (btScalar)ciposition.y, (btScalar)ciposition.z);
			particle.linearVelocity.setValue((btScalar)civelocity.x, (btScalar)civelocity.y, (btScalar)civelocity.z);
			elapsed *= 1.0f - (float)rayResult.m_closestHitFraction;
			}break;
			
		case deParticleEmitterType::ecrDestroy:
			return false; // kill the particle
		}
	}
	
	return true;
}

void debpParticleEmitterInstanceType::ParticleSetEmitterControllers(const sParticle &particle,
deParticleEmitterInstance &instance, float linearVelocity){
	const deParticleEmitterType &engType = pInstance->GetParticleEmitter()->GetEmitter()->GetTypeAt(pType);
	int controllerIndex;
	
	controllerIndex = engType.GetEmitController(deParticleEmitterType::eecLifetime);
	if(controllerIndex != -1){
		instance.GetControllerAt(controllerIndex).SetValue(particle.lifetime);
		instance.NotifyControllerChangedAt(controllerIndex);
	}
	
	controllerIndex = engType.GetEmitController(deParticleEmitterType::eecMass);
	if(controllerIndex != -1){
		instance.GetControllerAt(controllerIndex).SetValue(particle.mass);
		instance.NotifyControllerChangedAt(controllerIndex);
	}
	
	controllerIndex = engType.GetEmitController(deParticleEmitterType::eecLinearVelocity);
	if(controllerIndex != -1){
		instance.GetControllerAt(controllerIndex).SetValue(linearVelocity);
		instance.NotifyControllerChangedAt(controllerIndex);
	}
	
	controllerIndex = engType.GetEmitController(deParticleEmitterType::eecAngularVelocity);
	if(controllerIndex != -1){
		instance.GetControllerAt(controllerIndex).SetValue(particle.angularVelocity);
		instance.NotifyControllerChangedAt(controllerIndex);
	}
}

void debpParticleEmitterInstanceType::ParticleUpdateTrailEmitter(const sParticle &particle){
	if(! particle.trailEmitter){
		return;
	}
	
	btVector3 direction(-particle.linearVelocity);
	
	// set position and orientation
	particle.trailEmitter->SetPosition(decDVector(particle.position.getX(), particle.position.getY(), particle.position.getZ()));
	
	// set orientation only if the linear velocity is not zero. otherwise keep the old orientation
	if(direction.length() > 0.001){
		direction.normalize();
		
		if(direction.getY() > 0.999){
			particle.trailEmitter->SetOrientation(decQuaternion(-0.707107f, 0.0f, 0.0f, 0.707107f));
			
		}else if(direction.getY() < -0.999){
			particle.trailEmitter->SetOrientation(decQuaternion(0.707107f, 0.0f, 0.0f, 0.707107f));
			
		}else{
			particle.trailEmitter->SetOrientation(decDMatrix::CreateVU(decDVector(direction.getX(),
				direction.getY(), direction.getZ()), decDVector(0.0, 1.0, 0.0)).ToQuaternion());
		}
	}
	
	// set controller values
	ParticleSetTrailEmitterControllers(particle, *particle.trailEmitter, particle.linearVelocity.length());
}

void debpParticleEmitterInstanceType::ParticleSetTrailEmitterControllers(const sParticle &particle,
deParticleEmitterInstance &instance, float linearVelocity){
	const deParticleEmitterType &engType = pInstance->GetParticleEmitter()->GetEmitter()->GetTypeAt(pType);
	int controllerIndex;
	
	controllerIndex = engType.GetTrailController(deParticleEmitterType::eecLifetime);
	if(controllerIndex != -1){
		instance.GetControllerAt(controllerIndex).SetValue(particle.lifetime);
		instance.NotifyControllerChangedAt(controllerIndex);
	}
	
	controllerIndex = engType.GetTrailController(deParticleEmitterType::eecMass);
	if(controllerIndex != -1){
		instance.GetControllerAt(controllerIndex).SetValue(particle.mass);
		instance.NotifyControllerChangedAt(controllerIndex);
	}
	
	controllerIndex = engType.GetTrailController(deParticleEmitterType::eecLinearVelocity);
	if(controllerIndex != -1){
		instance.GetControllerAt(controllerIndex).SetValue(linearVelocity);
		instance.NotifyControllerChangedAt(controllerIndex);
	}
	
	controllerIndex = engType.GetTrailController(deParticleEmitterType::eecAngularVelocity);
	if(controllerIndex != -1){
		instance.GetControllerAt(controllerIndex).SetValue(particle.angularVelocity);
		instance.NotifyControllerChangedAt(controllerIndex);
	}
}



// Private Functions
//////////////////////

void debpParticleEmitterInstanceType::pCleanUp(){
	KillAllParticles();
	
	if(pGraParticles){
		delete [] pGraParticles;
	}
	if(pParticles){
		delete [] pParticles;
	}
}
