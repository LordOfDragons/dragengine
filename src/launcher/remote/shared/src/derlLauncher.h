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

#ifndef _DERLLAUNCHER_H_
#define _DERLLAUNCHER_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/resources/network/deConnection.h>
#include <dragengine/resources/network/deServer.h>
#include <dragengine/resources/network/deNetworkState.h>


/**
 * \brief Drag[en]gine base remote launcher class.
 * 
 * Implements the DERemoteLauncher Network Protocol and does the heavy lifting.
 * User has to provide and implement a listener to react to the necessary events.
 */
class derlLauncher{
private:
	deConnection::Ref pConnection;
	deServer::Ref pServer;
	deNetworkState::Ref pNetworkState;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create remote launcher.
	 */
	derlLauncher();
	
	/** \brief Clean up remote launcher support. */
	virtual ~derlLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
	
	
private:
};

#endif
