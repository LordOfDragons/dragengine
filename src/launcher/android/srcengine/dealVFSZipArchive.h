/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALVFSZIPARCHIVE_H_
#define _DEALVFSZIPARCHIVE_H_

#include "unzip.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/threading/deMutex.h>

class dealEngineInstance;



/**
 * \brief Container mapping zip archive into virtual file system.
 * 
 * Only read-only supported.
 */
class dealVFSZipArchive : public deVFSContainer{
private:
	/** \brief Archive file. */
	class cArchiveFile : public deObject{
	private:
		/** \brief Filename. */
		decString pFilename;
		
		/** \brief Position in zip file. */
		unz_file_pos pArchivePosition;
		
		/** \brief Uncompressed file size. */
		uint64_t pFileSize;
		
		/** \brief Modification time. */
		TIME_SYSTEM pModificationTime;
		
		
		
	public:
		/** \brief Create archive file. */
		cArchiveFile( const char *filename, const unz_file_pos &archivePosition,
			uint64_t fileSize, TIME_SYSTEM modificationTime );
		
	protected:
		/** \brief Clean up archive file. */
		virtual ~cArchiveFile();
		
		
		
	public:
		/** \brief Filename. */
		inline const decString &GetFilename() const{ return pFilename; }
		
		/** \brief Position in zip file. */
		inline const unz_file_pos &GetArchivePosition() const{ return pArchivePosition; }
		
		/** \brief Uncompressed file size. */
		inline uint64_t GetFileSize() const{ return pFileSize; }
		
		/** \brief Modification time. */
		inline TIME_SYSTEM GetModificationTime() const{ return pModificationTime; }
	};
	
	
	
	/** \brief Archive directory. */
	class cArchiveDirectory : public deObject{
	private:
		/** \brief Filename. */
		decString pFilename;
		
		/** \brief Directories. */
		decObjectOrderedSet pDirectories;
		
		/** \brief Files. */
		decObjectOrderedSet pFiles;
		
		
		
	public:
		/** \brief Create archive file. */
		cArchiveDirectory( const char *filename );
		
	protected:
		/** \brief Clean up archive file. */
		virtual ~cArchiveDirectory();
		
		
		
	public:
		/** \brief Filename. */
		inline const decString &GetFilename() const{ return pFilename; }
		
		
		
		/** \brief Number of directories. */
		int GetDirectoryCount() const;
		
		/** \brief Directory at index. */
		cArchiveDirectory *GetDirectoryAt( int index ) const;
		
		/** \brief Directory with filename is present. */
		bool HasDirectoryNamed( const char *filename ) const;
		
		/** \brief Directory by filename or \em NULL if absent. */
		cArchiveDirectory *GetDirectoryNamed( const char *filename ) const;
		
		/** \brief Directory by filename adding it if absent. */
		cArchiveDirectory *GetOrAddDirectoryNamed( const char *filename );
		
		/** \brief Directory by path or \em NULL if absent. */
		cArchiveDirectory *GetDirectoryByPath( const decPath &path );
		
		/** \brief Add directory. */
		void AddDirectory( cArchiveDirectory *directory );
		
		
		
		/** \brief Number of files. */
		int GetFileCount() const;
		
		/** \brief File at index. */
		cArchiveFile *GetFileAt( int index ) const;
		
		/** \brief File with filename is present. */
		bool HasFileNamed( const char *filename ) const;
		
		/** \brief File by filename or \em NULL if absent. */
		cArchiveFile *GetFileNamed( const char *filename ) const;
		
		/** \brief File by path or \em NULL if absent. */
		cArchiveFile *GetFileByPath( const decPath &path ) const;
		
		/** \brief Add file. */
		void AddFile( cArchiveFile *file );
	};
	
	
	
private:
	dealEngineInstance &pEngineInstance;
	decBaseFileReader *pFileReader;
	unzFile pZipFile;
	cArchiveDirectory *pArchiveDirectory;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create container. */
	dealVFSZipArchive( dealEngineInstance &engineInstance,
		decBaseFileReader *fileReader, const decPath &pathRoot );
	
protected:
	/**
	 * \brief Clean up disk directory.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~dealVFSZipArchive();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief File reader.
	 * \warning For internal use only.
	 */
	inline decBaseFileReader &GetFileReader() const{ return *pFileReader; }
	
	
	
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
	void pCleanUp();
	void pBuildFileTable();
	void pLogArchiveContent();
	void pLogArchiveContent( const cArchiveDirectory &directory, const char *prefix );
	void pOpenZipFile();
};

#endif
