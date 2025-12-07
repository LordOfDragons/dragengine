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

#ifndef _DEVIRTUALFILESYSTEM_H_
#define _DEVIRTUALFILESYSTEM_H_

#include "deVFSContainer.h"
#include "../deObject.h"
#include "../common/collection/decObjectOrderedSet.h"

class decBaseFileReader;
class decBaseFileWriter;
class decPath;
class dePatternList;
class dePathList;
class deFileSearchVisitor;


/**
 * \brief Virtual file system.
 * 
 * Manages a virtual file system. The file system is by default empty and filled with
 * content using container objects. Each container object populates a given directory
 * with content. This process is not the same as file system mounting therefore the
 * content of a container object does not replace the content in the directory but
 * adds to it. Already existing files and directories are shadowed. Therefore the
 * order of the container objects is important. Container objects shadow files and
 * directories defined by container objects located before them.
 * 
 * \warning
 * 
 * VFS containers are thread-safe for all operations. This class is only thread-safe
 * if containers are modified only before using the VFS but not while using it.
 * If you need to change the containers while using the VFS use the deVFSModular
 * container. This container is safe to be modified while in use.
 */
class DE_DLL_EXPORT deVirtualFileSystem : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deVirtualFileSystem> Ref;
	
	
private:
	decObjectOrderedSet pContainers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create virtual file system. */
	deVirtualFileSystem();
	
protected:
	/**
	 * \brief Clean up virtual file system.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deVirtualFileSystem();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief File exists. */
	bool ExistsFile(const decPath &path) const;
	
	/** \brief File can be read from. */
	bool CanReadFile(const decPath &path) const;
	
	/** \brief File can be written to. */
	bool CanWriteFile(const decPath &path) const;
	
	/** \brief File can be deleted. */
	bool CanDeleteFile(const decPath &path) const;
	
	
	
	/**
	 * \brief Open file for reading.
	 * 
	 * Throws an exception if file reader can not be created.
	 * 
	 * \warning File reader has reference counted added. Use decBaseFileReader::Ref::TakeOver().
	 */
	decBaseFileReader *OpenFileForReading(const decPath &path) const;
	
	/**
	 * \brief Open file for writing.
	 * 
	 * Throws an exception if file writer can not be created.
	 * 
	 * \warning File reader has reference counted added. Use decBaseFileWriter::Ref::TakeOver().
	 */
	decBaseFileWriter *OpenFileForWriting(const decPath &path) const;
	
	/**
	 * \brief Delete specified file.
	 * 
	 * Throws an exception if file can not be deleted.
	 */
	void DeleteFile(const decPath &path) const;
	
	/**
	 * \brief Touch file setting the modification time to the current time.
	 */
	void TouchFile(const decPath &path) const;
	
	/** \brief Search files. */
	void SearchFiles(const decPath &directory, deFileSearchVisitor &visitor) const;
	
	/**
	 * \brief Type of file.
	 * 
	 * Throws an exception if file type can not be retrieved.
	 */
	deVFSContainer::eFileTypes GetFileType(const decPath &path) const;
	
	/**
	 * \brief Size of file.
	 * 
	 * Throws an exception if file type can not be retrieved.
	 */
	uint64_t GetFileSize(const decPath &path) const;
	
	/**
	 * \brief Modification time of file.
	 * 
	 * Throws an exception if file type can not be retrieved.
	 */
	TIME_SYSTEM GetFileModificationTime(const decPath &path) const;
	/*@}*/
	
	
	
	/** \name Containers */
	/*@{*/
	/** \brief Number of containers. */
	int GetContainerCount() const;
	
	/** \brief Container at position. */
	deVFSContainer *GetContainerAt(int index) const;
	
	/** \brief Index of container or -1 if absent. */
	int IndexOfContainer(deVFSContainer *container) const;
	
	/** \brief Container is present. */
	bool HasContainer(deVFSContainer *container) const;
	
	/**
	 * \brief Add container.
	 * 
	 * \warning Breaks thread-safety if called after VFS is in use.
	 */
	void AddContainer(deVFSContainer *container);
	
	/**
	 * \brief Remove container.
	 * 
	 * \warning Breaks thread-safety if called after VFS is in use.
	 */
	void RemoveContainer(deVFSContainer *container);
	
	/**
	 * \brief Remove all containers.
	 * 
	 * \warning Breaks thread-safety if called after VFS is in use.
	 */
	void RemoveAllContainers();
	/*@}*/
	
	
	
private:
	bool pMatchContainer(deVFSContainer &container,
		const decPath &absolutePath, decPath &realtivePath) const;
	bool pMatchContainerParent(deVFSContainer &container, const decPath &path) const;
};

#endif
