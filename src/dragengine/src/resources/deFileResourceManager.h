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

#ifndef _DEFILERESOURCEMANAGER_H_
#define _DEFILERESOURCEMANAGER_H_

#include "deResourceManager.h"
#include "../common/utils/decDateTime.h"

class decBaseFileReader;
class decBaseFileWriter;
class decPath;
class deVirtualFileSystem;


/**
 * \brief Base File Resource Manager Class.
 *
 * Provides basic support for resource managers. The actual management
 * of resource the subclass should implement using the resource list
 * class provided (although you are free to do differently if you really
 * know what you are doing). The RemoveResource function is protected
 * and has to be used only by a deResource derived object to notify the
 * resource manager that this object has to be freed from memory. Do not
 * do any reference counting inside the subclass. If a resource is ready
 * to be freed it will tell you by calling RemoveResource.
 */
class deFileResourceManager : public deResourceManager{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file resource manager. */
	deFileResourceManager( deEngine *engine, eResourceType type );
	
	/** \brief Clean up resource manager. */
	virtual ~deFileResourceManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Path to file relative to base path.
	 * \returns true if found and \em path has been set otherwise false.
	 */
	bool FindFileForReading( decPath &path, const deVirtualFileSystem &vfs,
		const char *filename, const char *basePath ) const;
	
	/** \brief Open file for reading using. */
	decBaseFileReader *OpenFileForReading( const deVirtualFileSystem &vfs,
		const char *filename ) const;
	
	/** \brief Open file for writing using. */
	decBaseFileWriter *OpenFileForWriting( const deVirtualFileSystem &vfs,
		const char *filename ) const;
	/*@}*/
};

#endif
