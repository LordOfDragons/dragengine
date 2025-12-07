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

#ifndef _DECONNECTIONMANAGER_H_
#define _DECONNECTIONMANAGER_H_ 

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deConnection;


/**
 * \brief Connection Resource Manager.
 */
class DE_DLL_EXPORT deConnectionManager : public deResourceManager{
private:
	deResourceList pConnections;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create connection resource manager linked to the given engine. */
	deConnectionManager(deEngine *engine);
	
	/** \brief Clean up connection resource manager and reports leaking resources. */
	~deConnectionManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of connections. */
	int GetConnectionCount() const;
	
	/** \brief Root connection resource for iteration purpose. */
	deConnection *GetRootConnection() const;
	
	/** \brief Create connection. */
	deConnection *CreateConnection();
	
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
	void RemoveResource(deResource *resource);
	/*@}*/
};

#endif
