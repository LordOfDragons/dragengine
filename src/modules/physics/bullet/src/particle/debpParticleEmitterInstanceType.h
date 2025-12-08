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

#ifndef _DEBPPROPPARTICLEEMITTERINSTANCETYPE_H_
#define _DEBPPROPPARTICLEEMITTERINSTANCETYPE_H_

#include "LinearMath/btVector3.h"

#include <dragengine/resources/particle/deParticleEmitterInstanceType.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>

class debpForceField;
class debpParticleEmitterInstance;
class debpComponent;



/**
 * @brief Particle Emitter Instance Type.
 */
class debpParticleEmitterInstanceType{
public:
	struct sParticle{
		btVector3 position;
		btVector3 linearVelocity;
		btVector3 force;
		float rotation;
		float angularVelocity;
		btVector3 gravity;
		float forceFieldDirect;
		float forceFieldSurface;
		float forceFieldMass;
		float forceFieldSpeed;
		float timeToLive;
		float lifetimeFactor;
		float lifetime;
		float size;
		float mass;
		float brown;
		float damp;
		float drag;
		float elasticity;
		float roughness;
		
		float castSize;
		float castMass;
		float castRotation;
		float castLinearVelocity;
		float castAngularVelocity;
		float castBrown;
		float castDamp;
		float castDrag;
		btVector3 castGravity;
		float castLocalGravity;
		float castForceFieldDirect;
		float castForceFieldSurface;
		float castForceFieldMass;
		float castForceFieldSpeed;
		float castElasticity;
		float castRoughness;
		float castEmitDirection;
		float castEmitParticleCount;
		
		float castEmissivity;
		unsigned char castRed;
		unsigned char castGreen;
		unsigned char castBlue;
		unsigned char castTransparency;
		
		deParticleEmitterInstance::Ref trailEmitter;
	};
	
private:
	debpParticleEmitterInstance *pInstance;
	int pType;
	
	debpComponent *pComponent;
	
	sParticle *pParticles;
	int pParticleCount;
	int pParticleSize;
	
	float pCastIntervalMin;
	float pCastIntervalGap;
	float pNextCastTimer;
	
	float pBurstTimer;
	int pBurstLastCurvePoint;
	
	deParticleEmitterInstanceType::sParticle *pGraParticles;
	int pGraParticleSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new type. */
	debpParticleEmitterInstanceType();
	/** Cleans up the type. */
	~debpParticleEmitterInstanceType();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent instance. */
	inline debpParticleEmitterInstance *GetInstance() const{ return pInstance; }
	/** Sets the parent instance. */
	void SetInstance(debpParticleEmitterInstance *instance);
	/** Retrieves the type number. */
	inline int GetType() const{ return pType; }
	/** Sets the type number. */
	void SetType(int type);
	
	/** Retrieves the particles. */
	inline sParticle *GetParticles() const{ return pParticles; }
	/** Retrieves the number of particles. */
	inline int GetParticlesCount() const{ return pParticleCount; }
	
	/** Prepare stepping. */
	void PrepareParticles(bool casting, float elapsed, float travelledDistance);
	/** Applies forces caused by a force field. */
	void ApplyForceField(const debpForceField &forceField, float elapsed);
	/** Steps the particles. */
	void StepParticles(float elapsed);
	/** Finish stepping. */
	void FinishStepping();
	/** Update graphic particles. */
	void UpdateGraphicParticles();
	/** Update cast intervals. */
	void UpdateCastIntervals();
	
	/** Reset burst. */
	void ResetBurst();
	/** Type changed notification. */
	void OnTypeChanged();
	
	/** Cast a particle. */
	void CastParticle(float distance, float timeOffset);
	/** Kill a particle. */
	void KillParticle(int index);
	/** Kill all particles. */
	void KillAllParticles();
	
	/** \brief Cast single particle. */
	void CastSingleParticle(float distance, float timeOffset);
	
	/** \brief Cast beam particle. */
	void CastBeamParticle(float distance);
	
	/** Set the cast values of a particle. */
	void ParticleSetCastParams(sParticle &particle, float distance, float timeOffset);
	/** Calculate for a particle the cast matrix. */
	void ParticleCastMatrix(decDMatrix &matrix);
	/** Create trail emitter for a particle. */
	void ParticleCreateTrailEmitter(sParticle &particle);
	/** Set particle progress parameters for a point in time from cast parameters using the particle lifetime value. */
	void ParticleSetProgressParams(sParticle &particle);
	
	/** Simulate particle. Returns false if the particle has to be killed due to a collision or true to keep it alive. */
	bool ParticleSimulate(sParticle &particle, float elapsed);
	/** Test for particle collision. Returns false if the particle has to be killed due to a collision or true to keep it alive. */
	bool ParticleTestCollision(sParticle &particle, float elapsed);
	/** Set controllers of a trail or impact emitter. */
	void ParticleSetEmitterControllers(const sParticle &particle,
		deParticleEmitterInstance &instance, float linearVelocity);
	/** Update particle trail emitter if existing. */
	void ParticleUpdateTrailEmitter(const sParticle &particle);
	/** Set controllers of a trail or impact emitter. */
	void ParticleSetTrailEmitterControllers(const sParticle &particle,
		deParticleEmitterInstance &instance, float linearVelocity);
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
