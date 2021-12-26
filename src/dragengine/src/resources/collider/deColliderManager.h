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

#ifndef _DECOLLIDERMANAGER_H_
#define _DECOLLIDERMANAGER_H_ 

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deCollider;
class deColliderVolume;
class deColliderRig;
class deColliderComponent;


/**
 * \brief World Collider Resource Manager.
 *
 */
class DE_DLL_EXPORT deColliderManager : public deResourceManager{
private:
	deResourceList pColliders;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new world collider resource manager linked to the given engine. */
	deColliderManager( deEngine *engine );
	
	/** \brief Clean up world collider resource manager and reports leaking resources. */
	virtual ~deColliderManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of world colliders. */
	int GetColliderCount() const;
	
	/** \brief Root collider resource for iteration purpose. */
	deCollider *GetRootCollider() const;
	
	/** \brief Create new world collider volume. */
	deColliderVolume *CreateColliderVolume();
	
	/** \brief Create new world collider rigged. */
	deColliderRig *CreateColliderRig();
	
	/** \brief Create new world collider component. */
	deColliderComponent *CreateColliderComponent();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemPhysicsLoad();
	void SystemPhysicsUnload();
	void SystemScriptingLoad();
	void SystemScriptingUnload();
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
