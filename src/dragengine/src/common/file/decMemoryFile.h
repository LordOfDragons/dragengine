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
	decMemoryFile(const char *filename);
	
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
	void SetModificationTime(TIME_SYSTEM time);
	
	/** \brief Pointer to file data. */
	inline char *GetPointer() const{ return pData; }
	
	/**
	 * \brief Resize memory file data buffer.
	 * 
	 * If larger the additional data has undefined values.
	 * 
	 * \throws deeInvalidParam \em size is less than 0.
	 */
	void Resize(int size, bool compact = true);
	
	/** \brief Touch file setting the modification time to the current system time. */
	void Touch();
	/*@}*/
};

#endif
