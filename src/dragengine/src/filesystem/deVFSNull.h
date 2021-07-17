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

#ifndef _DEVFSNULL_H_
#define _DEVFSNULL_H_

#include "deVFSContainer.h"


/**
 * \brief Null virtual file system container.
 * 
 * A container that is always empty. Acts as a sink where all written files are
 * immediately forgotten. Useful as dummy container for volatile data directories
 * where no permanent storage is desired or possible.
 */
class deVFSNull : public deVFSContainer{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deVFSNull> Ref;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create null container with root path. */
	deVFSNull( const decPath &rootPath );
	
protected:
	/**
	 * \brief Clean up container.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deVFSNull();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief File exists.
	 * \returns false.
	 */
	virtual bool ExistsFile( const decPath &path );
	
	/**
	 * \brief File can be read from.
	 * \returns false.
	 */
	virtual bool CanReadFile( const decPath &path );
	
	/**
	 * \brief File can be written to.
	 * \returns true.
	 */
	virtual bool CanWriteFile( const decPath &path );
	
	/**
	 * \brief File can be deleted.
	 * \returns true.
	 */
	virtual bool CanDeleteFile( const decPath &path );
	
	
	
	/**
	 * \brief Open file for reading.
	 * 
	 * Throws file not found exception.
	 */
	virtual decBaseFileReader *OpenFileForReading( const decPath &path );
	
	/**
	 * \brief Open file for writing.
	 * \returns Null writer.
	 */
	virtual decBaseFileWriter *OpenFileForWriting( const decPath &path );
	
	/**
	 * \brief Delete file.
	 * 
	 * Does nothing.
	 */
	virtual void DeleteFile( const decPath &path );
	
	/**
	 * \brief Touch file setting the modification time to the current time.
	 * 
	 * Does nothing.
	 */
	virtual void TouchFile( const decPath &path );
	
	/**
	 * \brief Search for all files and directories.
	 */
	virtual void SearchFiles( const decPath &directory, deContainerFileSearch &searcher );
	
	/**
	 * \brief Type of file.
	 * 
	 * Throws file not found exception.
	 */
	virtual eFileTypes GetFileType( const decPath &path );
	
	/**
	 * \brief Size of file.
	 * 
	 * Throws file not found exception.
	 */
	virtual uint64_t GetFileSize( const decPath &path );
	
	/**
	 * \brief Modification time of file.
	 * 
	 * Throws file not found exception.
	 */
	virtual TIME_SYSTEM GetFileModificationTime( const decPath &path );
	/*@}*/
};

#endif
