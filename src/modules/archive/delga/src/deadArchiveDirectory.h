/* 
 * Drag[en]gine DELGA Archive Module
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

#ifndef _DEADARCHIVEDIRECTORY_H_
#define _DEADARCHIVEDIRECTORY_H_

#include "unzip.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/utils/decDateTime.h>
#include <dragengine/filesystem/dePathList.h>


class deArchiveDelga;
class deadArchiveFile;
class deVirtualFileSystem;
class deFileSearchVisitor;


/**
 * \brief Archive directory entry.
 * 
 * Directories are virtual. They are added if one or more file uses the directory.
 */
class deadArchiveDirectory : public deObject{
public:
	typedef deTObjectReference<deadArchiveDirectory> Ref;
	
	
	
private:
	deArchiveDelga &pModule;
	
	decString pFilename;
	decObjectOrderedSet pDirectories;
	decObjectOrderedSet pFiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create directory entry. */
	deadArchiveDirectory( deArchiveDelga &module, const char *filename );
	
	/** \brief Clean up directory entry. */
	virtual ~deadArchiveDirectory();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	/*@}*/
	
	
	
	/** \name Directories */
	/*@{*/
	/** \brief Number of directories. */
	int GetDirectoryCount() const;
	
	/** \brief Directory at index. */
	deadArchiveDirectory *GetDirectoryAt( int index ) const;
	
	/** \brief Directory with filename is present. */
	bool HasDirectoryNamed( const char *filename ) const;
	
	/** \brief Directory by filename or \em NULL if absent. */
	deadArchiveDirectory *GetDirectoryNamed( const char *filename ) const;
	
	/** \brief Directory by filename adding it if absent. */
	deadArchiveDirectory *GetOrAddDirectoryNamed( const char *filename );
	
	/** \brief Directory by path or \em NULL if absent. */
	deadArchiveDirectory *GetDirectoryByPath( const decPath &path );
	
	/** \brief Add directory. */
	void AddDirectory( deadArchiveDirectory *directory );
	/*@}*/
	
	
	
	/** \name Files */
	/*@{*/
	/** \brief Number of files. */
	int GetFileCount() const;
	
	/** \brief File at index. */
	deadArchiveFile *GetFileAt( int index ) const;
	
	/** \brief File with filename is present. */
	bool HasFileNamed( const char *filename ) const;
	
	/** \brief File by filename or \em NULL if absent. */
	deadArchiveFile *GetFileNamed( const char *filename ) const;
	
	/** \brief File by path or \em NULL if absent. */
	deadArchiveFile *GetFileByPath( const decPath &path ) const;
	
	/** \brief Add file. */
	void AddFile( deadArchiveFile *file );
	/*@}*/
};

#endif

