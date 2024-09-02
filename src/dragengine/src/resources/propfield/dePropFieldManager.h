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

#ifndef _DEPROPFIELDMANAGER_H_
#define _DEPROPFIELDMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class dePropField;


/**
 * \brief Prop Field Manager.
 *
 * Manages a list of prop fields.
 */
class DE_DLL_EXPORT dePropFieldManager : public deResourceManager{
private:
	deResourceList pFields;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new resource manager. */
	dePropFieldManager( deEngine *engine );
	
	/** \brief Clean up resource manager and reports leaking resources. */
	virtual ~dePropFieldManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of prop fields. */
	int GetPropFieldCount() const;
	
	/** \brief Root prop field resource for iteration purpose. */
	dePropField *GetRootPropField() const;
	
	/** \brief Create new prop field. */
	dePropField *CreatePropField();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Graphic System Peers of all stored resources have to be created. */
	virtual void SystemGraphicLoad();
	
	/** \brief Graphic System Peers of all stored resources have to be freed. */
	virtual void SystemGraphicUnload();
	
	/** \brief Physics System Peers of all stored resources have to be created. */
	virtual void SystemPhysicsLoad();
	
	/** \brief Physics System Peers of all stored resources have to be freed. */
	virtual void SystemPhysicsUnload();
	
	/** \brief Scripting System Peers of all stored resources have to be created. */
	virtual void SystemScriptingLoad();
	
	/** \brief Scripting System Peers of all stored resources have to be freed. */
	virtual void SystemScriptingUnload();
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
