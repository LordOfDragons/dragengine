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

#ifndef _DEOGLVIDEOPLAYER_H_
#define _DEOGLVIDEOPLAYER_H_

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/resources/video/deVideoDecoderReference.h>
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
	deVideoDecoderReference pVideoDecoder;
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
