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

#ifndef _DEARCHIVE_H_
#define _DEARCHIVE_H_

#include "../deFileResource.h"

class deArchiveContainer;
class deArchiveManager;
class deBaseArchiveContainer;


/**
 * \brief Archive Resource.
 *
 * Provides access to files stored in a file archive. The archive can be compressed
 * or just grouping files. The archive can be placed in a virtual file system by
 * obtaining a deArchiveContainer.
 * 
 * To actually access the files you have to create an Archive Container using
 * deArchiveManager::CreateContainer(). Archive containers can be read-write if the
 * underlaying Archive Module supports writing to the archive file. In general
 * archive containers are read-only.
 */
class DE_DLL_EXPORT deArchive : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deArchive> Ref;
	
	
	
private:
	deBaseArchiveContainer *pPeerContainer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create archive resource. */
	deArchive( deArchiveManager *manager, deVirtualFileSystem *vfs,
		const char *filename, TIME_SYSTEM modificationTime );
	
protected:
	/**
	 * \brief Clean up archive resource.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deArchive();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/*@}*/
	
	
	
	/** \name Peers */
	/*@{*/
	/** \brief Container peer. */
	deBaseArchiveContainer *GetPeerContainer() const{ return pPeerContainer; }
	
	/** \brief Set peer container. */
	void SetPeerContainer( deBaseArchiveContainer *peer );
	/*@}*/
};

#endif
