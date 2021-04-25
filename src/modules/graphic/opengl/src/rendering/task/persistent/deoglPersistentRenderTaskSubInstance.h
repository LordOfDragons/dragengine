/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLPERSISTENTRENDERTASKSUBINSTANCE_H_
#define _DEOGLPERSISTENTRENDERTASKSUBINSTANCE_H_

#include <dragengine/common/collection/decPointerLinkedList.h>

class deoglPersistentRenderTaskPool;
class deoglPersistentRenderTaskInstance;
class deoglPersistentRenderTaskOwner;



/**
 * Persistent render task sub instance.
 */
class deoglPersistentRenderTaskSubInstance{
private:
	deoglPersistentRenderTaskPool &pPool;
	decPointerLinkedList::cListEntry pLLInstance;
	
	deoglPersistentRenderTaskInstance *pParentInstance;
	deoglPersistentRenderTaskOwner *pOwner;
	
	int pIndexInstance;
	int pFlags;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task instance. */
	deoglPersistentRenderTaskSubInstance( deoglPersistentRenderTaskPool &pool );
	
	/** Clean up render task instance. */
	~deoglPersistentRenderTaskSubInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent instance. */
	inline deoglPersistentRenderTaskInstance *GetParentInstance() const{ return pParentInstance; }
	
	/** Set parent instance. */
	void SetParentInstance( deoglPersistentRenderTaskInstance *instance );
	
	/** Owner object or NULL. */
	inline deoglPersistentRenderTaskOwner *GetOwner() const{ return pOwner; }
	
	/** Set owner object or NULL. */
	void SetOwner( deoglPersistentRenderTaskOwner *owner );
	
	
	
	/** Instance index. */
	inline int GetIndexInstance() const{ return pIndexInstance; }
	
	/** Set instance index. */
	void SetIndexInstance( int indexInstance );
	
	/** Flags. */
	inline int GetFlags() const{ return pFlags; }
	
	/** Set flags. */
	void SetFlags( int flags );
	
	
	
	/** Clear. */
	void Clear();
	
	
	
	/** Render task linked list. */
	inline decPointerLinkedList::cListEntry &GetLLInstance(){ return pLLInstance; }
	inline const decPointerLinkedList::cListEntry &GetLLInstance() const{ return pLLInstance; }
	/*@}*/
};

#endif
