/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
