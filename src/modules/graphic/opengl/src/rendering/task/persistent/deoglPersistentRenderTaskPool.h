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

#ifndef _DEOGLPERSISTENTRENDERTASKPOOL_H_
#define _DEOGLPERSISTENTRENDERTASKPOOL_H_

#include <dragengine/common/collection/decPointerList.h>

class deoglPersistentRenderTaskSubInstance;
class deoglPersistentRenderTaskInstance;
class deoglPersistentRenderTaskOwner;
class deoglPersistentRenderTaskPipeline;
class deoglPersistentRenderTaskTexture;
class deoglPersistentRenderTaskVAO;


/**
 * GLobal pool for persistent render task resources.
 */
class deoglPersistentRenderTaskPool{
private:
	decPointerList pOwners;
	decPointerList pPipelines;
	decPointerList pTextures;
	decPointerList pVAOs;
	decPointerList pInstances;
	decPointerList pSubInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create persistent render task pool. */
	deoglPersistentRenderTaskPool();
	
	/** Clean up render task pool. */
	~deoglPersistentRenderTaskPool();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Get owner. */
	deoglPersistentRenderTaskOwner *GetOwner();
	
	/** Get pipeline. */
	deoglPersistentRenderTaskPipeline *GetPipeline();
	
	/** Get texture. */
	deoglPersistentRenderTaskTexture *GetTexture();
	
	/** Get VAO. */
	deoglPersistentRenderTaskVAO *GetVAO();
	
	/** Get instance. */
	deoglPersistentRenderTaskInstance *GetInstance();
	
	/** Get sub instance. */
	deoglPersistentRenderTaskSubInstance *GetSubInstance();
	
	
	
	/** Return owner. */
	void ReturnOwner( deoglPersistentRenderTaskOwner *owner );
	
	/** Return pipeline. */
	void ReturnPipeline( deoglPersistentRenderTaskPipeline *pipeline );
	
	/** Return texture. */
	void ReturnTexture( deoglPersistentRenderTaskTexture *texture );
	
	/** Return VAO. */
	void ReturnVAO( deoglPersistentRenderTaskVAO *vao );
	
	/** Return instance. */
	void ReturnInstance( deoglPersistentRenderTaskInstance *instance );
	
	/** Return sub instance. */
	void ReturnSubInstance( deoglPersistentRenderTaskSubInstance *subInstance );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateInitial();
};

#endif
