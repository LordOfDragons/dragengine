/* 
 * Drag[en]gine DELGA Archive Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

