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

#ifndef _DEOGLTEXTURESTAGEMANAGER_H_
#define _DEOGLTEXTURESTAGEMANAGER_H_

#include "../deoglBasics.h"
#include "../skin/channel/deoglSkinChannel.h"

#define OGL_MAX_TEXTURE_STAGES 16

class deoglArrayTexture;
class deoglCubeMap;
class deoglRenderThread;
class deoglRSkin;
class deoglTexSamplerConfig;
class deoglTexture;
class deoglTexture1D;



/**
 * \brief Texture stage manager.
 * 
 * Allows to enable and disable stages and bind different kinds of textures.
 * Keeps track of bound textures avoiding not require state changes.
 */
class deoglTextureStageManager{
public:
	/** \brief Filtering. */
	enum eTextureFiltering{
		/** \brief Nearest. */
		etfNearest,
		
		/** \brief Nearest with mipmap if possible. */
		etfNearestMipMap,
		
		/** \brief Nearest with linear mipmap if possible. */
		etfNearestMipMapLinear,
		
		/** \brief Linear no mipmap. */
		etfLinear,
		
		/** \brief Linear with mipmap if possible. */
		etfLinearMipMap,
		
		/** \brief Linear with nearest mipmap if possible. */
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
	/** \brief Create texture stage manager. */
	deoglTextureStageManager( deoglRenderThread &renderThread );
	
	/** \brief Clean up texture stage manager. */
	~deoglTextureStageManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of enabled stages. */
	int GetEnabledStageCount() const;
	
	/** \brief Type of texture bound to stage. */
	GLenum GetStageType( int stage ) const;
	
	/**
	 * \brief Enable stage and bind array texture.
	 * \details Sets no texture parameters. Useful if an array texture has to be
	 *          bound only for manipulating opengl states and is unbound
	 *          immediately afterwards.
	 */
	void EnableBareArrayTexture( int stage, const deoglArrayTexture &texture );
	
	/**
	 * \brief Enable stage and bind texture
	 * \details Sets no texture parameters. Useful if a texture has to be bound only
	 *          for manipulating opengl states and is unbound immediately afterwards.
	 */
	void EnableBareTexture( int stage, const deoglTexture &texture );
	
	/**
	 * \brief Enable stage and bind texture.
	 * \details Sets no texture parameters. Useful if a texture has to be bound only
	 *          for manipulating opengl states and is unbound immediately afterwards.
	 */
	void EnableBareTexture1D( int stage, const deoglTexture1D &texture );
	
	/**
	 * \brief Enable stage and bind cube map.
	 * \details Sets no texture parameters. Useful if a cube map has to be bound only
	 *          for manipulating opengl states and is unbound immediately afterwards.
	 */
	void EnableBareCubeMap( int stage, const deoglCubeMap &cubemap );
	
	/**
	 * \brief Enable stage and bind tbo without sampler.
	 * \details Sets no texture paramters. Useful if a tbo has to be bound for for
	 *          manipulating opegl states and is unbound immediately afterwards.
	 */
	void EnableBareTBO( int stage, GLuint tbo );
	
	
	
	/** \brief Enable stage and bind texture setting texture properties. */
	void EnableTexture( int stage, deoglTexture &texture, deoglTexSamplerConfig &samplerConfig );
	
	/** \brief Enable given stage and bind texture setting texture properties. */
	void EnableTexture1D( int stage, deoglTexture1D &texture, deoglTexSamplerConfig &samplerConfig );
	
	/** \brief Enable stage and bind cube map setting texture properties. */
	void EnableCubeMap( int stage, deoglCubeMap &cubemap, deoglTexSamplerConfig &samplerConfig );
	
	/** \brief Enable stage and bind array texture setting texture properties. */
	void EnableArrayTexture( int stage, deoglArrayTexture &texture, deoglTexSamplerConfig &samplerConfig );
	
	/** \brief Enable given stage and bind texture buffer object. */
	void EnableTBO( int stage, GLuint tbo, deoglTexSamplerConfig &samplerConfig );
	
	/**
	 * \brief Enable stage and bind texture channel from a skin.
	 * \details This function is aware of different texture properties assigned to the given
	 *          texture and sets the texture parameters accordingly. If the texture in the skin
	 *          is for some reason invalid the given default texture is used. Should this also
	 *          fail the stage will be disabled.
	 */
	void EnableSkin( int stage, deoglRSkin &skin, int texture, deoglSkinChannel::eChannelTypes channel,
		deoglTexture *defaultTexture, deoglTexSamplerConfig &samplerConfig );
	
	
	
	/** \brief Disable stage. */
	void DisableStage( int stage );
	
	/**
	 * \brief Disable all stages above stage.
	 * \details If you have for example 4 enabled stages and you want to disable stages
	 *          2 and 3 then you have to call this function with the stage of the last
	 *          stage to remain enabled thus 1 in this case.
	 */
	void DisableStagesAbove( int stage );
	
	/** \brief Disable all stages. */
	void DisableAllStages();
	
	
	
	/**
	 * \brief Activate stage and bind texture if not bound already.
	 * \details Keeps the currently bound texture sampler configuration the same.
	 */
	void BindTexture( int stage, GLuint texture, GLenum type );
	
	/**
	 * \brief Activate stage and bind texture if not bound already.
	 * \details Binds also the texture sampler configuration if different than the current one.
	 */
	void BindTexture( int stage, GLuint texture, GLenum type, GLuint samplerConfig );
	
	
	
	/** \brief Dump the current state to the logger. */
	void LogCurrentState() const;
	/*@}*/
};

#endif
