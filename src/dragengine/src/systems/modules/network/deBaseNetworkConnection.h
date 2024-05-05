/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEBASENETWORKCONNECTION_H_
#define _DEBASENETWORKCONNECTION_H_

#include "../../../dragengine_export.h"


class deNetworkMessage;
class deNetworkState;


/**
 * \brief Network Pipe Module Connection Peer.
 */
class DE_DLL_EXPORT deBaseNetworkConnection{
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
