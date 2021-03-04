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

#ifndef _DEOGLADDTORENDERTASKGIMATERIAL_H_
#define _DEOGLADDTORENDERTASKGIMATERIAL_H_

#include "../../deoglBasics.h"
#include "../../skin/deoglSkinTexture.h"

#include <dragengine/common/math/decMath.h>

class deoglCollideList;
class deoglCollideListComponent;
class deoglRComponentLOD;
class deoglRenderTask;
class deoglRenderTaskTexture;
class deoglRenderThread;
class deoglShaderProgram;
class deoglSkinShader;
class deoglSkinState;
class deoglSkinTexture;
class deoglSPBlockUBO;
class deoglTexture;
class deoglTexUnitsConfig;


/**
 * \brief Add elements to render task.
 * 
 * Provides support for filtering the elements using texture properties.
 */
class deoglAddToRenderTaskGIMaterial{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRenderTask &pRenderTask;
	deoglSkinTexture::eShaderTypes pSkinShaderType;
	int pMaterialMapCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new add to render task. */
	deoglAddToRenderTaskGIMaterial( deoglRenderThread &renderThread, deoglRenderTask &renderTask );
	
	/** \brief Clean up add to render task . */
	~deoglAddToRenderTaskGIMaterial();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Shader type to be used for skin shaders. */
	inline deoglSkinTexture::eShaderTypes GetSkinShaderType() const{ return pSkinShaderType; }
	
	/** \brief Set shader type to be used for skin shaders. */
	void SetSkinShaderType( deoglSkinTexture::eShaderTypes shaderType );
	
	
	
	/** \brief Reset render task parameters. */
	void Reset();
	
	/** \brief Material map index count. */
	inline int GetMaterialMapCount() const{ return pMaterialMapCount; }
	
	
	
	/**
	 * \brief Add component texture.
	 * 
	 * Component is supposed to be updated, and model, skin and parent world exist.
	 */
	deoglRenderTaskTexture *AddComponentTexture( deoglRComponentLOD &component, int texture );
	/*@}*/
	
	
	
private:
	bool pFilterReject( const deoglSkinTexture *skinTexture ) const;
	
	deoglRenderTaskTexture *pGetTaskTexture( deoglSkinTexture *skinTexture, deoglTexUnitsConfig *tuc );
};

#endif
