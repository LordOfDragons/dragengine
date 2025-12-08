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

#ifndef _DEVFSREDIRECT_H_
#define _DEVFSREDIRECT_H_

#include "deVFSContainer.h"
#include "deVirtualFileSystem.h"



/**
 * \brief Container redirecting to a virtual file system.
 * 
 * Can redirect to another container or an entire virtual file system.
 * 
 * \warning Do not produce cycling path links. Containers get relative path so it is
 * not possible for them to detect cycling path links resulting in stack
 * overflows.
 */
class deVFSRedirect : public deVFSContainer{
private:
	const decPath pRedirectPath;
	deVFSContainer::Ref pContainer;
	deVirtualFileSystem::Ref pVFS;
	bool pHoldVFSReference;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create container redirecting to \em redirectPath in a container.
	 * 
	 * \param[in] rootPath Root path the container is visible at.
	 * \param[in] redirectPath Path to redirect to in \em vfs.
	 * \param[in] container Container to redirect to.
	 */
	deVFSRedirect(const decPath &rootPath, const decPath &redirectPath, deVFSContainer *container);
	
	/**
	 * \brief Create container redirecting to \em redirectPath in a virtual file system.
	 * 
	 * \warning Make sure to set \em holdVFSReference correctly. If wrong the virtual
	 * file system leaks. If the virtual file system is the same as the one
	 * the container is going to be added to use false to avoid leaking.
	 * If the container is a foreign virtual file system use true or
	 * the link turns suddenly dead resulting in segmentation faults.
	 * 
	 * \param[in] rootPath Root path the container is visible at.
	 * \param[in] redirectPath Path to redirect to in \em vfs.
	 * \param[in] vfs Virtual file system to redirect to.
	 * \param[in] holfVFSReference Container holds a reference to \em vfs.
	 */
	deVFSRedirect(const decPath &rootPath, const decPath &redirectPath,
		deVirtualFileSystem *vfs, bool holdVFSReference);
	
protected:
	/**
	 * \brief Clean up container.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deVFSRedirect() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Redirect path in the same virtual file system. */
	inline const decPath &GetRedirectPath(){ return pRedirectPath; }
	
	
	
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
};

#endif
