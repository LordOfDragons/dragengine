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

#ifndef _DEOGLPERSISTENTRENDERTASKPIPELINE_H_
#define _DEOGLPERSISTENTRENDERTASKPIPELINE_H_

#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/collection/decPointerDictionaryExt.h>

class deoglPersistentRenderTask;
class deoglPersistentRenderTaskPool;
class deoglPipeline;
class deoglPersistentRenderTaskTexture;
class deoglSPBlockUBO;
class deoglTexUnitsConfig;


/**
 * Persistent render task pipeline.
 */
class deoglPersistentRenderTaskPipeline{
private:
	deoglPersistentRenderTaskPool &pPool;
	decPointerLinkedList::cListEntry pLLTask;
	
	deoglPersistentRenderTask *pParentTask;
	const deoglPipeline *pPipeline;
	deoglSPBlockUBO *pParamBlock;
	int pSPBInstanceIndexBase;
	int pDrawIDOffset;
	decPointerLinkedList pTextures;
	decPointerDictionaryExt pTexturesMap;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create persistent render task pipeline. */
	deoglPersistentRenderTaskPipeline( deoglPersistentRenderTaskPool &pool );
	
	/** Clean up persistent render task pipeline. */
	~deoglPersistentRenderTaskPipeline();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Total amount of points in this pipeline. */
	int GetTotalPointCount() const;
	
	/** Total amount of vaos in this pipeline. */
	int GetTotalVAOCount() const;
	
	/** Total amount of instances in this pipeline. */
	int GetTotalInstanceCount() const;
	
	/** Total amount of subinstances in this pipeline. */
	int GetTotalSubInstanceCount() const;
	
	/** Parent task. */
	inline deoglPersistentRenderTask *GetParentTask() const{ return pParentTask; }
	
	/** Set parent task. */
	void SetParentTask( deoglPersistentRenderTask *task );
	
	/** Pipeline. */
	inline const deoglPipeline *GetPipeline() const{ return pPipeline; }
	
	/** Set pipeline. */
	void SetPipeline( const deoglPipeline *pipeline );
	
	/** Pipeline parameter block or \em NULL if not used. */
	inline deoglSPBlockUBO *GetParameterBlock() const{ return pParamBlock; }
	
	/** Set pipeline parameter block or \em NULL if not used. */
	void SetParameterBlock( deoglSPBlockUBO *block );
	
	/** Pipeline parameter index of pSPBInstanceIndexBase or -1. */
	inline int GetSPBInstanceIndexBase() const{ return pSPBInstanceIndexBase; }
	
	/** Set pipeline parameter index of pSPBInstanceIndexBase or -1. */
	void SetSPBInstanceIndexBase( int parameter );
	
	/** Pipeline parameter index of pDrawIDOffset or -1. */
	inline int GetDrawIDOffset() const{ return pDrawIDOffset; }
	
	/** Set pipeline parameter index of pDrawIDOffset or -1. */
	void SetDrawIDOffset( int parameter );
	
	
	
	/** Number of textures. */
	int GetTextureCount() const;
	
	/** Root texture. */
	decPointerLinkedList::cListEntry *GetRootTexture() const;
	
	/** Texture with TUC or NULL. */
	deoglPersistentRenderTaskTexture *GetTextureWith( const deoglTexUnitsConfig *tuc ) const;
	
	/** Add texture. */
	deoglPersistentRenderTaskTexture *AddTexture( const deoglTexUnitsConfig *tuc );
	
	/** Remove texture. */
	void RemoveTexture( deoglPersistentRenderTaskTexture *texture );
	
	/** Remove all textuures. */
	void RemoveAllTextures();
	
	
	
	/** Clear. */
	void Clear();
	
	/** Remove from parent if empty. */
	void RemoveFromParentIfEmpty();
	
	
	
	/** Render task linked list. */
	inline decPointerLinkedList::cListEntry &GetLLTask(){ return pLLTask; }
	inline const decPointerLinkedList::cListEntry &GetLLTask() const{ return pLLTask; }
	/*@}*/
};

#endif
