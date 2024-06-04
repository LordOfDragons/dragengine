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

#ifndef _DEOGLRRTSHADER_H_
#define _DEOGLRRTSHADER_H_

#include "../deoglBasics.h"
#include "../shaders/paramblock/deoglSPBlockSSBO.h"

class deoglLightShaderManager;
class deoglRenderThread;
class deoglShaderManager;
class deoglShaderProgram;
class deoglShaderDefines;
class deoglSkinShaderManager;
class deoglTexSamplerConfig;
class deoglTexUnitsConfigList;


/**
 * Render thread shader related objects.
 */
class deoglRTShader{
public:
	/** Texture sampler configurations. */
	enum eTextureSamplerConfigurations{
		/** Clamp to edge with nearest filtering without mip mapping. */
		etscClampNearest,
		
		/** Clamp to edge with linear filtering without mip mapping. */
		etscClampLinear,
		
		/** Clamp to edge with nearest filtering with mip mapping. */
		etscClampNearestMipMap,
		
		/** Clamp to edge with linear filtering with mip mapping. */
		etscClampLinearMipMap,
		
		/** Clamp to edge with linear filtering with nearest mip mapping. */
		etscClampLinearMipMapNearest,
		
		/** Repeating with nearest filtering without mip mapping. */
		etscRepeatNearest,
		
		/** Repeating with linear filtering without mip mapping. */
		etscRepeatLinear,
		
		/** Repeating with linear filtering with mip mapping. */
		etscRepeatLinearMipMap,
		
		/** Clamped shadow testing with nearest filtering. */
		etscShadowClampNearest,
		
		/** Clamped shadow testing with linear filtering. */
		etscShadowClampLinear,
		
		/** Clamped shadow testing with linear filtering for inverse depth testing. */
		etscShadowClampLinearInverse,
		
		/** Repeating shadow testing with nearest filtering. */
		etscShadowRepeatNearest,
		
		/** Repeating shadow testing with linear filtering. */
		etscShadowRepeatLinear,
		
		/** Number of texture sampler configurations. */
		ETSC_COUNT
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexUnitsConfigList *pTexUnitsConfigList;
	deoglTexSamplerConfig *pTexSamplerConfigs[ ETSC_COUNT ];
	
	deoglShaderManager *pShaderManager;
	deoglSkinShaderManager *pSkinShaderManager;
	deoglLightShaderManager *pLightShaderManager;
	const deoglShaderProgram *pCurShaderProg;
	
	deoglSPBlockSSBO::Ref pSSBOSkinTextures;
	bool pDirtySSBOSkinTextures;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread texture related object. */
	deoglRTShader( deoglRenderThread &renderThread );
	
	/** Clean up render thread texture related object. */
	virtual ~deoglRTShader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Texture units configuration manager. */
	inline deoglTexUnitsConfigList &GetTexUnitsConfigList() const{ return *pTexUnitsConfigList; }
	
	/** Texture sampler configuration. */
	deoglTexSamplerConfig *GetTexSamplerConfig( const eTextureSamplerConfigurations type ) const;
	
	/** Shader manager. */
	inline deoglShaderManager &GetShaderManager() const{ return *pShaderManager; }
	
	/** Skin shader manager. */
	inline deoglSkinShaderManager &GetSkinShaderManager() const{ return *pSkinShaderManager; }
	
	/** Light shader manager. */
	inline deoglLightShaderManager &GetLightShaderManager() const{ return *pLightShaderManager; }
	
	/** Current shader program or NULL if none is set. */
	inline const deoglShaderProgram *GetActiveShader() const{ return pCurShaderProg; }
	
	/** Activate shader if not active yet. */
	void ActivateShader( const deoglShaderProgram *shader );
	
	/** Add common defines. */
	void SetCommonDefines( deoglShaderDefines &defines ) const;
	
	
	
	/** SSBO skin textures. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOSkinTextures() const{ return pSSBOSkinTextures; }
	
	/** Invalidate SSBO skin textures. */
	void InvalidateSSBOSkinTextures();
	
	/** Update SSBO skin textures if required. */
	void UpdateSSBOSkinTextures();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateTexSamplerConfigs();
};

#endif
