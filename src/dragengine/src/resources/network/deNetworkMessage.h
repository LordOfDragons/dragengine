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
	void SetDataLength(int dataLength);
	
	/** \brief Clear message data. */
	void Clear();
	
	/** \brief Time stamp. */
	inline TIME_SYSTEM GetTimeStamp() const{ return pTimeStamp; }
	
	/** \brief Set time stamp. */
	void SetTimeStamp(TIME_SYSTEM timeStamp);
	/*@}*/
};

#endif
