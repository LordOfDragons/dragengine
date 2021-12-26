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

#ifndef _DEMODELMANAGER_H_
#define _DEMODELMANAGER_H_

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

class deEngine;
class deModel;
class deModelBuilder;


/**
 * \brief Model Resourse Manager.
 */
class DE_DLL_EXPORT deModelManager : public deFileResourceManager{
private:
	deFileResourceList pModels;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model resource manager linked to the given engine. */
	deModelManager( deEngine *engine );
	
	/** \brief Clean up model resource manager and reports leaking resources. */
	~deModelManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of models. */
	int GetModelCount() const;
	
	/** \brief Root model resource for iteration purpose. */
	deModel *GetRootModel() const;
	
	/** \brief Model with the given filename or NULL if not loaded yet. */
	deModel *GetModelWith( const char *filename ) const;
	
	/** \brief Model with the given filename or NULL if not loaded yet. */
	deModel *GetModelWith( deVirtualFileSystem *vfs, const char *filename ) const;
	
	/** \brief Create new and empty model. */
	deModel *CreateModel( const char *filename, deModelBuilder &builder );
	
	/** \brief Create new and empty model. */
	deModel *CreateModel( deVirtualFileSystem *vfs, const char *filename, deModelBuilder &builder );
	
	/** \brief Loads a model from the given file relative to the given base path. */
	deModel *LoadModel( const char *filename, const char *basePath );
	
	/** \brief Loads a model from the given file relative to the given base path. */
	deModel *LoadModel( deVirtualFileSystem *vfs, const char *filename, const char *basePath );
	
	/**
	 * \brief Add loaded and fully prepared model.
	 * 
	 * This method is to be used only by the resource loader to add an model that has
	 * been loaded asynchronously.
	 */
	void AddLoadedModel( deModel *model );
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemGraphicLoad();
	void SystemGraphicUnload();
	void SystemPhysicsLoad();
	void SystemPhysicsUnload();
	void SystemAudioLoad();
	void SystemAudioUnload();
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
