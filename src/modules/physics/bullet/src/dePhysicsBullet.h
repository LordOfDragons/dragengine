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

#ifndef _DEPHYSICSBULLETMODULE_H_
#define _DEPHYSICSBULLETMODULE_H_

#include "devmode/debpDeveloperMode.h"
#include "debug/debpDebug.h"

#include <dragengine/systems/modules/physics/deBasePhysicsModule.h>

class debpConfiguration;
class debpCommandExecuter;
class debpParameterList;
class deCollisionInfo;
class debpCollisionDetection;



// physics bullet interface
class dePhysicsBullet : public deBasePhysicsModule{
private:
	debpConfiguration *pConfiguration;
	debpDeveloperMode pDeveloperMode;
	debpCommandExecuter *pCommandExecuter;
	debpParameterList *pParameters;
	
	deCollisionInfo *pColInfo;
	debpCollisionDetection *pCollisionDetection;
	
	debpDebug pDebug;
	
public:
	// constructor, destructor
	dePhysicsBullet( deLoadableModule &loadableModule );
	~dePhysicsBullet();
	// management
	bool Init();
	void CleanUp();
	
	
	
	/** \name Parameters */
	/*@{*/
	/** \brief Number of parameters. */
	virtual int GetParameterCount() const;
	
	/**
	 * \brief Get information about parameter.
	 * \param[in] index Index of the parameter
	 * \param[in] parameter Object to fill with informations about the parameter
	 */
	virtual void GetParameterInfo( int index, deModuleParameter &parameter ) const;
	
	/** \brief Index of named parameter or -1 if not found. */
	virtual int IndexOfParameterNamed( const char *name ) const;
	
	/** \brief Value of named parameter. */
	virtual decString GetParameterValue( const char *name ) const;
	
	/** \brief Set value of named parameter. */
	virtual void SetParameterValue( const char *name, const char *value );
	/*@}*/
	
	
	
	/** @name Debugging */
	/*@{*/
	/**
	 * Sends a command to the module and retrieves an answer from it.
	 * At least the 'help' command has to be understood answering a
	 * list of possible commands. The command is provided in the 'command'
	 * parameter and the answer has to be written into 'answer'. The default
	 * implementation simply answers only to help with itself.
	 */
	virtual void SendCommand( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the shared collision info. */
	inline deCollisionInfo *GetCollisionInfo() const{ return pColInfo; }
	
	/** \brief Collision detection. */
	inline debpCollisionDetection &GetCollisionDetection() const{ return *pCollisionDetection; }
	
	/** Creates a peer for the given component object. */
	virtual deBasePhysicsComponent *CreateComponent( deComponent *comp );
	/** Creates a peer for the given model object. */
	virtual deBasePhysicsModel *CreateModel( deModel *model );
	/** Creates a peer for the given skin object. */
	virtual deBasePhysicsSkin *CreateSkin( deSkin *skin );
	/** Creates a peer for the given rig object. */
	virtual deBasePhysicsRig *CreateRig( deRig *rig );
	/** Creates a peer for the given world object. */
	virtual deBasePhysicsWorld *CreateWorld( deWorld *world );
	/** Creates a peer for the given collider. */
	virtual deBasePhysicsCollider *CreateCollider( deCollider *collider );
	/** Creates a peer for the given decal. */
	virtual deBasePhysicsDecal *CreateDecal( deDecal *decal );
	/** Creates a peer for the given touch sensor. */
	virtual deBasePhysicsTouchSensor *CreateTouchSensor( deTouchSensor *touchSensor );
	/** Creates a peer for the given height terrain. */
	virtual deBasePhysicsHeightTerrain *CreateHeightTerrain( deHeightTerrain *heightTerrain );
	/** Creates a peer for the given prop field. */
	virtual deBasePhysicsPropField *CreatePropField( dePropField *propField );
	/** Creates a peer for the given force field. */
	virtual deBasePhysicsForceField *CreateForceField( deForceField *forceField );
	/** Creates a peer for the given particle emitter. */
	virtual deBasePhysicsParticleEmitter *CreateParticleEmitter( deParticleEmitter *emitter );
	/** Creates a peer for the given particle emitter instance. */
	virtual deBasePhysicsParticleEmitterInstance *CreateParticleEmitterInstance( deParticleEmitterInstance *instance );
	/** Creates a peer for the given smoke emitter. */
	virtual deBasePhysicsSmokeEmitter *CreateSmokeEmitter( deSmokeEmitter *smokeEmitter );
	/*@}*/
	
	/** Retrieves the configuration. */
	inline debpConfiguration *GetConfiguration() const{ return pConfiguration; }
	
	/** \brief Developer mode. */
	inline debpDeveloperMode &GetDeveloperMode(){ return pDeveloperMode; }
	inline const debpDeveloperMode &GetDeveloperMode() const{ return pDeveloperMode; }
	
	inline debpDebug &GetDebug(){ return pDebug; }
	inline const debpDebug &GetDebug() const{ return pDebug; }
};

#endif
