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

#ifndef _DEPHYSICSSYSTEM_H_
#define _DEPHYSICSSYSTEM_H_

#include "deBaseSystem.h"

class deLoadableModule;
class deBasePhysicsModule;
class deComponent;
class deDecal;
class deModel;
class deSkin;
class deRig;
class deWorld;
class deCollider;
class deTouchSensor;
class deHeightTerrain;
class dePropField;
class deForceField;
class deParticleEmitter;
class deParticleEmitterInstance;
class deSmokeEmitter;


/**
 * \brief Physics System.
 *
 * Provides physical simulation support.
 */
class dePhysicsSystem : public deBaseSystem{
private:
	deBasePhysicsModule *pActiveModule;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new physics system linked to the given engine. */
	dePhysicsSystem( deEngine *engine );
	
	/** \brief Clean up physics system. */
	~dePhysicsSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline deBasePhysicsModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Create peer object for the given component using the active module and assigns it. */
	void LoadComponent( deComponent *component );
	
	/** \brief Create peer object for the given model using the active module and assigns it. */
	void LoadModel( deModel *model );
	
	/** \brief Create peer object for the given skin using the active module and assigns it. */
	void LoadSkin( deSkin *skin );
	
	/** \brief Create peer object for the given rig using the active module and assigns it. */
	void LoadRig( deRig *rig );
	
	/** \brief Create peer object for the given world using the active module and assigns it. */
	void LoadWorld( deWorld *world );
	
	/** \brief Create peer object for the given collider using the active module and assigns it. */
	void LoadCollider( deCollider *collider );
	
	/** \brief Create peer object for the given decal using the active module and assigns it. */
	void LoadDecal( deDecal *decal );
	
	/** \brief Create peer object for the given touch sensor using the active module and assigns it. */
	void LoadTouchSensor( deTouchSensor *touchSensor );
	
	/** \brief Create peer object for the given height terrain using the active module and assigns it. */
	void LoadHeightTerrain( deHeightTerrain *heightTerrain );
	
	/** \brief Create peer object for the given prop field using the active module and assigns it. */
	void LoadPropField( dePropField *propField );
	
	/** \brief Create peer object for the given force field using the active module and assigns it. */
	void LoadForceField( deForceField *forceField );
	
	/** \brief Create peer for the particle emitter using the active module and assigns it. */
	void LoadParticleEmitter( deParticleEmitter *emitter );
	
	/** \brief Create peer for the particle emitter instance using the active module and assigns it. */
	void LoadParticleEmitterInstance( deParticleEmitterInstance *instance );
	
	/** \brief Create peer for the smoke emitter using the active module and assigns it. */
	void LoadSmokeEmitter( deSmokeEmitter *smokeEmitter );
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void SetActiveModule( deLoadableModule *module );
	
	/**
	 * \brief Clearcross references and links that could lead to memory leaks.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void ClearPermanents();
	
	/** \brief Carry out here actions right after the system started up. */
	virtual void PostStart();
	
	/** \brief Carry out here actions right before the system shuts down. */
	virtual void PreStop();
	/*@}*/
};

#endif
