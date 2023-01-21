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

#ifndef _DEOGLVIDEO_H_
#define _DEOGLVIDEO_H_

#include "../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicVideo.h>
#include <dragengine/common/math/decMath.h>

class deGraphicOpenGl;
class deoglRVideo;
class deoglTexture;

class deVideo;



/**
 * Video peer.
 */
class deoglVideo : public deBaseGraphicVideo{
public:
	deGraphicOpenGl &pOgl;
	deVideo &pVideo;
	
	deoglRVideo *pRVideo;
	
	int pCachedFrameCount;
	bool pCacheFrames;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglVideo( deGraphicOpenGl &ogl, deVideo &video );
	
	/** Clean up peer. */
	virtual ~deoglVideo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deGraphicOpenGl &GetOpenGL() const{ return pOgl; }
	
	/** Video resource. */
	inline deVideo &GetVideo() const{ return pVideo; }
	
	/** Render video or \em NULL if not existing. */
	inline deoglRVideo *GetRVideo() const{ return pRVideo; }
	
	
	
	/**
	 * Video player has to use and update the cached frames.
	 */
	inline bool CacheFrames() const{ return pCacheFrames; }
	
	/**
	 * All video frames are cached and ready to be used.
	 * \retval true Video player does not have to do caching work. Any frames can
	 *              be directly used for rendering without further work.
	 * \retval false Video player has to check if the desired frame is already
	 *               cached. If not the video player has to update the cached
	 *               frame if no other frame is already being updated.
	 */
	bool AllFramesAreCached() const;
	
	/** Frame is cached and ready. */
	bool IsFrameReady( int frame ) const;
	
	/** Can cache frame. */
	bool CanCacheFrame( int frame ) const;
	
	/**
	 * Cache frame.
	 * \returns Previously set pixel buffer or nullptr.
	 */
	deoglPixelBuffer::Ref CacheFrame( int frame, deoglPixelBuffer *pixelBuffer );
	
	/** Cache frame texture or \em NULL if not ready. */
	deoglTexture *GetCachedFrameTexture( int frame ) const;
	/*@}*/
	
	
	
private:
	void pDetermineCacheParams();
};

#endif
