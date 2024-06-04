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

#ifndef _DEOGLTEXTURESTAGEMANAGER_H_
#define _DEOGLTEXTURESTAGEMANAGER_H_

#include "../deoglBasics.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../texture/cubemap/deoglCubeMap.h"

#define OGL_MAX_TEXTURE_STAGES 16

class deoglArrayTexture;
class deoglRenderThread;
class deoglRSkin;
class deoglTexSamplerConfig;
class deoglTexture;



/**
 * Texture stage manager.
 * 
 * Allows to enable and disable stages and bind different kinds of textures.
 * Keeps track of bound textures avoiding not require state changes.
 */
class deoglTextureStageManager{
public:
	/** Filtering. */
	enum eTextureFiltering{
		/** Nearest. */
		etfNearest,
		
		/** Nearest with mipmap if possible. */
		etfNearestMipMap,
		
		/** Nearest with linear mipmap if possible. */
		etfNearestMipMapLinear,
		
		/** Linear no mipmap. */
		etfLinear,
		
		/** Linear with mipmap if possible. */
		etfLinearMipMap,
		
		/** Linear with nearest mipmap if possible. */
		etfLinearMipMapNearest,
	};
	
private:
	struct sStage{
		GLuint texture;
		GLenum type;
		GLuint samplerConfig;
		sStage();
	};
	
private:
	deoglRenderThread &pRenderThread;
	sStage pStages[ OGL_MAX_TEXTURE_STAGES ];
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create texture stage manager. */
	deoglTextureStageManager( deoglRenderThread &renderThread );
	
	/** Clean up texture stage manager. */
	~deoglTextureStageManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of enabled stages. */
	int GetEnabledStageCount() const;
	
	/** Type of texture bound to stage. */
	GLenum GetStageType( int stage ) const;
	
	/**
	 * Enable stage and bind array texture.
	 * \details Sets no texture parameters. Useful if an array texture has to be
	 *          bound only for manipulating opengl states and is unbound
	 *          immediately afterwards.
	 */
	void EnableBareArrayTexture( int stage, const deoglArrayTexture &texture );
	
	/**
	 * Enable stage and bind texture
	 * \details Sets no texture parameters. Useful if a texture has to be bound only
	 *          for manipulating opengl states and is unbound immediately afterwards.
	 */
	void EnableBareTexture( int stage, const deoglTexture &texture );
	
	/**
	 * Enable stage and bind cube map.
	 * \details Sets no texture parameters. Useful if a cube map has to be bound only
	 *          for manipulating opengl states and is unbound immediately afterwards.
	 */
	void EnableBareCubeMap( int stage, const deoglCubeMap &cubemap );
	
	/**
	 * Enable stage and bind tbo without sampler.
	 * \details Sets no texture paramters. Useful if a tbo has to be bound for for
	 *          manipulating opegl states and is unbound immediately afterwards.
	 */
	void EnableBareTBO( int stage, GLuint tbo );
	
	
	
	/** Enable stage and bind texture setting texture properties. */
	void EnableTexture( int stage, const deoglTexture &texture, deoglTexSamplerConfig &samplerConfig );
	
	/** Enable stage and bind cube map setting texture properties. */
	void EnableCubeMap( int stage, const deoglCubeMap &cubemap, deoglTexSamplerConfig &samplerConfig );
	
	/** Enable stage and bind cube map face setting texture properties. */
	void EnableCubeMapFace( int stage, const deoglCubeMap &cubemap,
		deoglCubeMap::eFaces face, deoglTexSamplerConfig &samplerConfig );
	
	/** Enable stage and bind array texture setting texture properties. */
	void EnableArrayTexture( int stage, const deoglArrayTexture &texture, deoglTexSamplerConfig &samplerConfig );
	
	/** Enable given stage and bind texture buffer object. */
	void EnableTBO( int stage, GLuint tbo, deoglTexSamplerConfig &samplerConfig );
	
	/**
	 * Enable stage and bind texture channel from a skin.
	 * \details This function is aware of different texture properties assigned to the given
	 *          texture and sets the texture parameters accordingly. If the texture in the skin
	 *          is for some reason invalid the given default texture is used. Should this also
	 *          fail the stage will be disabled.
	 */
	void EnableSkin( int stage, const deoglRSkin &skin, int texture,
		deoglSkinChannel::eChannelTypes channel, deoglTexture *defaultTexture,
		deoglTexSamplerConfig &samplerConfig );
	
	
	
	/** Disable stage. */
	void DisableStage( int stage );
	
	/**
	 * Disable all stages above stage.
	 * \details If you have for example 4 enabled stages and you want to disable stages
	 *          2 and 3 then you have to call this function with the stage of the last
	 *          stage to remain enabled thus 1 in this case.
	 */
	void DisableStagesAbove( int stage );
	
	/** Disable all stages. */
	void DisableAllStages();
	
	
	
	/**
	 * Activate stage and bind texture if not bound already.
	 * \details Keeps the currently bound texture sampler configuration the same.
	 */
	void BindTexture( int stage, GLuint texture, GLenum type );
	
	/**
	 * Activate stage and bind texture if not bound already.
	 * \details Binds also the texture sampler configuration if different than the current one.
	 */
	void BindTexture( int stage, GLuint texture, GLenum type, GLuint samplerConfig );
	
	
	
	/** Dump the current state to the logger. */
	void LogCurrentState() const;
	/*@}*/
};

#endif
