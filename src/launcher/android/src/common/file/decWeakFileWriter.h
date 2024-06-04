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

#ifndef _DECWEAKFILEWRITER_H_
#define _DECWEAKFILEWRITER_H_

#include "decBaseFileWriter.h"
#include "decBaseFileWriterReference.h"


/**
 * \brief Weak file writer.
 * 
 * Weak wraps a file writer. The wrapped file writer can be dropped any time causing
 * all future operations to fail. This is used for readers created by engine modules
 * to avoid problems if the engine module is unloaded while the writer is still held.
 */
class decWeakFileWriter : public decBaseFileWriter{
private:
	decBaseFileWriterReference pWriter;
	
	
	
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
	decWeakFileWriter( decBaseFileWriter *writer );
	
protected:
	/**
	 * \brief Close file and cleans up.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~decWeakFileWriter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Wrapped writer is valid.
	 * 
	 * If false is returned any future operations will fail.
	 */
	bool IsValid() const;
	
	/** \brief Drop wrapped writer if not dropped already. */
	void DropWriter();
	
	
	
	/** \brief Name of the file. */
	virtual const char *GetFilename();
	
	/**
	 * \brief Write \em size bytes from \em buffer and advances the file pointer.
	 * \throws deeInvalidParam \em buffer is NULL.
	 * \throws deeInvalidParam \em size is less than 0.
	 * \throws deeInvalidParam Error compressing data.
	 */
	virtual void Write( const void *buffer, int size );
	/*@}*/
};

#endif
