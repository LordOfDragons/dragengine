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

#ifndef _DENETWORKSYSTEM_H_
#define _DENETWORKSYSTEM_H_

#include "deBaseSystem.h"

class deLoadableModule;
class deBaseNetworkModule;
class deWorld;
class deServer;
class deConnection;
class deNetworkState;


/**
 * \brief Network System.
 *
 * Provides network communication support..
 */
class DE_DLL_EXPORT deNetworkSystem : public deBaseSystem{
private:
	deBaseNetworkModule *pActiveModule;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new network system linked to the given engine. */
	deNetworkSystem(deEngine *engine);
	
	/** \brief Clean up network system. */
	~deNetworkSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline deBaseNetworkModule *GetActiveModule() const{ return pActiveModule; }
	/*@}*/
	
	
	
	/**
	 * \name Runtime Functions
	 * Run-Time functions can only be called if the audio system
	 * is running. Otherwise those functions cause an exception.
	 */
	/*@{*/
	/** \brief Process network. */
	void ProcessNetwork();
	
	/** \brief Create peer object for the given server using the active module and assigns it. */
	void LoadServer(deServer *server);
	
	/** \brief Create peer object for the given connection using the active module and assigns it. */
	void LoadConnection(deConnection *connection);
	
	/** \brief Create peer object for the given state using the active module and assigns it. */
	void LoadState(deNetworkState *state);
	
	/** \brief Create peer object for the given world using the active module and assigns it. */
	void LoadWorld(deWorld *world);
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void SetActiveModule(deLoadableModule *module);
	
	/**
	 * \brief Clearcross references and links that could lead to memory leaks.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void ClearPermanents();
	
	/** \brief Carry out here actions right after the system started up. */
	virtual void PostStart();
	
	/** \brief Carry out here actions right before the system shuts down. */
	virtual void PreStop();
	/*@}*/
};

#endif
