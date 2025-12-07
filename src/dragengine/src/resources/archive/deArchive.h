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

#ifndef _DEARCHIVE_H_
#define _DEARCHIVE_H_

#include "../deFileResource.h"

class deArchiveContainer;
class deArchiveManager;
class deBaseArchiveContainer;


/**
 * \brief Archive Resource.
 *
 * Provides access to files stored in a file archive. The archive can be compressed
 * or just grouping files. The archive can be placed in a virtual file system by
 * obtaining a deArchiveContainer.
 * 
 * To actually access the files you have to create an Archive Container using
 * deArchiveManager::CreateContainer(). Archive containers can be read-write if the
 * underlaying Archive Module supports writing to the archive file. In general
 * archive containers are read-only.
 */
class DE_DLL_EXPORT deArchive : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deArchive> Ref;
	
	
private:
	deBaseArchiveContainer *pPeerContainer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create archive resource. */
	deArchive(deArchiveManager *manager, deVirtualFileSystem *vfs,
		const char *filename, TIME_SYSTEM modificationTime);
	
protected:
	/**
	 * \brief Clean up archive resource.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deArchive();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/*@}*/
	
	
	
	/** \name Peers */
	/*@{*/
	/** \brief Container peer. */
	deBaseArchiveContainer *GetPeerContainer() const{ return pPeerContainer; }
	
	/** \brief Set peer container. */
	void SetPeerContainer(deBaseArchiveContainer *peer);
	/*@}*/
};

#endif
