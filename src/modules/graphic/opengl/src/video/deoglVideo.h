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
	
	deoglRVideo::Ref pRVideo;
	
	int pCachedFrameCount;
	bool pCacheFrames;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglVideo(deGraphicOpenGl &ogl, deVideo &video);
	
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
	bool IsFrameReady(int frame) const;
	
	/** Can cache frame. */
	bool CanCacheFrame(int frame) const;
	
	/**
	 * Cache frame.
	 * \returns Previously set pixel buffer or nullptr.
	 */
	deoglPixelBuffer::Ref CacheFrame(int frame, deoglPixelBuffer *pixelBuffer);
	
	/** Cache frame texture or \em NULL if not ready. */
	deoglTexture *GetCachedFrameTexture(int frame) const;
	/*@}*/
	
	
	
private:
	void pDetermineCacheParams();
};

#endif
