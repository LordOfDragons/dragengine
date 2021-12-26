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

#ifndef _DEBASENETWORKMODULE_H_
#define _DEBASENETWORKMODULE_H_

#include "../deBaseModule.h"

class deBaseNetworkWorld;
class deBaseNetworkServer;
class deBaseNetworkState;
class deBaseNetworkConnection;
class deWorld;
class deServer;
class deConnection;
class deNetworkState;


/**
 * \brief Base Network Module.
 */
class DE_DLL_EXPORT deBaseNetworkModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseNetworkModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseNetworkModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Called to init the module.
	 * 
	 * Returns true on success or false otherwise.
	 */
	virtual bool Init() = 0;
	
	/**
	 * \brief Called to cleanup the module.
	 * 
	 * All resources have to be freed and running threads stopped or killed if needed.
	 */
	virtual void CleanUp() = 0;
	
	/** \brief Process network. */
	virtual void ProcessNetwork() = 0;
	
	/**
	 * \brief Frame-per-second rate averaged over the last couple of frames.
	 * \version 1.6
	 * 
	 * Returns 0 if module is not using a separate thread.
	 */
	virtual int GetFPSRate();
	/*@}*/
	
	
	
	/** \name Frame Management */
	/*@{*/
	/** \brief Create peer for world. */
	virtual deBaseNetworkWorld *CreateWorld( deWorld *world ) = 0;
	
	/** \brief Create peer for server. */
	virtual deBaseNetworkServer *CreateServer( deServer *server ) = 0;
	
	/** \brief Create peer for connection. */
	virtual deBaseNetworkConnection *CreateConnection( deConnection *connection ) = 0;
	
	/** \brief Create peer for state. */
	virtual deBaseNetworkState *CreateState( deNetworkState *state ) = 0;
	/*@}*/
};

#endif
