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

#ifndef _DEOGLPERSISTENTRENDERTASKSUBINSTANCE_H_
#define _DEOGLPERSISTENTRENDERTASKSUBINSTANCE_H_

#include <dragengine/common/collection/decPointerLinkedList.h>

class deoglPersistentRenderTaskPool;
class deoglPersistentRenderTaskInstance;



/**
 * Persistent render task sub instance.
 */
class deoglPersistentRenderTaskSubInstance{
private:
	deoglPersistentRenderTaskPool &pPool;
	decPointerLinkedList::cListEntry pLLInstance;
	
	deoglPersistentRenderTaskInstance *pParentInstance;
	
	int pIndexInstance;
	int pFlags;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task instance. */
	deoglPersistentRenderTaskSubInstance(deoglPersistentRenderTaskPool &pool);
	
	/** Clean up render task instance. */
	~deoglPersistentRenderTaskSubInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent instance. */
	inline deoglPersistentRenderTaskInstance *GetParentInstance() const{return pParentInstance;}
	
	/** Set parent instance. */
	void SetParentInstance(deoglPersistentRenderTaskInstance *instance);
	
	/** Instance index. */
	inline int GetIndexInstance() const{return pIndexInstance;}
	
	/** Set instance index. */
	void SetIndexInstance(int indexInstance);
	
	/** Flags. */
	inline int GetFlags() const{return pFlags;}
	
	/** Set flags. */
	void SetFlags(int flags);
	
	
	
	/** Clear. */
	void Clear();
	
	
	
	/** Render task linked list. */
	inline decPointerLinkedList::cListEntry &GetLLInstance(){return pLLInstance;}
	inline const decPointerLinkedList::cListEntry &GetLLInstance() const{return pLLInstance;}
	/*@}*/
};

#endif
