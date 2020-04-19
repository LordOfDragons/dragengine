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

#ifndef _DEBPPROPPARTICLEEMITTERINSTANCETYPE_H_
#define _DEBPPROPPARTICLEEMITTERINSTANCETYPE_H_

#include "LinearMath/btVector3.h"

#include <dragengine/resources/particle/deParticleEmitterInstanceType.h>

class debpForceField;
class debpParticleEmitterInstance;
class debpComponent;
class deParticleEmitterInstance;



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
		
		deParticleEmitterInstance *trailEmitter;
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
	void SetInstance( debpParticleEmitterInstance *instance );
	/** Retrieves the type number. */
	inline int GetType() const{ return pType; }
	/** Sets the type number. */
	void SetType( int type );
	
	/** Retrieves the particles. */
	inline sParticle *GetParticles() const{ return pParticles; }
	/** Retrieves the number of particles. */
	inline int GetParticlesCount() const{ return pParticleCount; }
	
	/** Prepare stepping. */
	void PrepareParticles( float elapsed, float travelledDistance );
	/** Applies forces caused by a force field. */
	void ApplyForceField( const debpForceField &forceField, float elapsed );
	/** Steps the particles. */
	void StepParticles( float elapsed );
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
	void CastParticle( float distance, float timeOffset );
	/** Kill a particle. */
	void KillParticle( int index );
	/** Kill all particles. */
	void KillAllParticles();
	
	/** \brief Cast single particle. */
	void CastSingleParticle( float distance, float timeOffset );
	
	/** \brief Cast beam particle. */
	void CastBeamParticle( float distance );
	
	/** Set the cast values of a particle. */
	void ParticleSetCastParams( sParticle &particle, float distance, float timeOffset );
	/** Calculate for a particle the cast matrix. */
	void ParticleCastMatrix( decDMatrix &matrix );
	/** Create trail emitter for a particle. */
	void ParticleCreateTrailEmitter( sParticle &particle );
	/** Set particle progress parameters for a point in time from cast parameters using the particle lifetime value. */
	void ParticleSetProgressParams( sParticle &particle );
	
	/** Simulate particle. Returns false if the particle has to be killed due to a collision or true to keep it alive. */
	bool ParticleSimulate( sParticle &particle, float elapsed );
	/** Test for particle collision. Returns false if the particle has to be killed due to a collision or true to keep it alive. */
	bool ParticleTestCollision( sParticle &particle, float elapsed );
	/** Set controllers of a trail or impact emitter. */
	void ParticleSetEmitterControllers( const sParticle &particle,
		deParticleEmitterInstance &instance, float linearVelocity );
	/** Update particle trail emitter if existing. */
	void ParticleUpdateTrailEmitter( const sParticle &particle );
	/** Set controllers of a trail or impact emitter. */
	void ParticleSetTrailEmitterControllers( const sParticle &particle,
		deParticleEmitterInstance &instance, float linearVelocity );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
