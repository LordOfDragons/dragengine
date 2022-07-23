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

#ifndef _DEOGLIMAGESTAGEMANAGER_H_
#define _DEOGLIMAGESTAGEMANAGER_H_

#include "../deoglBasics.h"
#include "../skin/channel/deoglSkinChannel.h"

#define OGL_MAX_IMAGE_STAGES 16

class deoglArrayTexture;
class deoglCubeMap;
class deoglRenderThread;
class deoglRSkin;
class deoglTexture;
class deoglTexture1D;


/**
 * Image stage manager.
 * 
 * Allows to enable and disable stages and bind different kinds of images.
 * Keeps track of bound images avoiding not require state changes.
 */
class deoglImageStageManager{
public:
	/** Access. */
	enum eAccess{
		eaRead,
		eaWrite,
		eaReadWrite
	};
	
private:
	struct sStage{
		GLuint texture;
		GLenum type;
		GLint level;
		GLint layer;
		eAccess access;
		bool layered;
		GLenum format;
		sStage();
	};
	
private:
	deoglRenderThread &pRenderThread;
	sStage pStages[ OGL_MAX_IMAGE_STAGES ];
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create image stage manager. */
	deoglImageStageManager( deoglRenderThread &renderThread );
	
	/** Clean up image stage manager. */
	~deoglImageStageManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of enabled stages. */
	int GetEnabledStageCount() const;
	
	/** Type of imagebound to stage. */
	GLenum GetStageType( int stage ) const;
	
	/** Enable stage and bind image. */
	void Enable( int stage, const deoglTexture &texture, int level, eAccess access );
	
	/** Enable stage and bind image. */
	void Enable( int stage, const deoglTexture1D &texture, int level, eAccess access );
	
	/** Enable stage and bind cube map. */
	void Enable( int stage, const deoglCubeMap &cubemap, int level, eAccess access );
	
	/** Enable stage and bind cube map face. */
	void Enable( int stage, const deoglCubeMap &cubemap, int level, int face, eAccess access );
	
	/** Enable stage and bind array texture. */
	void Enable( int stage, const deoglArrayTexture &texture, int level, eAccess access );
	
	/** Enable stage and bind array texture layer. */
	void Enable( int stage, const deoglArrayTexture &texture, int level, int layer, eAccess access );
	
	/** Enable stage and bind texture buffer object. */
	void EnableTBO( int stage, GLuint tbo, eAccess access, GLenum format );
	
	/** Enable stage and bind texture channel from a skin. */
	void EnableSkin( int stage, const deoglRSkin &skin, int texture,
		deoglSkinChannel::eChannelTypes channel, deoglTexture *defaultTexture,
		eAccess access );
	
	/** Disable stage. */
	void DisableStage( int stage );
	
	/**
	 * Disable all stages above stage. If you have for example 4 enabled stages and you want
	 * to disable stages 2 and 3 then you have to call this function with the stage of the
	 * last stage to remain enabled thus 1 in this case.
	 */
	void DisableStagesAbove( int stage );
	
	/** Disable all stages. */
	void DisableAllStages();
	
	
	
	/** Activate stage and bind texture if not bound already. */
	void BindImage( int stage, GLuint texture, GLenum type, int level, int layer,
		eAccess access, bool layered, GLenum format );
	
	
	
	/** Dump the current state to the logger. */
	void LogCurrentState() const;
	/*@}*/
};

#endif
