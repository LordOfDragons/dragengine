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

#ifndef _DEBASENETWORKMODULE_H_
#define _DEBASENETWORKMODULE_H_

#include "../deBaseModule.h"

class deBaseNetworkWorld;
class deBaseNetworkServer;
class deBaseNetworkState;
class deBaseNetworkConnection;
class deWorld;
class deServer;
class deConnection;
class deNetworkState;


/**
 * \brief Base Network Module.
 */
class DE_DLL_EXPORT deBaseNetworkModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseNetworkModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseNetworkModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Called to init the module.
	 * 
	 * Returns true on success or false otherwise.
	 */
	virtual bool Init() = 0;
	
	/**
	 * \brief Called to cleanup the module.
	 * 
	 * All resources have to be freed and running threads stopped or killed if needed.
	 */
	virtual void CleanUp() = 0;
	
	/** \brief Process network. */
	virtual void ProcessNetwork() = 0;
	
	/**
	 * \brief Frame-per-second rate averaged over the last couple of frames.
	 * \version 1.6
	 * 
	 * Returns 0 if module is not using a separate thread.
	 */
	virtual int GetFPSRate();
	/*@}*/
	
	
	
	/** \name Frame Management */
	/*@{*/
	/** \brief Create peer for world. */
	virtual deBaseNetworkWorld *CreateWorld( deWorld *world ) = 0;
	
	/** \brief Create peer for server. */
	virtual deBaseNetworkServer *CreateServer( deServer *server ) = 0;
	
	/** \brief Create peer for connection. */
	virtual deBaseNetworkConnection *CreateConnection( deConnection *connection ) = 0;
	
	/** \brief Create peer for state. */
	virtual deBaseNetworkState *CreateState( deNetworkState *state ) = 0;
	/*@}*/
};

#endif
