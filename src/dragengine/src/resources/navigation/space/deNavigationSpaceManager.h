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

#ifndef _DENAVIGATIONSPACEMANAGER_H_
#define _DENAVIGATIONSPACEMANAGER_H_

#include "../../deResourceManager.h"
#include "../../deResourceList.h"

class deNavigationSpace;


/**
 * \brief Navigation Space Manager.
 */
class DE_DLL_EXPORT deNavigationSpaceManager : public deResourceManager{
private:
	deResourceList pNavSpaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new manager. */
	deNavigationSpaceManager( deEngine *engine );
	
	/** \brief Clean up manager. */
	virtual ~deNavigationSpaceManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of navigation spaces. */
	int GetNavigationSpaceCount() const;
	
	/** \brief Root navigation space resource for iteration purpose. */
	deNavigationSpace *GetRootNavigationSpace() const;
	
	/** \brief Create new navigation space. */
	deNavigationSpace *CreateNavigationSpace();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief AI System Peers of all stored resources have to be created. */
	virtual void SystemAILoad();
	
	/** \brief AI System Peers of all stored resources have to be freed. */
	virtual void SystemAIUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
