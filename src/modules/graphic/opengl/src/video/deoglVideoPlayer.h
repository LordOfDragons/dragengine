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
 * \brief Video player peer.
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
	/** \brief Create new peer. */
	deoglVideoPlayer( deGraphicOpenGl &ogl, deVideoPlayer &videoPlayer );
	
	/** \brief Clean up peer. */
	virtual ~deoglVideoPlayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenGL module. */
	inline deGraphicOpenGl &GetOpenGL(){ return pOgl; }
	inline const deGraphicOpenGl &GetOpenGL() const{ return pOgl; }
	
	/** \brief Video player. */
	inline const deVideoPlayer &GetVideoPlayer() const{ return pVideoPlayer; }
	
	/** \brief Current frame. */
	inline int GetCurrentFrame() const{ return pCurFrame; }
	
	/** \brief Render video player or \em NULL if not existing. */
	inline deoglRVideoPlayer *GetRVideoPlayer() const{ return pRVideoPlayer; }
	
	/** \brief Set current frame. */
	void SetCurrentFrame( int frame );
	
	/** \brief Update next frame. */
	void UpdateNextFrame();
	
	
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** \brief Renderables to notify about dirty events. */
	inline decPointerSet &GetNotifyRenderables(){ return pNotifyRenderables; }
	inline const decPointerSet &GetNotifyRenderables() const{ return pNotifyRenderables; }
	
	/** \brief Canvas to notify about dirty events. */
	inline decPointerSet &GetNotifyCanvas(){ return pNotifyCanvas; }
	inline const decPointerSet &GetNotifyCanvas() const{ return pNotifyCanvas; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Sound source changed. */
	virtual void SourceChanged();
	
	/** \brief Looping changed. */
	virtual void LoopingChanged();
	
	/** \brief Play range changed. */
	virtual void PlayRangeChanged();
	
	/** \brief Play speed changed. */
	virtual void PlaySpeedChanged();
	
	/**
	 * \brief Play position changed.
	 * \param[in] seeking Changed due to seeking or by deVideoPlayer::Update().
	 */
	virtual void PlayPositionChanged( bool seeking );
	
	/** \brief Play state changed. */
	virtual void PlayStateChanged();
	/*@}*/
	
	
	
private:
	void pPredictNextFrame();
	void pRequiresSync();
};

#endif
