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

#ifndef _DEVFSREDIRECT_H_
#define _DEVFSREDIRECT_H_

#include "deVFSContainer.h"

class deVirtualFileSystem;


/**
 * \brief Container redirecting to a virtual file system.
 * 
 * Can redirect to another container or an entire virtual file system.
 * 
 * \warning Do not produce cycling path links. Containers get relative path so it is
 * not possible for them to detect cycling path links resulting in stack
 * overflows.
 */
class deVFSRedirect : public deVFSContainer{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deVFSRedirect> Ref;
	
	
	
private:
	const decPath pRedirectPath;
	deVFSContainer *pContainer;
	deVirtualFileSystem *pVFS;
	bool pHoldVFSReference;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create container redirecting to \em redirectPath in a container.
	 * 
	 * \param[in] rootPath Root path the container is visible at.
	 * \param[in] redirectPath Path to redirect to in \em vfs.
	 * \param[in] container Container to redirect to.
	 */
	deVFSRedirect( const decPath &rootPath, const decPath &redirectPath, deVFSContainer *container );
	
	/**
	 * \brief Create container redirecting to \em redirectPath in a virtual file system.
	 * 
	 * \warning Make sure to set \em holdVFSReference correctly. If wrong the virtual
	 * file system leaks. If the virtual file system is the same as the one
	 * the container is going to be added to use false to avoid leaking.
	 * If the container is a foreign virtual file system use true or
	 * the link turns suddenly dead resulting in segmentation faults.
	 * 
	 * \param[in] rootPath Root path the container is visible at.
	 * \param[in] redirectPath Path to redirect to in \em vfs.
	 * \param[in] vfs Virtual file system to redirect to.
	 * \param[in] holfVFSReference Container holds a reference to \em vfs.
	 */
	deVFSRedirect( const decPath &rootPath, const decPath &redirectPath,
		deVirtualFileSystem *vfs, bool holdVFSReference );
	
protected:
	/**
	 * \brief Clean up container.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deVFSRedirect();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Redirect path in the same virtual file system. */
	inline const decPath &GetRedirectPath(){ return pRedirectPath; }
	
	
	
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
};

#endif
