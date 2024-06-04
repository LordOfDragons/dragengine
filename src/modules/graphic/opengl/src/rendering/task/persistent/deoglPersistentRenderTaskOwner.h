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

#ifndef _DEOGLPERSISTENTRENDERTASKOWNER_H_
#define _DEOGLPERSISTENTRENDERTASKOWNER_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/math/decMath.h>

class deoglRComponent;
class deoglPersistentRenderTaskInstance;
class deoglPersistentRenderTaskSubInstance;


/**
 * Persistent render task owner. Keeps track of what instances and subinstance a tracked
 * object has added to the persistent render task. This allows to remove them quicker.
 */
class deoglPersistentRenderTaskOwner{
private:
	decPointerLinkedList::cListEntry pLLTask;
	
	void *pOwner;
	unsigned int pHash;
	bool pUpdateMarker;
	
	deoglRComponent *pComponent;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	
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
	inline void *GetOwner() const{ return pOwner; }
	
	/** Hash. */
	inline unsigned int GetHash() const{ return pHash; }
	
	/** Set owner object or NULL. */
	void SetOwner( void *owner, unsigned int hash );
	
	/** Update marker. */
	inline bool GetUpdateMarker() const{ return pUpdateMarker; }
	
	/** Set update marker. */
	inline void SetUpdateMarker( bool marker ){ pUpdateMarker = marker; }
	
	/** Minimum extend. */
	inline const decDVector &GetMinExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decDVector &GetMaxExtend() const{ return pMaxExtend; }
	
	/** Set extends. */
	void SetExtends( const decDVector &minExtend, const decDVector &maxExtend );
	
	
	
	/** Owner component or NULL. */
	inline deoglRComponent *GetComponent() const{ return pComponent; }
	
	/** Set owner component or NULL. */
	void SetComponent( deoglRComponent *component );
	
	
	
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
