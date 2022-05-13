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

#ifndef _DEVFSCONTAINER_H_
#define _DEVFSCONTAINER_H_

#include <stdint.h>

#include "../deObject.h"
#include "../common/file/decPath.h"
#include "../common/utils/decDateTime.h"

class deContainerFileSearch;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Virtual file system container.
 * 
 * A virtual file system container provides a collection of real or virtuel
 * files. Containers can be queried for a listing of files, the existance
 * of files and to open files for reading and writing. Each container has
 * a path assigned where the root of the container is. All files and
 * directories inside the container are inserted into the virtual file
 * system under this path. The content does not replace the content of the
 * given path as a mount command would do but adds the its own content to
 * the content already existing. Duplicate files are shadowed in this
 * process. The order of containers therefore matters.
 * 
 * Containers can be hidden. Hidden containers do not show up in file
 * listings unless the listing is done from a path inside the container
 * path. This allows to add special containers that should not be easily
 * visible to game scripts unless the path is known.
 */
class DE_DLL_EXPORT deVFSContainer : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deVFSContainer> Ref;
	
	
	
public:
	/** \brief File types */
	enum eFileTypes{
		/** \brief Regular file. */
		eftRegularFile,
		
		/** \brief Directory. */
		eftDirectory,
		
		/** \brief A special file which is neither a regular file nor a directory. */
		eftSpecial
	};
	
	
	
private:
	const decPath pRootPath;
	bool pHidden;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create virtual file system container with the root path '/'. */
	deVFSContainer();
	
	/** \brief Create virtual file system container with the given root path. */
	deVFSContainer( const decPath &rootPath );
	
protected:
	/**
	 * \brief Clean up virtual file system container.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deVFSContainer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Root path. */
	inline const decPath &GetRootPath(){ return pRootPath; }
	
	/** \brief Container is hidden. */
	inline bool GetHidden() const{ return pHidden; }
	
	/** \brief Set if container is hidden. */
	void SetHidden( bool hidden );
	
	/**
	 * \brief File exists.
	 * 
	 * Path is elative to the root path.
	 */
	virtual bool ExistsFile( const decPath &path ) = 0;
	
	/**
	 * \brief File can be read.
	 * 
	 * Path is relative to the root path. The result of this call
	 * is usually the same as of ExistsFile unless permissions prevent
	 * reading of an existing file.
	 */
	virtual bool CanReadFile( const decPath &path ) = 0;
	
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
	virtual bool CanWriteFile( const decPath &path ) = 0;
	
	/**
	 * \brief File can be deleted.
	 * 
	 * The path is relative to the root path.
	 */
	virtual bool CanDeleteFile( const decPath &path ) = 0;
	
	
	
	/**
	 * \brief Open file for reading.
	 * 
	 * The path is relative to the root path. If the file can not be
	 * found an exception is raised. Use the CanReadFile function to
	 * test if a file can be opened for reading.
	 */
	virtual decBaseFileReader *OpenFileForReading( const decPath &path ) = 0;
	
	/**
	 * \brief Open file for writing.
	 * 
	 * The path is relative to the root path. If the file can not be
	 * found an exception is raised. Use the CanWriteFile function to
	 * test if a file can be opened for writing. Not existing
	 * directories have to be created if the CanWriteFile function
	 * returns true for a file whose parent directory does not exist yet.
	 */
	virtual decBaseFileWriter *OpenFileForWriting( const decPath &path ) = 0;
	
	/**
	 * \brief Delete file.
	 * 
	 * Path is relative to the root path.
	 */
	virtual void DeleteFile( const decPath &path ) = 0;
	
	/** \brief Touch file setting the modification time to the current time. */
	virtual void TouchFile( const decPath &path ) = 0;
	
	/**
	 * \brief Search all visible files and directories.
	 * 
	 * All found files and directories are added to the file listing.
	 */
	virtual void SearchFiles( const decPath &directory, deContainerFileSearch &searcher ) = 0;
	
	/**
	 * \brief Type of file.
	 * 
	 * If the file does not exist an exception is thrown.
	 */
	virtual eFileTypes GetFileType( const decPath &path ) = 0;
	
	/**
	 * \brief Size of file.
	 * 
	 * If the file does not exist an exception is thrown.
	 */
	virtual uint64_t GetFileSize( const decPath &path ) = 0;
	
	/**
	 * \brief Modification time of file.
	 * 
	 * If the file does not exist an exception is thrown.
	 */
	virtual TIME_SYSTEM GetFileModificationTime( const decPath &path ) = 0;
	/*@}*/
};

#endif
