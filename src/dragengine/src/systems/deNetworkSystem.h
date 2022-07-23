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

#ifndef _DENETWORKSYSTEM_H_
#define _DENETWORKSYSTEM_H_

#include "deBaseSystem.h"

class deLoadableModule;
class deBaseNetworkModule;
class deWorld;
class deServer;
class deConnection;
class deNetworkState;


/**
 * \brief Network System.
 *
 * Provides network communication support..
 */
class DE_DLL_EXPORT deNetworkSystem : public deBaseSystem{
private:
	deBaseNetworkModule *pActiveModule;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new network system linked to the given engine. */
	deNetworkSystem( deEngine *engine );
	
	/** \brief Clean up network system. */
	~deNetworkSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline deBaseNetworkModule *GetActiveModule() const{ return pActiveModule; }
	/*@}*/
	
	
	
	/**
	 * \name Runtime Functions
	 * Run-Time functions can only be called if the audio system
	 * is running. Otherwise those functions cause an exception.
	 */
	/*@{*/
	/** \brief Process network. */
	void ProcessNetwork();
	
	/** \brief Create peer object for the given server using the active module and assigns it. */
	void LoadServer( deServer *server );
	
	/** \brief Create peer object for the given connection using the active module and assigns it. */
	void LoadConnection( deConnection *connection );
	
	/** \brief Create peer object for the given state using the active module and assigns it. */
	void LoadState( deNetworkState *state );
	
	/** \brief Create peer object for the given world using the active module and assigns it. */
	void LoadWorld( deWorld *world );
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
