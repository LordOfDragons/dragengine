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

#ifndef _DEDSRESOURCELOADERTASK_H_
#define _DEDSRESOURCELOADERTASK_H_

#include "dedsResourceLoader.h"

#include <dragengine/common/string/decString.h>

class deFileResource;
class dsValue;
class dsRealObject;
class deScriptingDragonScript;


/**
 * \brief DragonScript Resource Loader Wrapper Task.
 *
 * Task for the wrapper around the engine resource loader. Stores for each request the script listeners.
 */
class dedsResourceLoaderTask{
private:
	deScriptingDragonScript *pDS;
	
	decString pFilename;
	deResourceLoader::eResourceType pResourceType;
	
	dsValue **pListeners;
	int pListenerCount;
	int pListenerSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new task. */
	dedsResourceLoaderTask(deScriptingDragonScript *ds, const char *filename,
		deResourceLoader::eResourceType resourceType);
	/** Cleans up the task. */
	~dedsResourceLoaderTask();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	/** Retrieves the resource type. */
	inline deResourceLoader::eResourceType GetResourceType() const{ return pResourceType; }
	/** Determines if this task matches the given filename/resourceType. */
	bool Matches(const char *filename, deResourceLoader::eResourceType resourceType) const;
	
	/** Retrieves the number of listeners. */
	inline int GetListenerCount() const{ return pListenerCount; }
	/** Adds a listener. */
	void AddListener(dsRealObject *listener);
	/** Removes a listener. */
	void RemoveListener(dsRealObject *listener);
	
	/** Notifies all that the resource finished loading successfully. */
	void NotifyLoadingFinished(deFileResource *resource);
	/** Notifies all that the resource failed to load. */
	void NotifyLoadingFailed();
	/*@}*/
	
private:
	void pClearListeners();
};

// end of include only once
#endif
