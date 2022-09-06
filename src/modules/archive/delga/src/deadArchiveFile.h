/* 
 * Drag[en]gine DELGA Archive Module
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

