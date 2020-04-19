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

#ifndef _DEOCCLUSIONMESHMANAGER_H_
#define _DEOCCLUSIONMESHMANAGER_H_

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

class deOcclusionMesh;
class deOcclusionMeshBuilder;


/**
 * \brief Occlusion Mesh Manager.
 */
class deOcclusionMeshManager : public deFileResourceManager{
private:
	deFileResourceList pMeshes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new manager. */
	deOcclusionMeshManager( deEngine *engine );
	
	/** \brief Clean up manager. */
	virtual ~deOcclusionMeshManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of occlusion meshes. */
	int GetOcclusionMeshCount() const;
	
	/** \brief Root occlusion mesh for iteration purpose. */
	deOcclusionMesh *GetRootOcclusionMesh() const;
	
	/** \brief Occlusion mesh with the given filename or NULL if not loaded yet. */
	deOcclusionMesh *GetOcclusionMeshWith( const char *filename ) const;
	
	/** \brief Occlusion mesh with the given filename or NULL if not loaded yet. */
	deOcclusionMesh *GetOcclusionMeshWith( deVirtualFileSystem *vfs, const char *filename ) const;
	
	/** \brief Creates an occlusion mesh using a builder. */
	deOcclusionMesh *CreateOcclusionMesh( const char *filename, deOcclusionMeshBuilder &builder );
	
	/** \brief Creates an occlusion mesh using a builder. */
	deOcclusionMesh *CreateOcclusionMesh( deVirtualFileSystem *vfs, const char *filename,
		deOcclusionMeshBuilder &builder );
	
	/** \brief Loads an occlusion mesh from the given file relative to the given base path. */
	deOcclusionMesh *LoadOcclusionMesh( const char *filename, const char *basePath );
	
	/** \brief Loads an occlusion mesh from the given file relative to the given base path. */
	deOcclusionMesh *LoadOcclusionMesh( deVirtualFileSystem *vfs, const char *filename, const char *basePath );
	
	/**
	 * Adds a loaded and fully prepared occlusion mesh. This method is to be used only by
	 * the resource loader to add an occlusion mesh that has been loaded asynchronously.
	 */
	void AddLoadedOcclusionMesh( deOcclusionMesh *occmesh );
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Graphic System Peers of all stored resources have to be created. */
	virtual void SystemGraphicLoad();
	
	/** \brief Graphic System Peers of all stored resources have to be freed. */
	virtual void SystemGraphicUnload();
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
