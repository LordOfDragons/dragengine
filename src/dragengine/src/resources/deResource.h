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

#ifndef _DERESOURCE_H_
#define _DERESOURCE_H_

#include "../deObject.h"

class deResourceManager;
class deEngine;
class decBaseFileReader;


/**
 * \brief Base resource class.
 * 
 * Provides the basic class for all resources. Each resource is assigned
 * the resource manager it originates from and a reference count. If you
 * want to add a reference to this resource do so using the AddReference
 * function. If you want to free the reference you have added before use
 * the FreeReference function and do not call 'delete' on the resource.
 * Once you receive as resource manager subclass the RemoveResource call
 * you have to use 'delete' to dispose of the resource and only then.
 */
class deResource : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deResource> Ref;
	
	
	
private:
	deResourceManager *pResourceManager;
	deResource *pLLManagerPrev;
	deResource *pLLManagerNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create resource. */
	deResource( deResourceManager *resourceManager );
	
protected:
	/**
	 * \brief Clean up resource.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deResource();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Resource manager or NULL if resource is leaking. */
	inline deResourceManager *GetResourceManager() const{ return pResourceManager; }
	
	/**
	 * \brief Game engine object from resource manager.
	 * \throw deeInvalidAction if resource is leaking and has no resource manager anymore.
	 */
	deEngine *GetEngine() const;
	/*@}*/
	
	
	
	/** \name Resource manager linked list */
	/*@{*/
	/** \brief Previous resource in the resource manager linked list. */
	inline deResource *GetLLManagerPrev() const{ return pLLManagerPrev; }
	
	/**
	 * \brief Set next resource in the resource manager linked list.
	 * \warning For use by resource managers only.
	 */
	void SetLLManagerPrev( deResource *resource );
	
	/** \brief Next resource in the resource manager linked list. */
	inline deResource *GetLLManagerNext() const{ return pLLManagerNext; }
	
	/**
	 * \brief Set next resource in the resource manager linked list.
	 * \warning For use by resource managers only.
	 */
	void SetLLManagerNext( deResource *resource );
	
	/**
	 * \brief Marks the resource leaking.
	 * \warning For use by resource managers only.
	 */
	void MarkLeaking();
	/*@}*/
};

#endif
