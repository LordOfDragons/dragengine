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

#ifndef _DERESOURCELOADERINFO_H_
#define _DERESOURCELOADERINFO_H_

#include "deResourceLoader.h"
#include "../../common/string/decString.h"

class deFileResource;


/**
 * \brief Resource loader info.
 *
 * Info structure used query finished task details. Stores the path, resource
 * and resource type. If the resource is NULL the loading failed. Otherwise the
 * loaded resource is stored.
 */
class DE_DLL_EXPORT deResourceLoaderInfo{
private:
	decString pPath;
	deResourceLoader::eResourceType pResourceType;
	deFileResource *pResource;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create info object. */
	deResourceLoaderInfo();
	
	/** \brief Clean up info object. */
	~deResourceLoaderInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Path. */
	inline const decString &GetPath() const{return pPath;}
	
	/** \brief Set path. */
	void SetPath(const char *path);
	
	/** \brief Resource type. */
	inline deResourceLoader::eResourceType GetResourceType() const{return pResourceType;}
	
	/** \brief Set resource type. */
	void SetResourceType(deResourceLoader::eResourceType resourceType);
	
	/** \brief Resource or NULL if not ready yet. */
	inline deFileResource *GetResource() const{return pResource;}
	
	/** \brief Set resource or NULL if not ready yet. */
	void SetResource(deFileResource *resource);
	/*@}*/
};

#endif
