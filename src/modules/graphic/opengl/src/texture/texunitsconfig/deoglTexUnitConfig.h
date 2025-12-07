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

#ifndef _DEOGLTEXUNITCONFIG_H_
#define _DEOGLTEXUNITCONFIG_H_

#include "../../deoglBasics.h"
#include "../../skin/channel/deoglSkinChannel.h"

class deoglArrayTexture;
class deoglCubeMap;
class deoglRDynamicSkin;
class deoglSkinChannel;
class deoglSkinState;
class deoglSkinTexture;
class deoglTexSamplerConfig;
class deoglTexture;
class deoglRenderThread;



/**
 * Configuration of a texture unit.
 * 
 * The texture unit is disabled if no texture or cube map is defined. Only
 * enabled texture units are set during rendering. Disabled texture units are
 * left alone to avoid state changes if not used at all by the shader.
 */
class deoglTexUnitConfig{
public:
	/** Special Textures. */
	enum eSpecialTextures{
		/** No special texture. */
		estNone,
		
		/** Depth map of the previous render pass. */
		estPrevDepth,
		
		/** Color of the previous render pass. */
		estPrevColor,
		
		/** Direct environment map active map. */
		estDirectEnvMapActive,
		
		/** Direct environment map fading map. */
		estDirectEnvMapFading,
		
		/** XRay depth. */
		estXRayDepth,
		
		/** Constant to add to opengl texture names for hashing. */
		EST_BASE_GLNAME
	};
	
	
	
public:
	deoglTexture *pTexture;
	deoglCubeMap *pCubeMap;
	deoglArrayTexture *pArrayTexture;
	GLuint pTBO;
	int pSpecial;
	deoglTexSamplerConfig *pSampler;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create texture unit configuration. */
	deoglTexUnitConfig();
	
	/** Clean up texture unit configuration. */
	~deoglTexUnitConfig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Texture or \em NULL if not used. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** Cube map or \em NULL if not used. */
	inline deoglCubeMap *GetCubeMap() const{ return pCubeMap; }
	
	/** Array texture or \em NULL if not used. */
	inline deoglArrayTexture *GetArrayTexture() const{ return pArrayTexture; }
	
	/** Texture buffer object or 0 if not used. */
	inline GLuint GetTBO() const{ return pTBO; }
	
	/** Special texture or eSpecialTextures::estNone if not used. */
	inline int GetSpecial() const{ return pSpecial; }
	
	/** Texture sampler configuration or \em NULL if not used. */
	inline deoglTexSamplerConfig *GetSampler() const{ return pSampler; }
	
	/** Apply texture unit configuration. */
	void Apply(deoglRenderThread &renderThread, int stage) const;
	
	/** Set texture sampler configuration or \em NULL if not used. */
	void SetSampler(deoglTexSamplerConfig *sampler);
	
	/** Unit is enabled. */
	bool IsEnabled() const;
	
	/** Unit is disabled. */
	bool IsDisabled() const;
	
	/** Disable unit. */
	void Disable();
	
	/** Enable texture. */
	void EnableTexture(deoglTexture *texture, deoglTexSamplerConfig *sampler);
	
	/** Enabled texture from skin texture channel. */
	void EnableTextureFromChannel(deoglRenderThread &renderThread, const deoglSkinTexture &skinTexture,
		deoglSkinChannel::eChannelTypes skinChannel, const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, deoglTexture *defaultTexture);
	
	/** Enable cube map. */
	void EnableCubeMap(deoglCubeMap *cubemap, deoglTexSamplerConfig *sampler);
	
	/** Enables cube map from a skin texture channel. */
	void EnableCubeMapFromChannel(deoglRenderThread &renderThread, const deoglSkinTexture &skinTexture,
		deoglSkinChannel::eChannelTypes skinChannel, const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, deoglCubeMap *defaultCubemap);
	
	/** Enable array texture. */
	void EnableArrayTexture(deoglArrayTexture *texture, deoglTexSamplerConfig *sampler);
	
	/** Enabled array texture from skin texture channel. */
	void EnableArrayTextureFromChannel(deoglRenderThread &renderThread, const deoglSkinTexture &skinTexture,
		deoglSkinChannel::eChannelTypes skinChannel, const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, deoglArrayTexture *defaultTexture);
	
	/** Enable texture buffer object. */
	void EnableTBO(GLuint tbo);
	
	/** Enable special texture. */
	void EnableSpecial(int special, deoglTexSamplerConfig *sampler);
	
	/** Object equals another one. */
	bool Equals(const deoglTexUnitConfig &tuc) const;
	
	/** Set parameters from another texture unit configuration. */
	void SetFrom(const deoglTexUnitConfig &tuc);
	/*@}*/
};

#endif
