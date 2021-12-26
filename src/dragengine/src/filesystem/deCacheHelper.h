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

#ifndef _DECACHEHELPER_H_
#define _DECACHEHELPER_H_

#include "../common/string/decStringList.h"
#include "../common/file/decPath.h"

class deLogger;
class decBaseFileWriter;
class decBaseFileReader;
class deVirtualFileSystem;


/**
 * \brief Helper for accessing files in a virtual file system cache.
 * 
 * The cache helper organizes files in a cache virtual file directory
 * using a unique string identifier of variable length. The identifer
 * can be any kind of string typically a file path or concatenations
 * of file path and other information. The cache helper stores these
 * files as fN with N starting at 0. The file mapping is kept in memory
 * as a slot list so new files are stored at the next free slot. The
 * files themselves store first a 2-byte length value followed by the
 * identifier followed by the actual cache file content. While opening
 * or saving a file a file reader/writer is returned with the file
 * pointer set to the starting position of the cache content. The user
 * of the cache helper is responsible to write into the cache content
 * any data required to detect outdated cache content. The file mapping
 * is not stored on disk. Each time the helper is created the files in
 * the cache directory are scanned for their identifier and the mapping
 * build from them.
 */
class DE_DLL_EXPORT deCacheHelper{
public:
	/** \brief Compression methods for new cached files. */
	enum eCompressionMethods{
		/** \brief Do not compress new cached file content. */
		ecmNoCompression,
		
		/** \brief Compress new cached file content using Zlib compression. */
		ecmZCompression
	};
	
	
	
private:
	deVirtualFileSystem *pVFS;
	decPath pCachePath;
	
	decStringList pMapping;
	
	eCompressionMethods pCompressionMethod;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create cache helper building the mapping from the cache directory. */
	deCacheHelper( deVirtualFileSystem *vfs, const decPath &cachePath );
	
	/** \brief Clean up cache helper. */
	~deCacheHelper();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Compression method to use for new cached files. */
	inline eCompressionMethods GetCompressionMethod() const{ return pCompressionMethod; }
	
	/** \brief Set compression method to use for new cached files. */
	void SetCompressionMethod( eCompressionMethods compressionMethod );
	
	
	
	/**
	 * \brief Open cache file by identifier for reading if existing.
	 * \returns NULL if file is absent.
	 */
	decBaseFileReader *Read( const char *id );
	
	/** \brief Open cache file by identifier for writing. */
	decBaseFileWriter *Write( const char *id );
	
	/** \brief Delete cache file by identifier if present. */
	void Delete( const char *id );
	
	/** \brief Delete all cache files. */
	void DeleteAll();
	
	/** \brief Build file mapping by scanning all files in the cache directory. */
	void BuildMapping();
	
	/** \brief Debug print stats about the cache to a logger. */
	void DebugPrint( deLogger &logger, const char *loggingSource );
	/*@}*/
};

#endif
