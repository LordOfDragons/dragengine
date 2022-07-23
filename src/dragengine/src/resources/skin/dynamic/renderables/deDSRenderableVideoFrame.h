/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEDSRENDERABLEVIDEOFRAME_H_
#define _DEDSRENDERABLEVIDEOFRAME_H_

#include "deDSRenderable.h"
#include "../../../video/deVideoPlayerReference.h"


/**
 * \brief Dynamic Skin Renderable Video Frame.
 *
 * The texture is defined by a frame of a video.
 */
class DE_DLL_EXPORT deDSRenderableVideoFrame : public deDSRenderable{
private:
	deVideoPlayerReference pVideoPlayer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new renderable. */
	deDSRenderableVideoFrame( const char *name );
	
	/** \brief Clean up renderable. */
	virtual ~deDSRenderableVideoFrame();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Video player or NULL if not set. */
	inline deVideoPlayer *GetVideoPlayer() const{ return pVideoPlayer; }
	
	/** \brief Set video player or NULL if not set. */
	void SetVideoPlayer( deVideoPlayer *videoPlayer );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the renderable. */
	virtual void Visit( deDSRenderableVisitor &visitor );
};

#endif
