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
class DE_DLL_EXPORT deVFSCacheDiskDirectory : public deVFSDiskDirectory{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deVFSCacheDiskDirectory> Ref;
	
	
private:
	int pMaxCacheSize;
	int pCacheSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create cache disk directory mapping a disk path to the root path. */
	deVFSCacheDiskDirectory(const decPath &diskPath);
	
	/** \brief Create cache disk directory mapping a disk path to a root path. */
	deVFSCacheDiskDirectory(const decPath &rootPath, const decPath &diskPath);
	
protected:
	/**
	 * \brief Clean up virtual file system disk directory.
	 * 
	 * \note
	 * Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deVFSCacheDiskDirectory() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Maximum cache in kilobytes. */
	inline int GetMaxCacheSize() const{ return pMaxCacheSize; }
	
	/** \brief Set maximum cache size in kilobytes. */
	void SetMaxCacheSize(int kilobytes);
	
	/** \brief Current cache size in kilobytes. */
	inline int GetCacheSize() const{ return pCacheSize; }
	
	/** \brief Set current cache size in kilobytes. */
	void SetCacheSize(int kilobytes);
	
	
	
	/** \brief Calculate size of cache in bytes. */
	uint64_t CalculateCacheSize();
	
	/** \brief Calculate size of a native path directory in bytes. */
	uint64_t CalculateDirectorySize(const char *path);
	
	/** \brief Find the oldest file. */
	bool FindOldestFiles(decPath &oldestFile, uint64_t &oldestFileSize);
	
	/** \brief Scan directory for the oldest. */
	void FindOldestFilesIn(decPath &oldestFile, uint64_t &oldestFileSize,
		TIME_SYSTEM &oldestAccesTime, bool &hasOldestFile, const char *path);
	
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
	decBaseFileWriter *OpenFileForWriting(const decPath &path) override;
	
	/**
	 * \brief Delete file.
	 * 
	 * Path is relative to the root path.
	 */
	void DeleteFile(const decPath &path) override;
	/*@}*/
};

#endif
