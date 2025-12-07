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

#ifndef _DESKYINSTANCEMANAGER_H_
#define _DESKYINSTANCEMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deSkyInstance;


/**
 * \brief Sky instance resource manager.
 */
class DE_DLL_EXPORT deSkyInstanceManager : public deResourceManager{
private:
	deResourceList pInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky instance resource manager. */
	deSkyInstanceManager(deEngine *engine);
	
	/** \brief Clean up sky instance resource manager and report leaking resources. */
	virtual ~deSkyInstanceManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of sky instance resource. */
	int GetSkyInstanceCount() const;
	
	/** \brief Root sky instance resource for iteration purpose. */
	deSkyInstance *GetRootSkyInstance() const;
	
	/** \brief Create sky instance. */
	deSkyInstance *CreateSkyInstance();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Graphic system peers of all stored resources have to be created. */
	virtual void SystemGraphicLoad();
	
	/** \brief Graphic system peers of all stored resources have to be freed. */
	virtual void SystemGraphicUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * 
	 * \warning For internal use only. Never call these function on your own.
	 */
	/*@{*/
	/** \warning For internal use only. Never call this function on your own. */
	void RemoveResource(deResource *resource);
	/*@}*/
};

#endif
