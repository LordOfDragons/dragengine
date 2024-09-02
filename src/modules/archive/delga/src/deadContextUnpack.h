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

