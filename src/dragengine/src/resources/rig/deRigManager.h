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
