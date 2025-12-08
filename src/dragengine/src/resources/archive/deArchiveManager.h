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

#ifndef _DEARCHIVEMANAGER_H_
#define _DEARCHIVEMANAGER_H_

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

class deArchive;
class deArchiveContainer;


/**
 * \brief Archive resource manager.
 */
class DE_DLL_EXPORT deArchiveManager : public deFileResourceManager{
private:
	deFileResourceList pArchives;
	
	deArchiveContainer *pContainerRoot;
	deArchiveContainer *pContainerTail;
	int pContainerCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create archive resource manager. */
	deArchiveManager(deEngine *engine);
	
	/** \brief Clean up archive resource manager and report leaking resources. */
	~deArchiveManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of archives. */
	int GetArchiveCount() const;
	
	/** \brief Root archive resource for iteration purpose. */
	deArchive *GetRootArchive() const;
	
	/** \brief Archive with filename or NULL if not loaded yet. */
	deArchive *GetArchiveWith(const char *filename) const;
	
	/** \brief Archive with filename or NULL if not loaded yet. */
	deArchive *GetArchiveWith(deVirtualFileSystem *vfs, const char *filename) const;
	
	/** \brief Open archive from file relative to base path. */
	deArchive::Ref OpenArchive(const char *filename, const char *basePath);
	
	/** \brief Open archive from file relative to base path. */
	deArchive::Ref OpenArchive(deVirtualFileSystem *vfs, const char *filename, const char *basePath);
	
	/** \brief Create archive container. */
	deArchiveContainer *CreateContainer(const decPath &rootPath,
		deArchive *archive, const decPath &archivePath);
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * \warning For internal use only. Never call on your own!
	 */
	/*@{*/
	void RemoveResource(deResource *resource) override;
	void RemoveContainer(deArchiveContainer *container);
	/*@}*/
};

#endif
