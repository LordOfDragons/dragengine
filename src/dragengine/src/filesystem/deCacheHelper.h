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

#ifndef _DECACHEHELPER_H_
#define _DECACHEHELPER_H_

#include "../common/string/decStringList.h"
#include "../common/file/decPath.h"

class deLogger;
class decBaseFileWriter;
class decBaseFileReader;
#include "deVirtualFileSystem.h"


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
	deVirtualFileSystem::Ref pVFS;
	decPath pCachePath;
	
	decStringList pMapping;
	
	eCompressionMethods pCompressionMethod;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create cache helper building the mapping from the cache directory. */
	deCacheHelper(deVirtualFileSystem *vfs, const decPath &cachePath);
	
	/** \brief Clean up cache helper. */
	~deCacheHelper();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Compression method to use for new cached files. */
	inline eCompressionMethods GetCompressionMethod() const{ return pCompressionMethod; }
	
	/** \brief Set compression method to use for new cached files. */
	void SetCompressionMethod(eCompressionMethods compressionMethod);
	
	
	
	/**
	 * \brief Open cache file by identifier for reading if existing.
	 * \returns NULL if file is absent.
	 */
	decBaseFileReader *Read(const char *id);
	
	/** \brief Open cache file by identifier for writing. */
	decBaseFileWriter *Write(const char *id);
	
	/** \brief Delete cache file by identifier if present. */
	void Delete(const char *id);
	
	/** \brief Delete all cache files. */
	void DeleteAll();
	
	/** \brief Build file mapping by scanning all files in the cache directory. */
	void BuildMapping();
	
	/** \brief Debug print stats about the cache to a logger. */
	void DebugPrint(deLogger &logger, const char *loggingSource);
	/*@}*/
};

#endif
