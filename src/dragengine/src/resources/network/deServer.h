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

#ifndef _DESERVER_H_
#define _DESERVER_H_

#include "../deResource.h"
#include "../../common/string/decString.h"

class deServerManager;
class deBaseNetworkServer;
class deBaseScriptingServer;


/**
 * \brief Network Server Class.
 *
 * Manages a server for a network game. The server is a connection point
 * for players connecting to a network game. Every server listens on one
 * address. For connecting players a new connection object is created
 * handling the communication.
 */
class deServer : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deServer> Ref;
	
	
	
private:
	decString pAddress;
	
	deBaseNetworkServer *pPeerNetwork;
	deBaseScriptingServer *pPeerScripting;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create server object. */
	deServer( deServerManager *manager );
	
protected:
	/**
	 * \brief Clean up server object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deServer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Address. */
	inline const decString &GetAddress() const{ return pAddress; }
	
	/**
	 * \brief Set address.
	 * 
	 * Intended to be used by the network module only so  do not call it otherwise.
	 */
	void SetAddress( const char *address );
	
	/**
	 * \brief Start listening on address for incoming connections.
	 * 
	 * The format of the address is specific to the network module.
	 * Returns true if listening or false if something went wrong.
	 */
	bool ListenOn( const char *address );
	
	/** \brief Stop listening. */
	void StopListening();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Network system peer object. */
	inline deBaseNetworkServer *GetPeerNetwork() const{ return pPeerNetwork; }
	
	/** \brief Set network system peer object. */
	void SetPeerNetwork( deBaseNetworkServer *peer );
	
	/** \brief Scripting system peer object. */
	inline deBaseScriptingServer *GetPeerScripting() const{ return pPeerScripting; }
	
	/** \brief Set scripting system peer object. */
	void SetPeerScripting( deBaseScriptingServer *peer );
	/*@}*/
};

#endif
