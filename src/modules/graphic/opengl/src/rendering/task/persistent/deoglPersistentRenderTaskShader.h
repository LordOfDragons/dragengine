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

#ifndef _DEOGLPERSISTENTRENDERTASKSHADER_H_
#define _DEOGLPERSISTENTRENDERTASKSHADER_H_

#include <dragengine/common/collection/decPointerLinkedList.h>

class deoglPersistentRenderTaskPool;
class deoglShaderProgram;
class deoglPersistentRenderTaskTexture;
class deoglSPBlockUBO;
class deoglTexUnitsConfig;


/**
 * Persistent render task shader.
 */
class deoglPersistentRenderTaskShader{
private:
	deoglPersistentRenderTaskPool &pPool;
	decPointerLinkedList::cListEntry pLLTask;
	
	deoglShaderProgram *pShader;
	deoglSPBlockUBO *pParamBlock;
	int pSPBInstanceIndexBase;
	decPointerLinkedList pTextures;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create persistent render task shader. */
	deoglPersistentRenderTaskShader( deoglPersistentRenderTaskPool &pool );
	
	/** Clean up persistent render task shader. */
	~deoglPersistentRenderTaskShader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Total amount of points in this shader. */
	int GetTotalPointCount() const;
	
	/** Total amount of vaos in this shader. */
	int GetTotalVAOCount() const;
	
	/** Total amount of instances in this shader. */
	int GetTotalInstanceCount() const;
	
	/** Total amount of subinstances in this shader. */
	int GetTotalSubInstanceCount() const;
	
	/** Shader. */
	inline deoglShaderProgram *GetShader() const{ return pShader; }
	
	/** Set shader. */
	void SetShader( deoglShaderProgram *shader );
	
	/** Shader parameter block or \em NULL if not used. */
	inline deoglSPBlockUBO *GetParameterBlock() const{ return pParamBlock; }
	
	/** Set shader parameter block or \em NULL if not used. */
	void SetParameterBlock( deoglSPBlockUBO *block );
	
	/** Shader parameter index of pSPBInstanceIndexBase or -1. */
	inline int GetSPBInstanceIndexBase() const{ return pSPBInstanceIndexBase; }
	
	/** Set shader parameter index of pSPBInstanceIndexBase or -1. */
	void SetSPBInstanceIndexBase( int parameter );
	
	
	
	/** Number of textures. */
	int GetTextureCount() const;
	
	/** Root texture. */
	decPointerLinkedList::cListEntry *GetRootTexture() const;
	
	/** Texture with TUC or NULL. */
	deoglPersistentRenderTaskTexture *GetTextureWith( deoglTexUnitsConfig *tuc ) const;
	
	/** Add texture. */
	deoglPersistentRenderTaskTexture *AddTexture( deoglTexUnitsConfig *tuc );
	
	/** Remove texture. */
	void RemoveTexture( deoglPersistentRenderTaskTexture *texture );
	
	/** Remove all textuures. */
	void RemoveAllTextures();
	
	
	
	/** Clear. */
	void Clear();
	
	
	
	/** Render task linked list. */
	inline decPointerLinkedList::cListEntry &GetLLTask(){ return pLLTask; }
	inline const decPointerLinkedList::cListEntry &GetLLTask() const{ return pLLTask; }
	/*@}*/
};

#endif
