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

#ifndef _DEOGLRCANVASVIDEOPLAYER_H_
#define _DEOGLRCANVASVIDEOPLAYER_H_

#include "deoglRCanvas.h"

class deoglRVideoPlayer;


/**
 * \brief Render canvas video player.
 */
class deoglRCanvasVideoPlayer : public deoglRCanvas{
private:
	deoglRVideoPlayer *pVideoPlayer;
	decTexMatrix2 pTCTransform;
	decVector2 pTCClampMin;
	decVector2 pTCClampMax;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglRCanvasVideoPlayer( deoglRenderThread &renderThread );
	
	/** \brief Clean up peer. */
	virtual ~deoglRCanvasVideoPlayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Video player or \em NULL if not set. */
	inline deoglRVideoPlayer *GetVideoPlayer() const{ return pVideoPlayer; }
	
	/** \brief Set video player or \em NULL if not set. */
	void SetVideoPlayer( deoglRVideoPlayer *videoPlayer );
	
	/** \brief Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTCTransform() const{ return pTCTransform; }
	
	/** \brief Set texture coordinate transformation matrix. */
	void SetTCTransform( const decTexMatrix2 &transform );
	
	/** \brief Texture coordinates clamp minimum. */
	inline const decVector2 &GetTCClampMinimum() const{ return pTCClampMin; }
	
	/** \brief Set texture coordinates clamp minimum. */
	void SetTCClampMinimum( const decVector2 &clamp );
	
	/** \brief Texture coordinates clamp maximum. */
	inline const decVector2 &GetTCClampMaximum() const{ return pTCClampMax; }
	
	/** \brief Set texture coordinates clamp maximum. */
	void SetTCClampMaximum( const decVector2 &clamp );
	
	
	
	/** \brief Prepare for rendering. */
	virtual void PrepareForRender();
	
	/** \brief Render. */
	virtual void Render( const deoglRenderCanvasContext &context );
	/*@}*/
};

#endif
