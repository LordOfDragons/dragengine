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

#ifndef _DEOCCLUSIONMESHMANAGER_H_
#define _DEOCCLUSIONMESHMANAGER_H_

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

class deOcclusionMesh;
class deOcclusionMeshBuilder;


/**
 * \brief Occlusion Mesh Manager.
 */
class DE_DLL_EXPORT deOcclusionMeshManager : public deFileResourceManager{
private:
	deFileResourceList pMeshes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new manager. */
	deOcclusionMeshManager(deEngine *engine);
	
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
	deOcclusionMesh *GetOcclusionMeshWith(const char *filename) const;
	
	/** \brief Occlusion mesh with the given filename or NULL if not loaded yet. */
	deOcclusionMesh *GetOcclusionMeshWith(deVirtualFileSystem *vfs, const char *filename) const;
	
	/** \brief Creates an occlusion mesh using a builder. */
	deOcclusionMesh *CreateOcclusionMesh(const char *filename, deOcclusionMeshBuilder &builder);
	
	/** \brief Creates an occlusion mesh using a builder. */
	deOcclusionMesh *CreateOcclusionMesh(deVirtualFileSystem *vfs, const char *filename,
		deOcclusionMeshBuilder &builder);
	
	/** \brief Loads an occlusion mesh from the given file relative to the given base path. */
	deOcclusionMesh *LoadOcclusionMesh(const char *filename, const char *basePath);
	
	/** \brief Loads an occlusion mesh from the given file relative to the given base path. */
	deOcclusionMesh *LoadOcclusionMesh(deVirtualFileSystem *vfs, const char *filename, const char *basePath);
	
	/**
	 * Adds a loaded and fully prepared occlusion mesh. This method is to be used only by
	 * the resource loader to add an occlusion mesh that has been loaded asynchronously.
	 */
	void AddLoadedOcclusionMesh(deOcclusionMesh *occmesh);
	
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
	void RemoveResource(deResource *resource);
	/*@}*/
};

#endif
