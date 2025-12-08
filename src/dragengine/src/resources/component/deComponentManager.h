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

#ifndef _DECOMPONENTMANAGER_H_
#define _DECOMPONENTMANAGER_H_ 

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deComponent;
class deModel;
class deSkin;


/**
 * \brief Scene Component Resource Manager.
 */
class DE_DLL_EXPORT deComponentManager : public deResourceManager{
private:
	deResourceList pComponents;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new scene component resource manager linked to the given engine. */
	deComponentManager(deEngine *engine);
	
	/** \brief Clean up scene component resource manager and reports leaking resources. */
	~deComponentManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of scene components. */
	int GetComponentCount() const;
	
	/** \brief Root component resource for iteration purpose. */
	deComponent *GetRootComponent() const;
	
	/** \brief Create component. */
	deComponent::Ref CreateComponent(deModel *model = NULL, deSkin *skin = NULL);
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemGraphicLoad() override;
	void SystemGraphicUnload() override;
	void SystemPhysicsLoad() override;
	void SystemPhysicsUnload() override;
	void SystemAudioLoad() override;
	void SystemAudioUnload() override;
	
	/** \brief Animator system peers of all stored resources have to be created. */
	void SystemAnimatorLoad() override;
	
	/** \brief Animator system seers of all stored resources have to be freed. */
	void SystemAnimatorUnload() override;
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource(deResource *resource) override;
	/*@}*/
};

#endif
