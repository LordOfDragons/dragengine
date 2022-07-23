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

#ifndef _DEBASENETWORKSERVER_H_
#define _DEBASENETWORKSERVER_H_

#include "../../../dragengine_export.h"

class deConnection;
class deNetworkState;


/**
 * \brief Network Module Server Peer.
 */
class DE_DLL_EXPORT deBaseNetworkServer{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseNetworkServer();
	
	/** \brief Clean up peer. */
	virtual ~deBaseNetworkServer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Start listening on address for incoming connections.
	 * 
	 * The format of the address is specific to the network module.
	 * Returns true if listening or false if something went wrong.
	 */
	virtual bool ListenOn( const char *address );
	
	/** \brief Stop listening. */
	virtual void StopListening();
	/*@}*/
};

#endif
