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

#ifndef _DEPARTICLEEMITTERINSTANCEMANAGER_H_
#define _DEPARTICLEEMITTERINSTANCEMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deParticleEmitterInstance;


/**
 * \brief Particle Emitter Instance Manager.
 */
class deParticleEmitterInstanceManager : public deResourceManager{
private:
	deResourceList pInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new resource manager. */
	deParticleEmitterInstanceManager( deEngine *engine );
	
	/** \brief Clean up resource manager and reports leaking resources. */
	virtual ~deParticleEmitterInstanceManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of instances. */
	int GetInstanceCount() const;
	
	/** \brief Root instance resource for iteration purpose. */
	deParticleEmitterInstance *GetRootInstance() const;
	
	/** \brief Create new instance. */
	deParticleEmitterInstance *CreateInstance();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Graphic System Peers of all stored resources have to be created. */
	virtual void SystemGraphicLoad();
	
	/** \brief Graphic System Peers of all stored resources have to be freed. */
	virtual void SystemGraphicUnload();
	
	/** \brief Physics System Peers of all stored resources have to be created. */
	virtual void SystemPhysicsLoad();
	
	/** \brief Physics System Peers of all stored resources have to be freed. */
	virtual void SystemPhysicsUnload();
	
	/** \brief Scripting System Peers of all stored resources have to be created. */
	virtual void SystemScriptingLoad();
	
	/** \brief Scripting System Peers of all stored resources have to be freed. */
	virtual void SystemScriptingUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
