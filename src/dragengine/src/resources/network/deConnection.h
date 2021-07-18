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

#ifndef _DECONNECTION_H_
#define _DECONNECTION_H_

#include <stdint.h>

#include "../deResource.h"
#include "../../common/string/decString.h"

class deConnectionManager;
class deServer;
class deNetworkState;
class deNetworkMessage;
class deBaseNetworkConnection;
class deBaseScriptingConnection;


/**
 * \brief Connection Class.
 *
 * Manages a connection in a network game. Connections are usually made
 * between the player and a server but can also be established between
 * servers themselves or between individual players.
 */
class deConnection : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deConnection> Ref;
	
	
	
private:
	decString pLocalAddress;
	decString pRemoteAddress;
	bool pConnected;
	
	deBaseNetworkConnection *pPeerNetwork;
	deBaseScriptingConnection *pPeerScripting;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create connection object. */
	deConnection( deConnectionManager *manager );
	
protected:
	/**
	 * \brief Clean up connection object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deConnection();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Local address. */
	inline const decString &GetLocalAddress() const{ return pLocalAddress; }
	
	/**
	 * \brief Set local address.
	 * 
	 * Intended to be used by the network module only so do not call it otherwise.
	 */
	void SetLocalAddress( const char *address );
	
	/** \brief Remote address. */
	inline const decString &GetRemoteAddress() const{ return pRemoteAddress; }
	
	/**
	 * \brief Set remote address.
	 * 
	 * Intended to be used by the network module only so do not call otherwise.
	 */
	void SetRemoteAddress( const char *address );
	
	/** \brief Connection to a remote host is established. */
	inline bool GetConnected() const{ return pConnected; }
	
	/**
	 * \brief Set if connection to remote host is established.
	 * 
	 * Intended to be used by the network module only so do not call otherwise.
	 */
	void SetConnected( bool connected );
	
	/**
	 * \brief Connect to connection object on host at address.
	 * 
	 * Returns true if the connection request has been send or false if an
	 * error occurred. Connection success or failure is reported through the
	 * scripting peer. The format of the address is specific to the network
	 * module.
	 */
	bool ConnectTo( const char *address );
	
	/**
	 * \brief Disconnect from remote connection if connected.
	 */
	void Disconnect();
	
	/**
	 * \brief Send message to remote connection if connected.
	 * 
	 * The message can be queued and send at a later time to optimize throughput.
	 * The message will be not delayed longer than the given amount of milliseconds.
	 * The message is send unreliable and it is acceptable for the message to get
	 * lost due to transmission failure.
	 */
	void SendMessage( deNetworkMessage *message, int maxDelay );
	
	/**
	 * \brief Send reliable message to remote connection if connected.
	 * 
	 * The  message is append to already waiting reliable messages and send as
	 * soon as possible. Reliable messages always arrive in the same order
	 * they have been queued.
	 */
	void SendReliableMessage( deNetworkMessage *message );
	
	/**
	 * \brief Link network state to remote network state.
	 * 
	 * The message contains information for the remote system to know what state to link to.
	 * The request is queued and carried out as soon as possible. The local state
	 * is considered the master state and the remote state the slave state.
	 * By default only the master state can apply changes.
	 */
	void LinkState( deNetworkMessage *message, deNetworkState *state, bool readOnly );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Network system peer object. */
	inline deBaseNetworkConnection *GetPeerNetwork() const{ return pPeerNetwork; }
	
	/** \brief Set network system peer object. */
	void SetPeerNetwork( deBaseNetworkConnection *peer );
	
	/** \brief Scripting system peer object. */
	inline deBaseScriptingConnection *GetPeerScripting() const{ return pPeerScripting; }
	
	/** \brief Set scripting system peer object. */
	void SetPeerScripting( deBaseScriptingConnection *peer );
	/*@}*/
};

#endif
