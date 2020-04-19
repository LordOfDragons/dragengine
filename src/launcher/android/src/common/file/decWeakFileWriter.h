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

#ifndef _DECWEAKFILEWRITER_H_
#define _DECWEAKFILEWRITER_H_

#include "decBaseFileWriter.h"
#include "decBaseFileWriterReference.h"


/**
 * \brief Weak file writer.
 * 
 * Weak wraps a file writer. The wrapped file writer can be dropped any time causing
 * all future operations to fail. This is used for readers created by engine modules
 * to avoid problems if the engine module is unloaded while the writer is still held.
 */
class decWeakFileWriter : public decBaseFileWriter{
private:
	decBaseFileWriterReference pWriter;
	
	
	
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
	decWeakFileWriter( decBaseFileWriter *writer );
	
protected:
	/**
	 * \brief Close file and cleans up.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~decWeakFileWriter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Wrapped writer is valid.
	 * 
	 * If false is returned any future operations will fail.
	 */
	bool IsValid() const;
	
	/** \brief Drop wrapped writer if not dropped already. */
	void DropWriter();
	
	
	
	/** \brief Name of the file. */
	virtual const char *GetFilename();
	
	/**
	 * \brief Write \em size bytes from \em buffer and advances the file pointer.
	 * \throws deeInvalidParam \em buffer is NULL.
	 * \throws deeInvalidParam \em size is less than 0.
	 * \throws deeInvalidParam Error compressing data.
	 */
	virtual void Write( const void *buffer, int size );
	/*@}*/
};

#endif
