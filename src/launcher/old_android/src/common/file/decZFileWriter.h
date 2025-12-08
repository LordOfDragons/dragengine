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
	decBaseFileWriter::Ref pWriter;
	
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
	decZFileWriter(decBaseFileWriter *writer);
	
protected:
	/**
	 * \brief Close file and cleans up.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~decZFileWriter() override;
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
	 * \throws deeInvalidParam Error compressing data.
	 */
	void Write(const void *buffer, int size) override;
	
	/**
	 * \brief End writing flushing remaining data and closing the z-stream.
	 * \throws deeInvalidParam Error compressing data.
	 */
	void EndZWriting();
	/*@}*/
};

#endif
