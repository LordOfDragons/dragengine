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

#ifndef _DEFILERESOURCE_H_
#define _DEFILERESOURCE_H_

#include "deResource.h"
#include "../common/string/decString.h"
#include "../common/utils/decDateTime.h"

class deVirtualFileSystem;
class deFileResourceManager;


/**
 * \brief Base file resource class.
 *
 * Provides the basic class for all resources loadable from a file. The asynchron
 * flag indicates if this resource is currently handled by the resource loader.
 * Asynchron resources exist in a thread alongside the main thread and can require
 * special treatment in single type modules.
 */
class DE_DLL_EXPORT deFileResource : public deResource{
private:
	deVirtualFileSystem *pVirtualFileSystem;
	decString pFilename;
	TIME_SYSTEM pModificationTime;
	bool pAsynchron;
	bool pOutdated;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deFileResource> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file resource. */
	deFileResource(deFileResourceManager *resourceManager, deVirtualFileSystem *vfs,
		const char *filename, TIME_SYSTEM modificationTime);
	
protected:
	/**
	 * \brief Clean up file resource.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deFileResource();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Virtual file system or NULL if build from memory. */
	inline deVirtualFileSystem *GetVirtualFileSystem() const{return pVirtualFileSystem;}
	
	/** \brief Filename or empty string if build from memory. */
	inline const decString &GetFilename() const{return pFilename;}
	
	/** \brief Modification time used to detect resources changing on disk while loaded. */
	inline TIME_SYSTEM GetModificationTime() const{return pModificationTime;}
	
	/**
	 * \brief Set modification time used to detect resources changing on disk while loaded.
	 * \warning Internal Use Only. Do not call!
	 */
	void SetModificationTime(TIME_SYSTEM modificationTime);
	
	/** \brief Resource is asynchron. */
	inline bool GetAsynchron() const{return pAsynchron;}
	
	/** \brief Set if resource is asynchron. */
	void SetAsynchron(bool asynchron);
	/*@}*/
	
	
	
	/**
	 * \name Internal Use Only
	 * \warning Internal Use Only. Do not call!
	 */
	/*@{*/
	inline bool GetOutdated() const{return pOutdated;}
	void MarkOutdated();
	/*@}*/
};

#endif
