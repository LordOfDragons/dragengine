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

