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
	
	deoglRVideo::Ref pVideo;
	deoglTexture *pCachedFrameTexture;
	int pUpdateCachedFrameTexture;
	
	int pWidth;
	int pHeight;
	int pComponentCount;
	
	deoglPixelBuffer::Ref pPixelBuffer;
	deoglTexture *pTexture;
	bool pDirtyTexture;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRVideoPlayer> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create video player target. */
	deoglRVideoPlayer(deoglRenderThread &renderThread);
	
	/** Clean up video player target. */
	virtual ~deoglRVideoPlayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render video or \em NULL if not set. */
	inline const deoglRVideo::Ref &GetVideo() const{ return pVideo; }
	
	/** Set render video or \em NULL if not set. */
	void SetVideo(deoglRVideo *video);
	
	/** Cached frame or -1 if not set. */
	inline deoglTexture *GetCachedFrameTexture() const{ return pCachedFrameTexture; }
	
	/** Set cached frame or -1 if not set. */
	void SetCachedFrameTexture(deoglTexture *texture);
	
	/** Set update cached frame texture. */
	void SetUpdateCachedFrameTexture(int updateCachedFrameTexture);
	
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
	void SetVideoSize(int width, int height, int componentCount);
	
	/**
	 * Set pixel buffer to update texture with.
	 * \returns Previously set pixel buffer.
	 */
	deoglPixelBuffer::Ref SetPixelBuffer(deoglPixelBuffer *pixelBuffer);
	
	/** Texture or \em NULL if not existing. */
	deoglTexture *GetTexture() const;
	
	
	
	/** Update texture if dirty. */
	void UpdateTexture();
	/*@}*/
};

#endif
