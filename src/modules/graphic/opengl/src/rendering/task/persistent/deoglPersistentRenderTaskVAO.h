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

#ifndef _DEOGLPERSISTENTRENDERTASKVAO_H_
#define _DEOGLPERSISTENTRENDERTASKVAO_H_

#include "../../../deoglBasics.h"

#include <dragengine/common/collection/decPointerDictionaryExt.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/math/decMath.h>

class deoglPersistentRenderTaskPool;
class deoglPersistentRenderTaskTexture;
class deoglQuickSorter;
class deoglPersistentRenderTaskInstance;
class deoglSharedSPB;
class deoglSharedSPBRTIGroup;
class deoglVAO;


/**
 * Persistent render task VAO.
 */
class deoglPersistentRenderTaskVAO{
private:
	deoglPersistentRenderTaskPool &pPool;
	decPointerLinkedList::cListEntry pLLTexture;
	
	deoglPersistentRenderTaskTexture *pParentTexture;
	const deoglVAO *pVAO;
	decPointerLinkedList pInstances;
	decPointerDictionaryExt pInstancesMap;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create persistent render task vao. */
	deoglPersistentRenderTaskVAO( deoglPersistentRenderTaskPool &pool );
	
	/** Clean up persistent render task vao. */
	~deoglPersistentRenderTaskVAO();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent texture. */
	inline deoglPersistentRenderTaskTexture *GetParentTexture() const{ return pParentTexture; }
	
	/** Set parent texture. */
	void SetParentTexture( deoglPersistentRenderTaskTexture *texture );
	
	/** VAO. */
	inline const deoglVAO *GetVAO() const{ return pVAO; }
	
	/** Set VAO. */
	void SetVAO( const deoglVAO *vao );
	
	
	
	/** Total amount of points. */
	int GetTotalPointCount() const;
	
	/** Total amount of sub-instances. */
	int GetTotalSubInstanceCount() const;
	
	
	
	/** Number of instances. */
	int GetInstanceCount() const;
	
	/** Root instance. */
	decPointerLinkedList::cListEntry *GetRootInstance() const;
	
	/** Instance with shared sub instance spb. */
	deoglPersistentRenderTaskInstance *GetInstanceWith( const deoglSharedSPBRTIGroup *group ) const;
	
	/** Add instance. */
	deoglPersistentRenderTaskInstance *AddInstance( deoglSharedSPB *spb = NULL,
		const deoglSharedSPBRTIGroup *group = NULL );
	
	/** Remove instance. */
	void RemoveInstance( deoglPersistentRenderTaskInstance *instance );
	
	/** Remove all instances. */
	void RemoveAllInstances();
	
	
	
	/** Clear. */
	void Clear();
	
	/** Remove from parent if empty. */
	void RemoveFromParentIfEmpty();
	
	
	
	/** Render task linked list. */
	inline decPointerLinkedList::cListEntry &GetLLTexture(){ return pLLTexture; }
	inline const decPointerLinkedList::cListEntry &GetLLTexture() const{ return pLLTexture; }
	/*@}*/
};

#endif
