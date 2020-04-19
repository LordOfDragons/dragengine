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

#include "unzip.h"

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/utils/decDateTime.h>


class deArchiveDelga;
class deadContainer;
class deadArchiveDirectory;
class deadArchiveFile;
class decWeakFileReader;
class decWeakFileWriter;



/**
 * \brief Unpacking context.
 * 
 * Contexts are claimed by file readers to unpack file content. If not claimed contexts
 * reside in a list of claimable contexts to reduce overhead.
 */
class deadContextUnpack{
private:
	deArchiveDelga &pModule;
	deadContainer *pContainer;
	
	unzFile pZipFile;
	
	long pBlockPosition;
	long pBlockSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create unpacking context. */
	deadContextUnpack( deArchiveDelga &module, deadContainer *container );
	
	/** \brief Clean up unpacking context. */
	~deadContextUnpack();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deArchiveDelga &GetModule() const{ return pModule; }
	
	/** \brief Container or \em NULL if dropped. */
	inline deadContainer *GetContainer() const{ return pContainer; }
	
	/** \brief Drop container. */
	void DropContainer();
	
	
	
	/** \brief Block position. */
	inline long GetBlockPosition() const{ return pBlockPosition; }
	
	/** \brief Block size. */
	inline long GetBlockSize() const{ return pBlockSize; }
	
	/** \brief File position is inside block. */
	bool IsPositionInsideBlock( long position ) const;
	
	
	
	/**
	 * \brief Open file for reading.
	 * 
	 * The path is relative to the root path. If the file can not be
	 * found an exception is raised. Use the CanReadFile function to
	 * test if a file can be opened for reading.
	 * 
	 * \note This method is called while the container holds the lock.
	 */
	decWeakFileReader *OpenFileForReading( const deadArchiveFile &file );
	
	/**
	 * \brief Open file for writing.
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
	
	
	
	/**
	 * \brief Read data.
	 * 
	 * If read leaves the current block the next block is read until the read is completed.
	 */
	void ReadData( void *buffer, long size );
	
	/**
	 * \brief Get current file position.
	 */
	long GetFilePosition() const;
	
	/**
	 * \brief Set file position.
	 * 
	 * If new position is outside the current block the required block is read.
	 */
	void SeekFile( int origin, long offset );
	
	/**
	 * \brief Read file table.
	 */
	deadArchiveDirectory *ReadFileTable();
	/*@}*/
};

#endif

