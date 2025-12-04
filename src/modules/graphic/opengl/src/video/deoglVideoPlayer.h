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

#ifndef _DEOGLVIDEOPLAYER_H_
#define _DEOGLVIDEOPLAYER_H_

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/resources/video/deVideoDecoder.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicVideoPlayer.h>

class deoglVideo;
class deoglVideoDecodeThread;
class deoglRVideoPlayer;

class deVideoPlayer;
class deGraphicOpenGl;



/**
 * Video player peer.
 */
class deoglVideoPlayer : public deBaseGraphicVideoPlayer{
public:
	deGraphicOpenGl &pOgl;
	deVideoPlayer &pVideoPlayer;
	
	int pCurFrame;
	int pPredictedFrame;
	bool pDirtyFrame;
	bool pDirtySource;
	
	deoglVideo *pVideo;
	
	bool pBrokenVideoDecoder;
	deVideoDecoder::Ref pVideoDecoder;
	deoglVideoDecodeThread *pDecodeThread;
	
	deoglRVideoPlayer *pRVideoPlayer;
	
	decPointerSet pNotifyRenderables;
	decPointerSet pNotifyCanvas;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new peer. */
	deoglVideoPlayer( deGraphicOpenGl &ogl, deVideoPlayer &videoPlayer );
	
	/** Clean up peer. */
	virtual ~deoglVideoPlayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenGL module. */
	inline deGraphicOpenGl &GetOpenGL(){ return pOgl; }
	inline const deGraphicOpenGl &GetOpenGL() const{ return pOgl; }
	
	/** Video player. */
	inline const deVideoPlayer &GetVideoPlayer() const{ return pVideoPlayer; }
	
	/** Current frame. */
	inline int GetCurrentFrame() const{ return pCurFrame; }
	
	/** Render video player or \em NULL if not existing. */
	inline deoglRVideoPlayer *GetRVideoPlayer() const{ return pRVideoPlayer; }
	
	/** Set current frame. */
	void SetCurrentFrame( int frame );
	
	/** Update next frame. */
	void UpdateNextFrame();
	
	
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** Renderables to notify about dirty events. */
	inline decPointerSet &GetNotifyRenderables(){ return pNotifyRenderables; }
	inline const decPointerSet &GetNotifyRenderables() const{ return pNotifyRenderables; }
	
	/** Canvas to notify about dirty events. */
	inline decPointerSet &GetNotifyCanvas(){ return pNotifyCanvas; }
	inline const decPointerSet &GetNotifyCanvas() const{ return pNotifyCanvas; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Sound source changed. */
	virtual void SourceChanged();
	
	/** Looping changed. */
	virtual void LoopingChanged();
	
	/** Play range changed. */
	virtual void PlayRangeChanged();
	
	/** Play speed changed. */
	virtual void PlaySpeedChanged();
	
	/**
	 * Play position changed.
	 * \param[in] seeking Changed due to seeking or by deVideoPlayer::Update().
	 */
	virtual void PlayPositionChanged( bool seeking );
	
	/** Play state changed. */
	virtual void PlayStateChanged();
	/*@}*/
	
	
	
private:
	void pPredictNextFrame();
	void pRequiresSync();
};

#endif
