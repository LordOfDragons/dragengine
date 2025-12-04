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

#ifndef _DENETWORKMESSAGEREADER_H_
#define _DENETWORKMESSAGEREADER_H_

#include "deNetworkMessage::Ref.h"
#include "../../common/file/decBaseFileReader.h"


/**
 * \brief Network message reader.
 */
class DE_DLL_EXPORT deNetworkMessageReader : public decBaseFileReader{
public:
	/** \brief Type holding strong . */
	typedef deTObjectReference<deNetworkMessageReader> Ref;
	
	
	
private:
	deNetworkMessage::Ref pMessage;
	int pPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network message reader. */
	deNetworkMessageReader( deNetworkMessage *message );
	
private:
	deNetworkMessageReader( const deNetworkMessageReader &reader );
	
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
