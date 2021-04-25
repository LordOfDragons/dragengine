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

#ifndef _DEOGLPERSISTENTRENDERTASKOWNER_H_
#define _DEOGLPERSISTENTRENDERTASKOWNER_H_

#include <dragengine/deObjectReference.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/collection/decPointerLinkedList.h>

class deoglPersistentRenderTaskInstance;
class deoglPersistentRenderTaskSubInstance;


/**
 * Persistent render task owner. Keeps track of what instances and subinstance a tracked
 * object has added to the persistent render task. This allows to remove them quicker.
 */
class deoglPersistentRenderTaskOwner{
private:
	decPointerLinkedList::cListEntry pLLTask;
	
	deObjectReference pOwner;
	unsigned int pHash;
	bool pUpdateMarker;
	
	decPointerList pInstances;
	decPointerList pSubInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task owner. */
	deoglPersistentRenderTaskOwner();
	
	/** Clean up render task owner. */
	~deoglPersistentRenderTaskOwner();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Owner object or NULL. */
	inline deObject *GetOwner() const{ return pOwner; }
	
	/** Hash. */
	inline unsigned int GetHash() const{ return pHash; }
	
	/** Set owner object or NULL. */
	void SetOwner( deObject *owner, unsigned int hash );
	
	/** Update marker. */
	inline bool GetUpdateMarker() const{ return pUpdateMarker; }
	
	/** Set update marker. */
	inline void SetUpdateMarker( bool marker ){ pUpdateMarker = marker; }
	
	
	
	/** Count of instances. */
	int GetInstanceCount() const;
	
	/** Instance at index. */
	deoglPersistentRenderTaskInstance *GetInstanceAt( int index ) const;
	
	/** Add instance. */
	void AddInstance( deoglPersistentRenderTaskInstance *instance );
	
	/** Remove all instances. */
	void RemoveAllInstances();
	
	
	
	/** Count of sub instances. */
	int GetSubInstanceCount() const;
	
	/** Sub instance at index. */
	deoglPersistentRenderTaskSubInstance *GetSubInstanceAt( int index ) const;
	
	/** Add sub instance. */
	void AddSubInstance( deoglPersistentRenderTaskSubInstance *subInstance );
	
	/** Remove all sub instances. */
	void RemoveAllSubInstances();
	
	
	
	/** Clear. */
	void Clear();
	
	
	
	/** Render task linked list. */
	inline decPointerLinkedList::cListEntry &GetLLTask(){ return pLLTask; }
	inline const decPointerLinkedList::cListEntry &GetLLTask() const{ return pLLTask; }
	/*@}*/
};

#endif
