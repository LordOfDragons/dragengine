/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEBASEPHYSICSMODULE_H_
#define _DEBASEPHYSICSMODULE_H_

#include "../deBaseModule.h"

class deBasePhysicsComponent;
class deBasePhysicsModel;
class deBasePhysicsSkin;
class deBasePhysicsRig;
class deBasePhysicsWorld;
class deBasePhysicsCollider;
class deBasePhysicsDecal;
class deBasePhysicsTouchSensor;
class deBasePhysicsHeightTerrain;
class deBasePhysicsPropField;
class deBasePhysicsForceField;
class deBasePhysicsParticleEmitter;
class deBasePhysicsParticleEmitterInstance;
class deBasePhysicsSmokeEmitter;
class deModel;
class deSkin;
class deRig;
class deWorld;
class deDecal;
class deComponent;
class deCollider;
class deTouchSensor;
class deHeightTerrain;
class dePropField;
class deForceField;
class deParticleEmitter;
class deParticleEmitterInstance;
class deSmokeEmitter;


/**
 * \brief Base Physics Module.
 * 
 * The physics module provides physical simulations on colliders.
 */
class deBasePhysicsModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new module. */
	deBasePhysicsModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBasePhysicsModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initialize physics module. */
	virtual bool Init() = 0;
	
	/** \brief Shut down physics module and cleans up. */
	virtual void CleanUp() = 0;
	
	/**
	 * \brief Frame-per-second rate averaged over the last couple of frames.
	 * \version 1.6
	 * 
	 * Returns 0 if module is not using a separate thread.
	 */
	virtual int GetFPSRate();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create peer for the given component object. */
	virtual deBasePhysicsComponent *CreateComponent( deComponent *comp ) = 0;
	
	/** \brief Create peer for the given model object. */
	virtual deBasePhysicsModel *CreateModel( deModel *model ) = 0;
	
	/** \brief Create peer for the given skin object. */
	virtual deBasePhysicsSkin *CreateSkin( deSkin *skin ) = 0;
	
	/** \brief Create peer for the given rig object. */
	virtual deBasePhysicsRig *CreateRig( deRig *rig ) = 0;
	
	/** \brief Create peer for the given world object. */
	virtual deBasePhysicsWorld *CreateWorld( deWorld *world ) = 0;
	
	/** \brief Create peer for the given collider. */
	virtual deBasePhysicsCollider *CreateCollider( deCollider *collider ) = 0;
	
	/** \brief Create peer for the given decal. */
	virtual deBasePhysicsDecal *CreateDecal( deDecal *decal ) = 0;
	
	/** \brief Create peer for the given touch sensor. */
	virtual deBasePhysicsTouchSensor *CreateTouchSensor( deTouchSensor *touchSensor ) = 0;
	
	/** \brief Create peer for the given height terrain. */
	virtual deBasePhysicsHeightTerrain *CreateHeightTerrain( deHeightTerrain *heightTerrain ) = 0;
	
	/** \brief Create peer for the given prop field. */
	virtual deBasePhysicsPropField *CreatePropField( dePropField *propField ) = 0;
	
	/** \brief Create peer for the given force field. */
	virtual deBasePhysicsForceField *CreateForceField( deForceField *forceField ) = 0;
	
	/** \brief Create peer for the given particle emitter. */
	virtual deBasePhysicsParticleEmitter *CreateParticleEmitter( deParticleEmitter *emitter ) = 0;
	
	/** \brief Create peer for the given particle emitter instance. */
	virtual deBasePhysicsParticleEmitterInstance *CreateParticleEmitterInstance(
		deParticleEmitterInstance *instance ) = 0;
	
	/** \brief Create peer for the given smoke emitter. */
	virtual deBasePhysicsSmokeEmitter *CreateSmokeEmitter( deSmokeEmitter *smokeEmitter ) = 0;
	/*@}*/
};

#endif
