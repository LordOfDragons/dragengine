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

#ifndef _DEOGLDSRENDERABLEVIDEOFRAME_H_
#define _DEOGLDSRENDERABLEVIDEOFRAME_H_

#include "deoglDSRenderable.h"

class deoglRDSRenderableVideoFrame;
class deoglVideoPlayer;

class deDSRenderableVideoFrame;



/**
 * Dynamic skin image renderable.
 */
class deoglDSRenderableVideoFrame : public deoglDSRenderable{
public:
	const deDSRenderableVideoFrame &pRenderableVideoFrame;
	deoglRDSRenderableVideoFrame *pRRenderableVideoFrame;
	deoglVideoPlayer *pVideoPlayer;
	bool pDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderable. */
	deoglDSRenderableVideoFrame( deoglDynamicSkin &dynamicSkin, const deDSRenderableVideoFrame &renderable );
	
	/** Clean up peer. */
	virtual ~deoglDSRenderableVideoFrame();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Renderable. */
	inline const deDSRenderableVideoFrame &GetRenderableVideoFrame() const{ return pRenderableVideoFrame; }
	
	/** Render renderable. */
	virtual deoglRDSRenderable *GetRRenderable() const;
	
	/** Renderable changed. */
	virtual void RenderableChanged();
	
	/** Update render thread counterpart if required. */
	virtual void SyncToRender();
	
	/** Video player requires sync. */
	void VideoPlayerRequiresSync();
	
	/** Drop video player. */
	void DropVideoPlayer();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
