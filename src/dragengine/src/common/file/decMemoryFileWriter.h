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

#ifndef _DECMEMORYFILEWRITER_H_
#define _DECMEMORYFILEWRITER_H_

#include "decBaseFileWriter.h"

#include "decMemoryFile.h"


/**
 * \brief Write memory file.
 */
class DE_DLL_EXPORT decMemoryFileWriter : public decBaseFileWriter{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decMemoryFileWriter> Ref;
	
	
private:
	decMemoryFile::Ref pFile;
	int pPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create memory file writer.
	 * \throws deeInvalidParam \em memoryFile is NULL.
	 */
	decMemoryFileWriter(decMemoryFile *memoryFile, bool append);
	
private:
	decMemoryFileWriter(const decMemoryFileWriter &writer);
	
protected:
	/**
	 * \brief Clean up memory file reader.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~decMemoryFileWriter() override;
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
