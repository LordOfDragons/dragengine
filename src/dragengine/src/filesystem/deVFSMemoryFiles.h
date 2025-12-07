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

#ifndef _DEVFSMEMORYFILES_H_
#define _DEVFSMEMORYFILES_H_

#include "deVFSContainer.h"
#include "dePathList.h"
#include "../common/collection/decObjectList.h"

class decMemoryFile;


/**
 * \brief Container mapping a list of memory files into the virtual file system.
 * 
 * Memory files are chunks of data in memory representing files. This
 * class is useful to add a small number of individual memory files
 * to the virtual file system. Each memory file is an individual chunk
 * of memory. For mapping a large block of memory into a virtual file
 * system containing various files inside this memory block the block
 * memory file class is better suited.
 */
class DE_DLL_EXPORT deVFSMemoryFiles : public deVFSContainer{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deVFSMemoryFiles> Ref;
	
	
private:
	decObjectList pFiles;
	dePathList pDirectories;
	bool pDirtyDirectories;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create memory files with root path. */
	deVFSMemoryFiles(const decPath &rootPath);
	
protected:
	/**
	 * \brief Clean up memory files.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deVFSMemoryFiles() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
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
	
	/**
	 * \brief Search all visible files and directories.
	 */
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
	
	
	
	/** \name Memory Files */
	/*@{*/
	/** \brief Number of memory files. */
	int GetMemoryFileCount() const;
	
	/** \brief Memory file at position. */
	decMemoryFile *GetMemoryFileAt(int index) const;
	
	/** \brief Index of memory file or -1 if absent. */
	int IndexOfMemoryFile(decMemoryFile *memoryFile) const;
	
	/** \brief Index of memory file with path or -1 if absent. */
	int IndexOfMemoryFileWith(const char *path) const;
	
	/** \brief Memory file is present. */
	bool HasMemoryFile(decMemoryFile *memoryFile) const;
	
	/** \brief Memory file with path is present. */
	bool HasMemoryFileWith(const char *path) const;
	
	/** \brief Add memory file. */
	void AddMemoryFile(decMemoryFile *memoryFile);
	
	/** \brief Remove memory file. */
	void RemoveMemoryFile(decMemoryFile *memoryFile);
	
	/** \brief Remove all memory files. */
	void RemoveAllMemoryFiles();
	/*@}*/
	
	
	
private:
	void pRebuildDirectories();
};

#endif
