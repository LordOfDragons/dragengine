/* 
 * Drag[en]gine DragonScript Script Module
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
	dedsResourceLoaderTask( deScriptingDragonScript *ds, const char *filename,
		deResourceLoader::eResourceType resourceType );
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
	bool Matches( const char *filename, deResourceLoader::eResourceType resourceType ) const;
	
	/** Retrieves the number of listeners. */
	inline int GetListenerCount() const{ return pListenerCount; }
	/** Adds a listener. */
	void AddListener( dsRealObject *listener );
	/** Removes a listener. */
	void RemoveListener( dsRealObject *listener );
	
	/** Notifies all that the resource finished loading successfully. */
	void NotifyLoadingFinished( deFileResource *resource );
	/** Notifies all that the resource failed to load. */
	void NotifyLoadingFailed();
	/*@}*/
	
private:
	void pClearListeners();
};

// end of include only once
#endif
