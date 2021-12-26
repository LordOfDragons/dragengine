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

#ifndef _DECOMPONENTMANAGER_H_
#define _DECOMPONENTMANAGER_H_ 

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deComponent;
class deModel;
class deSkin;


/**
 * \brief Scene Component Resource Manager.
 */
class DE_DLL_EXPORT deComponentManager : public deResourceManager{
private:
	deResourceList pComponents;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new scene component resource manager linked to the given engine. */
	deComponentManager( deEngine *engine );
	
	/** \brief Clean up scene component resource manager and reports leaking resources. */
	virtual ~deComponentManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of scene components. */
	int GetComponentCount() const;
	
	/** \brief Root component resource for iteration purpose. */
	deComponent *GetRootComponent() const;
	
	/** \brief Create component. */
	deComponent *CreateComponent( deModel *model = NULL, deSkin *skin = NULL );
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemGraphicLoad();
	void SystemGraphicUnload();
	void SystemPhysicsLoad();
	void SystemPhysicsUnload();
	void SystemAudioLoad();
	void SystemAudioUnload();
	
	/** \brief Animator system peers of all stored resources have to be created. */
	virtual void SystemAnimatorLoad();
	
	/** \brief Animator system seers of all stored resources have to be freed. */
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
