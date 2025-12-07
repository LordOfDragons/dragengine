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

#ifndef _DEVFSNULL_H_
#define _DEVFSNULL_H_

#include "deVFSContainer.h"


/**
 * \brief Null virtual file system container.
 * 
 * A container that is always empty. Acts as a sink where all written files are
 * immediately forgotten. Useful as dummy container for volatile data directories
 * where no permanent storage is desired or possible.
 */
class DE_DLL_EXPORT deVFSNull : public deVFSContainer{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deVFSNull> Ref;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create null container with root path. */
	deVFSNull(const decPath &rootPath);
	
protected:
	/**
	 * \brief Clean up container.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deVFSNull() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief File exists.
	 * \returns false.
	 */
	bool ExistsFile(const decPath &path) override;
	
	/**
	 * \brief File can be read from.
	 * \returns false.
	 */
	bool CanReadFile(const decPath &path) override;
	
	/**
	 * \brief File can be written to.
	 * \returns true.
	 */
	bool CanWriteFile(const decPath &path) override;
	
	/**
	 * \brief File can be deleted.
	 * \returns true.
	 */
	bool CanDeleteFile(const decPath &path) override;
	
	
	
	/**
	 * \brief Open file for reading.
	 * 
	 * Throws file not found exception.
	 */
	decBaseFileReader *OpenFileForReading(const decPath &path) override;
	
	/**
	 * \brief Open file for writing.
	 * \returns Null writer.
	 */
	decBaseFileWriter *OpenFileForWriting(const decPath &path) override;
	
	/**
	 * \brief Delete file.
	 * 
	 * Does nothing.
	 */
	void DeleteFile(const decPath &path) override;
	
	/**
	 * \brief Touch file setting the modification time to the current time.
	 * 
	 * Does nothing.
	 */
	void TouchFile(const decPath &path) override;
	
	/**
	 * \brief Search for all files and directories.
	 */
	void SearchFiles(const decPath &directory, deContainerFileSearch &searcher) override;
	
	/**
	 * \brief Type of file.
	 * 
	 * Throws file not found exception.
	 */
	eFileTypes GetFileType(const decPath &path) override;
	
	/**
	 * \brief Size of file.
	 * 
	 * Throws file not found exception.
	 */
	uint64_t GetFileSize(const decPath &path) override;
	
	/**
	 * \brief Modification time of file.
	 * 
	 * Throws file not found exception.
	 */
	TIME_SYSTEM GetFileModificationTime(const decPath &path) override;
	/*@}*/
};

#endif
