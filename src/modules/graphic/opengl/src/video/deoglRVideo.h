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

#ifndef _DEOGLRVIDEO_H_
#define _DEOGLRVIDEO_H_

#include <dragengine/deObject.h>

class deVideo;
class deoglPixelBuffer;
class deoglRenderThread;
class deoglTexture;



/**
 * \brief Render video.
 * 
 * Stores cached frames if the video is small enough to warrant keeping it
 * in memory. Video player peers are responsible to store cached frames
 * during synchronization time. During rendering time the cached frames
 * can be accessed.
 * 
 * Only one video player can upload a new cached texture. If a cached
 * texture is set all other video players have to skip storing another
 * cached texture. This allows to keep the storing procedure simple while
 * being a little bit less optimal. The chance of multiple video players
 * to randomly access the same video file is small. In the case they still
 * do the video is preloaded by one video player in short time anyways.
 */
class deoglRVideo : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	const int pWidth;
	const int pHeight;
	
	deoglTexture **pFrames;
	int pFrameCount;
	int pFrameCountToCache;
	
	deoglPixelBuffer *pPixelBuffer;
	int pUpdateFrame;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render video. */
	deoglRVideo( deoglRenderThread &renderThread, int width, int height, int frameCount );
	
	/** \brief Clean up render video. */
	virtual ~deoglRVideo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Frame texture or \em NULL if not ready. */
	deoglTexture *GetTexture( int frame ) const;
	
	/** \brief Number of frames. */
	inline int GetFrameCount() const{ return pFrameCount; }
	
	/** \brief Remaining number of frames to cache or -1 if no caching is used. */
	inline int GetFrameCountToCache() const{ return pFrameCountToCache; }
	
	
	/**
	 * \brief Set pixel buffer to update frame texture with.
	 * \details Does not free the old pixel buffer. If a pixel buffer is still set
	 *          during destruction the pixel buffer is freed. This allows to swap
	 *          pixel buffer with the decode thread.
	 * \returns Previously set pixel buffer or \em NULL.
	 */
	deoglPixelBuffer *SetPixelBuffer( int frame, deoglPixelBuffer *pixelBuffer );
	
	/** \brief Frame to update or -1 if not set to update a frame. */
	inline int GetUpdateFrame() const{ return pUpdateFrame; }
	
	
	
	/** \brief Update texture if required. */
	void UpdateTexture();
	/*@}*/
	
	
	
private:
};

#endif
