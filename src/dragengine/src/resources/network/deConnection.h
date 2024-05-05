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
class DE_DLL_EXPORT deConnection : public deResource{
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
