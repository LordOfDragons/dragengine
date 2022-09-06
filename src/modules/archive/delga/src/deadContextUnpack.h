/* 
 * Drag[en]gine DELGA Archive Module
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

#ifndef _DEADCONTEXTUNPACK_H_
#define _DEADCONTEXTUNPACK_H_

#include "deadArchiveDirectory.h"
#include "deadArchiveFileReader.h"
#include "unzip.h"

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/utils/decDateTime.h>


class deArchiveDelga;
class deadContainer;
class deadArchiveFile;
class decWeakFileWriter;



/**
 * Contexts are claimed by file readers to unpack file content. If not claimed contexts
 * reside in a list of claimable contexts to reduce overhead.
 */
class deadContextUnpack{
private:
	deArchiveDelga &pModule;
	deadContainer *pContainer;
	decBaseFileReader::Ref pReader;
	
	unzFile pZipFile;
	bool pZipFileOpen;
	unz_file_pos pArchiveFilePosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create unpacking context. */
	deadContextUnpack( deArchiveDelga &module, deadContainer *container );
	
	/** Clean up unpacking context. */
	~deadContextUnpack();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deArchiveDelga &GetModule() const{ return pModule; }
	
	/** Container or \em NULL if dropped. */
	inline deadContainer *GetContainer() const{ return pContainer; }
	
	/** Drop container. */
	void DropContainer();
	
	
	
	/**
	 * Open file for reading.
	 * 
	 * The path is relative to the root path. If the file can not be
	 * found an exception is raised. Use the CanReadFile function to
	 * test if a file can be opened for reading.
	 * 
	 * \note This method is called while the container holds the lock.
	 */
	deadArchiveFileReader::Ref OpenFileForReading( const deadArchiveFile &file );
	
	/**
	 * Open file for writing.
	 * 
	 * The path is relative to the root path. If the file can not be
	 * found an exception is raised. Use the CanWriteFile function to
	 * test if a file can be opened for writing. Not existing
	 * directories have to be created if the CanWriteFile function
	 * returns true for a file whose parent directory does not exist yet.
	 * 
	 * \note This method is called while the container holds the lock.
	 */
	decWeakFileWriter *OpenFileForWriting( const deadArchiveFile &file );
	
	/** Close file. */
	void CloseFile();
	
	
	
	/** Read zip file data. */
	void ReadZipFileData( void *buffer, long size );
	
	/** Seek in open zip file. */
	void SeekMoveZipFile( int position );
	void SeekSetZipFile( int position );
	
	
	
	/** Read data. */
	void ReadData( void *buffer, long size );
	
	/** Get current file position. */
	long GetFilePosition() const;
	
	/** Set file position. */
	void SeekFile( int origin, long offset );
	
	/** Read file table. */
	deadArchiveDirectory::Ref ReadFileTable();
	/*@}*/
};

#endif

