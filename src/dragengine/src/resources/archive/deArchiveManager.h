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

#ifndef _DEARCHIVEMANAGER_H_
#define _DEARCHIVEMANAGER_H_

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

class deArchive;
class deArchiveContainer;


/**
 * \brief Archive resource manager.
 */
class deArchiveManager : public deFileResourceManager{
private:
	deFileResourceList pArchives;
	
	deArchiveContainer *pContainerRoot;
	deArchiveContainer *pContainerTail;
	int pContainerCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create archive resource manager. */
	deArchiveManager( deEngine *engine );
	
	/** \brief Clean up archive resource manager and report leaking resources. */
	virtual ~deArchiveManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of archives. */
	int GetArchiveCount() const;
	
	/** \brief Root archive resource for iteration purpose. */
	deArchive *GetRootArchive() const;
	
	/** \brief Archive with filename or NULL if not loaded yet. */
	deArchive *GetArchiveWith( const char *filename ) const;
	
	/** \brief Archive with filename or NULL if not loaded yet. */
	deArchive *GetArchiveWith( deVirtualFileSystem *vfs, const char *filename ) const;
	
	/** \brief Open archive from file relative to base path. */
	deArchive *OpenArchive( const char *filename, const char *basePath );
	
	/** \brief Open archive from file relative to base path. */
	deArchive *OpenArchive( deVirtualFileSystem *vfs, const char *filename, const char *basePath );
	
	/** \brief Create archive container. */
	deArchiveContainer *CreateContainer( const decPath &rootPath,
		deArchive *archive, const decPath &archivePath );
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * \warning For internal use only. Never call on your own!
	 */
	/*@{*/
	virtual void RemoveResource( deResource *resource );
	void RemoveContainer( deArchiveContainer *container );
	/*@}*/
};

#endif
