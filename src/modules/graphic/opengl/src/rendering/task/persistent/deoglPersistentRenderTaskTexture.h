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

#ifndef _DEOGLPERSISTENTRENDERTASKTEXTURE_H_
#define _DEOGLPERSISTENTRENDERTASKTEXTURE_H_

#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/collection/decPointerDictionaryExt.h>

class deoglPersistentRenderTaskPool;
class deoglPersistentRenderTaskShader;
class deoglTexUnitsConfig;
class deoglPersistentRenderTaskVAO;
class deoglSPBlockUBO;
class deoglVAO;


/**
 * Persistent render task texture.
 */
class deoglPersistentRenderTaskTexture{
private:
	deoglPersistentRenderTaskPool &pPool;
	decPointerLinkedList::cListEntry pLLShader;
	
	deoglPersistentRenderTaskShader *pParentShader;
	deoglTexUnitsConfig *pTUC;
	deoglSPBlockUBO *pParamBlock;
	decPointerLinkedList pVAOs;
	decPointerDictionaryExt pVAOsMap;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new render task shader. */
	deoglPersistentRenderTaskTexture( deoglPersistentRenderTaskPool &pool );
	
	/** Cleans up the render task shader. */
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
	
	/** Parent shader. */
	inline deoglPersistentRenderTaskShader *GetParentShader() const{ return pParentShader; }
	
	/** Set parent shader. */
	void SetParentShader( deoglPersistentRenderTaskShader *shader );
	
	/** Texture units configuration. */
	inline deoglTexUnitsConfig *GetTUC() const{ return pTUC; }
	
	/** Set texture units configuration. */
	void SetTUC( deoglTexUnitsConfig *tuc );
	
	/** Shader parameter block or NULL if not used. */
	inline deoglSPBlockUBO *GetParameterBlock() const{ return pParamBlock; }
	
	/** Set shader parameter block or NULL if not used. */
	void SetParameterBlock( deoglSPBlockUBO *block );
	
	
	
	/** Number of VAOs. */
	int GetVAOCount() const;
	
	/** Root VAO. */
	decPointerLinkedList::cListEntry *GetRootVAO() const;
	
	/** Texture with TUC or NULL. */
	deoglPersistentRenderTaskVAO *GetVAOWith( deoglVAO *vao ) const;
	
	/** Add texture. */
	deoglPersistentRenderTaskVAO *AddVAO( deoglVAO *vao );
	
	/** Remove vao. */
	void RemoveVAO( deoglPersistentRenderTaskVAO *vao );
	
	/** Remove all VAOs. */
	void RemoveAllVAOs();
	
	
	
	/** Clear. */
	void Clear();
	
	/** Remove from parent if empty. */
	void RemoveFromParentIfEmpty();
	
	
	
	/** Render task linked list. */
	inline decPointerLinkedList::cListEntry &GetLLShader(){ return pLLShader; }
	inline const decPointerLinkedList::cListEntry &GetLLShader() const{ return pLLShader; }
	/*@}*/
};

#endif
