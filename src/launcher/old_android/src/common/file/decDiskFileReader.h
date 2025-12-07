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

#ifndef _DECDISKFILEREADER_H_
#define _DECDISKFILEREADER_H_

#include <stdio.h>

#include "decBaseFileReader.h"


/**
 * \brief Reads data from files stored on disc.
 */
class decDiskFileReader : public decBaseFileReader{
private:
	decString pFilename;
	FILE *pFile;
	int pLength;
	TIME_SYSTEM pModificationTime;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create file reader object for a file.
	 * 
	 * The file is opened immediatly for reading only. The filename specified
	 * has to be an absolute path.
	 * 
	 * \throws deeFileNotFound \em filename does not exist.
	 * \throws deeFileNotFound \em filename can not be opened for reading.
	 */
	decDiskFileReader(const char *filename);
	
protected:
	/**
	 * \brief Close file and clean up.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~decDiskFileReader() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of the file. */
	const char *GetFilename() override;
	
	/** \brief Length of the file. */
	int GetLength() override;
	
	/** \brief Modification time. */
	TIME_SYSTEM GetModificationTime() override;
	
	/** \brief Current reading position in the file. */
	int GetPosition() override;
	
	/** \brief Set file position for the next read action. */
	void SetPosition(int position) override;
	
	/** \brief Move file position by the given offset. */
	void MovePosition(int offset) override;
	
	/** \brief Set file position to the given position measured from the end of the file. */
	void SetPositionEnd(int position) override;
	
	/**
	 * \brief Read \em size bytes into \em buffer and advances the file pointer.
	 * \throws deeInvalidParam \em buffer is NULL.
	 * \throws deeInvalidParam \em size is less than 1.
	 * \throws deeReadFile Can not read from file.
	 */
	void Read(void *buffer, int size) override;
	/*@}*/
};

#endif
