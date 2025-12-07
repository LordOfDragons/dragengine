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
	 * The file pointer has to be set to the starting position of the z-compressed data.
	 * 
	 * \throws deeInvalidParam \em reader is NULL.
	 */
	decZFileReader(decBaseFileReader *reader);
	
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
	virtual void SetPosition(int position);
	
	/** \brief Move file position by the given offset. */
	virtual void MovePosition(int offset);
	
	/** \brief Set file position to the given position measured from the end of the file. */
	virtual void SetPositionEnd(int position);
	
	/**
	 * \brief Read \em size bytes into \em buffer and advances the file pointer.
	 * \throws deeInvalidParam \em buffer is NULL.
	 * \throws deeInvalidParam \em size is less than 1.
	 * \throws deeInvalidParam Error decompressing content.
	 */
	virtual void Read(void *buffer, int size);
	/*@}*/
	
	
	
private:
	/** \brief Set content position ensuring the content is decompressed up to this point. */
	void pSetContentPosition(int position);
	
	/** \brief Decompress the entire image. Required for seek-end to work. */
	void pDecompressAll();
	/*@}*/
};

#endif
