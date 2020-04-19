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
	void Update();
	
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
