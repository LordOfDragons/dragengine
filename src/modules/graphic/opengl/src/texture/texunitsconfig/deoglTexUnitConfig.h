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
 * \brief Configuration of a texture unit.
 * 
 * The texture unit is disabled if no texture or cube map is defined. Only
 * enabled texture units are set during rendering. Disabled texture units are
 * left alone to avoid state changes if not used at all by the shader.
 */
class deoglTexUnitConfig{
public:
	/** \brief Special Textures. */
	enum eSpecialTextures{
		/** \brief No special texture. */
		estNone,
		
		/** \brief Depth map of the previous render pass. */
		estPrevDepth,
		
		/** \brief Color of the previous render pass. */
		estPrevColor,
		
		/** \brief Direct environment map active map. */
		estDirectEnvMapActive,
		
		/** \brief Direct environment map fading map. */
		estDirectEnvMapFading,
		
		/** \brief Constant to add to opengl texture names for hashing. */
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
	/** \brief Create texture unit configuration. */
	deoglTexUnitConfig();
	
	/** \brief Clean up texture unit configuration. */
	~deoglTexUnitConfig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Texture or \em NULL if not used. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** \brief Cube map or \em NULL if not used. */
	inline deoglCubeMap *GetCubeMap() const{ return pCubeMap; }
	
	/** \brief Array texture or \em NULL if not used. */
	inline deoglArrayTexture *GetArrayTexture() const{ return pArrayTexture; }
	
	/** \brief Texture buffer object or 0 if not used. */
	inline GLuint GetTBO() const{ return pTBO; }
	
	/** \brief Special texture or eSpecialTextures::estNone if not used. */
	inline int GetSpecial() const{ return pSpecial; }
	
	/** \brief Texture sampler configuration or \em NULL if not used. */
	inline deoglTexSamplerConfig *GetSampler() const{ return pSampler; }
	
	/** \brief Apply texture unit configuration. */
	void Apply( deoglRenderThread &renderThread, int stage );
	
	/** \brief Set texture sampler configuration or \em NULL if not used. */
	void SetSampler( deoglTexSamplerConfig *sampler );
	
	/** \brief Unit is enabled. */
	bool IsEnabled() const;
	
	/** \brief Unit is disabled. */
	bool IsDisabled() const;
	
	/** \brief Disable unit. */
	void Disable();
	
	/** \brief Enable texture. */
	void EnableTexture( deoglTexture *texture, deoglTexSamplerConfig *sampler );
	
	/** \brief Enabled texture from skin texture channel. */
	void EnableTextureFromChannel( deoglRenderThread &renderThread, const deoglSkinTexture &skinTexture,
		deoglSkinChannel::eChannelTypes skinChannel, const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, deoglTexture *defaultTexture );
	
	/** \brief Enable cube map. */
	void EnableCubeMap( deoglCubeMap *cubemap, deoglTexSamplerConfig *sampler );
	
	/** \brief Enables cube map from a skin texture channel. */
	void EnableCubeMapFromChannel( deoglRenderThread &renderThread, const deoglSkinTexture &skinTexture,
		deoglSkinChannel::eChannelTypes skinChannel, const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, deoglCubeMap *defaultCubemap );
	
	/** \brief Enable array texture. */
	void EnableArrayTexture( deoglArrayTexture *texture, deoglTexSamplerConfig *sampler );
	
	/** \brief Enabled array texture from skin texture channel. */
	void EnableArrayTextureFromChannel( deoglRenderThread &renderThread, const deoglSkinTexture &skinTexture,
		deoglSkinChannel::eChannelTypes skinChannel, const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, deoglArrayTexture *defaultTexture );
	
	/** \brief Enable texture buffer object. */
	void EnableTBO( GLuint tbo );
	
	/** \brief Enable special texture. */
	void EnableSpecial( int special, deoglTexSamplerConfig *sampler );
	
	/** \brief Object equals another one. */
	bool Equals( const deoglTexUnitConfig &tuc ) const;
	
	/** \brief Set parameters from another texture unit configuration. */
	void SetFrom( const deoglTexUnitConfig &tuc );
	/*@}*/
};

#endif
