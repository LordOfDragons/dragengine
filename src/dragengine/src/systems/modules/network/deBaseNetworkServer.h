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

#ifndef _DEBASENETWORKSERVER_H_
#define _DEBASENETWORKSERVER_H_

#include "../../../dragengine_export.h"

class deConnection;
class deNetworkState;


/**
 * \brief Network Module Server Peer.
 */
class DE_DLL_EXPORT deBaseNetworkServer{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseNetworkServer();
	
	/** \brief Clean up peer. */
	virtual ~deBaseNetworkServer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Start listening on address for incoming connections.
	 * 
	 * The format of the address is specific to the network module.
	 * Returns true if listening or false if something went wrong.
	 */
	virtual bool ListenOn(const char *address);
	
	/** \brief Stop listening. */
	virtual void StopListening();
	/*@}*/
};

#endif
