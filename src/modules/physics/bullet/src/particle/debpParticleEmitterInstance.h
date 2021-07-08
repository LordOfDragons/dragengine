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

#ifndef _DEBPPROPPARTICLEEMITTERINSTANCE_H_
#define _DEBPPROPPARTICLEEMITTERINSTANCE_H_

#include "LinearMath/btVector3.h"
#include "../forcefield/debpForceFieldFluctuation.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/systems/modules/physics/deBasePhysicsParticleEmitterInstance.h>

class debpWorld;
class debpForceField;
class dePhysicsBullet;
class debpParticleEmitter;
class debpParticleEmitterInstanceType;



/**
 * @brief Particle Emitter.
 */
class debpParticleEmitterInstance : public deBasePhysicsParticleEmitterInstance{
private:
	dePhysicsBullet *pBullet;
	deParticleEmitterInstance *pInstance;
	debpWorld *pParentWorld;
	
	debpParticleEmitterInstanceType *pTypes;
	int pTypeCount;
	
	decDMatrix pEmitterMatrix;
	
	debpForceFieldFluctuation pForceFieldSimulation;
	
	float pBurstTimer;
	
	decDVector pLastPosition;
	
	bool pDirtyEmitterMatrix;
	bool pCheckForLastParticle;
	bool pCanCollide;
	bool pEnsureCastOnce;
	
	
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new peer. */
	debpParticleEmitterInstance( dePhysicsBullet *bullet, deParticleEmitterInstance *instance );
	/** Cleans up the peer. */
	virtual ~debpParticleEmitterInstance();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline dePhysicsBullet *GetBullet() const{ return pBullet; }
	/** Retrieves the particle emitter instance. */
	inline deParticleEmitterInstance *GetInstance() const{ return pInstance; }
	
	/** Retrieves the parent world. */
	inline debpWorld *GetParentWorld() const{ return pParentWorld; }
	/** Sets the parent world. */
	void SetParentWorld( debpWorld *parentWorld );
	
	/** Retrieves the particle emitter or null if not set. */
    debpParticleEmitter *GetParticleEmitter() const;
	
	/** \brief Force field simulation. */
	inline const debpForceFieldFluctuation &GetForceFieldFluctuation() const{ return pForceFieldSimulation; }
	
	/** Retrieves the last position. */
	inline const decDVector &GetLastPosition() const{ return pLastPosition; }
	
	/** Retrieves the number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	/** Retrieves a type by index. */
	debpParticleEmitterInstanceType &GetTypeAt( int index );
	const debpParticleEmitterInstanceType &GetTypeAt( int index ) const;
	
	/** Retrieves the emitter matrix. */
	const decDMatrix &GetEmitterMatrix();
	/** Retrieves the emitter matrix by distance. */
	decDMatrix GetEmitterMatrixByDistance( float distance ) const;
	
	/** Determines if the particles can collide at all. */
	inline bool GetCanCollide() const{ return pCanCollide; }
	
	/** \brief Prepare particles for simulation. */
	void PrepareParticles( float elapsed );
	
	/** \brief Apply forces caused by a force field. */
	void ApplyForceFields( float elapsed );
	
	/** Steps the particles. */
	void StepParticles( float elapsed );
	/** Finish stepping. */
	void FinishStepping();
	
	/** Marks the instance to check at the next chance if all particles have died. */
	void RequestCheckForLastParticle();
	/** Check if the last particle has died and notify the script module if this is the case. */
	void CheckForLastParticle();
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/** Emitter changed. */
	virtual void EmitterChanged();
	/** Position changed. */
	virtual void PositionChanged();
	/** Orientation changed. */
	virtual void OrientationChanged();
	/** Enable casting changed. */
	virtual void EnableCastingChanged();
	/** Warm up time changed. */
	virtual void WarmUpTimeChanged();
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	/** Controller changed. */
	virtual void ControllerChanged( int controller );
	/** Type changed. */
	virtual void TypeChanged( int type );
	
	/** Reset burst particles. */
	virtual void ResetBurst();
	/** Kill all particles. */
	virtual void KillAllParticles();
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateTypes();
	void pSimulateWarpUp();
};

#endif
