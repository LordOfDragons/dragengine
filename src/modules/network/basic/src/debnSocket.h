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

#ifndef _DEBNSOCKET_H_
#define _DEBNSOCKET_H_

#include <dragengine/dragengine_configuration.h>

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
	

	#ifdef OS_W32
		SOCKET pSocket;
	#else
		int pSocket;
	#endif

	debnSocket *pPreviousSocket;
	debnSocket *pNextSocket;
	bool pIsRegistered;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<debnSocket> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create socket. */
	debnSocket(deNetworkBasic &netBasic);
	
	/** Clean up socket object. */
	virtual ~debnSocket();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Address. */
	inline debnAddress &GetAddress(){return pAddress;}
	inline const debnAddress &GetAddress() const{return pAddress;}
	
	/** Bind socket to stored address. */
	void Bind();
	
	/**
	 * Receive datagram from socket.
	 * \returns true if a message has been receives or false otherwise.
	 */
	bool ReceiveDatagram(deNetworkMessage &stream, debnAddress &address);
	
	/**
	 * Send datagram.
	 */
	void SendDatagram(const deNetworkMessage &stream, const debnAddress &address);
	
	/** Throw socket error. */
	static void ThrowSocketError(const char *message);
	
	/** Find public addresses. */
	static void FindAddresses(decStringList &list, bool onlyPublic);
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** Previous socket. */
	inline debnSocket *GetPreviousSocket() const{return pPreviousSocket;}
	
	/** Set previous socket. */
	void SetPreviousSocket(debnSocket *bnSocket);
	
	/** Next socket. */
	inline debnSocket *GetNextSocket() const{return pNextSocket;}
	
	/** \breif Set next socket. */
	void SetNextSocket(debnSocket *bnSocket);
	
	/** Connection is registered. */
	inline bool GetIsRegistered() const{return pIsRegistered;}
	
	/** Set if connection is registered. */
	void SetIsRegistered(bool isRegistered);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	static uint32_t pScopeIdFor(const sockaddr_in6 &address);
};

#endif
