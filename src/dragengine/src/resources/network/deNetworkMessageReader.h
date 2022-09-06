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

#ifndef _DENETWORKMESSAGEREADER_H_
#define _DENETWORKMESSAGEREADER_H_

#include "deNetworkMessageReference.h"
#include "../../common/file/decBaseFileReader.h"


/**
 * \brief Network message reader.
 */
class DE_DLL_EXPORT deNetworkMessageReader : public decBaseFileReader{
public:
	/** \brief Type holding strong . */
	typedef deTObjectReference<deNetworkMessageReader> Ref;
	
	
	
private:
	deNetworkMessageReference pMessage;
	int pPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network message reader. */
	deNetworkMessageReader( deNetworkMessage *message );
	
protected:
	/**
	 * \brief Clean up network message reader.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deNetworkMessageReader();
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
	
	/**
	 * \brief Set file position for the next read action.
	 * \throws deeInvalidParam \em position is less than 0.
	 * \throws deeInvalidParam \em position is larger than GetLength().
	 */
	virtual void SetPosition( int position );
	
	/**
	 * \brief Move file position by the given offset.
	 * \throws deeInvalidParam GetPosition() + \em position is less than 0.
	 * \throws deeInvalidParam GetPosition() + \em position is larger than GetLength().
	 */
	virtual void MovePosition( int offset );
	
	/**
	 * \brief Set file position to the given position measured from the end of the file.
	 * \throws deeInvalidParam \em position is less than 0.
	 * \throws deeInvalidParam \em position is larger than GetLength().
	 */
	virtual void SetPositionEnd( int position );
	
	/**
	 * \brief Read \em size bytes into \em buffer and advances the file pointer.
	 * \throws deeInvalidParam \em buffer is NULL.
	 * \throws deeInvalidParam \em size is less than 1.
	 * \throws deeInvalidParam GetPosition() + \em size is larger than GetLength().
	 */
	virtual void Read( void *buffer, int size );
	
	/** \brief Duplicate file reader. */
	virtual decBaseFileReader::Ref Duplicate();
	/*@}*/
};

#endif
