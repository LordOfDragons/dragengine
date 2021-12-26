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

#ifndef _DENAVIGATORMANAGER_H_
#define _DENAVIGATORMANAGER_H_

#include "../../deResourceManager.h"
#include "../../deResourceList.h"

class deNavigator;


/**
 * \brief Navigator Manager.
 */
class DE_DLL_EXPORT deNavigatorManager : public deResourceManager{
private:
	deResourceList pNavigators;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new manager. */
	deNavigatorManager( deEngine *engine );
	
	/** \brief Clean up manager. */
	virtual ~deNavigatorManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of navigators. */
	int GetNavigatorCount() const;
	
	/** \brief Root navigator resource for iteration purpose. */
	deNavigator *GetRootNavigator() const;
	
	/** \brief Create new navigator. */
	deNavigator *CreateNavigator();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief AI System Peers of all stored resources have to be created. */
	virtual void SystemAILoad();
	
	/** \brief AI System Peers of all stored resources have to be freed. */
	virtual void SystemAIUnload();
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
