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

#ifndef _DENETWORKMESSAGEWRITER_H_
#define _DENETWORKMESSAGEWRITER_H_

#include "deNetworkMessageReference.h"
#include "../../common/file/decBaseFileWriter.h"

class deNetworkMessage;


/**
 * \brief Write network message.
 */
class DE_DLL_EXPORT deNetworkMessageWriter : public decBaseFileWriter{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deNetworkMessageWriter> Ref;
	
	
	
private:
	deNetworkMessageReference pMessage;
	int pPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network message writer. */
	deNetworkMessageWriter( deNetworkMessage *message, bool append );
	
private:
	deNetworkMessageWriter( const deNetworkMessageWriter &writer );
	
protected:
	/**
	 * \brief Clean up network message reader.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deNetworkMessageWriter();
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
