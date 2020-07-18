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

#ifndef _DEBASENETWORKCONNECTION_H_
#define _DEBASENETWORKCONNECTION_H_


class deNetworkMessage;
class deNetworkState;


/**
 * \brief Network Pipe Module Connection Peer.
 */
class deBaseNetworkConnection{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseNetworkConnection();
	
	/** \brief Clean up peer. */
	virtual ~deBaseNetworkConnection();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Connect to connection object on host at address.
	 * 
	 * Returns true if the connection request has been send or false if an
	 * error occurred. Connection success or failure is reported through the
	 * scripting peer. The format of the address is specific to the network
	 * module.
	 */
	virtual bool ConnectTo( const char *address );
	
	/**
	 * \brief Disconnect from remote connection if connected.
	 */
	virtual void Disconnect();
	
	/**
	 * \brief Send message to remote connection if connected.
	 * 
	 * The message can be queued and send at a later time to optimize throughput.
	 * The message  will be not delayed longer than the given amount of milliseconds.
	 * The message is send unreliable and it is acceptable for the message to get
	 * lost due to transmission failure.
	 */
	virtual void SendMessage( deNetworkMessage *message, int maxDelay );
	
	/**
	 * \brief Send reliable message to remote connection if connected.
	 * 
	 * The message is append to already waiting reliable messages and send as
	 * soon as possible. Reliable messages always arrive in the same order
	 * they have been queued.
	 */
	virtual void SendReliableMessage( deNetworkMessage *message );
	
	/**
	 * \brief Link network state to remote network state.
	 * 
	 * The message contains information for the remote system to know what state to link to.
	 * The request is queued and carried out as soon as possible. The local state
	 * is considered the master state and the remote state the slave state.
	 * By default only the master state can apply changes.
	 */
	virtual void LinkState( deNetworkMessage *message, deNetworkState *state, bool readOnly );
	/*@}*/
};

#endif
