/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECMEMORYFILE_H_
#define _DECMEMORYFILE_H_

#include "../string/decString.h"
#include "../utils/decDateTime.h"
#include "../../deObject.h"


/**
 * \brief Memory file.
 */
class DE_DLL_EXPORT decMemoryFile : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decMemoryFile> Ref;
	
	
	
private:
	decString pFilename;
	char *pData;
	int pSize;
	int pCapacity;
	TIME_SYSTEM pModificationTime;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create memory file.
	 * \throws deeInvalidParam \em filename is NULL.
	 */
	decMemoryFile( const char *filename );
	
protected:
	/**
	 * \brief Clean up memory file.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~decMemoryFile();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief File path. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** \brief Length of data. */
	inline int GetLength() const{ return pSize; }
	
	/** \brief Modification time. */
	inline TIME_SYSTEM GetModificationTime() const{ return pModificationTime; }
	
	/** \brief Set modification time. */
	void SetModificationTime( TIME_SYSTEM time );
	
	/** \brief Pointer to file data. */
	inline char *GetPointer() const{ return pData; }
	
	/**
	 * \brief Resize memory file data buffer.
	 * 
	 * If larger the additional data has undefined values.
	 * 
	 * \throws deeInvalidParam \em size is less than 0.
	 */
	void Resize( int size, bool compact = true );
	
	/** \brief Touch file setting the modification time to the current system time. */
	void Touch();
	/*@}*/
};

#endif
