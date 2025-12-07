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

#ifndef _DEDYNAMICSKINMANAGER_H_
#define _DEDYNAMICSKINMANAGER_H_

#include "../../deResourceManager.h"
#include "../../deResourceList.h"

class deDynamicSkin;


/**
 * \brief Dynamic Skin Resource Manager.
 *
 * Manages dynamic skin resources.
 */
class DE_DLL_EXPORT deDynamicSkinManager : public deResourceManager{
private:
	deResourceList pSkins;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new manager. */
	deDynamicSkinManager(deEngine *engine);
	
	/** \brief Clean up manager. */
	virtual ~deDynamicSkinManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of dynamic skins. */
	int GetDynamicSkinCount() const;
	
	/** \brief Root dynamic skin resource for iteration purpose. */
	deDynamicSkin *GetRootDynamicSkin() const;
	
	/** \brief Create new dynamic skin. */
	deDynamicSkin *CreateDynamicSkin();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemGraphicLoad();
	void SystemGraphicUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource(deResource *resource);
	/*@}*/
};

#endif
