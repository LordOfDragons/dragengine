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

#ifndef _DEOGLRRTSHADER_H_
#define _DEOGLRRTSHADER_H_

#include "../deoglBasics.h"

class deoglLightShaderManager;
class deoglRenderThread;
class deoglShaderManager;
class deoglShaderProgram;
class deoglSkinShaderManager;
class deoglTexSamplerConfig;
class deoglTexUnitsConfigList;



/**
 * \brief Render thread shader related objects.
 */
class deoglRTShader{
public:
	/** \brief Texture sampler configurations. */
	enum eTextureSamplerConfigurations{
		/** \brief Clamp to edge with nearest filtering without mip mapping. */
		etscClampNearest,
		
		/** \brief Clamp to edge with linear filtering without mip mapping. */
		etscClampLinear,
		
		/** \brief Clamp to edge with nearest filtering with mip mapping. */
		etscClampNearestMipMap,
		
		/** \brief Clamp to edge with linear filtering with mip mapping. */
		etscClampLinearMipMap,
		
		/** \brief Clamp to edge with linear filtering with nearest mip mapping. */
		etscClampLinearMipMapNearest,
		
		/** \brief Repeating with nearest filtering without mip mapping. */
		etscRepeatNearest,
		
		/** \brief Repeating with linear filtering without mip mapping. */
		etscRepeatLinear,
		
		/** \brief Repeating with linear filtering with mip mapping. */
		etscRepeatLinearMipMap,
		
		/** \brief Clamped shadow testing with nearest filtering. */
		etscShadowClampNearest,
		
		/** \brief Clamped shadow testing with linear filtering. */
		etscShadowClampLinear,
		
		/** \brief Clamped shadow testing with linear filtering for inverse depth testing. */
		etscShadowClampLinearInverse,
		
		/** \brief Repeating shadow testing with nearest filtering. */
		etscShadowRepeatNearest,
		
		/** \brief Repeating shadow testing with linear filtering. */
		etscShadowRepeatLinear,
		
		/** \brief Number of texture sampler configurations. */
		ETSC_COUNT
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexUnitsConfigList *pTexUnitsConfigList;
	deoglTexSamplerConfig *pTexSamplerConfigs[ ETSC_COUNT ];
	
	deoglShaderManager *pShaderManager;
	deoglSkinShaderManager *pSkinShaderManager;
	deoglLightShaderManager *pLightShaderManager;
	deoglShaderProgram *pCurShaderProg;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render thread texture related object. */
	deoglRTShader( deoglRenderThread &renderThread );
	
	/** \brief Clean up render thread texture related object. */
	virtual ~deoglRTShader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Texture units configuration manager. */
	inline deoglTexUnitsConfigList &GetTexUnitsConfigList() const{ return *pTexUnitsConfigList; }
	
	/** \brief Texture sampler configuration. */
	deoglTexSamplerConfig *GetTexSamplerConfig( const eTextureSamplerConfigurations type ) const;
	
	/** \brief Shader manager. */
	inline deoglShaderManager &GetShaderManager() const{ return *pShaderManager; }
	
	/** \brief Skin shader manager. */
	inline deoglSkinShaderManager &GetSkinShaderManager() const{ return *pSkinShaderManager; }
	
	/** \brief Light shader manager. */
	inline deoglLightShaderManager &GetLightShaderManager() const{ return *pLightShaderManager; }
	
	/** \brief Current shader program or NULL if none is set. */
	inline deoglShaderProgram *GetActiveShader() const{ return pCurShaderProg; }
	
	/** \brief Activate shader if not active yet. */
	void ActivateShader( deoglShaderProgram *shader );
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateTexSamplerConfigs();
};

#endif
