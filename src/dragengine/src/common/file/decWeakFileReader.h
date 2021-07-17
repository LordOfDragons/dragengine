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

#ifndef _DECWEAKFILEREADER_H_
#define _DECWEAKFILEREADER_H_

#include "decBaseFileReader.h"
#include "decBaseFileReaderReference.h"


/**
 * \brief Weak file reader.
 * 
 * Weak wraps a file reader. The wrapped file reader can be dropped any time causing
 * all future operations to fail. This is used for readers created by engine modules
 * to avoid problems if the engine module is unloaded while the reader is still held.
 */
class decWeakFileReader : public decBaseFileReader{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decWeakFileReader> Ref;
	
	
	
private:
	decBaseFileReaderReference pReader;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create weak file reader.
	 * \param[in] reader Reader to wrap.
	 * 
	 * \throws deeInvalidParam \em reader is NULL.
	 */
	decWeakFileReader( decBaseFileReader *reader );
	
protected:
	/**
	 * \brief Close file and cleans up.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~decWeakFileReader();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Wrapped reader is valid.
	 * 
	 * If false is returned any future operations will fail.
	 */
	bool IsValid() const;
	
	/** \brief Drop wrapped reader if not dropped already. */
	void DropReader();
	
	
	
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
};

#endif
