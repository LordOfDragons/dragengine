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

#include "../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/deObject.h>

class deoglRVideo;
class deoglRenderThread;
class deoglTexture;



/**
 * Render video player.
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
	
	deoglPixelBuffer::Ref pPixelBuffer;
	deoglTexture *pTexture;
	bool pDirtyTexture;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create video player target. */
	deoglRVideoPlayer( deoglRenderThread &renderThread );
	
	/** Clean up video player target. */
	virtual ~deoglRVideoPlayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render video or \em NULL if not set. */
	inline deoglRVideo *GetVideo() const{ return pVideo; }
	
	/** Set render video or \em NULL if not set. */
	void SetVideo( deoglRVideo *video );
	
	/** Cached frame or -1 if not set. */
	inline deoglTexture *GetCachedFrameTexture() const{ return pCachedFrameTexture; }
	
	/** Set cached frame or -1 if not set. */
	void SetCachedFrameTexture( deoglTexture *texture );
	
	/** Set update cached frame texture. */
	void SetUpdateCachedFrameTexture( int updateCachedFrameTexture );
	
	/** Has cached frame texture or update cached frame. */
	bool HasCachedFrameTexture() const;
	
	
	
	/** Width of video. */
	inline int GetWidth() const{ return pWidth; }
	
	/** Height of video. */
	inline int GetHeight() const{ return pHeight; }
	
	/** Component count. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/**
	 * Set video size.
	 * \details Deletes the texture and pixel buffer if set.
	 */
	void SetVideoSize( int width, int height, int componentCount );
	
	/**
	 * Set pixel buffer to update texture with.
	 * \returns Previously set pixel buffer.
	 */
	deoglPixelBuffer::Ref SetPixelBuffer( deoglPixelBuffer *pixelBuffer );
	
	/** Texture or \em NULL if not existing. */
	deoglTexture *GetTexture() const;
	
	
	
	/** Update texture if dirty. */
	void UpdateTexture();
	/*@}*/
};

#endif
