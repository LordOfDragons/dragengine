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

#ifndef _DEDSRENDERABLEVIDEOFRAME_H_
#define _DEDSRENDERABLEVIDEOFRAME_H_

#include "deDSRenderable.h"
#include "../../../video/deVideoPlayer.h"


/**
 * \brief Dynamic Skin Renderable Video Frame.
 *
 * The texture is defined by a frame of a video.
 */
class DE_DLL_EXPORT deDSRenderableVideoFrame : public deDSRenderable{
private:
	deVideoPlayer::Ref pVideoPlayer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new renderable. */
	deDSRenderableVideoFrame(const char *name);
	
	/** \brief Clean up renderable. */
	virtual ~deDSRenderableVideoFrame();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Video player or NULL if not set. */
	inline deVideoPlayer *GetVideoPlayer() const{ return pVideoPlayer; }
	
	/** \brief Set video player or NULL if not set. */
	void SetVideoPlayer(deVideoPlayer *videoPlayer);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the renderable. */
	virtual void Visit(deDSRenderableVisitor &visitor);
};

#endif
