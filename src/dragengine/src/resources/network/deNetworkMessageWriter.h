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

#include "deNetworkMessage.h"
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
	deNetworkMessage::Ref pMessage;
	int pPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network message writer. */
	deNetworkMessageWriter(deNetworkMessage *message, bool append);
	
private:
	deNetworkMessageWriter(const deNetworkMessageWriter &writer);
	
protected:
	/**
	 * \brief Clean up network message reader.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deNetworkMessageWriter() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of the file. */
	const char *GetFilename() override;
	
	/** \brief Current writing position in the file. */
	int GetPosition() override;
	
	/** \brief Set file position for the next write action. */
	void SetPosition(int position) override;
	
	/** \brief Move file position by the given offset. */
	void MovePosition(int offset) override;
	
	/** \brief Set file position to the given position measured from the end of the file. */
	void SetPositionEnd(int position) override;
	
	/**
	 * \brief Write \em size bytes from \em buffer and advances the file pointer.
	 * \throws deeInvalidParam \em buffer is NULL.
	 * \throws deeInvalidParam \em size is less than 0.
	 */
	void Write(const void *buffer, int size) override;
	
	/** \brief Duplicate file writer. */
	decBaseFileWriter::Ref Duplicate() override;
	/*@}*/
};

#endif
