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

#ifndef _DECWEAKFILEREADER_H_
#define _DECWEAKFILEREADER_H_

#include "decBaseFileReader.h"


/**
 * \brief Weak file reader.
 * 
 * Weak wraps a file reader. The wrapped file reader can be dropped any time causing
 * all future operations to fail. This is used for readers created by engine modules
 * to avoid problems if the engine module is unloaded while the reader is still held.
 */
class DE_DLL_EXPORT decWeakFileReader : public decBaseFileReader{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decWeakFileReader> Ref;
	
	
private:
	decBaseFileReader::Ref pReader;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create weak file reader.
	 * \param[in] reader Reader to wrap.
	 * 
	 * \throws deeInvalidParam \em reader is NULL.
	 */
	decWeakFileReader(decBaseFileReader *reader);
	
protected:
	/**
	 * \brief Close file and cleans up.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~decWeakFileReader();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Wrapped reader is valid.
	 * 
	 * If false is returned any future operations will fail.
	 */
	bool IsValid() const;
	
	/** \brief Drop wrapped reader if not dropped already. */
	void DropReader();
	
	
	
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
	
	/** \brief Duplicate file reader. */
	virtual decBaseFileReader::Ref Duplicate();
	/*@}*/
};

#endif
