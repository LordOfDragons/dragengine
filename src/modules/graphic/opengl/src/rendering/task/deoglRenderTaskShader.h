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

#include <dragengine/common/collection/decPointerList.h>

class deoglRenderTaskSharedShader;
class deoglRenderTaskSharedTexture;
class deoglRenderTaskTexture;



/**
 * Render Task Shader.
 */
class deoglRenderTaskShader{
private:
	const deoglRenderTaskSharedShader *pShader;
	
	decPointerList pTextures;
	int pTextureCount;
	
	deoglRenderTaskTexture **pHasTexture;
	int pHasTextureCount;
	int pHasTextureSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task shader. */
	deoglRenderTaskShader();
	
	/** Clean up render task shader. */
	~deoglRenderTaskShader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset shader. */
	void Reset();
	
	/** Total amount of points in this shader. */
	int GetTotalPointCount() const;
	
	/** Total amount of vaos in this shader. */
	int GetTotalVAOCount() const;
	
	/** Total amount of instances in this shader. */
	int GetTotalInstanceCount() const;
	
	/** Total amount of subinstances in this shader. */
	int GetTotalSubInstanceCount() const;
	
	/** Shader. */
	inline const deoglRenderTaskSharedShader *GetShader() const{ return pShader; }
	
	/** Set shader. */
	void SetShader( const deoglRenderTaskSharedShader *shader );
	
	/** Number of render task textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** Render task texture at index. */
	deoglRenderTaskTexture *GetTextureAt( int index ) const;
	
	/** Add render task texture. */
	deoglRenderTaskTexture *AddTexture( deoglRenderTaskSharedTexture *texture );
	/*@}*/
};

#endif
