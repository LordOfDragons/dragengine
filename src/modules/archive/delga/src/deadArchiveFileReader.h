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

#ifndef _DEADARCHIVEFILEREADER_H_
#define _DEADARCHIVEFILEREADER_H_

#include <unzip.h>

#include <dragengine/common/file/decBaseFileReader.h>

class deadContextUnpack;
class deadArchiveFile;


/**
 * Archive file entry reader.
 */
class deadArchiveFileReader : public decBaseFileReader{
private:
	deadContextUnpack *pContext;
	const unz_file_pos pArchivePosition;
	const decString pFilename;
	const int pFilesize;
	const TIME_SYSTEM pModificationTime;
	
	int pPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create reader. */
	deadArchiveFileReader( deadContextUnpack *context, const deadArchiveFile &file );
	
	/** Clean up reader. */
	virtual ~deadArchiveFileReader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Name of the file. */
	virtual const char *GetFilename();
	
	/** Length of the file. */
	virtual int GetLength();
	
	/** Modification time. */
	virtual TIME_SYSTEM GetModificationTime();
	
	/** Current reading position in the file. */
	virtual int GetPosition();
	
	/** Set file position for the next read action. */
	virtual void SetPosition( int position );
	
	/** Move file position by the given offset. */
	virtual void MovePosition( int offset );
	
	/** Set file position to the given position measured from the end of the file. */
	virtual void SetPositionEnd( int position );
	
	/**
	 * Read \em size bytes into \em buffer and advances the file pointer.
	 * \throws deeInvalidParam \em buffer is NULL.
	 * \throws deeInvalidParam \em size is less than 1.
	 */
	virtual void Read( void *buffer, int size );
	
	/** Duplicate file reader. */
	virtual Ref Duplicate();
	/*@}*/
};

#endif

