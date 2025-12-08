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

#ifndef _DEDECALMANAGER_H_
#define _DEDECALMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deDecal;


/**
 * \brief Decal Resourse Manager.
 *
 */
class DE_DLL_EXPORT deDecalManager : public deResourceManager{
private:
	deResourceList pDecals;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new decal resource manager linked to the given engine. */
	deDecalManager(deEngine *engine);
	
	/** \brief Clean up decal resource manager and reports leaking resources. */
	~deDecalManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of decals. */
	int GetDecalCount() const;
	
	/** \brief Root decal resource for iteration purpose. */
	deDecal *GetRootDecal() const;
	
	/** \brief Create new and empty decal. */
	deDecal::Ref CreateDecal();
	
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
