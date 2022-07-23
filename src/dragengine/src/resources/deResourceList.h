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

#ifndef _DERESOURCELIST_H_
#define _DERESOURCELIST_H_

#include "../dragengine_export.h"

class deResource;


/**
 * \brief Resource list.
 * 
 * The resource list class is the backend for resource management. This list
 * contains the main reference to the resource object. The list does not
 * call 'delete' on the resource object but removes the pointer to it.
 * The resource object calls 'delete' on itself thus never call it yourself
 * except one situation. If you have created a resource object and have not
 * added it to the resource list yet you have to use call 'delete' on it
 * yourself to avoid triggering RemoveResource on the resource manager.
 * 
 * \note Resources removed from the list are marked leaking. Basically this
 * clears their resource manager. 
 */
class DE_DLL_EXPORT deResourceList{
private:
	deResource *pRoot;
	deResource *pTail;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create resource list. */
	deResourceList();
	
	/** \brief Clean up resource list. */
	virtual ~deResourceList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of resources. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief Root resource. */
	inline deResource *GetRoot() const{ return pRoot; }
	
	/** \brief Tail resource. */
	inline deResource *GetTail() const{ return pTail; }
	
	/** \brief Resource is present. */
	bool Has( deResource *resource ) const;
	
	/** \brief Add resource. */
	void Add( deResource *resource );
	
	/** \brief Remove resource. */
	void Remove( deResource *resource );
	
	/** \brief Remove resource if present. */
	void RemoveIfPresent( deResource *resource );
	
	/** \brief Remove all resources. */
	void RemoveAll();
	/*@}*/
	
	
	
private:
	void pBareRemove( deResource *resource );
};

#endif
