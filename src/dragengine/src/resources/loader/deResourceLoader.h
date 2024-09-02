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

#ifndef _DERESOURCELOADER_H_
#define _DERESOURCELOADER_H_

#include "../../common/collection/decThreadSafeObjectOrderedSet.h"

class deResourceLoaderTask;
class deResourceLoaderInfo;
class deFileResource;
class deEngine;
class deVirtualFileSystem;


/**
 * \brief Resource Loader.
 * 
 * Loads resources asynchronously. To load a resource a scripting module places a load request.
 * Requests are processed using <em>Parallel Processing</em> system of the game engine. The
 * scripting module has to query the resource loader during each frame update for new entries
 * in the retrieval queue. Once there are no more entries in the queue the scripting module
 * can carry on with the fram update.
 * 
 * Requests for the same resource are grouped together and placed in the retrieval queue only
 * once. The scripting module is responsible to track multiple requests for the same resource.
 * A resource is uniquely identified by the path and resource type. This information are also
 * returned in retrieval queries and can be used to match up with one or more requests made by
 * the game scripts.
 * 
 * Loading certain resources can add internal loading requests not started by the scripting
 * module. The scripting module has to deal with resources finished loading which have not
 * been requested.
 * 
 * Resource loading is supported for the following resource types:
 * - Animation
 * - Font
 * - Image
 * - LanguagePack
 * - Model
 * - OcclusionMesh
 * - Rig
 * - Skin
 * - Sound
 * - Video
 */
class DE_DLL_EXPORT deResourceLoader{
public:
	/** \brief Resource types. */
	enum eResourceType{
		/** \brief Animation. */
		ertAnimation,
		
		/** \brief Font. */
		ertFont,
		
		/** \brief Image. */
		ertImage,
		
		/** \brief LanguagePack. */
		ertLanguagePack,
		
		/** \brief Model. */
		ertModel,
		
		/** \brief Occlusion Mesh. */
		ertOcclusionMesh,
		
		/** \brief Rig. */
		ertRig,
		
		/** \brief Skin. */
		ertSkin,
		
		/** \brief Sound. */
		ertSound,
		
		/** \brief Video. */
		ertVideo
	};
	
	
	
private:
	deEngine &pEngine;
	
	decThreadSafeObjectOrderedSet pPendingTasks;
	decThreadSafeObjectOrderedSet pFinishedTasks;
	
	bool pLoadAsynchron;
	bool pOutputDebugMessages;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create resource loader. */
	deResourceLoader( deEngine &engine );
	
	/** \brief Clean up resource loader. */
	~deResourceLoader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Debug messages are logger. */
	inline bool GetOutputDebugMessages() const{ return pOutputDebugMessages; }
	
	/** \brief Set if debug messages are logged. */
	void SetOutputDebugMessages( bool outputDebugMessages );
	
	/**
	 * \brief Add request for loading a resource.
	 * 
	 * \param[in] vfs Virtual file system to use.
	 * \param[in] path Path to use.
	 * \param[in] resourceType Type of resource to load
	 * 
	 * \returns parallel task processing the request to use as parallel task dependency.
	 * If the resource is already loaded the task is marked finished. Do not add
	 * the task to the parallel processor. This has been done already if required.
	 * 
	 * \warning The returned task is not held by the caller. The resource loader holds
	 * references to the task as long as it is pending, being process or finished
	 * waiting to be collected. Once the task has been collected the resource
	 * loader holds no references anymore. If you need to work with the task any
	 * time later you have to add a reference yourself.
	 * \throws deeInvalidParam \em vfs is NULL.
	 * \throws deeInvalidParam \em path is NULL.
	 */
	deResourceLoaderTask *AddLoadRequest( deVirtualFileSystem *vfs, const char *path,
		eResourceType resourceType );
	
	/**
	 * \brief Add request for saving a resource.
	 * 
	 * \returns parallel task processing the request to use as parellel task dependency.
	 * If the resource is already loaded the task is marked finished.
	 * 
	 * \warning The returned task is not held by the caller. The resource loader holds
	 * references to the task as long as it is pending, being process or finished
	 * waiting to be collected. Once the task has been collected the resource
	 * loader holds no references anymore. If you need to work with the task any
	 * time later you have to add a reference yourself.
	 */
	deResourceLoaderTask *AddSaveRequest( deVirtualFileSystem *vfs, const char *path,
		deFileResource *resource );
	
	/**
	 * \brief Information about next finished request
	 * 
	 * \retval true A request finished. Information have been written to \em info.
	 * \retval false No request finished. \em info is unchanged.
	 */
	bool NextFinishedRequest( deResourceLoaderInfo &info );
	
	/**
	 * \brief Stop loading and remove all tasks.
	 * 
	 * Used by the game engine to clear pending tasks upon cleanup avoiding problems.
	 */
	void RemoveAllTasks();
	/*@}*/
	
	
	
	/**
	 * \name Internal use only
	 * \warning Do not call directly.
	 */
	/*@{*/
	void FinishTask( deResourceLoaderTask *task );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	bool pHasTaskWith( deVirtualFileSystem *vfs, const char *path,
		eResourceType resourceType ) const;
	
	deResourceLoaderTask *pGetTaskWith( deVirtualFileSystem *vfs, const char *path,
		eResourceType resourceType ) const;
};

#endif
