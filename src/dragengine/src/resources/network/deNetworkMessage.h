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

#ifndef _DENETWORKMESSAGE_H_
#define _DENETWORKMESSAGE_H_

#include <stdint.h>

#include "../../deObject.h"
#include "../../common/utils/decDateTime.h"


/**
 * \brief Network Message Class.
 *
 * Manages a network message of variable length. Supports reading and
 * writing to the message providing basic data types. Reading and
 * writing is done like with a file stream with a stream position which
 * is the same for reading and writing as well as end of message
 * functions. An exception is raised if an attempt is made to read past
 * the end of the message. The buffer is increased in size if a write
 * attempt is made past the end of the message. This way the buffer can
 * be filled with data making the message grow to the required size to
 * hold all data. To Read data of a given size the SetLength call can
 * be used to resize the buffer to at last a given length before reading
 * the data into the messagchare.
 */
class DE_DLL_EXPORT deNetworkMessage : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deNetworkMessage> Ref;
	
	
	
private:
	uint8_t *pBuffer;
	int pBufferSize;
	int pDataLength;
	TIME_SYSTEM pTimeStamp;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network message object. */
	deNetworkMessage();
	
protected:
	/**
	 * \brief Clean up network message object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deNetworkMessage();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Buffer data. */
	inline uint8_t *GetBuffer() const{ return pBuffer; }
	
	/** \brief Length of the message data. */
	inline int GetDataLength() const{ return pDataLength; }
	
	/** \brief Set length of the message data. */
	void SetDataLength( int dataLength );
	
	/** \brief Clear message data. */
	void Clear();
	
	/** \brief Time stamp. */
	inline TIME_SYSTEM GetTimeStamp() const{ return pTimeStamp; }
	
	/** \brief Set time stamp. */
	void SetTimeStamp( TIME_SYSTEM timeStamp );
	/*@}*/
};

#endif
