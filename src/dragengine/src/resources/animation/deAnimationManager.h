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

#ifndef _DEANIMATIONMANAGER_H_
#define _DEANIMATIONMANAGER_H_

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

class deEngine;
class deAnimation;
class deAnimationBuilder;


/**
 * \brief Animation Resource Manager.
 *
 */
class DE_DLL_EXPORT deAnimationManager : public deFileResourceManager{
private:
	deFileResourceList pAnimations;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation resource manager linked to the given engine. */
	deAnimationManager(deEngine *engine);
	
	/** \brief Clean up animation resource manager and reports leaking resources. */
	virtual ~deAnimationManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of animation resource. */
	int GetAnimationCount() const;
	
	/** \brief Root animation for iteration purpose. */
	deAnimation *GetRootAnimation() const;
	
	/** \brief Animation with the given filename or NULL if not loaded yet. */
	deAnimation *GetAnimationWith(const char *filename) const;
	
	/** \brief Animation with the given filename or NULL if not loaded yet. */
	deAnimation *GetAnimationWith(deVirtualFileSystem *vfs, const char *filename) const;
	
	/** \brief Create new animation object using a builder. */
	deAnimation *CreateAnimation(const char *filename, deAnimationBuilder &builder);
	
	/** \brief Create new animation object using a builder. */
	deAnimation *CreateAnimation(deVirtualFileSystem *vfs, const char *filename, deAnimationBuilder &builder);
	
	/** \brief Loads an animation from the given file relative to the given base path. */
	deAnimation *LoadAnimation(const char *filename, const char *basePath);
	
	/** \brief Loads an animation from the given file relative to the given base path. */
	deAnimation *LoadAnimation(deVirtualFileSystem *vfs, const char *filename, const char *basePath);
	
	/**
	 * \brief Save animation to file.
	 * \version 1.6
	 */
	void SaveAnimation(const deAnimation &animation, const char *filename);
	
	/**
	 * \brief Save animation to file.
	 * \version 1.6
	 */
	void SaveAnimation(const deAnimation &animation, deVirtualFileSystem &vfs, const char *filename);
	
	/**
	 * \brief Add loaded and fully prepared animation.
	 * 
	 * This method is to be used only by the resource loader to add an animation that
	 * has been loaded asynchronously.
	 */
	void AddLoadedAnimation(deAnimation *animation);
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Graphic System Peers of all stored resources have to be created. */
	virtual void SystemGraphicLoad();
	
	/** \brief Graphic System Peers of all stored resources have to be freed. */
	virtual void SystemGraphicUnload();
	
	/** \brief Animator System Peers of all stored resources have to be created. */
	virtual void SystemAnimatorLoad();
	
	/** \brief Animator System Peers of all stored resources have to be freed. */
	virtual void SystemAnimatorUnload();
	/*@}*/
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
