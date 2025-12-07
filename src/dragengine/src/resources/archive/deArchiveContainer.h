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

#ifndef _DEARCHIVECONTAINER_H_
#define _DEARCHIVECONTAINER_H_

#include "../../filesystem/deVFSContainer.h"
#include <dragengine/src/resources/archive/deArchive.h>



/**
 * \brief Archive resource virtual file system container.
 *
 * Shows the content of an archive in a virtual file system. Containers can be
 * read-write if the underlaying Archive Module supports writing to the archive
 * file. In general archive containers are read-only.
 */
class DE_DLL_EXPORT deArchiveContainer : public deVFSContainer{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deArchiveContainer> Ref;
	
	
private:
	const decPath pArchivePath;
	deArchive::Ref pArchive;
	
	deArchiveContainer *pLLManagerPrev;
	deArchiveContainer *pLLManagerNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create archive container.
	 * \warning For internal use only. Never call on your own!
	 */
	deArchiveContainer(const decPath &rootPath, deArchive *archive, const decPath &archivePath);
	
protected:
	/**
	 * \brief Clean up archive container.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deArchiveContainer() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Archive. */
	inline const deArchive::Ref &GetArchive() const{ return pArchive; }
	
	/** \brief Archive path to map as root. */
	inline const decPath &GetArchivePath(){ return pArchivePath; }
	
	
	
	/**
	 * \brief File exists.
	 * 
	 * Path is elative to the root path.
	 */
	bool ExistsFile(const decPath &path) override;
	
	/**
	 * \brief File can be read.
	 * 
	 * Path is relative to the root path. The result of this call
	 * is usually the same as of ExistsFile unless permissions prevent
	 * reading of an existing file.
	 */
	bool CanReadFile(const decPath &path) override;
	
	/**
	 * \brief File can be written.
	 * 
	 * The path is relative to the root path. If a file has parent
	 * directories not existing yet they have to be created in a
	 * subsequent call to OpenFileForWriting. If this is the case
	 * return true only if the creation of the parent directories
	 * is also allowed in addition to creating a new file. If the
	 * file exists permission flags can prevent writing.
	 */
	bool CanWriteFile(const decPath &path) override;
	
	/**
	 * \brief File can be deleted.
	 * 
	 * The path is relative to the root path.
	 */
	bool CanDeleteFile(const decPath &path) override;
	
	/**
	 * \brief Open file for reading.
	 * 
	 * The path is relative to the root path. If the file can not be
	 * found an exception is raised. Use the CanReadFile function to
	 * test if a file can be opened for reading.
	 * 
	 * \note A note to module developers. You have to return an instance of decWeakFileReader
	 * wrapping the actual file reader to be able to safely drop it when the module is
	 * unloaded while the reader is still held by somebody.
	 */
	decBaseFileReader *OpenFileForReading(const decPath &path) override;
	
	/**
	 * \brief Open file for writing.
	 * 
	 * The path is relative to the root path. If the file can not be
	 * found an exception is raised. Use the CanWriteFile function to
	 * test if a file can be opened for writing. Not existing
	 * directories have to be created if the CanWriteFile function
	 * returns true for a file whose parent directory does not exist yet.
	 * 
	 * \note A note to module developers. You have to return an instance of decWeakFileWriter
	 * wrapping the actual file writer to be able to safely drop it when the module is
	 * unloaded while the writer is still held by somebody.
	 */
	decBaseFileWriter *OpenFileForWriting(const decPath &path) override;
	
	/**
	 * \brief Delete file.
	 * 
	 * Path is relative to the root path.
	 */
	void DeleteFile(const decPath &path) override;
	
	/** \brief Touch file setting the modification time to the current time. */
	void TouchFile(const decPath &path) override;
	
	/** \brief Search files. */
	void SearchFiles(const decPath &directory, deContainerFileSearch &searcher) override;
	
	/**
	 * \brief Type of file.
	 * 
	 * If the file does not exist an exception is thrown.
	 */
	eFileTypes GetFileType(const decPath &path) override;
	
	/**
	 * \brief Size of file.
	 * 
	 * If the file does not exist an exception is thrown.
	 */
	uint64_t GetFileSize(const decPath &path) override;
	
	/**
	 * \brief Modification time of file.
	 * 
	 * If the file does not exist an exception is thrown.
	 */
	TIME_SYSTEM GetFileModificationTime(const decPath &path) override;
	/*@}*/
	
	
	
	/**
	 * \name Resource manager linked list
	 * \warning For internal use only. Never call on your own!
	 */
	/*@{*/
	/** \brief Previous resource in the resource manager linked list. */
	inline deArchiveContainer *GetLLManagerPrev() const{ return pLLManagerPrev; }
	
	/**
	 * \brief Set next resource in the resource manager linked list.
	 * \warning For internal use only. Never call on your own!
	 */
	void SetLLManagerPrev(deArchiveContainer *resource);
	
	/** \brief Next resource in the resource manager linked list. */
	inline deArchiveContainer *GetLLManagerNext() const{ return pLLManagerNext; }
	
	/**
	 * \brief Set next resource in the resource manager linked list.
	 * \warning For internal use only. Never call on your own!
	 */
	void SetLLManagerNext(deArchiveContainer *resource);
	
	/**
	 * \brief Marks the resource leaking.
	 * \warning For internal use only. Never call on your own!
	 */
	void MarkLeaking();
	/*@}*/
};

#endif
