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

#ifndef _DEBNSOCKET_H_
#define _DEBNSOCKET_H_

#include <stdint.h>

#include "debnAddress.h"

#ifdef OS_W32
#	include <iphlpapi.h>
#endif

#include <dragengine/deObject.h>

class deNetworkBasic;
class deNetworkMessage;



/**
 * Socket class.
 */
class debnSocket : public deObject{
private:
	deNetworkBasic &pNetBasic;
	debnAddress pAddress;
	
	int pSocket;
	
	debnSocket *pPreviousSocket;
	debnSocket *pNextSocket;
	bool pIsRegistered;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create socket. */
	debnSocket( deNetworkBasic &netBasic );
	
	/** Clean up socket object. */
	virtual ~debnSocket();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Address. */
	inline debnAddress &GetAddress(){ return pAddress; }
	inline const debnAddress &GetAddress() const{ return pAddress; }
	
	/** Bind socket to stored address. */
	void Bind();
	
	/**
	 * Receive datagram from socket.
	 * \returns true if a message has been receives or false otherwise.
	 */
	bool ReceiveDatagram( deNetworkMessage &stream, debnAddress &address );
	
	/**
	 * Send datagram.
	 */
	void SendDatagram( const deNetworkMessage &stream, const debnAddress &address );
	
	/** Throw socket error. */
	static void ThrowSocketError( const char *message );
	
	/** Find public addresses. */
	static void FindAddresses( decStringList &list, bool onlyPublic );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** Previous socket. */
	inline debnSocket *GetPreviousSocket() const{ return pPreviousSocket; }
	
	/** Set previous socket. */
	void SetPreviousSocket( debnSocket *bnSocket );
	
	/** Next socket. */
	inline debnSocket *GetNextSocket() const{ return pNextSocket; }
	
	/** \breif Set next socket. */
	void SetNextSocket( debnSocket *bnSocket );
	
	/** Connection is registered. */
	inline bool GetIsRegistered() const{ return pIsRegistered; }
	
	/** Set if connection is registered. */
	void SetIsRegistered( bool isRegistered );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	static uint32_t pScopeIdFor( const sockaddr_in6 &address );
};

#endif
