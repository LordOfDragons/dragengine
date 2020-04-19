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

#ifndef _DEOGLRVIDEOPLAYER_H_
#define _DEOGLRVIDEOPLAYER_H_

#include <dragengine/deObject.h>

class deoglRVideo;
class deoglPixelBuffer;
class deoglRenderThread;
class deoglTexture;



/**
 * \brief Render video player.
 */
class deoglRVideoPlayer : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRVideo *pVideo;
	deoglTexture *pCachedFrameTexture;
	int pUpdateCachedFrameTexture;
	
	int pWidth;
	int pHeight;
	int pComponentCount;
	
	deoglPixelBuffer *pPixelBuffer;
	deoglTexture *pTexture;
	bool pDirtyTexture;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create video player target. */
	deoglRVideoPlayer( deoglRenderThread &renderThread );
	
	/** \brief Clean up video player target. */
	virtual ~deoglRVideoPlayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render video or \em NULL if not set. */
	inline deoglRVideo *GetVideo() const{ return pVideo; }
	
	/** \brief Set render video or \em NULL if not set. */
	void SetVideo( deoglRVideo *video );
	
	/** \brief Cached frame or -1 if not set. */
	inline deoglTexture *GetCachedFrameTexture() const{ return pCachedFrameTexture; }
	
	/** \brief Set cached frame or -1 if not set. */
	void SetCachedFrameTexture( deoglTexture *texture );
	
	/** \brief Set update cached frame texture. */
	void SetUpdateCachedFrameTexture( int updateCachedFrameTexture );
	
	/** \brief Has cached frame texture or update cached frame. */
	bool HasCachedFrameTexture() const;
	
	
	
	/** \brief Width of video. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height of video. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Component count. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/**
	 * \brief Set video size.
	 * \details Deletes the texture and pixel buffer if set.
	 */
	void SetVideoSize( int width, int height, int componentCount );
	
	/**
	 * \brief Set pixel buffer to update texture with.
	 * \details Sets pixel dirty true. Does not free the old pixel buffer. If a pixel
	 *          buffer is still set during destruction the pixel buffer is freed. This
	 *          allows to swap pixel buffer with the decode thread.
	 * \returns Previously set pixel buffer.
	 */
	deoglPixelBuffer *SetPixelBuffer( deoglPixelBuffer *pixelBuffer );
	
	/** \brief Texture or \em NULL if not existing. */
	deoglTexture *GetTexture() const;
	
	
	
	/** \brief Update texture if dirty. */
	void UpdateTexture();
	/*@}*/
};

#endif
