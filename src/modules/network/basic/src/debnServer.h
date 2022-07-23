/* 
 * Drag[en]gine Basic Network Module
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

#ifndef _DEBNSERVER_H_
#define _DEBNSERVER_H_

#include <dragengine/systems/modules/network/deBaseNetworkServer.h>

class deServer;
class debnAddress;
class deNetworkBasic;
class debnSocket;
class decBaseFileReader;



/**
 * \brief Server peer class.
 */
class debnServer : public deBaseNetworkServer{
private:
	deNetworkBasic *pNetBasic;
	deServer *pServer;
	
	debnSocket *pSocket;
	bool pListening;
	
	debnServer *pPreviousServer;
	debnServer *pNextServer;
	bool pIsRegistered;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create server. */
	debnServer( deNetworkBasic *netBasic, deServer *server );
	
	/** \brief Clean up server object. */
	virtual ~debnServer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Socket. */
	inline debnSocket *GetSocket() const{ return pSocket; }
	
	/** \brief Process connection request. */
	void ProcessConnectionRequest( debnAddress &address, decBaseFileReader &reader );
	
	/**
	 * \brief Start listening on address for incoming connections.
	 */
	virtual bool ListenOn( const char *address );
	
	/** \brief Stop listening. */
	virtual void StopListening();
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Previous server. */
	inline debnServer *GetPreviousServer() const{ return pPreviousServer; }
	
	/** \brief Set previous server. */
	void SetPreviousServer( debnServer *server );
	
	/** \brief Next server. */
	inline debnServer *GetNextServer() const{ return pNextServer; }
	
	/** \brief Set next server. */
	void SetNextServer( debnServer *server );
	
	/** \brief Server is registered. */
	inline bool GetIsRegistered() const{ return pIsRegistered; }
	
	/** \brief Set if server is registered. */
	void SetIsRegistered( bool isRegistered );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
