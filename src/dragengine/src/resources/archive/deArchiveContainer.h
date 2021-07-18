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

#ifndef _DEARCHIVECONTAINER_H_
#define _DEARCHIVECONTAINER_H_

#include "../../filesystem/deVFSContainer.h"

class deArchive;


/**
 * \brief Archive resource virtual file system container.
 *
 * Shows the content of an archive in a virtual file system. Containers can be
 * read-write if the underlaying Archive Module supports writing to the archive
 * file. In general archive containers are read-only.
 */
class deArchiveContainer : public deVFSContainer{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deArchiveContainer> Ref;
	
	
	
private:
	const decPath pArchivePath;
	deArchive *pArchive;
	
	deArchiveContainer *pLLManagerPrev;
	deArchiveContainer *pLLManagerNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create archive container.
	 * \warning For internal use only. Never call on your own!
	 */
	deArchiveContainer( const decPath &rootPath, deArchive *archive,
		const decPath &archivePath );
	
protected:
	/**
	 * \brief Clean up archive container.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deArchiveContainer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Archive. */
	inline deArchive *GetArchive() const{ return pArchive; }
	
	/** \brief Archive path to map as root. */
	inline const decPath &GetArchivePath(){ return pArchivePath; }
	
	
	
	/**
	 * \brief File exists.
	 * 
	 * Path is elative to the root path.
	 */
	virtual bool ExistsFile( const decPath &path );
	
	/**
	 * \brief File can be read.
	 * 
	 * Path is relative to the root path. The result of this call
	 * is usually the same as of ExistsFile unless permissions prevent
	 * reading of an existing file.
	 */
	virtual bool CanReadFile( const decPath &path );
	
	/**
	 * \brief File can be written.
	 * 
	 * The path is relative to the root path. If a file has parent
	 * directories not existing yet they have to be created in a
	 * subsequent call to OpenFileForWriting. If this is the case
	 * return true only if the creation of the parent directories
	 * is also allowed in addition to creating a new file. If the
	 * file exists permission flags can prevent writing.
	 */
	virtual bool CanWriteFile( const decPath &path );
	
	/**
	 * \brief File can be deleted.
	 * 
	 * The path is relative to the root path.
	 */
	virtual bool CanDeleteFile( const decPath &path );
	
	/**
	 * \brief Open file for reading.
	 * 
	 * The path is relative to the root path. If the file can not be
	 * found an exception is raised. Use the CanReadFile function to
	 * test if a file can be opened for reading.
	 * 
	 * \note A note to module developers. You have to return an instance of decWeakFileReader
	 * wrapping the actual file reader to be able to safely drop it when the module is
	 * unloaded while the reader is still held by somebody.
	 */
	virtual decBaseFileReader *OpenFileForReading( const decPath &path );
	
	/**
	 * \brief Open file for writing.
	 * 
	 * The path is relative to the root path. If the file can not be
	 * found an exception is raised. Use the CanWriteFile function to
	 * test if a file can be opened for writing. Not existing
	 * directories have to be created if the CanWriteFile function
	 * returns true for a file whose parent directory does not exist yet.
	 * 
	 * \note A note to module developers. You have to return an instance of decWeakFileWriter
	 * wrapping the actual file writer to be able to safely drop it when the module is
	 * unloaded while the writer is still held by somebody.
	 */
	virtual decBaseFileWriter *OpenFileForWriting( const decPath &path );
	
	/**
	 * \brief Delete file.
	 * 
	 * Path is relative to the root path.
	 */
	virtual void DeleteFile( const decPath &path );
	
	/** \brief Touch file setting the modification time to the current time. */
	virtual void TouchFile( const decPath &path );
	
	/** \brief Search files. */
	virtual void SearchFiles( const decPath &directory, deContainerFileSearch &searcher );
	
	/**
	 * \brief Type of file.
	 * 
	 * If the file does not exist an exception is thrown.
	 */
	virtual eFileTypes GetFileType( const decPath &path );
	
	/**
	 * \brief Size of file.
	 * 
	 * If the file does not exist an exception is thrown.
	 */
	virtual uint64_t GetFileSize( const decPath &path );
	
	/**
	 * \brief Modification time of file.
	 * 
	 * If the file does not exist an exception is thrown.
	 */
	virtual TIME_SYSTEM GetFileModificationTime( const decPath &path );
	/*@}*/
	
	
	
	/**
	 * \name Resource manager linked list
	 * \warning For internal use only. Never call on your own!
	 */
	/*@{*/
	/** \brief Previous resource in the resource manager linked list. */
	inline deArchiveContainer *GetLLManagerPrev() const{ return pLLManagerPrev; }
	
	/**
	 * \brief Set next resource in the resource manager linked list.
	 * \warning For internal use only. Never call on your own!
	 */
	void SetLLManagerPrev( deArchiveContainer *resource );
	
	/** \brief Next resource in the resource manager linked list. */
	inline deArchiveContainer *GetLLManagerNext() const{ return pLLManagerNext; }
	
	/**
	 * \brief Set next resource in the resource manager linked list.
	 * \warning For internal use only. Never call on your own!
	 */
	void SetLLManagerNext( deArchiveContainer *resource );
	
	/**
	 * \brief Marks the resource leaking.
	 * \warning For internal use only. Never call on your own!
	 */
	void MarkLeaking();
	/*@}*/
};

#endif
