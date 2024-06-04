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

#ifndef _DESERVERMANAGER_H_
#define _DESERVERMANAGER_H_ 

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deServer;


/**
 * \brief Server Resource Manager.
 *
 */
class DE_DLL_EXPORT deServerManager : public deResourceManager{
private:
	deResourceList pServers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create server resource manager linked to the given engine. */
	deServerManager( deEngine *engine );
	
	/** \brief Clean up server resource manager and reports leaking resources. */
	virtual ~deServerManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of servers. */
	int GetServerCount() const;
	
	/** \brief Root server resource for iteration purpose. */
	deServer *GetRootServer() const;
	
	/** \brief Create server. */
	deServer *CreateServer();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Network System Peers of all stored resources have to be created. */
	virtual void SystemNetworkLoad();
	
	/** \brief Network System Peers of all stored resources have to be freed. */
	virtual void SystemNetworkUnload();
	
	/** \brief Scripting System Peers of all stored resources have to be created. */
	virtual void SystemScriptingLoad();
	
	/** \brief Scripting System Peers of all stored resources have to be freed. */
	virtual void SystemScriptingUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * \warning For use by deResource only.
	 */
	/*@{*/
	/** \brief Removes the given resource from the manager without freeing it. */
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
