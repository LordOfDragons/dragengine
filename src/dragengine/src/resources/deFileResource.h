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

#ifndef _DEFILERESOURCE_H_
#define _DEFILERESOURCE_H_

#include "deResource.h"
#include "../common/string/decString.h"
#include "../common/utils/decDateTime.h"

class deVirtualFileSystem;
class deFileResourceManager;


/**
 * \brief Base file resource class.
 *
 * Provides the basic class for all resources loadable from a file. The asynchron
 * flag indicates if this resource is currently handled by the resource loader.
 * Asynchron resources exist in a thread alongside the main thread and can require
 * special treatment in single type modules.
 */
class DE_DLL_EXPORT deFileResource : public deResource{
private:
	deVirtualFileSystem *pVirtualFileSystem;
	decString pFilename;
	TIME_SYSTEM pModificationTime;
	bool pAsynchron;
	bool pOutdated;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file resource. */
	deFileResource( deFileResourceManager *resourceManager, deVirtualFileSystem *vfs,
		const char *filename, TIME_SYSTEM modificationTime );
	
protected:
	/**
	 * \brief Clean up file resource.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deFileResource();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Virtual file system or NULL if build from memory. */
	inline deVirtualFileSystem *GetVirtualFileSystem() const{ return pVirtualFileSystem; }
	
	/** \brief Filename or empty string if build from memory. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** \brief Modification time used to detect resources changing on disk while loaded. */
	inline TIME_SYSTEM GetModificationTime() const{ return pModificationTime; }
	
	/**
	 * \brief Set modification time used to detect resources changing on disk while loaded.
	 * \warning Internal Use Only. Do not call!
	 */
	void SetModificationTime( TIME_SYSTEM modificationTime );
	
	/** \brief Resource is asynchron. */
	inline bool GetAsynchron() const{ return pAsynchron; }
	
	/** \brief Set if resource is asynchron. */
	void SetAsynchron( bool asynchron );
	/*@}*/
	
	
	
	/**
	 * \name Internal Use Only
	 * \warning Internal Use Only. Do not call!
	 */
	/*@{*/
	inline bool GetOutdated() const{ return pOutdated; }
	void MarkOutdated();
	/*@}*/
};

#endif
