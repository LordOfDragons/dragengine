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

#ifndef _DECZFILEREADER_H_
#define _DECZFILEREADER_H_

#include <stdio.h>

#include "decBaseFileReader.h"


/**
 * \brief Z-Compressed file reader.
 * 
 * Reads data from another file writer using Zlib compression. The user is responsible
 * to place the file reader at the position where the Z-compressed data begins. This
 * allows the user to place arbitrary headers in front of the z-compressed data. The
 * compressed data is always read sequentially hence file readers unable to seek
 * can be used. The z-reader takes over the original reader pointer deleting it once
 * the z-reader itself is destroyed. The typical use case is to create a reader,
 * read the header, creating the z-reader and then handing back the z-reader to
 * read the compressed file content. This way pointers are handled properly and the
 * z-reader can be transparently used everywhere a file reader is used.
 */
class decZFileReader : public decBaseFileReader{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decZFileReader> Ref;
	
	
	
private:
	decBaseFileReader *pReader;
	int pFilePosition;
	int pFileLength;
	
	void *pZStream;
	
	void *pBufferIn;
	int pBufferInSize;
	int pBufferInPosition;
	
	void *pContent;
	int pContentSize;
	int pContentCapacity;
	int pContentPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create z-compressed file reader object for another file reader.
	 * 
	 * The file reader is taken over and deleted once the z-reader is deleted.
	 * The file pointer has to be set to the starting position of the compressed data.
	 * 
	 * \note To ensure future feature compatibility this class reads private data
	 *       stored in front of the actual compress data. To read a pure z compressed
	 *       stream use \ref decZFileReader(decBaseFileReader*,bool,int) .
	 * 
	 * \throws deeInvalidParam \em reader is NULL.
	 */
	decZFileReader( decBaseFileReader *reader );
	
	/**
	 * \brief Create z-compressed file reader object for another file reader.
	 * 
	 * The file reader is taken over and deleted once the z-reader is deleted.
	 * The file pointer has to be set to the starting position of the compressed data.
	 * 
	 * \param[in] pureMode If true does not read private compression data. Use this mode
	 *                     if you want to read pure z compressed data. If false behaves
	 *                     identical to \ref decZFileReader(decBaseFileReader*) .
	 * \param[in] pureLength If \em pureMode is true defines the length of the compressed data.
	 *                       The caller is responsible to obtain the correct length.
	 *                       
	 * 
	 * \throws deeInvalidParam \em reader is NULL.
	 */
	decZFileReader( decBaseFileReader *reader, bool pureMode, int pureLength );
	
protected:
	/**
	 * \brief Close file and cleans up.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~decZFileReader();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of the file. */
	virtual const char *GetFilename();
	
	/** \brief Length of the file. */
	virtual int GetLength();
	
	/** \brief Modification time. */
	virtual TIME_SYSTEM GetModificationTime();
	
	/** \brief Current reading position in the file. */
	virtual int GetPosition();
	
	/** \brief Set file position for the next read action. */
	virtual void SetPosition( int position );
	
	/** \brief Move file position by the given offset. */
	virtual void MovePosition( int offset );
	
	/** \brief Set file position to the given position measured from the end of the file. */
	virtual void SetPositionEnd( int position );
	
	/**
	 * \brief Read \em size bytes into \em buffer and advances the file pointer.
	 * \throws deeInvalidParam \em buffer is NULL.
	 * \throws deeInvalidParam \em size is less than 1.
	 * \throws deeInvalidParam Error decompressing content.
	 */
	virtual void Read( void *buffer, int size );
	/*@}*/
	
	
	
private:
	void pInit( decBaseFileReader *reader, bool pureMode, int pureLength );
	void pSetContentPosition( int position );
	void pDecompressAll();
};

#endif
