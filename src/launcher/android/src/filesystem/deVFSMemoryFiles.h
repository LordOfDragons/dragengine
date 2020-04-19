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

#ifndef _DEVFSMEMORYFILES_H_
#define _DEVFSMEMORYFILES_H_

#include "deVFSContainer.h"
#include "dePathList.h"
#include "../common/collection/decObjectList.h"

class decMemoryFile;


/**
 * \brief Container mapping a list of memory files into the virtual file system.
 * 
 * Memory files are chunks of data in memory representing files. This
 * class is useful to add a small number of individual memory files
 * to the virtual file system. Each memory file is an individual chunk
 * of memory. For mapping a large block of memory into a virtual file
 * system containing various files inside this memory block the block
 * memory file class is better suited.
 */
class deVFSMemoryFiles : public deVFSContainer{
private:
	decObjectList pFiles;
	dePathList pDirectories;
	bool pDirtyDirectories;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create memory files with root path. */
	deVFSMemoryFiles( const decPath &rootPath );
	
protected:
	/**
	 * \brief Clean up memory files.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deVFSMemoryFiles();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
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
	
	/**
	 * \brief Search all visible files and directories.
	 */
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
	
	
	
	/** \name Memory Files */
	/*@{*/
	/** \brief Number of memory files. */
	int GetMemoryFileCount() const;
	
	/** \brief Memory file at position. */
	decMemoryFile *GetMemoryFileAt( int index ) const;
	
	/** \brief Index of memory file or -1 if absent. */
	int IndexOfMemoryFile( decMemoryFile *memoryFile ) const;
	
	/** \brief Index of memory file with path or -1 if absent. */
	int IndexOfMemoryFileWith( const char *path ) const;
	
	/** \brief Memory file is present. */
	bool HasMemoryFile( decMemoryFile *memoryFile ) const;
	
	/** \brief Memory file with path is present. */
	bool HasMemoryFileWith( const char *path ) const;
	
	/** \brief Add memory file. */
	void AddMemoryFile( decMemoryFile *memoryFile );
	
	/** \brief Remove memory file. */
	void RemoveMemoryFile( decMemoryFile *memoryFile );
	
	/** \brief Remove all memory files. */
	void RemoveAllMemoryFiles();
	/*@}*/
	
	
	
private:
	void pRebuildDirectories();
};

#endif
