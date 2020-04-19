/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
class deResourceLoaderInfo{
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
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set path. */
	void SetPath( const char *path );
	
	/** \brief Resource type. */
	inline deResourceLoader::eResourceType GetResourceType() const{ return pResourceType; }
	
	/** \brief Set resource type. */
	void SetResourceType( deResourceLoader::eResourceType resourceType );
	
	/** \brief Resource or NULL if not ready yet. */
	inline deFileResource *GetResource() const{ return pResource; }
	
	/** \brief Set resource or NULL if not ready yet. */
	void SetResource( deFileResource *resource );
	/*@}*/
};

#endif
