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

#ifndef _DECOLLIDERMANAGER_H_
#define _DECOLLIDERMANAGER_H_ 

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deCollider;
class deColliderVolume;
class deColliderRig;
class deColliderComponent;


/**
 * \brief World Collider Resource Manager.
 *
 */
class DE_DLL_EXPORT deColliderManager : public deResourceManager{
private:
	deResourceList pColliders;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new world collider resource manager linked to the given engine. */
	deColliderManager( deEngine *engine );
	
	/** \brief Clean up world collider resource manager and reports leaking resources. */
	virtual ~deColliderManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of world colliders. */
	int GetColliderCount() const;
	
	/** \brief Root collider resource for iteration purpose. */
	deCollider *GetRootCollider() const;
	
	/** \brief Create new world collider volume. */
	deColliderVolume *CreateColliderVolume();
	
	/** \brief Create new world collider rigged. */
	deColliderRig *CreateColliderRig();
	
	/** \brief Create new world collider component. */
	deColliderComponent *CreateColliderComponent();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemPhysicsLoad();
	void SystemPhysicsUnload();
	void SystemScriptingLoad();
	void SystemScriptingUnload();
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
