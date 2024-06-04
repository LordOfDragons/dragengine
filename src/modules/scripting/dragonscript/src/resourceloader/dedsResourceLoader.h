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

#ifndef _DEDSRESOURCELOADER_H_
#define _DEDSRESOURCELOADER_H_

#include <dragengine/resources/loader/deResourceLoader.h>

class deScriptingDragonScript;
class dedsResourceLoaderTask;



/**
 * @brief DragonScript Resource Loader Wrapper.
 *
 * Wraps the engine resource loader.
 */
class dedsResourceLoader{
private:
	deScriptingDragonScript *pDS;
	
	dedsResourceLoaderTask **pTasks;
	int pTaskCount;
	int pTaskSize;
	
	
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new wrapper. */
	dedsResourceLoader( deScriptingDragonScript *ds );
	/** Cleans up the wrapper. */
	~dedsResourceLoader();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of tasks. */
	inline int GetTaskCount() const{ return pTaskCount; }
	
	/** Determines if there are still some resources left to load. */
	inline bool HasResourcesLeft() const{ return pTaskCount > 0; }
	/** Update. */
	void OnFrameUpdate();
	
	/** Adds a request. */
	void AddRequest( const char *filename, deResourceLoader::eResourceType resourceType,
		dsRealObject *listener );
	/** Cancel a request. */
	void CancelRequest( const char *filename, deResourceLoader::eResourceType resourceType,
		dsRealObject *listener );
	/** CancelAllRequests. */
	void CancelAllRequests();
	
private:
	int pIndexOfTaskWith( const char *filename, deResourceLoader::eResourceType resourceType ) const;
	void pRemoveTaskFrom( int index );
};

// end of include only once
#endif
