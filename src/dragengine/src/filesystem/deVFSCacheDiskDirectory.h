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

#ifndef _DEVFSCACHEDISKDIRECTORY_H_
#define _DEVFSCACHEDISKDIRECTORY_H_

#include "deVFSDiskDirectory.h"


/**
 * \brief Virtual file system cache disk directory.
 * 
 * Extension to the regular disk directory container adding cache handling.
 * Keeps track of the consumed disk space deleting the oldest files if too
 * much space would be consumed.
 */
class deVFSCacheDiskDirectory : public deVFSDiskDirectory{
private:
	int pMaxCacheSize;
	int pCacheSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create cache disk directory mapping a disk path to the root path. */
	deVFSCacheDiskDirectory( const decPath &diskPath );
	
	/** \brief Create cache disk directory mapping a disk path to a root path. */
	deVFSCacheDiskDirectory( const decPath &rootPath, const decPath &diskPath );
	
protected:
	/**
	 * \brief Clean up virtual file system disk directory.
	 * 
	 * \note
	 * Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deVFSCacheDiskDirectory();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Maximum cache in kilobytes. */
	inline int GetMaxCacheSize() const{ return pMaxCacheSize; }
	
	/** \brief Set maximum cache size in kilobytes. */
	void SetMaxCacheSize( int kilobytes );
	
	/** \brief Current cache size in kilobytes. */
	inline int GetCacheSize() const{ return pCacheSize; }
	
	/** \brief Set current cache size in kilobytes. */
	void SetCacheSize( int kilobytes );
	
	
	
	/** \brief Calculate size of cache in bytes. */
	uint64_t CalculateCacheSize();
	
	/** \brief Calculate size of a native path directory in bytes. */
	uint64_t CalculateDirectorySize( const char *path );
	
	/** \brief Find the oldest file. */
	bool FindOldestFiles( decPath &oldestFile, uint64_t &oldestFileSize );
	
	/** \brief Scan directory for the oldest. */
	void FindOldestFilesIn( decPath &oldestFile, uint64_t &oldestFileSize,
		TIME_SYSTEM &oldestAccesTime, bool &hasOldestFile, const char *path );
	
	/** \brief Update the cache deleting old files if required. */
	virtual void UpdateCache();
	
	/**
	 * \brief Open file for writing.
	 * 
	 * The path is relative to the root path. If the file can not be
	 * found an exception is raised. Use the CanWriteFile function to
	 * test if a file can be opened for writing. Not existing
	 * directories have to be created if the CanWriteFile function
	 * returns true for a file whose parent directory does not exist yet.
	 */
	virtual decBaseFileWriter *OpenFileForWriting( const decPath &path );
	
	/**
	 * \brief Delete file.
	 * 
	 * Path is relative to the root path.
	 */
	virtual void DeleteFile( const decPath &path );
	/*@}*/
};

#endif
