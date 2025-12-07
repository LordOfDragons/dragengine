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

#ifndef _DEOGLRVIDEO_H_
#define _DEOGLRVIDEO_H_

#include "../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/deObject.h>
#include <dragengine/resources/video/deVideo.h>

class deVideo;
class deoglRenderThread;
class deoglTexture;



/**
 * Render video.
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
	const int pComponentCount;
	
	deoglTexture **pFrames;
	int pFrameCount;
	int pFrameCountToCache;
	
	deoglPixelBuffer::Ref pPixelBuffer;
	int pUpdateFrame;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRVideo> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render video. */
	deoglRVideo(deoglRenderThread &renderThread, int width, int height, int componentCount, int frameCount);
	
	/** Clean up render video. */
	virtual ~deoglRVideo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Frame texture or \em NULL if not ready. */
	deoglTexture *GetTexture(int frame) const;
	
	/** Number of frames. */
	inline int GetFrameCount() const{return pFrameCount;}
	
	/** Remaining number of frames to cache or -1 if no caching is used. */
	inline int GetFrameCountToCache() const{return pFrameCountToCache;}
	
	
	/**
	 * Set pixel buffer to update frame texture with.
	 * \returns Previously set pixel buffer or nullptr.
	 */
	deoglPixelBuffer::Ref SetPixelBuffer(int frame, deoglPixelBuffer *pixelBuffer);
	
	/** Frame to update or -1 if not set to update a frame. */
	inline int GetUpdateFrame() const{return pUpdateFrame;}
	
	
	
	/** Update texture if required. */
	void UpdateTexture();
	/*@}*/
	
	
	
private:
};

#endif
