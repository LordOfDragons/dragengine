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

#ifndef _DEOGLCANVASVIDEOPLAYER_H_
#define _DEOGLCANVASVIDEOPLAYER_H_

#include "deoglCanvas.h"

class deoglRCanvasVideoPlayer;
class deoglVideoPlayer;

class deCanvasVideoPlayer;



/**
 * Canvas peer.
 */
class deoglCanvasVideoPlayer : public deoglCanvas{
private:
	deCanvasVideoPlayer &pCanvasVideoPlayer;
	deoglRCanvasVideoPlayer *pRCanvasVideoPlayer;
	deoglVideoPlayer *pVideoPlayer;
	bool pDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglCanvasVideoPlayer( deGraphicOpenGl &ogl, deCanvasVideoPlayer &canvas );
	
	/** Clean up peer. */
	virtual ~deoglCanvasVideoPlayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Drop render canvas if not \em NULL. */
	virtual void DropRCanvas();
	
	/**
	 * Prepare content for render thread counterpart.
	 * \details Called if content is dirty.
	 */
	virtual void SyncContentToRender();
	
	/** Video player requires sync. */
	void VideoPlayerRequiresSync();
	
	/** Drop video player. */
	void DropVideoPlayer();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Content changed. */
	virtual void ContentChanged();
	/*@}*/
	
	
	
protected:
	/** Create render canvas. Subclass responsibility. */
	virtual deoglRCanvas *CreateRCanvas();
};

#endif
