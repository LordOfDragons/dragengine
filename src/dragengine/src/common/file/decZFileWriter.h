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

#ifndef _DECZFILEWRITER_H_
#define _DECZFILEWRITER_H_

#include <stdio.h>

#include "decBaseFileWriter.h"


/**
 * \brief Z-Compressed file writer.
 * 
 * Writes data to another file writer using Zlib compression. The user is responsible
 * to place the file writer to the position where the Z-compressed data begins. This
 * allows the user to place arbitrary headers in front of the z-compressed data. The
 * compressed data is always written sequentially hence file writers unable to seek
 * can be used. The z-writer takes over the original writer pointer deleting it once
 * the z-writer itself is destroyed. The typical use case is to create a writer,
 * writing the header, creating the z-writer and then handing back the z-writer to
 * write the compressed file content. This way pointers are handled properly and the
 * z-writer can be transparently used everywhere a file writer is used.
 */
class decZFileWriter : public decBaseFileWriter{
private:
	decBaseFileWriter *pWriter;
	
	void *pZStream;
	
	void *pBufferIn;
	int pBufferInSize;
	int pBufferInPosition;
	
	void *pBufferOut;
	int pBufferOutSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create z-compressed file writer object for another file writer.
	 * 
	 * The file writer is taken over and deleted once the z-writer is deleted.
	 * The file pointer has to be set to the starting position of the z-compressed data.
	 * 
	 * \throws deeInvalidParam \em writer is NULL.
	 */
	decZFileWriter( decBaseFileWriter *writer );
	
protected:
	/**
	 * \brief Close file and cleans up.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~decZFileWriter();
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
	 * \throws deeInvalidParam Error compressing data.
	 */
	virtual void Write( const void *buffer, int size );
	
	/**
	 * \brief End writing flushing remaining data and closing the z-stream.
	 * \throws deeInvalidParam Error compressing data.
	 */
	void EndZWriting();
	/*@}*/
};

#endif
