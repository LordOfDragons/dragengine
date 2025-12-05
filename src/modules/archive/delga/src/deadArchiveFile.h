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

#ifndef _DEADARCHIVEFILE_H_
#define _DEADARCHIVEFILE_H_

#include "unzip.h"

#include <dragengine/deObject.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/utils/decDateTime.h>


class deArchiveDelga;


/**
 * \brief Archive file entry.
 */
class deadArchiveFile : public deObject{
public:
	typedef deTObjectReference<deadArchiveFile> Ref;
	
	
private:
	deArchiveDelga &pModule;
	
	decString pFilename;
	unz_file_pos pArchivePosition;
	int pFileSize;
	TIME_SYSTEM pModificationTime;
	int pCompressedSize;
	int pReadBlockSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file entry. */
	deadArchiveFile( deArchiveDelga &module, const char *filename,
		const unz_file_info &info, const unz_file_pos &archivePosition );
	
	/** \brief Clean up file entry. */
	virtual ~deadArchiveFile();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** \brief Position in zip file. */
	inline const unz_file_pos &GetArchivePosition() const{ return pArchivePosition; }
	
	/** \brief Uncompressed file size. */
	inline int GetFileSize() const{ return pFileSize; }
	
	/** \brief Modification time. */
	inline TIME_SYSTEM GetModificationTime() const{ return pModificationTime; }
	
	/** \brief Compressed file size. */
	inline int GetCompressedSize() const{ return pCompressedSize; }
	
	/** \brief Read block size. */
	inline int GetReadBlockSize() const{ return pReadBlockSize; }
	/*@}*/
};

#endif

