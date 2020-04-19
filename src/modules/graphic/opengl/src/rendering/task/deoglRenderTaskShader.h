/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRENDERTASKSHADER_H_
#define _DEOGLRENDERTASKSHADER_H_

class deoglShaderProgram;
class deoglRenderTaskTexture;
class deoglSPBlockUBO;



/**
 * \brief Render Task Shader.
 */
class deoglRenderTaskShader{
private:
	deoglShaderProgram *pShader;
	deoglSPBlockUBO *pParamBlock;
	int pSPBInstanceIndexBase;
	
	deoglRenderTaskTexture *pRootTexture;
	deoglRenderTaskTexture *pTailTexture;
	int pTextureCount;
	
	deoglRenderTaskTexture **pHasTexture;
	int pHasTextureCount;
	int pHasTextureSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render task shader. */
	deoglRenderTaskShader();
	
	/** \brief Clean up render task shader. */
	~deoglRenderTaskShader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Reset shader. */
	void Reset();
	
	/** \brief Total amount of points in this shader. */
	int GetTotalPointCount() const;
	
	/** \brief Total amount of vaos in this shader. */
	int GetTotalVAOCount() const;
	
	/** \brief Total amount of instances in this shader. */
	int GetTotalInstanceCount() const;
	
	/** \brief Total amount of subinstances in this shader. */
	int GetTotalSubInstanceCount() const;
	
	/** \brief Shader. */
	inline deoglShaderProgram *GetShader() const{ return pShader; }
	
	/** \brief Set shader. */
	void SetShader( deoglShaderProgram *shader );
	
	/** \brief Shader parameter block or \em NULL if not used. */
	inline deoglSPBlockUBO *GetParameterBlock() const{ return pParamBlock; }
	
	/** \brief Set shader parameter block or \em NULL if not used. */
	void SetParameterBlock( deoglSPBlockUBO *block );
	
	/** \brief Shader parameter index of pSPBInstanceIndexBase or -1. */
	inline int GetSPBInstanceIndexBase() const{ return pSPBInstanceIndexBase; }
	
	/** \brief Set shader parameter index of pSPBInstanceIndexBase or -1. */
	void SetSPBInstanceIndexBase( int parameter );
	
	/** \brief Root render task texture or \em NULL if there is none. */
	inline deoglRenderTaskTexture *GetRootTexture() const{ return pRootTexture; }
	
	/** \brief Number of render task textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** \brief Add render task texture. */
	void AddTexture( deoglRenderTaskTexture *texture );
	
	/** \brief Texture for texture units configuration index or \em NULL if absent. */
	deoglRenderTaskTexture *GetTextureForIndex( int tucIndex );
	/*@}*/
};

#endif
