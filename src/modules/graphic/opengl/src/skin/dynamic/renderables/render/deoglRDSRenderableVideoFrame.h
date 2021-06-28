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

#ifndef _DEOGLRRDSRENDERABLEVIDEOFRAME_H_
#define _DEOGLRRDSRENDERABLEVIDEOFRAME_H_

#include <dragengine/deObject.h>

#include "deoglRDSRenderable.h"

class deoglRVideoPlayer;



/**
 * Render dynamic skin image renderable.
 */
class deoglRDSRenderableVideoFrame : public deoglRDSRenderable{
private:
	deoglRVideoPlayer *pVideoPlayer;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render dynamic skin image renderable. */
	deoglRDSRenderableVideoFrame( deoglRDynamicSkin &dynamicSkin );
	
	/** Clean up render dynamic skin image renderable. */
	virtual ~deoglRDSRenderableVideoFrame();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Video player. */
	inline deoglRVideoPlayer *GetVideoPlayer() const{ return pVideoPlayer; }
	
	/** Set video player. */
	void SetVideoPlayer( deoglRVideoPlayer *videoPlayer );
	
	/** Prepare for render. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/**
	 * Get texture to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglTexture *GetRenderTexture();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
