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

#ifndef _DERIGMANAGER_H_
#define _DERIGMANAGER_H_

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

class deEngine;
class deRig;
class deRigBuilder;


/**
 * \brief Rig Resource manager.
 *
 */
class DE_DLL_EXPORT deRigManager : public deFileResourceManager{
private:
	deFileResourceList pRigs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new rig resource manager linked to the given engine. */
	deRigManager( deEngine *engine );
	
	/** \brief Clean up rig resource manager and reports leaking resources. */
	virtual ~deRigManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of rigs. */
	int GetRigCount() const;
	
	/** \brief Root rig resource for iteration purpose. */
	deRig *GetRootRig() const;
	
	/** \brief Rig with the given filename or NULL if not loaded yet. */
	deRig *GetRigWith( const char *filename ) const;
	
	/** \brief Rig with the given filename or NULL if not loaded yet. */
	deRig *GetRigWith( deVirtualFileSystem *vfs, const char *filename ) const;
	
	/** \brief Create new rig. */
	deRig *CreateRig( const char *filename, deRigBuilder &builder );
	
	/** \brief Create new rig. */
	deRig *CreateRig( deVirtualFileSystem *vfs, const char *filename, deRigBuilder &builder );
	
	/** \brief Loads a rig from the given file relative to the given base path. */
	deRig *LoadRig( const char *filename, const char *basePath );
	
	/** \brief Loads a rig from the given file relative to the given base path. */
	deRig *LoadRig( deVirtualFileSystem *vfs, const char *filename, const char *basePath );
	
	/**
	 * \brief Add loaded and fully prepared rig.
	 * 
	 * This method is to be used only by the resource loader to add an rig that has been
	 * loaded asynchronously.
	 */
	void AddLoadedRig( deRig *rig );
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemPhysicsLoad();
	void SystemPhysicsUnload();
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
