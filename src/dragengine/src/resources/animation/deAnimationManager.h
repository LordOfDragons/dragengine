/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
class deAnimationManager : public deFileResourceManager{
private:
	deFileResourceList pAnimations;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation resource manager linked to the given engine. */
	deAnimationManager( deEngine *engine );
	
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
	deAnimation *GetAnimationWith( const char *filename ) const;
	
	/** \brief Animation with the given filename or NULL if not loaded yet. */
	deAnimation *GetAnimationWith( deVirtualFileSystem *vfs, const char *filename ) const;
	
	/** \brief Create new animation object using a builder. */
	deAnimation *CreateAnimation( const char *filename, deAnimationBuilder &builder );
	
	/** \brief Create new animation object using a builder. */
	deAnimation *CreateAnimation( deVirtualFileSystem *vfs, const char *filename, deAnimationBuilder &builder );
	
	/** \brief Loads an animation from the given file relative to the given base path. */
	deAnimation *LoadAnimation( const char *filename, const char *basePath );
	
	/** \brief Loads an animation from the given file relative to the given base path. */
	deAnimation *LoadAnimation( deVirtualFileSystem *vfs, const char *filename, const char *basePath );
	
	/**
	 * \brief Save animation to file.
	 * \version 1.6
	 */
	void SaveAnimation( const deAnimation &animation, const char *filename );
	
	/**
	 * \brief Save animation to file.
	 * \version 1.6
	 */
	void SaveAnimation( const deAnimation &animation, deVirtualFileSystem &vfs, const char *filename );
	
	/**
	 * \brief Add loaded and fully prepared animation.
	 * 
	 * This method is to be used only by the resource loader to add an animation that
	 * has been loaded asynchronously.
	 */
	void AddLoadedAnimation( deAnimation *animation );
	
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
