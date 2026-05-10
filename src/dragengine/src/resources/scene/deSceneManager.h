/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DESCENEMANAGER_H_
#define _DESCENEMANAGER_H_

#include "deScene.h"
#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

class deEngine;


/**
 * \brief Scene Resource Manager.
 */
class DE_DLL_EXPORT deSceneManager : public deFileResourceManager{
private:
	deFileResourceList pScenes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new scene resource manager linked to the given engine. */
	deSceneManager(deEngine *engine);
	
	/** \brief Clean up scene resource manager and reports leaking resources. */
	~deSceneManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Scenes. */
	inline const deFileResourceList &GetScenes() const{ return pScenes; }
	
	/** \brief Count of scenes. */
	int GetSceneCount() const;
	
	/** \brief Root scene resource for iteration purpose. */
	deScene *GetRootScene() const;
	
	/** \brief Scene with the given filename or NULL if not loaded yet. */
	deScene *GetSceneWith(const char *filename) const;
	
	/** \brief Scene with the given filename or NULL if not loaded yet. */
	deScene *GetSceneWith(deVirtualFileSystem *vfs, const char *filename) const;
	
	/** \brief Loads a scene from the given file relative to the given base path. */
	deScene::Ref LoadScene(const char *filename, const char *basePath);
	
	/** \brief Loads a scene from the given file relative to the given base path. */
	deScene::Ref LoadScene(deVirtualFileSystem *vfs, const char *filename, const char *basePath);
	
	/**
	 * \brief Add loaded and fully prepared scene.
	 *
	 * This method is to be used only by the resource loader to add a scene that has
	 * been loaded asynchronously.
	 */
	void AddLoadedScene(deScene *scene);
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
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
