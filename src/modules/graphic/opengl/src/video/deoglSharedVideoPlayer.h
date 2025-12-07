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

#ifndef _DEOGLSHAREDVIDEOPLAYER_H_
#define _DEOGLSHAREDVIDEOPLAYER_H_

#include <dragengine/src/resources/video/deVideoPlayer.h>
class deGraphicOpenGl;
class deoglSharedVideoPlayerList;



/**
 * Shared video player.
 * 
 * Video players are shared if they share the same deVideo, the same current playback
 * frame and the same playback speed. The playback position and speed are taken
 * directly from the video player.
 */
class deoglSharedVideoPlayer{
public:
	deGraphicOpenGl &pOgl;
	deoglSharedVideoPlayerList &pList;
	deVideoPlayer::Ref pVideoPlayer;
	int pUsageCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared video player. */
	deoglSharedVideoPlayer(deGraphicOpenGl &ogl, deoglSharedVideoPlayerList &list, deVideoPlayer *videoPlayer);
	
	/** Clean up shared video player. */
	~deoglSharedVideoPlayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenGL module. */
	inline deGraphicOpenGl &GetOpenGL(){ return pOgl; }
	inline const deGraphicOpenGl &GetOpenGL() const{ return pOgl; }
	
	/** Video player. */
	inline const deVideoPlayer::Ref &GetVideoPlayer() const{ return pVideoPlayer; }
	
	/** Usage count. */
	inline int GetUsageCount() const{ return pUsageCount; }
	
	/** Increase usage count by one. */
	void AddUsage();
	
	/** Decrease usage count by one. */
	void FreeUsage();
	
	
	
	/**
	 * Update the video player.
	 * \details Called from the main thread.
	 */
	void Update(float elapsed);
	
	/**
	 * Update render thread counterparts.
	 * \details Called from the main thread during synchronizing.
	 */
	void SyncToRender();
	/*@}*/
};

#endif
