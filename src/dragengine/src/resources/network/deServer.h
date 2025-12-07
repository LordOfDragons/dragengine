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
class DE_DLL_EXPORT deServer : public deResource{
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
	deServer(deServerManager *manager);
	
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
	inline const decString &GetAddress() const{return pAddress;}
	
	/**
	 * \brief Set address.
	 * 
	 * Intended to be used by the network module only so  do not call it otherwise.
	 */
	void SetAddress(const char *address);
	
	/**
	 * \brief Start listening on address for incoming connections.
	 * 
	 * The format of the address is specific to the network module.
	 * Returns true if listening or false if something went wrong.
	 */
	bool ListenOn(const char *address);
	
	/** \brief Stop listening. */
	void StopListening();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Network system peer object. */
	inline deBaseNetworkServer *GetPeerNetwork() const{return pPeerNetwork;}
	
	/** \brief Set network system peer object. */
	void SetPeerNetwork(deBaseNetworkServer *peer);
	
	/** \brief Scripting system peer object. */
	inline deBaseScriptingServer *GetPeerScripting() const{return pPeerScripting;}
	
	/** \brief Set scripting system peer object. */
	void SetPeerScripting(deBaseScriptingServer *peer);
	/*@}*/
};

#endif
