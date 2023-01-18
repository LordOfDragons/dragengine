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

#ifndef _DEOGLSHAREDVIDEOPLAYER_H_
#define _DEOGLSHAREDVIDEOPLAYER_H_

class deVideoPlayer;
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
	deVideoPlayer *pVideoPlayer;
	int pUsageCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared video player. */
	deoglSharedVideoPlayer( deGraphicOpenGl &ogl, deoglSharedVideoPlayerList &list, deVideoPlayer *videoPlayer );
	
	/** Clean up shared video player. */
	~deoglSharedVideoPlayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenGL module. */
	inline deGraphicOpenGl &GetOpenGL(){ return pOgl; }
	inline const deGraphicOpenGl &GetOpenGL() const{ return pOgl; }
	
	/** Video player. */
	inline deVideoPlayer *GetVideoPlayer() const{ return pVideoPlayer; }
	
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
	void Update( float elapsed );
	
	/**
	 * Update render thread counterparts.
	 * \details Called from the main thread during synchronizing.
	 */
	void SyncToRender();
	/*@}*/
};

#endif
