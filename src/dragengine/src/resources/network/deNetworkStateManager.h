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

#ifndef _DENETWORKSTATEMANAGER_H_
#define _DENETWORKSTATEMANAGER_H_ 

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deNetworkState;


/**
 * \brief Network State Resource Manager.
 */
class DE_DLL_EXPORT deNetworkStateManager : public deResourceManager{
private:
	deResourceList pStates;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network state resource manager linked to the given engine. */
	deNetworkStateManager( deEngine *engine );
	
	/** \brief Clean up network state resource manager and reports leaking resources. */
	virtual ~deNetworkStateManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of network states. */
	int GetNetworkStateCount() const;
	
	/** \brief Root network state resource for iteration purpose. */
	deNetworkState *GetRootNetworkState() const;
	
	/** \brief Create network state. */
	deNetworkState *CreateState( bool readOnly );
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Network System Peers of all stored resources have to be created. */
	virtual void SystemNetworkLoad();
	
	/** \brief Network System Peers of all stored resources have to be freed. */
	virtual void SystemNetworkUnload();
	
	/** \brief Scripting System Peers of all stored resources have to be created. */
	virtual void SystemScriptingLoad();
	
	/** \brief Scripting System Peers of all stored resources have to be freed. */
	virtual void SystemScriptingUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * \warning For use by deResource only.
	 */
	/*@{*/
	/** \brief Removes the given resource from the manager without freeing it. */
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
