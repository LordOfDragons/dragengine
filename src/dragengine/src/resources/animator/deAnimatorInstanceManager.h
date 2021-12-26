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

#ifndef _DEANIMATORINSTANCEMANAGER_H_
#define _DEANIMATORINSTANCEMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deAnimatorInstance;


/**
 * \brief Animator Instance Resource Manager.
 */
class DE_DLL_EXPORT deAnimatorInstanceManager : public deResourceManager{
private:
	deResourceList pInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator instance resource manager linked to the given engine. */
	deAnimatorInstanceManager( deEngine *engine );
	
	/** \brief Clean up animator instance resource manager and reports leaking resources. */
	virtual ~deAnimatorInstanceManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of animator instance resource. */
	int GetAnimatorInstanceCount() const;
	
	/** \brief Root animator instance resource for iteration purpose. */
	deAnimatorInstance *GetRootAnimatorInstance() const;
	
	/** \brief Create new animator instance object. */
	deAnimatorInstance *CreateAnimatorInstance();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Animator System Peers of all stored resources have to be created. */
	virtual void SystemAnimatorLoad();
	
	/** \brief Animator System Peers of all stored resources have to be freed. */
	virtual void SystemAnimatorUnload();
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
