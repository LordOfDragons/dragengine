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

#ifndef _DEOGLPERSISTENTRENDERTASKTEXTURE_H_
#define _DEOGLPERSISTENTRENDERTASKTEXTURE_H_

#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/collection/decPointerDictionaryExt.h>

class deoglPersistentRenderTaskPool;
class deoglPersistentRenderTaskPipeline;
class deoglTexUnitsConfig;
class deoglPersistentRenderTaskVAO;
class deoglShaderParameterBlock;
class deoglVAO;


/**
 * Persistent render task texture.
 */
class deoglPersistentRenderTaskTexture{
private:
	deoglPersistentRenderTaskPool &pPool;
	decPointerLinkedList::cListEntry pLLTexture;
	
	deoglPersistentRenderTaskPipeline *pParentPipeline;
	const deoglTexUnitsConfig *pTUC;
	const deoglShaderParameterBlock *pParamBlock;
	decPointerLinkedList pVAOs;
	decPointerDictionaryExt pVAOsMap;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new render task pipeline. */
	deoglPersistentRenderTaskTexture( deoglPersistentRenderTaskPool &pool );
	
	/** Cleans up the render task pipeline. */
	~deoglPersistentRenderTaskTexture();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the total amount of points in this texture. */
	int GetTotalPointCount() const;
	
	/** Retrieves the total amount of instances in this texture. */
	int GetTotalInstanceCount() const;
	
	/** Retrieves the total amount of subinstances in this texture. */
	int GetTotalSubInstanceCount() const;
	
	/** Parent pipeline. */
	inline deoglPersistentRenderTaskPipeline *GetParentPipeline() const{ return pParentPipeline; }
	
	/** Set parent pipeline. */
	void SetParentPipeline( deoglPersistentRenderTaskPipeline *pipeline );
	
	/** Texture units configuration. */
	inline const deoglTexUnitsConfig *GetTUC() const{ return pTUC; }
	
	/** Set texture units configuration. */
	void SetTUC( const deoglTexUnitsConfig *tuc );
	
	/** Shader parameter block or NULL if not used. */
	inline const deoglShaderParameterBlock *GetParameterBlock() const{ return pParamBlock; }
	
	/** Set shader parameter block or NULL if not used. */
	void SetParameterBlock( const deoglShaderParameterBlock *block );
	
	
	
	/** Number of VAOs. */
	int GetVAOCount() const;
	
	/** Root VAO. */
	decPointerLinkedList::cListEntry *GetRootVAO() const;
	
	/** Texture with TUC or NULL. */
	deoglPersistentRenderTaskVAO *GetVAOWith( const deoglVAO *vao ) const;
	
	/** Add texture. */
	deoglPersistentRenderTaskVAO *AddVAO( const deoglVAO *vao );
	
	/** Remove vao. */
	void RemoveVAO( deoglPersistentRenderTaskVAO *vao );
	
	/** Remove all VAOs. */
	void RemoveAllVAOs();
	
	
	
	/** Clear. */
	void Clear();
	
	/** Remove from parent if empty. */
	void RemoveFromParentIfEmpty();
	
	
	
	/** Render task linked list. */
	inline decPointerLinkedList::cListEntry &GetLLTexture(){ return pLLTexture; }
	inline const decPointerLinkedList::cListEntry &GetLLShader() const{ return pLLTexture; }
	/*@}*/
};

#endif
