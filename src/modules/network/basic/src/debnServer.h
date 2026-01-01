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

#ifndef _DEBNSERVER_H_
#define _DEBNSERVER_H_

#include "debnSocket.h"

#include <dragengine/systems/modules/network/deBaseNetworkServer.h>

class deServer;
class debnAddress;
class deNetworkBasic;
class decBaseFileReader;



/**
 * \brief Server peer class.
 */
class debnServer : public deBaseNetworkServer{
private:
	deNetworkBasic *pNetBasic;
	deServer *pServer;
	
	debnSocket::Ref pSocket;
	bool pListening;
	
	debnServer *pPreviousServer;
	debnServer *pNextServer;
	bool pIsRegistered;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create server. */
	debnServer(deNetworkBasic *netBasic, deServer *server);
	
	/** \brief Clean up server object. */
	virtual ~debnServer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Socket. */
	inline const debnSocket::Ref &GetSocket() const{ return pSocket; }
	
	/** \brief Process connection request. */
	void ProcessConnectionRequest(debnAddress &address, decBaseFileReader &reader);
	
	/**
	 * \brief Start listening on address for incoming connections.
	 */
	virtual bool ListenOn(const char *address);
	
	/** \brief Stop listening. */
	virtual void StopListening();
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Previous server. */
	inline debnServer *GetPreviousServer() const{ return pPreviousServer; }
	
	/** \brief Set previous server. */
	void SetPreviousServer(debnServer *server);
	
	/** \brief Next server. */
	inline debnServer *GetNextServer() const{ return pNextServer; }
	
	/** \brief Set next server. */
	void SetNextServer(debnServer *server);
	
	/** \brief Server is registered. */
	inline bool GetIsRegistered() const{ return pIsRegistered; }
	
	/** \brief Set if server is registered. */
	void SetIsRegistered(bool isRegistered);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
