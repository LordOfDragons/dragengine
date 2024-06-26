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

#ifndef _DEVFSDISKDIRECTORY_H_
#define _DEVFSDISKDIRECTORY_H_

#include "deVFSContainer.h"


/**
 * \brief Container mapping the content of a disk directory into the virtual file system.
 */
class DE_DLL_EXPORT deVFSDiskDirectory : public deVFSContainer{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deVFSDiskDirectory> Ref;
	
	
	
private:
	const decPath pDiskPath;
	bool pReadOnly;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create disk directory with mapping disk path to root path. */
	deVFSDiskDirectory( const decPath &diskPath );
	
	/** \brief Create disk directory mapping disk path to root path. */
	deVFSDiskDirectory( const decPath &rootPath, const decPath &diskPath );
	
	/**
	 * \brief Create disk directory mapping disk path to root path.
	 * \version 1.5
	 */
	deVFSDiskDirectory( const decPath &rootPath, const decPath &diskPath, bool readonly );
	
protected:
	/**
	 * \brief Clean up disk directory.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deVFSDiskDirectory();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Disk path to map to the virtual file system. */
	inline const decPath &GetDiskPath(){ return pDiskPath; }
	
	/** \brief Disk path is read only. */
	inline bool GetReadOnly() const{ return pReadOnly; }
	
	/** \brief Set if disk path is read only. */
	void SetReadOnly( bool readOnly );
	
	
	
	/**
	 * \brief File exists.
	 * 
	 * Path is relative to the root path.
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
	 * 
	 * All found files and directories are added to the file listing.
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
	
private:
	void pEnsureDirectoryExists( const decPath &path );
};

#endif
