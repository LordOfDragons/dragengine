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

#ifndef _DECMEMORYFILEWRITER_H_
#define _DECMEMORYFILEWRITER_H_

#include "decBaseFileWriter.h"

class decMemoryFile;


/**
 * \brief Write memory file.
 */
class DE_DLL_EXPORT decMemoryFileWriter : public decBaseFileWriter{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decMemoryFileWriter> Ref;
	
	
	
private:
	decMemoryFile *pFile;
	const bool pAppend;
	int pPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create memory file writer.
	 * \throws deeInvalidParam \em memoryFile is NULL.
	 */
	decMemoryFileWriter( decMemoryFile *memoryFile, bool append );
	
protected:
	/**
	 * \brief Clean up memory file reader.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~decMemoryFileWriter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of the file. */
	virtual const char *GetFilename();
	
	/** \brief Current writing position in the file. */
	virtual int GetPosition();
	
	/** \brief Set file position for the next write action. */
	virtual void SetPosition( int position );
	
	/** \brief Move file position by the given offset. */
	virtual void MovePosition( int offset );
	
	/** \brief Set file position to the given position measured from the end of the file. */
	virtual void SetPositionEnd( int position );
	
	/**
	 * \brief Write \em size bytes from \em buffer and advances the file pointer.
	 * \throws deeInvalidParam \em buffer is NULL.
	 * \throws deeInvalidParam \em size is less than 0.
	 */
	virtual void Write( const void *buffer, int size );
	
	/** \brief Duplicate file writer. */
	virtual decBaseFileWriter::Ref Duplicate();
	/*@}*/
};

#endif
